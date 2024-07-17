#pragma once
#include "../core/ECS.hpp"
#include "../core/Manager/EntityManager.hpp"
#include "../core/Components/CAnimator.hpp"
#include "../core/Physics/Vec2D.hpp"
#include "Kit.hpp"
#include <vector>


class Player {

	std::shared_ptr<Entity> _entity = nullptr;
	
	bool _moving = false;
	bool _movingAnimationInitialized = false;
	bool _lookingLeft = false;
	bool _sprinting = false;
	void update_movement();
	std::vector<std::shared_ptr<Kit>> _kits;
	KitTypes _currentKitType = KitTypes::KIT_NONE;
	void init_animation(AnimationType type);
	void init_kits();
	void set_pos_relative_to_transform();
	bool _disabled = false;
public:
	Player(float spawnX, float spawnY);

	int currentLevel = 0;
	
	void SetKit(KitTypes kit);


	inline std::shared_ptr<Entity> GetEntityInstance() const { return this->_entity; };
	void Update();
	void Render(sf::RenderWindow& ctx);
	inline void Disable() {
		_disabled = true;
	};

	inline void Enable() {
		_disabled = false;
	}
	void SetPos(float x, float y);
	bool IsMoving();
	void HandleInput(sf::Event* e);
	Core::Physics::Vec2D GetPos() const;

	void HandleMovement();

	~Player() = default;

};