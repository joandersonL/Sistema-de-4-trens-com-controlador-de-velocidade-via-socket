
all:
	g++ -std=c++11 -c visualizador_ferrovia_2.cpp -I/home/joanderson/sfml/include -lpthread
	g++ -std=c++11 visualizador_ferrovia_2.o -o visualizador_ferrovia_2 -L/home/joanderson/sfml/lib -lpthread -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio
	export LD_LIBRARY_PATH=/home/joanderson/sfml/lib && ./visualizador_ferrovia_2&


	g++ -std=c++11 -c controle_velocidade_2.cpp -I/home/joanderson/sfml/include -lpthread
	g++ -std=c++11 controle_velocidade_2.o -o controle_velocidade_2 -L/home/joanderson/sfml/lib -lpthread -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio
	export LD_LIBRARY_PATH=/home/joanderson/sfml/lib && ./controle_velocidade_2&

run:
	echo "Makefile Executado!!"

clean:
	rm ./cliente.o
	rm ./cliente
