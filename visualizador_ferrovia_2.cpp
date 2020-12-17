#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

int dim_x  = 36;
int dim_y  = 36;
int x_inicial = 166;
int y_inicial = 38;
int velocidades[4] = {1,1,1,1};
int VelocidadeMinima = 1000000;
int N1 = (VelocidadeMinima*((100.0 - ((velocidades[0]-1)*10.0))/100.0));
int N2 = (VelocidadeMinima*((100.0 - ((velocidades[1]-1)*10.0))/100.0));
int N3 = (VelocidadeMinima*((100.0 - ((velocidades[2]-1)*10.0))/100.0));
int N4 = (VelocidadeMinima*((100.0 - ((velocidades[3]-1)*10.0))/100.0));

float calcular_velocidade(int n){
  return (VelocidadeMinima*((100.0 - ((n-1)*10.0))/100.0));
}

//janela
sf::RenderWindow window(sf::VideoMode(800, 400), "Visualizador da Ferrovia");

struct Trem {
  sf::Texture textura;
  sf::Sprite sprite;
  void draw(sf::RenderWindow *window){
    window->draw(sprite);
  }
};

Trem trens[4];
//Variaveis do painel_trens
std::vector<sf::Sprite> quadrados_linha1;
sf::Texture trilho_horizontal;
sf::Texture trilho_vertical;

sf::Texture trem_verde;
sf::Texture trem_vermelho;
sf::Texture trem_azul;
sf::Texture trem_rosa;
sf::Sprite trem_1;
sf::Sprite trem_2;
sf::Sprite trem_3;
sf::Sprite trem_4;
sf::Image icon;


sem_t sem1;//semáforos L3           -> 1
sem_t sem2;//semáforos L5           -> 1
sem_t sem3;//semáforos L4           -> 1
sem_t sem4;//semáforos L6           -> 1
sem_t sem5;//semáforos L10          -> 1
sem_t sem6;//semáforos L3 L4 L6     -> 2
sem_t sem7;//semáforos L5 L6 L10    -> 2
sem_t sem8;//semáforos L3 L4 L5 L10 -> 3

void run_trem_1(sf::Sprite *trem);
void run_trem_2(sf::Sprite *trem);
void run_trem_3(sf::Sprite *trem);
void run_trem_4(sf::Sprite *trem);


sf::Thread thread1(&run_trem_1, &trens[0].sprite);	// create a thread
sf::Thread thread2(&run_trem_2, &trens[1].sprite);	// create a thread
sf::Thread thread3(&run_trem_3, &trens[2].sprite);	// create a thread
sf::Thread thread4(&run_trem_4, &trens[3].sprite);	// create a thread



