#pragma once
#include "../core/ECS.hpp"
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"


typedef std::vector<std::shared_ptr<Enemy>> EnemyVec;

class EnemyManager {
	
private:
	sf::RenderWindow* _ctx{nullptr};
	EnemyVec _enemies;

public:
	EnemyManager(sf::RenderWindow* ctx);
	void AddEnemy(ENEMY_TYPES type, Core::Physics::Vec2D pos);

	EnemyVec& GetEnemyVec();

	void Update();
	void Render();

	void ClearEnemies();

	~EnemyManager();

};