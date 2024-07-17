#pragma once
#include "../core/Manager/EntityManager.hpp"
#include "../core/Physics/Vec2D.hpp"
#include <SFML/Graphics.hpp>

#define PARTICLE_COUNT 50
#define PARTICLE_SPEED 3
#define PARTICLE_VERTICES 4
#define PARTICLE_RADIUS 5

class Menu {
	
	sf::RenderWindow* _ctx;

	Core::Physics::Vec2D gen_random_particle_pos(sf::RenderWindow* ctx);


	void render_menu_text();

	void render_menu_particles();
	

	void init_menu_particles();

	void init_menu();
	void render_logo();
	

	void render_menu_buttons();



public:
	Menu( sf::RenderWindow* ctx);
	int GetMenuEvents() const;
	void Clean();
	void Render();

	~Menu();

};