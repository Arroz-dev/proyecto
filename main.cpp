#include <SFML/Graphics.hpp>
#include <iostream>
#include <conio.h>

using namespace sf;
using namespace std;

int main(){
	RenderWindow window(VideoMode(800,600),"Mi primer intento",Style::Default);
	//window.setSize(Vector2u(840,650)); //cambiar el tamaño de la ventana
	//window.setTitle("Hello world"); //cambiar el titulo

	
	//bool focus = window.hasFocus();
	Vector2u size = window.getSize();
	window.setVerticalSyncEnabled(true); //para que que las imagenes no se desfracmenten (no importa mucho ponerlo)
	window.setFramerateLimit(60); //para control la velocidad del juego
	//jamas ocupar estos 2 de arriba al mismo tiempo, ocacionara una ambiguedad 
	CircleShape circulo(50);
	circulo.setFillColor(Color::Black);
	circulo.setPosition(500,400);
	
	RectangleShape rectangulo(Vector2f(40,100));
	rectangulo.setFillColor(Color::White);
	rectangulo.setPosition(100,400);
	
	
	Texture hoja_sprites;
	if(!hoja_sprites.loadFromFile("imagenes/wolfsheet1.png")){
		std::cout<<"Error al cargar el sprite"<<std::endl;
		return -1;
	}
	
	Sprite sprite;
	sprite.setTexture(hoja_sprites);
	int ancho = hoja_sprites.getSize().x / 4;
	int alto = hoja_sprites.getSize().y;
	int margen = 1;
	sprite.setOrigin(ancho/2,alto/2);
	sprite.setPosition(size.x / 2, size.y/2);
	//posicion 
	sprite.setPosition(300.f,200.f);
	//rotacion
	sprite.setRotation(70.f);
	Font font;
	if(!font.loadFromFile("PixelifySans-Bold.ttf")){
		cout<<"Error no se cargo la fuente "<<endl;
	}
	
	Text text;
	text.setFont(font);
	text.setString("Hola mundo");
	text.setCharacterSize(30);
	text.setFillColor(Color::White);
	text.setPosition(200.f,100.f);

	
	Text PlayerMessage;
	PlayerMessage.setFont(font);
	PlayerMessage.setCharacterSize(25);
	PlayerMessage.setFillColor(Color::Red);
	PlayerMessage.setPosition(300.f,200.f);
	bool mostrar_mensaje = false;
	
	Clock clock;
	
	float duracion = 0.2f;
	int currentFrame = 0;
	
	
	
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			if(event.type == Event::Closed){
				window.close();
			}
			
		}
		
		if(Keyboard::isKeyPressed(Keyboard::Space)){
			mostrar_mensaje = true;
			PlayerMessage.setString("el usuario a precionado una tecla!!!!");
			getch();
		}else{
			mostrar_mensaje = false;
		}
		
		if(clock.getElapsedTime().asSeconds() > duracion){
			currentFrame = (currentFrame + 1) % 4;
			sprite.setTextureRect(IntRect(currentFrame * ancho + margen,0,ancho - 2 * margen,alto));
			clock.restart();
		}
		
		
		window.clear(Color::Black);
		window.draw(sprite);
		window.draw(text);
		if(mostrar_mensaje){
			window.draw(PlayerMessage);
		}
		//window.draw(circulo);
		//window.draw(rectangulo);
		window.display();
	}
	
	return 0;
}
