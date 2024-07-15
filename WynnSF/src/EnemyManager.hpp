#pragma once
#include "../core/ECS.hpp"
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"


typedef std::vector<std::shared_ptr<Enemy>> EnemyVec;

class EnemyManager {
	
private:
	sf::RenderWindow* ctx{nullptr};
	EnemyVec enemies;

public:
	EnemyManager(sf::RenderWindow* ctx) {
		this->ctx = ctx;
		
	};
	void AddEnemy(EnemyTypes type, Core::Physics::Vec2D pos) {
		std::shared_ptr<Enemy> e = std::make_shared<Enemy>(type, pos);
		enemies.push_back(e);
	};

	EnemyVec& GetEnemyVec() {
		return enemies;
	};

	void Update() {
		if (enemies.size() > 0) {
			for (auto& e : enemies) {

				e->Update();
			}
		}
	};
	void Render() {
		
		if (enemies.size() > 0) {
			
			for (auto& e : enemies) {
				e->Render(ctx);
			}
		}
	};

	void ClearEnemies() {
		enemies.clear();
	}

	~EnemyManager() {};

};