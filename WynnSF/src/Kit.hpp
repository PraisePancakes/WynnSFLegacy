#pragma once
#include <iostream>
#include "../core/Manager/EntityManager.hpp"
#include "../core/Components/CAnimator.hpp"
#include "../core/Components/CCollider.hpp"

#define __KIT_COLLIDER_RADIUS 25

enum class KitTypes {
	KIT_NONE = -1,
	KIT_ARCHER,
	KIT_ASSASSIN,
	KIT_WARRIOR,
	KIT_WIZARD,
	KIT_MAX_EXCLUSIVE
};


enum class AnimationType {
	IDLE,
	RUN,
	ATTACK,
	LOOKING_LEFT,
	LOOKING_RIGHT,

};

class Kit {
public:
	std::string kitTitle;


	Kit(const std::string& title) {
		this->kitTitle = title;
	};

	virtual CAnimator& GetCurrentAnimator() = 0;
	virtual void SetCurrentAnimator(AnimationType animatorType) = 0;
	virtual CCollider& GetCollider() = 0;

	std::string GetKitTitle() const {
		return kitTitle;
	}


	~Kit() {};
};

struct ArcherAnimationData {
	CAnimator idle{ "src/Assets/Sprites/Player/Archer/Sprites/Character/Idle.png", sf::IntRect(0, 0, 100, 100), 900, 100 };
	CAnimator run{ "src/Assets/Sprites/Player/Archer/Sprites/Character/Run.png", sf::IntRect(0, 0, 100, 100), 700, 100 };
	CAnimator attack{ "src/Assets/Sprites/Player/Archer/Sprites/Character/Attack.png", sf::IntRect(0, 0, 100, 100), 500 , 100};
	
};

struct AssassinAnimatorData {
	CAnimator idle{ "src/Assets/Sprites/Player/Assassin/Sprites/Character/Idle.png", sf::IntRect(0, 0, 200, 200), 600, 200 };
	CAnimator run{ "src/Assets/Sprites/Player/Assassin/Sprites/Character/Run.png", sf::IntRect(0, 0, 200, 200), 800, 200 };
	CAnimator attack{ "src/Assets/Sprites/Player/Assassin/Sprites/Character/Attack1.png", sf::IntRect(0, 0, 200, 200), 400, 200 };
};

struct WarriorAnimatorData {
	CAnimator idle{ "src/Assets/Sprites/Player/Warrior/Sprites/Character/Idle.png", sf::IntRect(0, 0, 162, 162), 1458, 162 };
	CAnimator run{ "src/Assets/Sprites/Player/Warrior/Sprites/Character/Run.png", sf::IntRect(0, 0, 162, 162), 1134, 162 };
};


struct WizardAnimatorData {
	CAnimator idle{ "src/Assets/Sprites/Player/Wizard/Sprites/Character/Idle.png", sf::IntRect(0, 0, 230, 190), 925, 230 };
	CAnimator run{ "src/Assets/Sprites/Player/Wizard/Sprites/Character/Run.png", sf::IntRect(0, 0, 230, 190), 1610, 230 };

};

class WizardKit : public Kit {
public:
	WizardAnimatorData animatorData;
	CAnimator currentAnimator = animatorData.idle;
	CCollider *collider;

	WizardKit() : Kit("Wizard") {
		this->currentAnimator.ScaleToNxN(128, 128);
		this->collider = new CCollider((currentAnimator.sprite.getGlobalBounds().width / 2 + currentAnimator.sprite.getOrigin().x) - __KIT_COLLIDER_RADIUS);
	};

	CAnimator& GetCurrentAnimator() override {
		return this->currentAnimator;
	}

	CCollider& GetCollider() override {
		return *collider;
	}


	void SetCurrentAnimator(AnimationType animatorType) override {
		switch (animatorType)
		{
		case AnimationType::IDLE:
			this->currentAnimator = animatorData.idle;
			break;
		case AnimationType::RUN:
			this->currentAnimator = animatorData.run;
			break;
		case AnimationType::ATTACK:
			
			break;
		case AnimationType::LOOKING_LEFT:
			currentAnimator.ScaleToNxN(-128, 128);
			break;
		case AnimationType::LOOKING_RIGHT:
			currentAnimator.ScaleToNxN(128, 128);
			break;
		default:
			break;
		}
	
	}
	void PlayCurrentAnimator(float dt) {
		currentAnimator.Play(dt);
	}

