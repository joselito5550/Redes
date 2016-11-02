#ifndef __JUGADOR__
#define __JUGADOR__
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Jugador{

private:
  string name;
  string pass;
  int id;
  bool logueado;
  int puntuacion;

public:
  Jugador(){
    name = "";
    setId(-1);
    setLogueado(false);
  };
  ~Jugador(){};

  string getName()const{return name;};
  void setName(const string &name1){name = name1;};

  string getPass()const{return pass;};
  void setPass(const string &password){pass = password;};

  int getId()const{return id;};
  void setId(const int &id1){id = id1;};

  bool getLogueado()const{return logueado;};
  void setLogueado(const bool login){logueado = login;};

  int getPuntuacion()const{return puntuacion;};
  void setPuntuacion(const int &punt){puntuacion = punt;};
  void sumarPuntuacion(const int &punt){puntuacion += punt;};

  

};

#endif
