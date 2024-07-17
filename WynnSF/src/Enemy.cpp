#pragma once
#include "Enemy.hpp"



//TO DO : FIX ENEMY HEALTH BAR UI


    BaseEnemyType::BaseEnemyType(const std::string& name, float health, float agroRadius, unsigned short int damage)
        : _name(name), _agroRadius(agroRadius), _damageOutput(damage) {
        this->_entity = EntityManager::GetInstance()->AddEntity("Enemy");

        this->_healthEntity = EntityManager::GetInstance()->AddEntity("Enemy-Health");
        auto healthC = this->_healthEntity->AddComponent<CHealth>(health);


        auto healthTxtC = _healthEntity->AddComponent<CText>("", "src/Assets/Fonts/PixelFont.ttf", 24, 0, 0, true);
        healthTxtC->text.setOrigin(healthTxtC->text.getGlobalBounds().width / 2, healthTxtC->text.getGlobalBounds().height / 2);



        _healthBarFill.setSize(sf::Vector2f(healthC->MaxHP, 5));
        _healthBarFill.setFillColor(sf::Color::Green);
        _healthBarFill.setOrigin(_healthBarFill.getSize().x / 2, _healthBarFill.getSize().y / 2);

        _healthBarContainer.setSize(sf::Vector2f(healthC->MaxHP, 5));
        _healthBarContainer.setFillColor(sf::Color::Black);
        _healthBarContainer.setOrigin(_healthBarContainer.getSize().x / 2, _healthBarContainer.getSize().y / 2);


        this->_entity->AddComponent<CTransform>(Core::Physics::Vec2D(0, 0), Core::Physics::Vec2D(0, 0), 0);

    }




    void BaseEnemyType::DestroyEnemy() {
        this->_entity->DestroyEntity();
    }

    const std::string BaseEnemyType::GetName() const {
        return this->_name;
    }

