#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <string>
#include <cstring>

//agregar en c++
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int analizar_servidor_info(char buffer[]){
	printf("%s",buffer);
	if((strcmp(buffer,"+Ok. Desconexión procesada\n")==0)||(strcmp(buffer,"+Ok. Desconectando servidor\n")==0))
		return 1;
return 0;
}

char * extraerCabecera(char buffer[]){
	char *cabecera=(char*)malloc(sizeof(buffer));
	int i;

	for(i=0;(buffer[i]!='\0')&&(buffer[i]!=' ');i++){
		cabecera[i]=buffer[i];
	}
	cabecera[i]='\0';

return cabecera;
}

int main(){

int descriptor; //Guardará el identificador del socket.
struct sockaddr_in socket_s; //Guarda la información del socket
socklen_t longitud_socket;
fd_set leerfds,auxfds;
char buffer[250];
int salir=0;
//Carton c;

	descriptor=socket(AF_INET,SOCK_STREAM,0); //SOCK_STREAM es el protocolo que indica que es una conexión TCP
	if (descriptor == -1){
		perror("No se ha podido abrir el socket del cliente");
		exit(1);
	}

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	socket_s.sin_family = AF_INET; //Indica la familia
	socket_s.sin_port = htons(2050); //Indica el puerto
	socket_s.sin_addr.s_addr =  inet_addr("127.0.0.1"); //Indica la drección ip a la que se conectará

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	longitud_socket = sizeof(socket_s);
	
	if (connect(descriptor, (struct sockaddr *)&socket_s, longitud_socket) == -1){ //Realiza la conexión
		perror ("Error en la conexión con el servidor");
		exit(1);
	}
    
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&leerfds);
    
    FD_SET(0,&leerfds);
    FD_SET(descriptor,&leerfds);

	//Una vez inicializadas todo, se entra en comunicación

	while(salir==0){
		auxfds=leerfds; //Copiamos en aux para que el select no lo modifique
		select(descriptor+1,&auxfds,NULL,NULL,NULL); //Llamada a select que espera hasta que lea algo por teclado o por el servidor
		bzero(buffer, sizeof(buffer)); //Ponemos a 0 el buffer

		//Se ha recibido algo por teclado o por el servidor, procedemos a comprobarlo

		if(FD_ISSET(descriptor,&auxfds)){ //El servidor ha enviado algo
			recv(descriptor,buffer,sizeof(buffer),0);
			
			char *cabecera=extraerCabecera(buffer); //Obtiene el comando recibido por el servidor

			if(strcmp(cabecera,"+Ok.")==0){
				salir=analizar_servidor_info(buffer);
			}	
			else if(strcmp(cabecera,"-Err.")==0)
				salir=analizar_servidor_info(buffer);
			//else if(strcmp(cabecera,"CARTON")==0){
			//	vector<int> v;
			//	c.setTachados(v);
			//	c.setValor(buffer);
			//	cout<<c<<endl;
			//}
			/*else if(strcmp(cabecera,"NÚMERO-OBTENIDO")==0){
				int i;
				for(i=0;buffer[i]!='<';i++);
				char num[3];
				num[0]=buffer[i+1];num[1]=buffer[i+2];num[2]='\0';
				c.tachar(atoi(num));

				salir=analizar_servidor_info(buffer);
				cout<<c<<endl;
			}*/
			//else
			//	cout<<"-Err. No se reconoce el formato del mensaje."<<endl;

		}
		else if(FD_ISSET(0,&auxfds)){ //Se ha escrito algo por teclado
			bzero(buffer, sizeof(buffer));
            fgets(buffer, sizeof(buffer),stdin);
			send(descriptor,buffer,sizeof(buffer),0);

		}
	}

	close(descriptor); //Cerramos el socket.
return 0;
}