void rotacao(sf::Sprite *trem,int x){
  switch (x) {
    case 1:
    trem->rotate(90);
    trem->move(0,-dim_y);
    break;
    case 2:
    trem->rotate(90);

    trem->move(dim_x,0);
    // trem->move(0,-dim_y);
    break;
    case 3:
    trem->rotate(90);
    trem->move(-dim_x,0);

    break;
    case 4:
    trem->rotate(90);
    trem->move(0,dim_y);
    break;
    case 5:
    trem->rotate(90);
    trem->move(dim_x,0);
    break;
    case 6:
    trem->rotate(90);
    trem->move(0,dim_y);

    break;
    case 7:
    trem->rotate(90);
    trem->move(-dim_x,0);

    break;
    case 8:
    trem->rotate(90);
    trem->move(0,-dim_y);
    break;



  }
}
void servidor();
sf::Thread thread5(&servidor);	// create a thread
void L1(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(0,-1);
      j++;
      usleep(*N/dim_x);
    }
    // trem->move(0,-dim_y);
    // usleep(*N);

    if(i==2)
    rotacao(trem,8);
  }

}
void L2(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j++;
      usleep(*N/dim_x);
    }
    // trem->move(dim_x,0);
    // usleep(*N);
  }
}
void L3(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==true){
    int j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j++;
      usleep(*N/dim_x);
    }
    rotacao(trem,5);

    // trem->move(dim_x,0);
    // usleep(*N);
    for(int i=0; i<3; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(0,1);
        j++;
        usleep(*N/dim_x);
      }
      // trem->move(0,dim_y);
      // usleep(*N);
    }
  }
  else{
    int j = 0;
    while(j<dim_x){
      trem->move(-1,0);
      j++;
      usleep(*N/dim_x);
    }
    // trem->move(-dim_x,0);
    rotacao(trem,7);
    // usleep(*N);
    // sem_post(&sem6);
    // sem_post(&sem4);
    // sem_post(&sem7);
    for(int i=0; i<4; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(0,-1.0);
        j++;
        usleep(*N/dim_x);
      }
      if(i==0){
        sem_post(&sem6);
        sem_post(&sem4);
        sem_post(&sem7);
        sem_post(&sem8);
      }
      // trem->move(0,-dim_y);
      if(i==3)
      rotacao(trem,8);
      // usleep(*N);
    }
    // sem_post(&sem1);
    // sem_post(&sem8);
    j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j++;
      usleep(*N/dim_x);
    }
    sem_post(&sem1);
    // sem_post(&sem8);
    // trem->move(dim_x,0);
    // usleep(*N);


  }
}
void L4(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false){
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(0,dim_y);
    rotacao(trem,6);
    // usleep(*N);

    for(int i=0; i<4; i++)
    {
      if(i==1){
        sem_post(&sem6);
        sem_post(&sem1);
        sem_post(&sem8);
      }
      int j = 0;
      while(j<dim_x){
        trem->move(-1,0);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      // trem->move(-dim_x,0);
      if(i==3)
      rotacao(trem,7);
      // usleep(*N);
    }
    j = 0;
    while(j<dim_x){
      trem->move(0,-1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    sem_post(&sem3);
    // trem->move(0,-dim_y);
    // usleep(*N);

  }
  else{
    int j = 0;
    while(j<dim_x){
      trem->move(0,-1.0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    rotacao(trem,1);

    // trem->move(0,-dim_y);
    // usleep(*N);
    for(int i=0; i<3; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(1,0);
        j++;
        usleep((*N/dim_x));
      }
      // trem->move(dim_x,0);
      // usleep(*N);
    }
  }
}
void L5(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==true)  {
    int j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(dim_x,0);
    // usleep(*N);
    rotacao(trem,5);
    for(int i=0; i<3; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(0,1);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      // trem->move(0,dim_y);
      // usleep(*N);
    }
  }
  else{
    int j = 0;
    while(j<dim_x){
      trem->move(-1,0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(-dim_x,0);
    // usleep(*N);
    rotacao(trem,7);

    for(int i=0; i<4; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(0,-1);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      if(i==0)
      sem_post(&sem5);
      // trem->move(0,-dim_y);
      // usleep(*N);
      if(i==3)
      rotacao(trem,8);
    }

    j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    sem_post(&sem8);
    sem_post(&sem7);
    sem_post(&sem2);
    // trem->move(dim_x,0);
    // usleep(*N);
  }
}
void L6(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false){
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    rotacao(trem,6);

    for(int i=0; i<3; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(-1,0);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      if(i==0){
        sem_post(&sem2);
      }
    }
  }
  else{
    for(int i=0; i<4; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(1,0);
        j++;
        usleep((*N/dim_x));
      }
      if(i==1){
        sem_post(&sem6);
        sem_post(&sem3);
      }
    }
  }
}
void L7(sf::Sprite *trem, int *N,bool flag){
  for(int i=0; i<2; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(dim_x,0);
    // usleep(*N);
    if(i==0  && flag){
      // sem_post(&sem1);
      // sem_post(&sem8);
    }
  }
}
void L8(sf::Sprite *trem, int *N,bool flag){
  for(int i=0; i<2; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(1,0);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(dim_x,0);
    // usleep(*N);
    if(i==0 && flag){
      // sem_post(&sem8);
      // sem_post(&sem7);
      // sem_post(&sem2);
    }
  }
  // rotacao(trem,5);
}
void L9(sf::Sprite *trem, int *N){
  // trem->move(dim_x,0);
  // usleep(*N);
  int j = 0;
  while(j<dim_x){
    trem->move(1,0);
    j+=1;
    usleep((*N/dim_x)*1);
  }
  rotacao(trem,5);
  for(int i=0; i<3; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(0,dim_y);
    // usleep(*N);
  }
}
void L10(sf::Sprite *trem, int *N,bool direcao){
  if(direcao==false){
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(0,dim_y);
    // usleep(*N);
    rotacao(trem,6);
    for(int i=0; i<3; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(-1,0);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      // trem->move(-dim_x,0);
      // usleep(*N);
    }
  }
  else{
    for(int i=0; i<5; i++)
    {
      int j = 0;
      while(j<dim_x){
        trem->move(1,0);
        j+=1;
        usleep((*N/dim_x)*1);
      }
      if(i==4){
        //rotacao(trem,2);
      }
      // trem->move(dim_x,0);
      // usleep(*N);
      if(i==1){
        sem_post(&sem4);
        sem_post(&sem8);
      }
    }
    rotacao(trem,2);
    // sem_post(&sem7);
    // sem_post(&sem5);
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j+=1;
      usleep((*N/dim_x)*1);
    }
    // trem->move(0,dim_y);
    // usleep(*N);
    sem_post(&sem7);
    sem_post(&sem5);

  }
}
void L11(sf::Sprite *trem, int *N){
  for(int i=0; i<3; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(0,-1);
      j++;
      usleep(*N/dim_x);
    }
    // trem->move(0,-dim_y);
    // usleep(*N);
  }
}
void L12(sf::Sprite *trem, int *N){

  for(int i=0; i<3; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(0,1);
      j++;
      usleep(*N/dim_x);
    }
    // trem->move(0,dim_y);
    // usleep(*N);
  }
  rotacao(trem,4);
}
void L13(sf::Sprite *trem, int *N){
  for(int i=0; i<12; i++)
  {
    int j = 0;
    while(j<dim_x){
      trem->move(-1,0);
      j++;
      usleep(*N/dim_x);
    }
    // /usleep(*N);
  }
  rotacao(trem,3);
}

