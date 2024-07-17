#include "EnemyManager.hpp"


	EnemyManager::EnemyManager(sf::RenderWindow* _ctx) {
		this->_ctx = _ctx;

	};
	void EnemyManager::AddEnemy(ENEMY_TYPES type, Core::Physics::Vec2D pos) {
		std::shared_ptr<Enemy> e = std::make_shared<Enemy>(type, pos);
		_enemies.push_back(e);
	};

	EnemyVec& EnemyManager::GetEnemyVec() {
		return _enemies;
	};

	void EnemyManager::Update() {
		if (_enemies.size() > 0) {
			for (auto& e : _enemies) {

				e->Update();
			}
		}
	};
	void EnemyManager::Render() {

		if (_enemies.size() > 0) {

			for (auto& e : _enemies) {
				e->Render(_ctx);
			}
		}
	};

	void EnemyManager::ClearEnemies() {
		_enemies.clear();
	}

	EnemyManager::~EnemyManager() = default;

