#ifndef RULETA_HPP
#define RULETA_HPP

#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include "jugador.hpp"
#include "partida.hpp"
#include <iostream>
#include <vector>
#include <ctime>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
//agregar en c++
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

#define MSG_SIZE 250
#define MAX_CLIENTS 40

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int &numJugadores, vector <Jugador> &arrayJugadores);
char * extraerCabecera(char buffer[]);
int buscarJugador(int id, const vector <Jugador> &j);
void eliminar_jugador_partida(int i,int &numPartidas, vector <Partida> &arrayPartidas, vector <Jugador> &arrayJugadores);
void eliminar_partida(int pos, vector <Partida> &arrayPartidas,int &numPartidas);
//void cantarEstado(int id,int estado,vector<Partida> &arrayPartidas,int &numPartidas);
int buscarPartida(int id, vector <Partida> p);
void enivar_mensaje_jugadores_partida(char buffer[],const Partida &p);
void registrar(int id, char buffer[],vector<Jugador> &jugadores);
void setUsuario(int id, char buffer[],vector<Jugador> &jugadores);
void loguear(int id, char buffer[],vector<Jugador> &jugadores);
string extraerFichero();
string ocultarOracion(const string oracion);

#endif
