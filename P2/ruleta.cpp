#include "ruleta.hpp"
#include <fstream>

void loguear(int id, char buffer[],vector<Jugador> &jugadores){
	char passwd[50];
	int i,j;

		for(i=9,j=0;(buffer[i]!='\0');i++,j++){
			passwd[j]=buffer[i];
		}
		passwd[j-1]='\0';

	jugadores[buscarJugador(id,jugadores)].setPassword(passwd);

	sprintf(buffer,"-Err. Error en la validación\n");
	if(jugadores[buscarJugador(id,jugadores)].getNombre()!=""){
		if(jugadores[buscarJugador(id,jugadores)].loguear("registro.txt"))
			sprintf(buffer,"+Ok. Usuario validado\n");	
	}
	
	send(id,buffer,strlen(buffer),0);	
}

void setUsuario(int id, char buffer[],vector<Jugador> &jugadores){
	char usuario[50];
	int i,j;

		for(i=8,j=0;(buffer[i]!='\0');i++,j++){
			usuario[j]=buffer[i];
		}
		usuario[j-1]='\0';

	jugadores[buscarJugador(id,jugadores)].setNombre(usuario);

	if(jugadores[buscarJugador(id,jugadores)].isRegistrado("registro.txt"))
		sprintf(buffer,"+Ok. Usuario correcto\n");	
	else
		sprintf(buffer,"-Err. Usuario incorrecto\n");	

	send(id,buffer,strlen(buffer),0);	
}
void registrar(int id, char buffer[],vector<Jugador> &jugadores){

	char usuario[50];
	char passwd[50];
	int i,j;

	for(i=12,j=0;(buffer[i]!='\0')&&(buffer[i]!='-');i++,j++){
		usuario[j]=buffer[i];
	}
	usuario[j-1]='\0';
	for(i=i+3,j=0;(buffer[i]!='\0')&&(buffer[i]!='-');i++,j++){
		passwd[j]=buffer[i];
	}
	passwd[j-1]='\0';

	if(strlen(usuario)>1 && strlen(passwd)>1){
		jugadores[buscarJugador(id,jugadores)].setNombre(usuario);
		jugadores[buscarJugador(id,jugadores)].setPassword(passwd);
		if(jugadores[buscarJugador(id,jugadores)].registrar("registro.txt"))
			sprintf(buffer,"+Ok. Usuario <%s> registrado\n",usuario);	
		else
			sprintf(buffer,"-Err. El usuario <%s> ya existe\n",usuario);
	}
	else
			sprintf(buffer,"-Err. Formato incorrecto\n");	
	
	send(id,buffer,strlen(buffer),0);	
}

void enivar_mensaje_jugadores_partida(char buffer[],const Partida &p){
	vector<Jugador> jugadores=p.getJugadores();
	for(unsigned int j=0;j<jugadores.size();j++){
		send(jugadores[j].getId(),buffer,strlen(buffer),0);
	}
}
int buscarPartida(int id, vector <Partida> p){
	for(unsigned int i=0;i<p.size();i++){
		if(p[i].buscar_jugador(id)!=-1)
			return i;
	}
return -1;
}

