#!/bin/bash

g++ -std=c++11 -c comtrole_velocidade_2.cpp -I/home/joanderson/sfml/include -lpthread
g++ -std=c++11 comtrole_velocidade_2.o -o comtrole_velocidade_2 -L/home/joanderson/sfml/lib -lpthread -lsfml-graphics -lsfml-window -lsfml-system  -lsfml-audio
export LD_LIBRARY_PATH=/home/joanderson/sfml/lib && ./comtrole_velocidade_2