void variaveis_painel_trens(){
  trilho_horizontal.loadFromFile("imagem/trilho_horizontal.png");
  trilho_vertical.loadFromFile("imagem/trilho_vertical.png");

  trens[0].textura.loadFromFile("imagem/trem_verde.png");
  trens[1].textura.loadFromFile("imagem/trem_rosa.png");
  trens[2].textura.loadFromFile("imagem/trem_vermelho.png");
  trens[3].textura.loadFromFile("imagem/trem_azul.png");
  for(int i=0;i<4;i++){
    trens[i].sprite.setTexture(trens[i].textura);
    trens[i].sprite.setPosition(x_inicial, y_inicial);
  }
  trens[1].sprite.move(dim_x*5,0);
  trens[2].sprite.move(dim_x*9,0);
  trens[3].sprite.move(0,dim_y*8);
  trens[3].sprite.rotate(270);
  trens[3].sprite.move(0,36);

  trem_verde.loadFromFile("imagem/trem_verde.png");
  trem_vermelho.loadFromFile("imagem/trem_vermelho.png");
  trem_azul.loadFromFile("imagem/trem_azul.png");
  trem_rosa.loadFromFile("imagem/trem_rosa.png");


  //Posicionando os trilhos
  for(int  j=0; j<3; j++){
    for (int i=0; i<13; i++)
    {
      sf::Sprite quadrado;
      quadrado.setTexture(trilho_horizontal);
      quadrado.setPosition(x_inicial+i*dim_x, y_inicial+j*dim_y*4);
      quadrados_linha1.push_back(quadrado);
    }
  }
  for(int  j=0; j<7; j++)  {
    if(j==3)
    {
      continue;
    }
    if(j<3)
    {
      for (int i=0; i<4; i++)
      {
        sf::Sprite quadrado;
        quadrado.setTexture(trilho_vertical);
        quadrado.setPosition(x_inicial+i*dim_x*4, y_inicial+dim_y+dim_y*j);
        quadrados_linha1.push_back(quadrado);
      }
    }
    else if(j>3)
    {
      for (int i=0; i<2; i++)
      {
        sf::Sprite quadrado;
        quadrado.setTexture(trilho_vertical);
        quadrado.setPosition(x_inicial+i*dim_x*4*3, y_inicial+dim_y+dim_y*j);
        quadrados_linha1.push_back(quadrado);
      }
    }
  }


  trem_1.setTexture(trem_verde);
  trem_1.setPosition(x_inicial, y_inicial);

  trem_2.setTexture(trem_rosa);
  trem_2.setPosition(x_inicial, y_inicial);
  trem_2.move(dim_x*5,0);

  trem_3.setTexture(trem_vermelho);
  trem_3.setPosition(x_inicial, y_inicial);
  trem_3.move(dim_x*9,0);

  trem_4.setTexture(trem_azul);
  trem_4.setPosition(x_inicial, y_inicial);
  trem_4.move(0,dim_y*8);
  trem_4.rotate(270);
  trem_4.move(0,36);

  //Ativiando as threads dos trens
}

