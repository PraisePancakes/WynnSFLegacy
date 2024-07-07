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
	void AddEnemy(const std::string& spritePath, sf::IntRect srcRect, const std::string& name, float agroR, int maxHP, Core::Physics::Vec2D pos) {
		std::shared_ptr<Enemy> e = std::make_shared<Enemy>(spritePath, srcRect, name, agroR, maxHP, pos);
		enemies.push_back(e);
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
				
				ctx->draw(e->entity->GetComponent<CSprite>()->sprite);
			}
		}
	};

	void ClearEnemies() {
		enemies.clear();
	}

	~EnemyManager() {};

};