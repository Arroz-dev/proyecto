#include <SFML/Graphics.hpp>
#include <vector> //para acceder a las posiciones de los sprites de los personajes
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <thread>
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
		int k = 23;
		RenderWindow* window;
		Font* font;
		Personajes(Texture& texture, int _vida, int _fuerza, int _defenza, int _velocidad, string _nombre, RenderWindow& _window, Font& _font)
        : window(&_window), font(&_font) {
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
		
			bool esquivar(){
				int probabilidadEsquivar = rand()%100;
				return probabilidadEsquivar < velocidad;
			}
		
		virtual void atacar(Personajes& objetivo) = 0;
		
		virtual void recibir_damage(int damage){
			
			Text text;
			text.setFont(*font);
			text.setCharacterSize(24);
			text.setFillColor(Color::White);
			text.setPosition(250,150);
			
			if(esquivar()){
				text.setString("Ataque esquivado con exito");
				window->draw(text);
				window->display();
				this_thread::sleep_for(chrono::seconds(1));
				
				return;
			}
			int damage_real = calcular_damage(damage);
			
			vida -=damage_real;
			if(vida < 0) vida = 0;
			
			text.setString("Recibido " + to_string(damage_real) + " puntos de daño");
			window->draw(text);
			window->display();
			
			this_thread::sleep_for(chrono::seconds(1));
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
		
		Slime(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font): 
    Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), slimetexture(texture) {}

		void atacar(Personajes& objetivo)override{
			objetivo.recibir_damage(25+rand()%35);
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
//tipo de espadas
struct TipoEspada{
	string tipo;
	int damage;
	TipoEspada(string _tipo,int _damage): tipo(_tipo), damage(_damage) {}
};

//animando a esta clase
class Guerrero: public Personajes{
	public:
		vector<Texture> guerrerotexture;
		vector<TipoEspada> espada;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		int indice_espada = 0;
		Guerrero(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), guerrerotexture(texture){
			espada.push_back(TipoEspada("Normal",14+rand() % 20));
			espada.push_back(TipoEspada("Rara",20+rand()%35));
			espada.push_back(TipoEspada("Lengendaria",45+rand()%80));
			}
			
			void atacar(Personajes& objetivo)override{
				int total_damage = fuerza + espada[indice_espada].damage;
				objetivo.recibir_damage(total_damage);
			}
			
			void espada_equipada(int indice){
				if(indice >= 0 && indice < espada.size()){
					indice_espada = indice;
				}
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
	Hechizos(string _nombre,int _poder): nombre(_nombre), poder(_poder){}
};

//animando al mago
class Mago: public Personajes{
	public:
		vector<Texture> magotexture;
		vector<Hechizos> magia;
		int currenFrame = 0;
		float frameDuration = 0.2f;
		Clock animationClock;
		int poder_magico;
		Mago(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), magotexture(texture){
			magia.push_back(Hechizos("Rayo",40+rand() % 65));
			magia.push_back(Hechizos("Fuego",20 + rand() % 35));
			magia.push_back(Hechizos("Agua",18 + rand() % 30));
			}
			
		
		void  atacar(Personajes& objetivo)override{
			int total_damage = magia[poder_magico].poder;
			objetivo.recibir_damage(total_damage);
		}
		
		void equipar_hechizo(int indice){
			if(indice >= 0 && indice < magia.size()){
				poder_magico = indice;
			}
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

struct TipoArco {
    string tipo;
    int damage_min;  // Daño mínimo
    int damage_max;  // Rango máximo

    TipoArco(string _tipo, int _damage_min, int _damage_max)
        : tipo(_tipo), damage_min(_damage_min), damage_max(_damage_max) {}
};

//animando al elfo
class Elfo: public Personajes{
	public:
		
		vector<Texture> elfotexture;
		vector<TipoArco> arco;
		Sprite flecha;
		Texture flechatexture1,flechatexture2;
		bool atacando = false;
		int currenFrame = 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		int arco_indice;
		float tiempoDeEspera = 1.0f;
		Sprite& getFlecha(){
			return flecha;
		}
		Clock tiempoAtaque;
		Elfo(vector<Texture>& texture, int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), elfotexture(texture){
			//vector<TipoArco>* arco = new vector<TipoArco>
			arco.push_back(TipoArco("Arco corto",21,26));
			arco.push_back(TipoArco("Arco largo",36,46));
			arco.push_back(TipoArco("Dead of dragon",55,70));
			}
		
	void atacar(Personajes& objetivo)override{
    if(tiempoAtaque.getElapsedTime().asSeconds() >= tiempoDeEspera){
        
        atacando = true;
        
        int random_damage = arco[arco_indice].damage_min + rand() % (arco[arco_indice].damage_max - arco[arco_indice].damage_min + 1);
        int total_damage = fuerza + random_damage;

        objetivo.recibir_damage(total_damage);

        tiempoAtaque.restart();  // Reiniciamos el reloj después del ataque
    }
}

		
		void equipar_arco(int indice){
			if(indice>= 0 && indice < arco.size()){
				arco_indice = indice;
			}
		}
		
		//animando el ataque
		void update_ataque_elfo(Personajes& objetivo){
    	if(atacando){
        // Mover la flecha hacia la derecha
        flecha.move(10.0f, 0.0f);

        // Verificar colisión con el objetivo
        if(flecha.getGlobalBounds().intersects(objetivo.getGlobalBounds())){
            int random_damage = arco[arco_indice].damage_min + rand() % (arco[arco_indice].damage_max - arco[arco_indice].damage_min + 1);
            objetivo.recibir_damage(fuerza + random_damage);
            
            this_thread::sleep_for(chrono::milliseconds(2));
            atacando = false;  // Finalizar ataque
        }

        // Si la flecha sale de la pantalla, finalizar ataque
        if(flecha.getPosition().x > 800){
            atacando = false;
            flecha.setPosition(-100, -100);  // Ocultar flecha
        }
    }
}


		//animando los sprites del elfo
		void update_Elfo(){
			if(animationClock.getElapsedTime().asSeconds() > animationDuration){
				currenFrame = (currenFrame + 1) % elfotexture.size();
				this->setTexture(elfotexture[currenFrame]);
				animationClock.restart();
			}
			this->setPosition(750.0f,400.0f);
		}
		
};


//animando al esqueleto
class Esqueleto: public Personajes{
	public:
		vector<Texture> esqueletotexture;
		int currenFrame= 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Esqueleto(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), esqueletotexture(texture) {}
			
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

//animando el siclope
class Siclope: public Personajes{
	public:
		vector<Texture> siclopetexture;
		int currenFrame = 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Siclope(vector<Texture>& texture,int _vida,int _fuerza,int _defensa,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defensa,_velocidad,_nombre,_window,_font), siclopetexture(texture) {}

};
//animando al minotauro
class Minotauro: public Personajes{
	public:
		vector<Texture> minotaurotexture;
		int currenFrame = 0;
		float animationDuration = 0.2f;
		Clock animationClock;
		Minotauro(vector<Texture>& texture,int _vida,int _fuerza,int _defenza,int _velocidad,string _nombre,RenderWindow& _window,Font& _font):
			Personajes(texture[0],_vida,_fuerza,_defenza,_velocidad,_nombre,_window,_font), minotaurotexture(texture) {}
			
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
//esta demas?
void cargarTexturasFlecha(vector<Texture>& flechatexture){
	for(int i=0;i<2;i++){
		ostringstream ss;
		ss<<(i+1);
		string flechaFilname = "Sprite1/Flecha"+ss.str()+".png";
		if(!flechatexture[i].loadFromFile(flechaFilname)){
			cout<<"Error al cargar la flecha";
		}
		
	}
}


int main(){
    srand(time(0));
    RenderWindow window(VideoMode(800, 600), "The war vs demon kings");

    // Cargando texturas del slime
    vector<Texture> slimetexture(3);
    for (int i = 0; i < 3; i++) {
        ostringstream ss;
        ss << (i + 1);
        string slimeFilename = "Sprite2/slime" + ss.str() + ".png";
        if (!slimetexture[i].loadFromFile(slimeFilename)) {
            cout << "Error al cargar la textura del slime";
        }
    }

    // Cargando texturas del elfo
    vector<Texture> elfotexture(2);
    for (int i = 0; i < 2; i++) {
        ostringstream ss;
        ss << (i + 1);
        string elfoFilename = "Sprite1/elf" + ss.str() + ".png";
        if (!elfotexture[i].loadFromFile(elfoFilename)) {
            cout << "Error al cargar la textura del elfo";
        }
    }
    
    vector<Texture> flechatexture(2);
    cargarTexturasFlecha(flechatexture);
    
    Texture fondotexture;
    if(!fondotexture.loadFromFile("imagenes/baatalla_slime.jpg")){
    	cout<<"Error al cargar el fondo";
	}
	
	Sprite fondo;
	fondo.setTexture(fondotexture);
	
	Vector2u windowSize = window.getSize();
	Vector2u fondoSize = fondotexture.getSize();
	
	float scalex = static_cast<float>(windowSize.x) / fondoSize.x;
	float scaley = static_cast<float>(windowSize.y) / fondoSize.y;
	
	fondo.setScale(scalex,scaley);
    
    

    // Cargando la fuente para los textos
    Font font;
    if(!font.loadFromFile("fuente.ttf")){ 
        cout << "Error al cargar la fuente";
        return -1;
    }
    
        // Crear personajes
    Slime slime(slimetexture, 100, 200, 5, 30, "Slime",window,font);
    Elfo elfo(elfotexture, 100, 10, 8, 15, "Elfo",window,font);

    // Configurar texto para mostrar la vida del Slime
    Text slimeLifeText;
    slimeLifeText.setFont(font);
    slimeLifeText.setCharacterSize(24); // Tamaño del texto
    slimeLifeText.setFillColor(Color::White); // Color del texto
    slimeLifeText.setPosition(100, 400); // Posición del texto

    // Configurar texto para mostrar la vida del Elfo
    Text elfoLifeText;
    elfoLifeText.setFont(font);
    elfoLifeText.setCharacterSize(24);
    elfoLifeText.setFillColor(Color::White);
    elfoLifeText.setPosition(600, 300);
    
    slime.setScale(0.5f,0.5f);
    elfo.setScale(-0.5f,0.5f);
    bool ataqueEjecutado = false;
    
    elfo.equipar_arco(0);
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed)
                window.close();
        }
        
        if(Keyboard::isKeyPressed(Keyboard::Return)){
        	if(!ataqueEjecutado){
        		elfo.atacar(slime);
        		ataqueEjecutado = true;
			}
		}else{
			ataqueEjecutado = false;
		}

        // Actualizar animaciones de los personajes
        slime.update();
        elfo.update_Elfo();
        elfo.update_ataque_elfo(slime);

        // Actualizar el texto de la vida de los personajes
        slimeLifeText.setString("Slime Vida: " + to_string(slime.mostrar_vida()));
        elfoLifeText.setString("Elfo Vida: " + to_string(elfo.mostrar_vida()));

        // Dibujar todo
        window.clear();
        window.draw(fondo);
        window.draw(slime);
        window.draw(elfo);
        window.draw(elfo.getFlecha());
        window.draw(slimeLifeText);
        window.draw(elfoLifeText);
        window.display();
    }

    return 0;
}
