#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

/* --------------------------------------------------------------------------------------
 
 Envía un número aleatorio al servidor, quien el devuelve el número incremetnado

---------------------------------------------------------------------------------------- */
 
int main (int argc, char **argv)
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer para datos 
	-----------------------------------------------------*/
	int Socket_Cliente;
	char mensaje [80];
	char respuesta [80];
	struct timeval timeout;
	fd_set lectura;
	int salida;
	
	//Inicializar la estructua timeval
	timeout.tv_sec = atoi(argv[2]);
	timeout.tv_usec = 0;

	//Inicializar los conjuntos fd_set
	FD_ZERO(&lectura);
	FD_SET(0,&lectura);

   	/* -----------------------------------------------------
   		Informaci\ufffdn del Servidor
   	-----------------------------------------------------*/
   	struct sockaddr_in Servidor;  
   	socklen_t Longitud_Servidor;
  
  
	/* --------------------------------------------------
		Se abre el socket cliente
	---------------------------------------------------*/
	Socket_Cliente = socket (AF_INET, SOCK_DGRAM, 0);
	if (Socket_Cliente == -1)
	{
		printf ("No se puede abrir el socket cliente\n");
		exit (-1);	
	}

		/*---------------------------------------------------------------------
		Necesitamos una estructura con la informaci\ufffdn del Servidor
		para poder solicitarle un servicio.
		----------------------------------------------------------------------*/
	Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr =  inet_addr(argv[1]);
	Longitud_Servidor = sizeof(Servidor);

  	int intentos = 0;
  	int recibido = 0;
	int opcion = 0;
 	do{
  	   	puts("1.- Para enviar DAY:");
  	   	puts("2.- Para enviar TIME:");
  	 	puts("3.- Para enviar DAYTIME:");
  	 	scanf("%d", &opcion);
  	}while(opcion < 1 || opcion > 3);
	if(opcion == 1){
		sprintf(mensaje, "DAY");
	}
	if(opcion == 2){
		sprintf(mensaje, "TIME");
	}
	if(opcion == 3){
		sprintf(mensaje, "DAYTIME");
	}

  	do{
  		printf("Enviado: %s\n", mensaje);
		int enviado = sendto (Socket_Cliente, (char *) &mensaje, sizeof(mensaje), 0,(struct sockaddr *) &Servidor, Longitud_Servidor);
  		FD_ZERO(&lectura);
		FD_SET(0,&lectura);
		salida = select(Socket_Cliente+1,&lectura,NULL,NULL,&timeout);

		if(salida == 0){
			printf("Se ha agotado el tiempo\n");
		}
		if(salida == -1){
			printf("Error al hacer el select\n");
			exit(-1);
		}
		recibido = recvfrom (Socket_Cliente, respuesta, sizeof(respuesta), 0, (struct sockaddr *) &Servidor, &Longitud_Servidor);
		intentos++;
  	}while(intentos < 3 && recibido < 1);
	

	/*----------------------------------------------------------------------
		Esperamos la respuesta del Servidor
		----------------------------------------------------------------------- */ 	
		if (recibido > 0)
  		printf ("Leido %s\n", respuesta);
		else
  		printf ("Error al leer del servidor\n");
	
	
	
	close(Socket_Cliente);
}