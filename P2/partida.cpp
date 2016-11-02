#include "partida.hpp"

bool Partida::salir(const Jugador &j){
	if(getNumJugadores()!=0){
		int encontrado=buscar_jugador(j.getId());	
		vector<Jugador> v=getJugadores();

		if(encontrado!=-1)
			v.erase(v.begin()+encontrado);
		else
			return false;
		setJugadores(v);
		setNumJugadores(getNumJugadores()-1);
		if(getNumJugadores()==0)
			setFinalizada(true);
		return true;
	}	
return false;
}

int Partida::buscar_jugador(int id){
	for(unsigned int i=0;i<getJugadores().size();i++){
		if(getJugadores()[i].getId()==id)
			return i;
	}
return -1;
}
bool Partida::nuevo_jugador(const Jugador &j){
	if((getNumJugadores()<4) and (not isComenzada())){
		vector<Jugador> v=getJugadores();
		v.push_back(j);
		setJugadores(v);
		setNumJugadores(getNumJugadores()+1);
		return true;
	}

return false;
};

bool Partida::iniciar_partida(){
	if((not isComenzada()) && (getNumJugadores()==4)){
		vector<Jugador> v=getJugadores();
		/*for(int i=0;i<4;i++){
			//Carton c;
			//c.generar_carton();
			//v[i].setCarton(c);
		}*/


		setJugadores(v);
		setComenzada(true);
		return true;
	}
return false;
};



