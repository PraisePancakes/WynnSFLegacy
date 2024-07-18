#include "Player.hpp"

#include "../core/Components/CTransform.hpp"
#include "../core/Components/CInput.hpp"
#include "../core/Components/CCollider.hpp"
#include "../core/Components/CHealth.hpp"
#include "../core/Components/CText.hpp"

//TO:DO REFACTOR KIT TYPE CASTING

Player::Player(float spawnX, float spawnY) {
	this->_entity = EntityManager::GetInstance()->AddEntity("Player");
	auto playerHealthE = EntityManager::GetInstance()->AddEntity("Player-Health");

	auto tc = _entity->AddComponent<CTransform>(Core::Physics::Vec2D(spawnX, spawnY), Core::Physics::Vec2D(0, 0), 0);
	_entity->AddComponent<CInput>();


	auto healthC = playerHealthE->AddComponent<CHealth>(100);
	auto healthTxtC= playerHealthE->AddComponent<CText>("", "src/Assets/Fonts/PixelFont.ttf", 24, 0, 0, true);
	init_kits();
};

void Player::init_kits() {

	this->_kits.push_back(std::make_shared<ArcherKit>());
	this->_kits.push_back(std::make_shared<AssassinKit>());
	this->_kits.push_back(std::make_shared<WarriorKit>());
	this->_kits.push_back(std::make_shared<WizardKit>());
}

void Player::SetKit(KitTypes kit) {

	this->_currentKitType = kit;
	std::shared_ptr<Kit> kitPtr = this->_kits[(int)_currentKitType];
	if (kitPtr) {
		auto c = this->_entity->AddComponent<CCollider>(kitPtr->GetCollider());
		
	}
}

void Player::init_animation(AnimationType type) {
	std::shared_ptr<Kit> kitPtr = this->_kits[(int)_currentKitType];

		if (kitPtr) {
			auto archerKit = dynamic_cast<ArcherKit*>(kitPtr.get());
			auto assassinKit = dynamic_cast<AssassinKit*>(kitPtr.get());
			auto warriorKit = dynamic_cast<WarriorKit*>(kitPtr.get());
			auto wizardKit = dynamic_cast<WizardKit*>(kitPtr.get());

			if (archerKit) {
				archerKit->SetCurrentAnimator(type);
			} else if (assassinKit) {
				assassinKit->SetCurrentAnimator(type);
			} else if (warriorKit) {
				warriorKit->SetCurrentAnimator(type);
			} else if (wizardKit) {
				wizardKit->SetCurrentAnimator(type);
			}
		}	
};


static void validateHealth() {
	
	std::shared_ptr<Entity> playerHealthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
	std::shared_ptr<CHealth> healthC = playerHealthE->GetComponent<CHealth>();

	if (healthC->CurrHp <= 0) {
		healthC->CurrHp = 0;
	}
	if (healthC->CurrHp >= healthC->MaxHP) {
		healthC->CurrHp = healthC->MaxHP;
	}
}

void Player::update_movement() {

	set_pos_relative_to_transform();
		if (_attacking && !IsMoving()) {
			init_animation(AnimationType::ATTACK);
		}
		if (IsMoving() && !_movingAnimationInitialized) {
			init_animation(AnimationType::RUN);
			_movingAnimationInitialized = true;
		}
		else if (!IsMoving() && _movingAnimationInitialized) {
			init_animation(AnimationType::IDLE);
			_movingAnimationInitialized = false;
		}
	
	

	if (_lookingLeft) {
		init_animation(AnimationType::LOOKING_LEFT);
	}
	else {
		init_animation(AnimationType::LOOKING_RIGHT);
	}
	

}


void Player::Update() {
	if (_disabled) {
		return;
	}
	validateHealth();
	update_movement();
	

}

void Player::Render(sf::RenderWindow& ctx) {
	if (_disabled) {
		return;
	}
	std::shared_ptr<Kit> kitPtr = this->_kits[(int)_currentKitType];

	if (kitPtr) {
		auto archerKit = dynamic_cast<ArcherKit*>(kitPtr.get());
		auto assassinKit = dynamic_cast<AssassinKit*>(kitPtr.get());
		auto warriorKit = dynamic_cast<WarriorKit*>(kitPtr.get());
		auto wizardKit = dynamic_cast<WizardKit*>(kitPtr.get());

		if (archerKit) {
			archerKit->PlayCurrentAnimator(.2f);
			ctx.draw(archerKit->GetCurrentAnimator().sprite);
		}
		else if (assassinKit) {
			assassinKit->PlayCurrentAnimator(.2f);
			ctx.draw(assassinKit->GetCurrentAnimator().sprite);
		}
		else if (warriorKit) {
			warriorKit->PlayCurrentAnimator(.2f);
			ctx.draw(warriorKit->GetCurrentAnimator().sprite);
		}
		else if (wizardKit) {
			wizardKit->PlayCurrentAnimator(.2f);
			ctx.draw(wizardKit->GetCurrentAnimator().sprite);
		}
	}

	
};