	~WizardKit() {
		delete collider;
	};
};

class AssassinKit : public Kit {
public:
	AssassinAnimatorData animatorData;
	CAnimator currentAnimator = animatorData.idle;
	CCollider* collider;

	AssassinKit() : Kit("Assassin") {
		this->currentAnimator.ScaleToNxN(128, 128);
		this->collider = new CCollider((currentAnimator.sprite.getGlobalBounds().width / 2 + currentAnimator.sprite.getOrigin().x) - __KIT_COLLIDER_RADIUS);
	
	};
	CAnimator& GetCurrentAnimator() override {
		return this->currentAnimator;
	}



	void SetCurrentAnimator(AnimationType animatorType) override {
		switch (animatorType)
		{
		case AnimationType::IDLE:
			this->currentAnimator = animatorData.idle;
			break;
		case AnimationType::RUN:
			this->currentAnimator = animatorData.run;
			break;
		case AnimationType::ATTACK:
			this->currentAnimator = animatorData.attack;
			break;
		case AnimationType::LOOKING_LEFT:
			currentAnimator.ScaleToNxN(-128, 128);
			break;
		case AnimationType::LOOKING_RIGHT:
			currentAnimator.ScaleToNxN(128, 128);
			break;
		default:
			break;
		}

	}

	CCollider& GetCollider() override {
		return *collider;
	}

	void PlayCurrentAnimator(float dt) {
		currentAnimator.Play(dt);
	}

	~AssassinKit() {
		delete collider;
	};
};

class ArcherKit : public Kit {
public:
	ArcherAnimationData animatorData;
	CAnimator currentAnimator = animatorData.idle;
	CCollider *collider;

	ArcherKit() : Kit("Archer") {
		this->currentAnimator.ScaleToNxN(128, 128);
		this->collider = new CCollider((currentAnimator.sprite.getGlobalBounds().width / 2 + currentAnimator.sprite.getOrigin().x) - __KIT_COLLIDER_RADIUS);
	};
	CAnimator& GetCurrentAnimator() override {
		return this->currentAnimator;
	}

	CCollider& GetCollider() override {
		return *collider;
	}

	void SetCurrentAnimator(AnimationType animatorType) override {
		switch (animatorType)
		{
		case AnimationType::IDLE:
			this->currentAnimator = animatorData.idle;
			break;
		case AnimationType::RUN:
			this->currentAnimator = animatorData.run;
			break;
		case AnimationType::ATTACK:
			this->currentAnimator = animatorData.attack;
			break;
		case AnimationType::LOOKING_LEFT:
			currentAnimator.ScaleToNxN(-128, 128);
			break;
		case AnimationType::LOOKING_RIGHT:
			currentAnimator.ScaleToNxN(128, 128);
			break;
		default:
			break;
		}



	}


	void PlayCurrentAnimator(float dt) {
		currentAnimator.Play(dt);
	}
	

	~ArcherKit() {
		delete collider;
	};

};

class WarriorKit : public Kit {
public:
	WarriorAnimatorData animatorData;
	CAnimator currentAnimator = animatorData.idle;
	CCollider *collider;
	WarriorKit() : Kit("Warrior") {
		this->currentAnimator.ScaleToNxN(128, 128);
		this->collider = new CCollider((currentAnimator.sprite.getGlobalBounds().width / 2 + currentAnimator.sprite.getOrigin().x) - __KIT_COLLIDER_RADIUS);
	};
	CAnimator& GetCurrentAnimator() override {
		return this->currentAnimator;
	}
	void SetCurrentAnimator(AnimationType animatorType) override {
		switch (animatorType)
		{
		case AnimationType::IDLE:
			this->currentAnimator = animatorData.idle;
			break;
		case AnimationType::RUN:
			this->currentAnimator = animatorData.run;
			break;
		case AnimationType::ATTACK:
			break;
		case AnimationType::LOOKING_LEFT:
			currentAnimator.ScaleToNxN(-128, 128);
			break;
		case AnimationType::LOOKING_RIGHT:
			currentAnimator.ScaleToNxN(128, 128);
			break;
		default:
			break;
		}

	}
	CCollider& GetCollider() override {
		return *collider;
	}

	void PlayCurrentAnimator(float dt) {
		currentAnimator.Play(dt);
	}


	~WarriorKit() {
		delete collider;
	};

};