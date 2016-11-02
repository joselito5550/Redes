//#include "carton.hpp"
#ifndef JUGADOR_HPP
#define JUGADOR_HPP
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Jugador{

	private:
		string nombre_;
		string password_;
		int id_;
		bool logueado_;
		int puntuacion;
		
	public:
		Jugador(){};
		Jugador(int id=-1,string nombre=""){
			nombre_=nombre;
			setId(id);
			setLogueado(false);
			puntuacion=0;
		};
		~Jugador(){};

		Jugador & operator=(const Jugador &j){
			if (this != &j){
				setNombre(j.getNombre());
				setPassword(j.getPassword());
				setId(j.getId());
				setPuntuacion(j.getPuntuacion());
			}
			return *this;
		};
		string getNombre()const{return nombre_;};
		void setNombre(const string &nombre){nombre_=nombre;};
		string getPassword()const{return password_;};
		void setPassword(const string &password){password_=password;};
		int getId()const{return id_;};
		void setId(const int &id){id_=id;};
		int getPuntuacion()const{return puntuacion;}
		void setPuntuacion(const int &puntuacion_){puntuacion=puntuacion_;};
		bool isLogueado()const{return logueado_;};
		void setLogueado(const bool &logueado){logueado_=logueado;};
		bool loguear(string registro){
			ifstream f(registro);
			if(f){
				string usuario;
				string pass;
				while(f>>usuario){
					f>>pass;
					if((usuario==getNombre())&&(pass==getPassword())){
						setLogueado(true);
						f.close();
						return true;
					}
				}
			}
			return false;
		}
		bool isRegistrado(string registro){
			ifstream f(registro);
			if(f){
				string usuario;
				string pass;
				while(f>>usuario){
					f>>pass;
					if(usuario==getNombre()){
						f.close();
						return true;
					}
				}
			}
			return false;
		};
		bool registrar(string registro){
			ofstream f (registro, ios::app);
			if(f && (not isRegistrado(registro))){
				f<<getNombre()<<" "<<getPassword()<<" "<<endl;
				f.close();
				setLogueado(true);
				return true;
			}
			return false;
		};
};
#endif
