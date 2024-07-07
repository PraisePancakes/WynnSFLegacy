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
	std::vector<std::shared_ptr<Scene>> sceneTable = {};
	Scenes currentSceneToProcess = Scenes::SCENE_MENU;
	std::unique_ptr<KitSelection> m_KitSelection;
	sf::RenderWindow* ctx;
	std::unique_ptr<Menu> menu;
	Player* player;
	std::string getSceneFilePath(Scenes id);
	std::shared_ptr<CText> currentIntroText;
	std::shared_ptr<sf::RectangleShape> transititionOverlay;
	EnemyManager* em;
	bool isTransitioning; 

	void initTable();
	void handleMenuEvent();
	void handleKitSelectionEvent();
	void initIntroduction();
	void initTransition();

	void updateTransitioningPos();
	void updateTransitioningPlayer();
	void updateTransitioningAnimation();
	void updateTransitioning();
	void updateIntroductionColor();
	void updateIntroductionPos();
	void updateIntroduction();
	void renderIntroduction();

public:
	SceneManager(EnemyManager* em, sf::RenderWindow* ctx, Player* player);

	void Update();
	void HandleEvents(sf::Event* e);
	void SetScene(Scenes scene);
	void RenderScene();
	inline bool IsTransitioning() const {
		return isTransitioning;
	}
	std::shared_ptr<Scene> GetCurrentScene() const;

	~SceneManager();
};