/*void cantarEstado(int id,int estado,vector<Partida> &arrayPartidas,int &numPartidas){
	char buffer[MSG_SIZE];
	char aux[MSG_SIZE];
	int partida=-1;
	int salida=0;
	int posJ;

	partida=buscarPartida(id,arrayPartidas);	
	if((partida!=-1)){
		posJ=arrayPartidas[partida].buscar_jugador(id);
		Jugador j=arrayPartidas[partida].getJugadores()[posJ];
		salida=arrayPartidas[partida].cantarEstado(j,estado);		
	}

	switch(estado){
		case 0:{
			if(salida==1)
				strcpy(aux,"una línea");
			else
				strcpy(aux,"UNA-LINEA");
		break;}
		case 1:{
			if(salida==1)
				strcpy(aux,"dos líneas");
			else
				strcpy(aux,"DOS-LINEAS");
		break;}	
		case 2:{
			if(salida==1)
				strcpy(aux,"bingo");
			else
				strcpy(aux,"BINGO");
		break;}
	}

	if(salida==1){
		bzero(buffer, sizeof(buffer));
		sprintf(buffer,"+Ok. Jugador <%s> ha cantado %s\n",arrayPartidas[partida].getJugadores()[posJ].getNombre().c_str(),aux);
		enivar_mensaje_jugadores_partida(buffer,arrayPartidas[partida]);
		cout<<buffer;
	}
	else if(salida==0){
		bzero(buffer, sizeof(buffer));
		sprintf(buffer,"-Err. El comando %s no procede\n",aux);
		send(id,buffer,strlen(buffer),0);
	}
	else{
		bzero(buffer, sizeof(buffer));
		sprintf(buffer,"-Err. Su cartón no satisface <%s> con los números actuales\n",aux);
		send(id,buffer,strlen(buffer),0);
	}
}*/

void eliminar_partida(int pos, vector <Partida> &arrayPartidas,int &numPartidas){
	char buffer[MSG_SIZE];	
	sprintf(buffer,"+Ok. Partida finalizada\n");
	enivar_mensaje_jugadores_partida(buffer,arrayPartidas[pos]);
	//Re-estructurar el array de partidas
	arrayPartidas.erase(arrayPartidas.begin()+pos);
	numPartidas--;
	cout<<"La partida "<<pos+1<<" ha finalizado."<<endl;
}

void eliminar_jugador_partida(int i,int &numPartidas, vector <Partida> &arrayPartidas, vector <Jugador> &arrayJugadores){
	char buffer[MSG_SIZE];
	int partida=-1;
	for(int j=0;j<numPartidas;j++){											
		if(arrayPartidas[j].buscar_jugador(i)!=-1)
			partida=j;
	}
	if(partida!=-1){
		if(arrayPartidas[partida].salir(arrayJugadores[buscarJugador(i,arrayJugadores)])){
			strcpy(buffer,"+Ok. Ha salido de la partida\n");
			send(i,buffer,strlen(buffer),0);

			sprintf(buffer,"+Ok. Usuario <%s> ha abandonado la partida\n",arrayJugadores[buscarJugador(i,arrayJugadores)].getNombre().c_str());
			enivar_mensaje_jugadores_partida(buffer,arrayPartidas[partida]);
			cout<<buffer;

			if(arrayPartidas[partida].getNumJugadores()==0){//Eliminamos la partida si se ha quedado vacía
				eliminar_partida(partida,arrayPartidas,numPartidas);
			}
		}
	}
	else{
		strcpy(buffer,"-Err. Usted no ha iniciado aún ninguna partida\n");
		send(i,buffer,strlen(buffer),0);
	}
}

int buscarJugador(int id, const vector <Jugador> &j){
for(unsigned int i=0;i<j.size();i++){
	if(j[i].getId()==id)
		return i;
}
return -1;
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

void salirCliente(int socket, fd_set * readfds, int &numJugadores, vector <Jugador> &arrayJugadores){
    close(socket);
    FD_CLR(socket,readfds);
    
	//Re-estructurar el array de clientes
	arrayJugadores.erase(arrayJugadores.begin()+buscarJugador(socket,arrayJugadores));
    
    numJugadores--;
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}


//HAY QUE COGERLA ALEATORIAMENTE
string extraerFichero(){
	ifstream F("frases.txt");
	
	//string linea;
	char linea[128];
		//while(!F.eof()){
			F.getline(linea,128);
		//}
	
	F.close();
	return linea;
};


string ocultarOracion(const string oracion){
	string aux;
	
	for(int i=9;i<int(oracion.length());i++){
		if(oracion[i]!=' '){
			aux+='-';
		}else{
			aux+=' ';
		}
	}
	//cout<<"Aux: "<<aux<<endl;
	//cout<<oracion.length()<<endl;
	return aux;
}

