#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../core/Manager/EntityManager.hpp"
#include "SceneManager.hpp"
#include "Player.hpp"

#include "GlobalChatManager.hpp"
#include "GUIManager.hpp"
#include <cmath>
#include <memory>
#include "KitSelection.hpp"

#define WINDOW_W 1280
#define WINDOW_H 720

#include "Quest.hpp"
#include "QuestBook.hpp"
#include "EnemyManager.hpp"

class Game {

	bool _running = false;
	sf::RenderWindow _window;

	sf::View _cam;
	std::shared_ptr<Player> _player = nullptr;
	std::unique_ptr<SceneManager> _sceneManager;
	std::unique_ptr<GUIManager> _gui;

	std::shared_ptr<QuestDB> _questData;
	std::unique_ptr<QuestBook> _questBook;
	std::unique_ptr<EnemyManager> _enemyManager;

	void s_user_input();
	void s_movement();
	void s_update();
	/*
		since we are using a circle collider, the collision system will work based positioning of each circle collider when collision occurs

		refer to diagrams/collision.png
		
		 
	*/

	void check_collision(Entity& e, EntityVec& ev);

	void s_collider();
	void update_cam();



	void spawn_player();

	void s_renderer();


public:
	Game(const std::string& title);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	~Game() = default;



	void Run();
	
};