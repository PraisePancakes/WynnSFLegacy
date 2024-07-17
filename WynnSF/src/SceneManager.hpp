#pragma once 
#include <iostream>
#include "Scene.hpp"
#include "Menu.hpp"
#include "GlobalChatManager.hpp"
#include "Player.hpp"
#include "KitSelection.hpp"
#include "Player.hpp"
#include "EnemyManager.hpp"

class KitSelection;
class SceneManager {
	std::vector<std::shared_ptr<Scene>> _sceneTable = {};
	SCENES _currentSceneToProcess = SCENES::SCENE_MENU;
	std::unique_ptr<KitSelection> _kitSelection;
	sf::RenderWindow* _ctx;
	std::unique_ptr<Menu> _menu;
	Player* _player;
	std::string getSceneFilePath(SCENES id);
	std::shared_ptr<CText> _currentIntroText;
	std::shared_ptr<sf::RectangleShape> _transititionOverlay;
	EnemyManager* _em;
	bool _isTransitioning;

	void init_table();
	void handle_menu_event();
	void handle_kit_selection_event();
	void init_introduction();
	void init_transition();

	void update_transitioning_pos();
	void update_transitioning_player();
	void update_transitioning_animation();
	void update_transitioning();
	void update_introduction_color();
	void update_introduction_pos();
	void update_introduction();
	void render_introduction();

public:
	SceneManager(EnemyManager* em, sf::RenderWindow* ctx, Player* player);

	void Update();
	void HandleEvents(sf::Event* e);
	void SetScene(SCENES scene);
	void RenderScene();
	inline bool IsTransitioning() const {
		return _isTransitioning;
	}
	std::shared_ptr<Scene> GetCurrentScene() const;

	~SceneManager();
};