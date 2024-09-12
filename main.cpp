#include <SFML/Graphics.hpp>
#include <vector> //para acceder a las posiciones de los sprites de los personajes
#include <iostream>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace sf; //para no poner sf:: en los atributos de SFML


//clase base donde estaran los atributos generales de los personajes
class Personajes: public Sprite{
	public:
		int vida = 100;
		int fuerza = 12;
		int defenza = 5;
		int velocidad = 10;
		string nombre;
		int k = 40;
		Personajes(Texture& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre){
			this->setTexture(texture);
			vida = _vida;
			fuerza = _fuerza;
			defenza = _defenza;
			velocidad = _velocidad;
			nombre = _nombre;
		}
		
		int mostrar_vida(){
			return vida;
		}
		
		string mostrar_nombre(){
			return nombre;
		}
		
		virtual void atacar(Personajes& objetivo) = 0;
		
		virtual void recibir_damage(int damage){
			int damage_real = calcular_damage(damage);
			
			vida -=damage_real;
			if(vida < 0) vida = 0;
		}
		//atacar teniendo en cuenta la defenza del oponente
		protected:
			int calcular_damage(int damage_base){
				return static_cast<int>(damage_base*(1.0 - static_cast<float>(defenza) / (defenza+k))); 
			}

};

//clase slime donde se animara sus sprites
class Slime: public Personajes{
	public:
		vector<Texture> slimetexture;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		Slime(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre): 
		Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), slimetexture(texture) {}
		void atacar(Personajes& objetivo)override{
			objetivo.recibir_damage(fuerza);
		}
		//animacion de los sprites
		void update(){
			if(animationClock.getElapsedTime().asSeconds() > frameDuration){
				currenFrame = (currenFrame + 1) % slimetexture.size();
				this->setTexture(slimetexture[currenFrame]);
				animationClock.restart();
			}
			//posicion del slime
			this->setPosition(100.0f,400.0f);
		}
		

};
//animando a esta clase
class Guerrero: public Personajes{
	public:
		vector<Texture> guerrerotexture;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		Guerrero(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), guerrerotexture(texture){}
			
			void atacar(Personajes& objetivo)override{
				objetivo.recibir_damage(fuerza);
			}
			//con esta funcion se animaran sus sprites
			void update_guerrero(){
				if(animationClock.getElapsedTime().asSeconds() > frameDuration){
					currenFrame =(currenFrame + 1) % guerrerotexture.size();
					this->setTexture(guerrerotexture[currenFrame]);
					animationClock.restart();
				}
			}
};


//etructura para los tipos de hechizos de la clase mago
struct Hechizos{
	string nombre;
	int poder;
	Hechizo(string _nombre,int _poder) {}
};

//animando al mago
class Mago: public Personajes{
	public:
		vector<Texture> magotexture;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		int poder_magico = 40;
		Mago(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,int _poder_magico):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), poder_magico(_poder_magico), magotexture(texture) {}
			
		
		void  atacar(Personajes& objetivo)override{
			objetivo.recibir_damage(poder_magico);
		}
		//funcion para animar los sprites
		void update_Mago(){
			if(animationClock.getElapsedTime().asSeconds() > frameDuration){
				currenFrame = (currenFrame + 1) % magotexture.size();
				this->setTexture(magotexture[currenFrame]);
				animationClock.restart();
			}
		}
		
};
//animando al elfo
class Elfo: public Personajes{
	public:
		vector<Texture> elfotexture;
		int currenFrame = 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Elfo(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), elfotexture(texture) {}
		
		void atacar(Personajes& objetivo)override{
			objetivo.recibir_damage(40);
		}
		//animando los sprites del elfo
		void update_Elfo(){
			if(animationClock.getElapsedTime().asSeconds() > animationDuration){
				currenFrame = (currenFrame + 1) % elfotexture.size();
				this->setTexture(elfotexture[currenFrame]);
				animationClock.restart();
			}
		}
};

//animando al esqueleto
class Esqueleto: public Personajes{
	public:
		vector<Texture> esqueletotexture;
		int currenFrame= 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Esqueleto(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), esqueletotexture(texture) {}
			
			void atacar(Personajes& objetivo)override{
				objetivo.recibir_damage(40);
				//probabilidad de atacar 2 veces
				if(rand() % 100 < 30){
					objetivo.recibir_damage(40);
				}
			}
			//animando los sprites del esqueleto
			void update_Esqueleto(){
				if(animationClock.getElapsedTime().asSeconds() > animationDuration){
					currenFrame = (currenFrame + 1) % esqueletotexture.size();
					this->setTexture(esqueletotexture[currenFrame]);
					animationClock.restart();
				}
			}
		
};
//animando al minotauro
class Minotauro: public Personajes{
	public:
		vector<Texture> minotaurotexture;
		int currenFrame = 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Minotauro(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), minotaurotexture(texture) {}
			
		void atacar(Personajes& objetivo)override{
			//verificar si hace un ataque critico
			if(rand() % 100 < 15){
				objetivo.recibir_damage(60);
			}
			//si no es la probabilidad, ara un ataque normal
			objetivo.recibir_damage(30);
		}
		//animando los sprites de el minotauro	
		void update_Minotauro(){
			if(animationClock.getElapsedTime().asSeconds() > animationDuration){
				currenFrame = (currenFrame + 1) % minotaurotexture.size();
				this->setTexture(minotaurotexture[currenFrame]);
				animationClock.restart();
			}
		}
};



int main(){
	srand(time(0));
	RenderWindow window(VideoMode(800,600),"The war vs demon kings");
	
	vector<Texture> slimetexture(4); //5 imagenes del slime;
	for(int i=0;i<4;i++){
		ostringstream ss;
		ss<<(i+1);
		string slimeFilename = "Sprite2/slime" + ss.str()+ ".jpg";
		if(!slimetexture[i].loadFromFile(slimeFilename)){
			cout<<"error al cargar la textura";
			return -1;
		}
	}
	Texture texture;
	if(!texture.loadFromFile("Sprite2/prueva2.png")){
		return -1;
	}
	
	Sprite sprite;
	sprite.setTexture(texture);
	
	Slime slime(slimetexture,100,12,14,12,"Slime");
	slime.setScale(0.5f,0.5f);
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			if(event.type == Event::Closed){
				window.close();
			}
		}
		
		slime.update();
		
		window.clear(Color::White);
		window.draw(sprite);
		window.draw(slime);
		window.display();
	}
	
	return 0;
}
