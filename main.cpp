#include <SFML/Graphics.hpp>
#include <vector> //para acceder a las posiciones de los sprites de los personajes
#include <iostream>
#include <sstream>
using namespace std;
using namespace sf; //para no poner sf:: en los atributos de SFML

//etructura para los tipos de hechizos de la clase mago
struct Hechizos{
	string nombre;
	int poder;
	int costo_mana;
	Hechizo(string _nombre,int _poder,int _costo_mana) {}
};


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
		
		protected:
			int calcular_damage(int damage_base){
				return static_cast<int>(damage_base*(1.0 - static_cast<float>(defenza) / (defenza+k))); //teniendo en cuenta la defenza y el ataque;
			}

};

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
		
		void update(){
			if(animationClock.getElapsedTime().asSeconds() > frameDuration){
				currenFrame = (currenFrame + 1) % slimetexture.size();
				this->setTexture(slimetexture[currenFrame]);
				animationClock.restart();
			}
			this->setPosition(100.0f,400.0f);
		}
		

};

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
			
			void update_guerrero(){
				if(animationClock.getElapsedTime().asSeconds() > frameDuration){
					currenFrame =(currenFrame + 1) % guerrerotexture.size();
				}
			}
};

class Mago: public Personajes{
	public:
		vector<Texture> magotexture;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		int poder_magico = 40;
		Mago(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,int _poder_magico):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre), poder_magico(_poder_magico), magotexture(texture) {}
			
			
};

int main(){
	RenderWindow window(VideoMode(800,600),"RPG maker");
	
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
		
		window.clear(Color::Black);
		window.draw(slime);
		window.display();
	}
	
	return 0;
}
