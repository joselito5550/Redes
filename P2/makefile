CXXFLAGS = -Wall -g -std=c++11
ejecutables: servidor.o ruleta.o ruleta.hpp jugador.hpp partida.o partida.hpp cliente.o
	g++ $(CXXFLAGS) servidor.o ruleta.o partida.o -o servidor
	g++ $(CXXFLAGS) cliente.o -o cliente

cliente.o: cliente.cpp 
	g++ -c $(CXXFLAGS) cliente.cpp

ruleta.o: ruleta.cpp ruleta.hpp partida.hpp jugador.hpp
	g++ -c $(CXXFLAGS) ruleta.cpp

partida.o: partida.cpp partida.hpp jugador.hpp 
	g++ -c $(CXXFLAGS) partida.cpp

clean: 
	rm *.o servidor cliente