//We create any enemy we want via deriving from BaseEnemyType
//each enemy has a different Update because each enemy will have a different dataset to update on.




    void Minotaur::update_healthbar_position() {

        const auto healthC = _healthEntity->GetComponent<CHealth>();

        if (healthC->CurrHp <= 0) {
            healthC->CurrHp = 0;
        }

        const float healthBarContainerX = this->_entity->GetComponent<CTransform>()->Position.x;
        const float healthBarContainerY = this->_entity->GetComponent<CTransform>()->Position.y - (_currentAnimator.sprite.getGlobalBounds().height / 2);

        const sf::Vector2f healthBarContainerPos(healthBarContainerX, healthBarContainerY);
        _healthBarContainer.setPosition(healthBarContainerPos);

        const sf::Vector2f healthBarFillPos(healthBarContainerX, healthBarContainerY);
        _healthBarFill.setPosition(healthBarFillPos);

        _healthBarFill.setSize(sf::Vector2f(healthC->CurrHp, 5));

    }

    void Minotaur::update_healthbar_fill() {

        const auto healthC = _healthEntity->GetComponent<CHealth>();
        const int YELLOW_MIN = (healthC->MaxHP / 3) * 2;
        const int YELLOW_MAX = (healthC->MaxHP / 2);
        const int RED_MAX = YELLOW_MAX;

        if (healthC->CurrHp <= YELLOW_MIN && healthC->CurrHp > YELLOW_MAX) {
            _healthBarFill.setFillColor(sf::Color::Yellow);
        }
        else if (healthC->CurrHp <= RED_MAX) {
            _healthBarFill.setFillColor(sf::Color::Red);
        }

    }

    void Minotaur::update_healthbar_text() {
        const auto healthTxtC = _healthEntity->GetComponent<CText>();
        const auto healthC = _healthEntity->GetComponent<CHealth>();

        const float healthTextXpos = _healthBarContainer.getPosition().x - (healthTxtC->text.getGlobalBounds().width / 2);
        const float healthTextYpos = _healthBarContainer.getPosition().y - _healthBarContainer.getGlobalBounds().height;
        sf::Vector2f healthTextPos(healthTextXpos, healthTextYpos);

        healthTxtC->text.setPosition(healthTextPos);
        healthTxtC->text.setString(std::to_string(healthC->CurrHp));
    }

    void Minotaur::update_healthbar_ui() {

        update_healthbar_position();
        update_healthbar_fill();
        update_healthbar_text();

    }

    void Minotaur::update_animator() {

        ENEMY_ANIMATION_TYPES newAnimationState;


        if (_state.state_agro) {
            if (_state.state_attack) {
                newAnimationState = ENEMY_ANIMATION_TYPES::ATTACK;
            }
            else {
                newAnimationState = ENEMY_ANIMATION_TYPES::RUN;

            }
        }
        else {
            newAnimationState = ENEMY_ANIMATION_TYPES::IDLE;
        }


        if (newAnimationState != _currentAnimationType) {
            SetCurrentAnimator(newAnimationState);
            _currentAnimationType = newAnimationState;

        }
        PlayCurrentAnimator(.2f);

    };

    void Minotaur::update_damage_output() {
        std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
        auto phealth = healthE->GetComponent<CHealth>();

        unsigned short int damage = this->_damageOutput;
        if (_state.state_attack && _currentAnimator.finishedCurrentAnimation) {
            phealth->CurrHp -= damage;
            //Test
            this->_healthEntity->GetComponent<CHealth>()->CurrHp -= damage;
            _currentAnimator.finishedCurrentAnimation = false;
        }
    }


    void Minotaur::update_agro() {
        std::shared_ptr<Entity> player = EntityManager::GetInstance()->GetEntities("Player")[0];
        auto playerPos = player->GetComponent<CTransform>()->Position;
        auto pR = player->GetComponent<CCollider>()->radius;

        auto etc = this->_entity->GetComponent<CTransform>();
        float eX = etc->Position.x;
        float eY = etc->Position.y;

        float plX = playerPos.x;
        float plY = playerPos.y;

        float xDiff = plX - eX;
        float yDiff = plY - eY;

        float distance = std::sqrt(xDiff * xDiff + yDiff * yDiff);
        float velX = 0;
        float velY = 0;

        ENEMY_ANIMATION_TYPES direction = ENEMY_ANIMATION_TYPES::LOOKING_LEFT;

        if (distance < pR + _agroRadius) {
            _state.SetAgro();

        }
        else {
            _state.SetIdle();

        }

        if (_state.state_agro) {

            if (distance < pR) {

                etc->Velocity.x += 0;
                etc->Velocity.y += 0;
                _state.SetAttack();

                if (xDiff >= 0) {
                    direction = ENEMY_ANIMATION_TYPES::LOOKING_RIGHT;
                }
                else {
                    direction = ENEMY_ANIMATION_TYPES::LOOKING_LEFT;
                }

            }
            else {

                if (xDiff >= 0) {
                    velX += ENEMY_SPEED;
                    direction = ENEMY_ANIMATION_TYPES::LOOKING_RIGHT;

                }
                if (xDiff < 0) {
                    velX -= ENEMY_SPEED;
                    direction = ENEMY_ANIMATION_TYPES::LOOKING_LEFT;
                }

                if (yDiff > 0) {
                    velY += ENEMY_SPEED;
                }
                if (yDiff < 0) {
                    velY -= ENEMY_SPEED;

                }

                etc->Velocity.x = velX;
                etc->Velocity.y = velY;
                etc->Velocity.Normalize();

                etc->Velocity.x *= ENEMY_SPEED;
                etc->Velocity.y *= ENEMY_SPEED;

                etc->Position += etc->Velocity;

            }
        }
        SetCurrentAnimator(direction);

        this->_currentAnimator.sprite.setPosition(etc->Position.x, etc->Position.y);

    };


    void Minotaur::Render(sf::RenderWindow* ctx) {
        const auto healthTxtC = this->_healthEntity->GetComponent<CText>();

        ctx->draw(GetCurrentAnimator().sprite);
        ctx->draw(this->_healthBarContainer);
        ctx->draw(this->_healthBarFill);
        ctx->draw(healthTxtC->text);
    };


    Minotaur::Minotaur() : BaseEnemyType("Minotaur", 150, 100, 20) {
        this->_currentAnimator.ScaleToNxN(128, 128);
        this->_entity->AddComponent<CCollider>(_currentAnimator.frameWidth / 2);

    }

    std::shared_ptr<Entity> Minotaur::GetEntityInstance() {
        return this->_entity;
    }



    void Minotaur::Update() {
        update_healthbar_ui();
        update_animator();
        update_agro();
        update_damage_output();

    }

    void Minotaur::SetCurrentAnimator(ENEMY_ANIMATION_TYPES animatorType) {
        switch (animatorType)
        {
        case ENEMY_ANIMATION_TYPES::IDLE:
            this->_currentAnimator = _animatorData.idle;

            break;
        case ENEMY_ANIMATION_TYPES::RUN:
            this->_currentAnimator = _animatorData.run;

            break;
        case ENEMY_ANIMATION_TYPES::ATTACK:
            this->_currentAnimator = _animatorData.attack;
            break;
        case ENEMY_ANIMATION_TYPES::LOOKING_LEFT:
            _currentAnimator.ScaleToNxN(-128, 128);
            break;
        case ENEMY_ANIMATION_TYPES::LOOKING_RIGHT:
            _currentAnimator.ScaleToNxN(128, 128);
            break;
        default:
            break;
        }
    };

    CAnimator& Minotaur::GetCurrentAnimator() {
        return this->_currentAnimator;
    };

    void Minotaur::PlayCurrentAnimator(float dt) {
        _currentAnimator.Play(dt);
    };

    Minotaur::~Minotaur() {};


    Enemy::Enemy(ENEMY_TYPES type, Core::Physics::Vec2D pos) {
        switch (type) {
        case ENEMY_TYPES::ENEMY_MINOTAUR:
            _enemyType = std::make_shared<Minotaur>();
            break;

        default:
            throw std::runtime_error("Unknown enemy type");
        }
        _enemyType->GetEntityInstance()->GetComponent<CTransform>()->Position = pos;
    }

    void Enemy::Update() {
        _enemyType->Update();
    }

    void Enemy::Render(sf::RenderWindow* ctx) {
        this->_enemyType->Render(ctx);
    }


    CAnimator& Enemy::GetCurrentAnimator() const {
        return this->_enemyType->GetCurrentAnimator();
    };

    Enemy::~Enemy() = default;




