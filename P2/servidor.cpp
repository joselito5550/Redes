#include "ruleta.hpp"
#include "partida.hpp"


int main (){
	srand(time(NULL)); //Generamos semilla para los números aleatorios.

	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
	fd_set readfds, auxfds;
	int salida;
	vector <Jugador> arrayJugadores; //MAX_CLIENTS
	vector <Partida> arrayPartidas; //10
	int numJugadores = 0;
	int numPartidas =0;
	struct timeval timeout;
	//contadores
	int i,j;
	int recibidos;
	string oracion,aux;
	int on;


	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
			exit (1);	
	}

	// Activaremos una propiedad del socket que permitir· que otros
	// sockets puedan reutilizar cualquier puerto al que nos enlacemos.
	// Esto permitir· en protocolos como el TCP, poder ejecutar un
	// mismo programa varias veces seguidas y enlazarlo siempre al
	// mismo puerto. De lo contrario habrÌa que esperar a que el puerto
	// quedase disponible (TIME_WAIT en el caso de TCP)
	on=1;
	setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}

	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			perror("Error en la operación de listen");
			exit(1);
		}

	//Inicializar los conjuntos fd_set
	FD_ZERO(&readfds);
	FD_ZERO(&auxfds);
	FD_SET(sd,&readfds);
	FD_SET(0,&readfds);


	//Capturamos la señal SIGINT (Ctrl+c)
	signal(SIGINT,manejador);

	//Inicializar la estructua timeval
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
	while(1){
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
   
        auxfds = readfds;
        
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,&timeout);
        
        if(salida > 0){
            
            
            for(i=0; i<FD_SETSIZE; i++){
                //Buscamos el socket por el que se ha establecido la comunicación
                if(FD_ISSET(i, &auxfds)) {
                    
                    if( i == sd){
                        
                        if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                            perror("Error aceptando peticiones");
                        }
                        else{
							//Nuevo cliente
                            if(numJugadores < MAX_CLIENTS){ 
								Jugador n(new_sd);
                                arrayJugadores.push_back(n);
                                numJugadores++;
                                FD_SET(new_sd,&readfds);

								strcpy(buffer,"+Ok. Usuario conectado\n");
								send(new_sd,buffer,strlen(buffer),0);

                            }
                            else{
								strcpy(buffer,"-Err. Demasiados clientes conectados\n");
                                send(new_sd,buffer,strlen(buffer),0);
                                close(new_sd);
                            }                                
                        }                           
                    }
                    else if (i == 0){ //Información del teclado del servidor.
                        //Se ha introducido información de teclado
						bzero(buffer, sizeof(buffer));
						fgets(buffer, sizeof(buffer),stdin);
                        
                        //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                        if(strcmp(buffer,"SALIR\n")==0){                             
                            for (j = 0; j < numJugadores; j++){
                                send(arrayJugadores[j].getId(), "+Ok. Desconectando servidor\n", strlen("+Ok. Desconectando servidor\n"),0);
                                close(arrayJugadores[j].getId());
                                FD_CLR(arrayJugadores[j].getId(),&readfds);
                            }
							cout<<"+Ok. Desconectando servidor\n";
                                close(sd);
                                exit(-1);                            
                        }
                        //Mensajes que se quieran mandar a los clientes (implementar)
                        
                    } 
                    else{                       
						recibidos=recv(i,buffer,sizeof(buffer),0);
                        
                        if(recibidos > 0){                                
                            if(strcmp(buffer,"SALIR\n") == 0){ 
								strcpy(buffer,"+Ok. Desconexión procesada\n");
								send(i,buffer,strlen(buffer),0);
								usleep(1000);
								eliminar_jugador_partida(i,numPartidas, arrayPartidas,arrayJugadores);                                   
                                salirCliente(i,&readfds,numJugadores,arrayJugadores);   
							}                          
                            else{
								//Manejo información recibida por el cliente. Implementar aquí el juego  
								int jugadorId=buscarJugador(i,arrayJugadores);	                                  
								char *cabecera=extraerCabecera(buffer);

								if(arrayJugadores[jugadorId].isLogueado()){
									if(strcmp(cabecera,"INICIAR-PARTIDA\n")==0){
										if(buscarPartida(i,arrayPartidas)==-1){
											if((numPartidas==0) || (arrayPartidas[numPartidas-1].isComenzada())){
												Partida p;
												arrayPartidas.push_back(p);
												numPartidas++;
												cout<<"Creando partida número "<<numPartidas<<endl;
												
											}
											//Añadimos el jugador a la partida.
											if(arrayPartidas[numPartidas-1].nuevo_jugador(arrayJugadores[jugadorId])){
												if(arrayPartidas[numPartidas-1].iniciar_partida()){
													vector<Jugador> vj=arrayPartidas[numPartidas-1].getJugadores();
											
													for(int j=0;j<2;j++){
														int identidad=vj[j].getId();
														strcpy(buffer,"+Ok. Empieza la partida\n");
														send(identidad,buffer,strlen(buffer),0);
														usleep(1000);
														//sprintf(buffer,"%s\n",vj[j].getCarton().getValor().c_str());
														send(identidad,buffer,strlen(buffer),0);
													}
												}
												else{
													strcpy(buffer,"+Ok. Petición Recibida. Quedamos a la espera de más jugadores\n");
													send(i,buffer,strlen(buffer),0);
												}											
											}
										else{
											strcpy(buffer,"-Err. Usted ya está jugando una partida\n");
											send(i,buffer,strlen(buffer),0);
										}
									}
									}
									if(strcmp(cabecera,"PARTIDA-INDIVIDUAL\n")==0){
										if(buscarPartida(i,arrayPartidas)==-1){
											Partida p;
											arrayPartidas.push_back(p);
											numPartidas++;
											cout<<"Creando partida numero"<<numPartidas<<endl;
											oracion = "+Frase. ";
											aux = "+Frase. ";
											oracion+=extraerFichero();
											cout<<"oracion "<<oracion;
											aux+=ocultarOracion(oracion);
											cout<<"aux "<<aux;
											p.setFrase(oracion);
											p.setFraseOculta(aux);
											strcpy(buffer,aux.c_str());
											send(i,buffer,strlen(buffer),0);
										}
									}

									else if(strcmp(cabecera,"SALIR-PARTIDA\n")==0){
										eliminar_jugador_partida(i,numPartidas,arrayPartidas, arrayJugadores);
									}
									else if(strcmp(cabecera,"VOCAL")==0){
										//RECIBIDA VOCAL

										Partida p = arrayPartidas[buscarPartida(i,arrayPartidas)];
										char letra = buffer[0];
										bool esta = p.buscarLetra(letra);
										if(esta) cout<<"bien";
										strcpy(buffer,p.getFraseOculta().c_str());
										send(i,buffer,strlen(buffer),0);
									}

									else if(strcmp(cabecera,"CONSONANTE")==0){
										//RECIBIDA CONSONANTE
									}

								}
								//Registro y logueo del usuario
								else{
									if(strcmp(cabecera,"REGISTRO")==0){
										registrar(i,buffer,arrayJugadores);
									}		
									if(strcmp(cabecera,"USUARIO")==0){
										setUsuario(i,buffer,arrayJugadores);
									}	
									if(strcmp(cabecera,"PASSWORD")==0){
										loguear(i,buffer,arrayJugadores);
									}	
								}

                            }                        
                        }
                        //Si el cliente introdujo ctrl+c
                        if(recibidos== 0)
                        {
                            cout<<"El socket "<<i<<", ha introducido ctrl+c"<<endl;
							eliminar_jugador_partida(i,numPartidas, arrayPartidas,arrayJugadores);
                            //Eliminar ese socket
                            salirCliente(i,&readfds,numJugadores,arrayJugadores);
                        }
                    }
                }
            }
        }
		else if(salida==0){	//Generamos una bola, si hay partidas y si han pasado x segundos
				for(int j=0;j<numPartidas;j++){
					if(arrayPartidas[j].isComenzada()){
						//int num=arrayPartidas[j].nuevo_numero();
						/*if((num!=0)&&(not arrayPartidas[j].isBingo())){
							sprintf(buffer,"NÚMERO-OBTENIDO <%d%d\n",num/10,num%10);
							enivar_mensaje_jugadores_partida(buffer,arrayPartidas[j]);
							cout<<"Partida "<<j+1<<" -> "<<"NÚMERO-OBTENIDO: "<<num<<endl;
						}
						else
							*/eliminar_partida(j,arrayPartidas,numPartidas);														
					}
				}
		   	//Inicializar la estructua timeval
			timeout.tv_sec = 5;
			timeout.tv_usec = 0;
		} 
	}
	close(sd);
}

