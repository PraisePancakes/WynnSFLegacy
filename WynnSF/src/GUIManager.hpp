#pragma once
#include "GlobalChatManager.hpp"
#include "../core/Manager/EntityManager.hpp"
#include "../core/Components/CHealth.hpp"
#include "../core/Components/CTransform.hpp"
#include <vector>

class GUIManager {

	sf::RenderWindow* _ctx{nullptr};
	sf::RectangleShape _healthBarContainer;
	sf::RectangleShape _healthBar;
	bool _disabled = false;
	void init_health_bar_ui();
	void update_health_bar_ui();

	void render_health_bar_ui();
	void render_chat_ui() const;

	void update_chat_ui() const;

public:
	GUIManager(sf::RenderWindow* ctx);



	void Update();

	void Render();

	void HandleEvents(sf::Event* e);


	~GUIManager();
};