void Player::set_pos_relative_to_transform() {
	
	auto tc = this->_entity->GetComponent<CTransform>();
	

	std::shared_ptr<Kit> kitPtr = this->_kits[(int)_currentKitType];
	if (kitPtr) {
		auto archerKit = dynamic_cast<ArcherKit*>(kitPtr.get());
		if (archerKit) {
			archerKit->GetCurrentAnimator().sprite.setPosition(tc->Position.x, tc->Position.y);
		}

		auto assassinKit = dynamic_cast<AssassinKit*>(kitPtr.get());
		if (assassinKit) {
			assassinKit->GetCurrentAnimator().sprite.setPosition(tc->Position.x, tc->Position.y);
			
		}

		auto warriorKit = dynamic_cast<WarriorKit*>(kitPtr.get());
		if (warriorKit) {
			warriorKit->GetCurrentAnimator().sprite.setPosition(tc->Position.x, tc->Position.y);

		}

		auto wizardKit = dynamic_cast<WizardKit*>(kitPtr.get());
		if (wizardKit) {
			wizardKit->GetCurrentAnimator().sprite.setPosition(tc->Position.x, tc->Position.y);

		}
	}


}


void Player::SetPos(float x, float y) {
	
	auto tc = this->_entity->GetComponent<CTransform>();
	tc->Position.x = x;
	tc->Position.y = y;

	std::shared_ptr<Kit> kitPtr = this->_kits[(int)_currentKitType];
	if (kitPtr) {
		auto archerKit = dynamic_cast<ArcherKit*>(kitPtr.get());
		if (archerKit) {
			archerKit->GetCurrentAnimator().sprite.setPosition(x, y);
		}

		auto assassinKit = dynamic_cast<AssassinKit*>(kitPtr.get());
		if (assassinKit) {
			
			assassinKit->GetCurrentAnimator().sprite.setPosition(x, y);
		}

		auto warriorKit = dynamic_cast<WarriorKit*>(kitPtr.get());
		if (warriorKit) {

			warriorKit->GetCurrentAnimator().sprite.setPosition(x, y);
		}

		auto wizardKit = dynamic_cast<WizardKit*>(kitPtr.get());
		if (wizardKit) {

			wizardKit->GetCurrentAnimator().sprite.setPosition(x, y);
		}

	}

	

}

bool Player::IsMoving() {
	if (_entity->GetComponent<CTransform>()->Velocity.x != 0 || _entity->GetComponent<CTransform>()->Velocity.y != 0) {
		_moving = true;
		return true;
	}
	_moving = false;
	return false;
}

void Player::HandleInput(sf::Event* e) {
	
	auto ic = _entity->GetComponent<CInput>();
	if (_disabled) {
			ic->iup = false;
			ic->ileft = false;
			ic->idown = false;
			ic->iright = false;
			_sprinting = false;
			_attacking = false;
			return;
	}
	if (e->type == sf::Event::KeyPressed) {

		if (e->key.scancode == sf::Keyboard::W) {
			ic->iup = true;
		}

		if (e->key.scancode == sf::Keyboard::A) {

			ic->ileft = true;
		}

		if (e->key.scancode == sf::Keyboard::S) {
			ic->idown = true;
		}

		if (e->key.scancode == sf::Keyboard::D) {
			ic->iright = true;
		}

		if (e->key.scancode == 128) {

			_sprinting = true;
		}


	}
	else if (e->type == sf::Event::KeyReleased) {
		if (e->key.scancode == sf::Keyboard::W) {
			ic->iup = false;
		}

		if (e->key.scancode == sf::Keyboard::A) {
			ic->ileft = false;
		}

		if (e->key.scancode == sf::Keyboard::S) {
			ic->idown = false;
		}

		if (e->key.scancode == sf::Keyboard::D) {
			ic->iright = false;
		}

		if (e->key.scancode == 128) {
			_sprinting = false;
		}
	}
	if (e->type == sf::Event::MouseButtonPressed) {
		if (e->mouseButton.button == sf::Mouse::Left) {
			_attacking = true;
		}
	}
	else if (e->type == sf::Event::MouseButtonReleased) {
		if (e->mouseButton.button == sf::Mouse::Left) {
			_attacking = false;
		}
	}

	
}


Core::Physics::Vec2D Player::GetPos() const {
	std::shared_ptr<CTransform> tc = _entity->GetComponent<CTransform>();
	return tc->Position;
};

void Player::HandleMovement() {
	float MOVEMENT_SPEED = 3.0f;

	auto playerInput = _entity->GetComponent<CInput>();
	
	auto tc = _entity->GetComponent<CTransform>();

	if (_sprinting) {
		MOVEMENT_SPEED = 4.f;
	}


	float velX = 0;
	float velY = 0;

	if (playerInput->idown) {
		velY += MOVEMENT_SPEED;
	}

	if (playerInput->ileft) {
		_lookingLeft = true;
		velX -= MOVEMENT_SPEED;
	}

	if (playerInput->iright) {
		_lookingLeft = false;
		velX += MOVEMENT_SPEED;
	}

	if (playerInput->iup) {
		velY -= MOVEMENT_SPEED;
	}

	tc->Velocity.x = velX;
	tc->Velocity.y = velY;


	tc->Velocity.Normalize();

	tc->Velocity.x *= MOVEMENT_SPEED;
	tc->Velocity.y *= MOVEMENT_SPEED;

	tc->Position += tc->Velocity;



}

