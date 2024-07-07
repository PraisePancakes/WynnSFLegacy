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

	bool m_running = false;
	sf::RenderWindow m_Window;

	sf::View m_Cam;
	std::shared_ptr<Player> m_Player = nullptr;
	std::unique_ptr<SceneManager> m_SceneManager;
	std::unique_ptr<GUIManager> m_Gui;

	std::shared_ptr<QuestDB> m_QuestData;
	std::unique_ptr<QuestBook> m_QuestBook;
	std::unique_ptr<EnemyManager> m_EnemyManager;

	void sUserInput();
	void sMovement();
	void sUpdate();
	/*
		since we are using a circle collider, the collision system will work based positioning of each circle collider when collision occurs

		refer to diagrams/collision.png
		
		 
	*/

	void checkCollision(EntityVec& ev);
	void sCollider();
	void updateCam();



	void spawnPlayer();

	void sRenderer();


public:
	Game(const std::string& title);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	~Game() = default;



	void Run();
	
};