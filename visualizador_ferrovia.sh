#!/bin/bash
g++ -std=c++11 -c visualizador_ferrovia_2.cpp -I/home/joanderson/sfml/include -lpthread
g++ -std=c++11 visualizador_ferrovia_2.o -o visualizador_ferrovia_2 -L/home/joanderson/sfml/lib -lpthread -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio
export LD_LIBRARY_PATH=/home/joanderson/sfml/lib && ./visualizador_ferrovia_2
