#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>


using namespace std;
void cliente();

int velocidades[4] = {1,1,1,1};
sf::Cursor cursor[10];
sf::Image icon;

sf::Thread thread5(&cliente);	// create a thread


struct Info_velocidae{
  sf::Texture textura[2];
  sf::Sprite sprite[2];
  sf::RectangleShape retangulo;
  sf::Text velocidade;//(to_string(velocidades[0]), font);
  sf::Text titulo; //("TREM 1", font);

  void draw(sf::RenderWindow *window){
    window->draw(sprite[0]);
    window->draw(sprite[1]);
    window->draw(retangulo);
    window->draw(velocidade);
    window->draw(titulo);
  }
};

sf::Color cor(int i){
  switch (i) {
    case 0:
    return (sf::Color::Green);
    case 1:
    return (sf::Color::Magenta);
    case 2:
    return (sf::Color::Red);
    case 3:
    return (sf::Color::Blue);


  }
  return sf::Color::Black;
}

sf::RenderWindow window(sf::VideoMode(800, 200), "Painel de controle de Velocidade");
sf::Font font;
Info_velocidae info_v[4];

sf::RectangleShape rectangle;
sf::Text text("Painel de Controle de Velocidade", font);

bool mudarCursor(sf::Vector2i position,int x,int y,int l,int h){
  if(position.x>=x && position.x<=x+l){
    if(position.y>=y && position.y<=y+h){
      return true;
    }
  }
  return false;
}
bool mudar_cursor = false;
int cursor_x=-1;
// int cursor_x2=-1;
void controle_velocidade(){
  sf::Vector2i position = sf::Mouse::getPosition(window);
  for(int i=0;i<4;i++){
    mudar_cursor = mudar_cursor || mudarCursor(position,179 + 133*i,87+30,70,30);

    mudar_cursor = mudar_cursor || mudarCursor(position,179 + 133*i,87-30,70,30);
    if(mudar_cursor){
      break;
    }
  }
  if(mudar_cursor){
    window.setMouseCursor(cursor[0]);

  }else{
    window.setMouseCursor(cursor[1]);
  }
  mudar_cursor = false;

  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
    for(int i=0;i<4;i++){
      if(position.x>=(179 + 133*i) && position.x<=((179 + 133*i)+70)){
        if(position.y>=87+30 && position.y<=87+30+30 ){
          if(velocidades[i]<10){
            velocidades[i]++;
          }
          info_v[i].velocidade.setString(to_string(velocidades[i]));
          break;
        }
        //Diminuir  nivel Velocidade do trem_1
        else if(position.y>=87-30 && position.y<=87){
          if(velocidades[i]>1){
            velocidades[i]--;
          }
          info_v[i].velocidade.setString(to_string(velocidades[i]));
          break;
        }

      }
    }
  }
}

void variaveis_painel_velocidade(){
  for(int i=0;i<4;i++){
    info_v[i].textura[0].loadFromFile("imagem/arrow-up.png");
    info_v[i].textura[1].loadFromFile("imagem/arrow-down.png");

    info_v[i].sprite[0].setTexture(info_v[i].textura[0]);
    info_v[i].sprite[0].setPosition(199 + i*133,120);

    info_v[i].sprite[1].setTexture(info_v[i].textura[1]);
    info_v[i].sprite[1].setPosition(199 + i*132.67,50);

    info_v[i].retangulo.setSize(sf::Vector2f(70, 28));
    info_v[i].retangulo.setOutlineColor(sf::Color::Red);
    info_v[i].retangulo.setOutlineThickness(1);
    info_v[i].retangulo.setPosition(179 + i*133,87);

    info_v[i].velocidade.setCharacterSize(25);
    info_v[i].velocidade.setStyle(sf::Text::Bold);
    info_v[i].velocidade.setFillColor(sf::Color::Black);
    info_v[i].velocidade.setPosition(204+i*133,87);
    info_v[i].velocidade.setFont(font);
    info_v[i].velocidade.setString(to_string(velocidades[i]));

    info_v[i].titulo.setCharacterSize(15);
    info_v[i].titulo.setStyle(sf::Text::Bold);
    info_v[i].titulo.setFillColor(cor(i));
    info_v[i].titulo.setPosition(185+i*134, 160);
    info_v[i].titulo.setFont(font);
    info_v[i].titulo.setString("Trem " + to_string(i+1));


    // (to_string(velocidades[0]), font);
    // sf::Text titulo; //("TREM 1", font);
  }
  rectangle.setSize(sf::Vector2f(796, 198));
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(2, 400-400);

  //TEXTO
  text.setCharacterSize(25);
  text.setStyle(sf::Text::Bold);
  text.setFillColor(sf::Color::Black);
  text.setPosition(211, 400-400);
  // Fim do painel de Controle

}
void tela_velocidades(){
  window.draw(rectangle);
  window.draw(text);
  for(int i=0;i<4;i++){
    info_v[i].draw(&window);
  }
}
void janela(){
  if (! font. loadFromFile ( "arial.ttf" ))
  {
    // erro ...
  }
  if (cursor[0].loadFromSystem(sf::Cursor::SizeAll))
  window.setMouseCursor(cursor[0]);
  if (cursor[1].loadFromSystem(sf::Cursor::Arrow))
  window.setMouseCursor(cursor[1]);
  // if (cursor[2].loadFromSystem(sf::Cursor::NotAllowed))
  // window.setMouseCursor(cursor[2]);

  while (window.isOpen()){
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
        thread5.terminate();
      }
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        window.close();
        thread5.terminate();
      }
      if(event.type == sf::Event::Resized){
        // sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
        if(event.size.width!=800 ||event.size.height!=200 )
        window.create(sf::VideoMode(800, 200), "Painel de controle de Velocidade");
        // window.setPosition(0,0);
      }
      controle_velocidade();
    }
    window.clear();
    tela_velocidades();
    window.display();
  }
}

int main(){
  if (!icon.loadFromFile("imagem/icon_2.png")) {
    return EXIT_FAILURE;
  }
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


  variaveis_painel_velocidade();
  thread5.launch();	// run it
  janela();
  return 0;
}

void cliente(){
  int sockfd;
  int len;
  struct sockaddr_in address;
  int result;
  //while(true){
  sockfd  = socket(AF_INET, SOCK_STREAM,0);  // criacao do socket

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr("127.0.0.1");
  address.sin_port = 500;

  len = sizeof(address);

  result = connect(sockfd, (struct sockaddr *) &address, len);

  if (result == -1){
    perror("Houve erro no cliente");
    exit(1);
  }

  while(true){
    send(sockfd, velocidades,sizeof(velocidades),0);
  }
  close(sockfd);
  //}
}