void tela_trens(){
  for(int i=0; i<quadrados_linha1.size(); i++)
  {
    window.draw(quadrados_linha1[i]);
  }
  //Fim do desenho dos trilhos

  //Desenhando os trens
  // window.draw(trem_1);
  // window.draw(trem_2);
  // window.draw(trem_3);
  // window.draw(trem_4);
  for(int i=0;i<4;i++){
    trens[i].draw(&window);
  }
}

void janela(){

  while (window.isOpen()){

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
        thread1.terminate();
        thread2.terminate();
        thread3.terminate();
        thread4.terminate();
        thread5.terminate();
      }
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
      {
        window.close();
        thread1.terminate();
        thread2.terminate();
        thread3.terminate();
        thread4.terminate();
        thread5.terminate();
      }

    }
    window.clear();
    tela_trens();
    window.display();
  }
}

void inicializar_semaforos(){
  int res;
  res = sem_init(&sem1, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem2, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem3, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem4, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem5, 0, 1);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem6, 0, 2);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem7, 0, 2);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
  res = sem_init(&sem8, 0, 3);
  if (res < 0)
  {
    perror("Semaphore initialization failed");
    exit(EXIT_FAILURE);
  }
}

int main(){
  if (!icon.loadFromFile("imagem/icon.png")) {
    return EXIT_FAILURE;
  }
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  inicializar_semaforos();
  variaveis_painel_trens();
  thread1.launch();	// run it
  thread2.launch();	// run it
  thread3.launch();	// run it
  thread4.launch();	// run it
  thread5.launch();	// run it
  janela();

  return 0;
}

void run_trem_1(sf::Sprite *trem){
  while(1)
  {
    L2(trem, &N1);
    sem_wait(&sem8);
    sem_wait(&sem6);
    sem_wait(&sem1);

    L3(trem, &N1,true);
    sem_wait(&sem3);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem8);
    // sem_post(&sem1);
    L4(trem, &N1,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem3);
    L1(trem, &N1);
  }
}
void run_trem_2(sf::Sprite *trem) {
  bool flag = false;
  while(1)
  {
    L7(trem, &N2,flag);
    sem_wait(&sem8);
    sem_wait(&sem7);
    sem_wait(&sem2);
    flag = true;

    L5(trem, &N2,true);
    sem_wait(&sem6);
    sem_wait(&sem4);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem2);

    L6(trem, &N2,false);
    sem_wait(&sem1);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem4);
    // sem_post(&sem7);
    // sem_post(&sem8);
    L3(trem, &N2,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem1);

  }
}
void run_trem_3(sf::Sprite *trem){
  bool flag = false;
  while(1)
  {

    L8(trem, &N3 ,flag);
    flag = true;
    L9(trem, &N3);
    sem_wait(&sem8);
    sem_wait(&sem7);
    sem_wait(&sem5);


    L10(trem, &N3,false);
    sem_wait(&sem2);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem5);
    L5(trem, &N3,false);
    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem8);
    // sem_post(&sem7);
    // sem_post(&sem2);
  }
}
void run_trem_4(sf::Sprite *trem){
  while(1)
  {
    L11(trem, &N4);
    sem_wait(&sem8);
    sem_wait(&sem6);
    sem_wait(&sem3);

    L4(trem, &N4,true);
    sem_wait(&sem7);
    sem_wait(&sem4);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem6);
    // sem_post(&sem3);
    L6(trem, &N4,true);
    sem_wait(&sem5);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem4);
    // sem_post(&sem8);
    L10(trem, &N4,true);

    //As funçoes sem_post estão sendo chamadas quando o trem entra na Linha seguinte
    // sem_post(&sem7);
    // sem_post(&sem5);
    L12(trem, &N4);

    L13(trem, &N4);
  }
}



void servidor(){
  int server_sockfd, client_sockfd;
  unsigned int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);  // cria um novo socket

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_port = 500;

  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *) &server_address, server_len);

  listen(server_sockfd, 5);
  while(1) {
    int vetor[4];

    client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_len);

    while(true){
      read(client_sockfd, vetor, sizeof(vetor));
      for(int i=0;i<4;i++){
        velocidades[i] = vetor[i];
      }
      N1 = calcular_velocidade(velocidades[0]);
      N2 = calcular_velocidade(velocidades[1]);
      N3 = calcular_velocidade(velocidades[2]);
      N4 = calcular_velocidade(velocidades[3]);
    }
    close(client_sockfd);
  }
}
