#ifndef PARTIDA_HPP
#define PARTIDA_HPP
#include "jugador.hpp"
#include <vector>
#include <iostream>

using namespace std;

class Partida{

	private:
		vector <Jugador> jugadores_;
		int numJugadores_;
		bool comenzada_;
		bool finalizada_;

		vector<int> estado_;
	public:
		Partida(){
			setNumJugadores(0);
			setComenzada(false);
			setFinalizada(false);

			vector<int> estado={0,0,0,0}; //3 estado línea, doble línea, bingo. Guarda la última bola.
			setEstado(estado);
		};
		~Partida(){};

		vector<Jugador> getJugadores()const{return jugadores_;};
		void setJugadores(const vector<Jugador> &jugadores){jugadores_=jugadores;};

		int getNumJugadores()const{return numJugadores_;};
		void setNumJugadores(const int & numJugadores){numJugadores_=numJugadores;};
		bool isComenzada()const{return comenzada_;};
		void setComenzada(const bool & comenzada){comenzada_=comenzada;};
		bool isFinalizada()const{return finalizada_;};
		void setFinalizada(const bool & finalizada){finalizada_=finalizada;};

		vector <int> getEstado()const{return estado_;};
		void setEstado(const vector<int> &estado){estado_=estado;};

		bool nuevo_jugador(const Jugador &j);
		bool iniciar_partida();
		bool salir(const Jugador &j);
		int buscar_jugador(int id);

		

};

#endif
