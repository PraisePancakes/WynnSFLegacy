#pragma once
#include "../core/ECS.hpp"
#include "../core/Components/CHealth.hpp"
#include "../core/Physics/Vec2D.hpp"
#include "../core/Components/CTransform.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Utils/Utils.hpp"
#include "../core/Components/CCollider.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Components/CHealth.hpp"

constexpr float ENEMY_SPEED = 1;

enum class EnemyTypes {
    ENEMY_MINOTAUR,
};


enum class EnemyAnimationType {
    IDLE,
    RUN,
    ATTACK,
    LOOKING_LEFT,
    LOOKING_RIGHT,

};

struct EnemyState {
    bool state_agro = false;
    bool state_idle = false;
    bool state_attack = false;
    bool state_attack_finished = false;

    EnemyState() {};
    void SetAgro() {
        state_agro = true;
        state_idle = false;
        state_attack = false;
    };

    void SetIdle() {
        state_agro = false;
        state_idle = true;
        state_attack = false;
    }

    void SetAttack() {
        state_agro = true;
        state_idle = false;
        state_attack = true;
    }

    ~EnemyState() {};
};

class BaseEnemyType {
protected:
    std::shared_ptr<Entity> entity = nullptr;
    std::string name;
    bool _agro = false;
    float _agroRadius = 100;
    unsigned short int _damageOutput = 0;

    
public:
    BaseEnemyType(const std::string& name, float health, float agroRadius, unsigned short int damage)
        : name(name), _agroRadius(agroRadius), _damageOutput(damage) {
        this->entity = EntityManager::GetInstance()->AddEntity("Enemy");
        this->entity->AddComponent<CHealth>(health);
        this->entity->AddComponent<CTransform>(Core::Physics::Vec2D(0, 0), Core::Physics::Vec2D(0, 0), 0);
       
        
      
    }

    virtual void SetCurrentAnimator(EnemyAnimationType animatorType) = 0;
    virtual std::shared_ptr<Entity> GetEntityInstance() = 0;
    virtual void Update() = 0;
    virtual CAnimator& GetCurrentAnimator() = 0;
    virtual void PlayCurrentAnimator(float dt) = 0;


    void DestroyEnemy() {
        this->entity->DestroyEntity();
    }

    const std::string GetName() const {
        return this->name;
    }

    virtual ~BaseEnemyType() {
        DestroyEnemy();
    }
};

//We create any enemy we want via deriving from BaseEnemyType
//each enemy has a different Update because each enemy will have a different dataset to update on.


struct MinotaurAnimatorData {
    CAnimator idle{ "src/Assets/Sprites/Enemy/Minotaur.png", sf::IntRect(0, 480, 96, 96), 400, 96 };
    CAnimator run{ "src/Assets/Sprites/Enemy/Minotaur.png", sf::IntRect(0, 96, 96, 96), 600, 96 };
    CAnimator attack{ "src/Assets/Sprites/Enemy/Minotaur.png", sf::IntRect(0, 288, 96, 96), 700, 96 };
};

class Minotaur : public BaseEnemyType {

    MinotaurAnimatorData animatorData;
    CAnimator currentAnimator = animatorData.idle;
    EnemyAnimationType currentAnimationType = EnemyAnimationType::IDLE;
    EnemyState state;


    void update_animator() {

        EnemyAnimationType newAnimationState;

        if (state.state_agro) {
            if (state.state_attack) {
                newAnimationState = EnemyAnimationType::ATTACK;
            }
            else {
                newAnimationState = EnemyAnimationType::RUN;

            }
        }
        else {
            newAnimationState = EnemyAnimationType::IDLE;
        }


        if (newAnimationState != currentAnimationType) {
            SetCurrentAnimator(newAnimationState);
            currentAnimationType = newAnimationState;

        }
        PlayCurrentAnimator(.2f);
      
    };

    void update_damage_output() {
        std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
        auto phealth = healthE->GetComponent<CHealth>();

        unsigned short int damage = this->_damageOutput;
        if (state.state_attack && currentAnimator.finishedCurrentAnimation) {
            phealth->CurrHp -= damage;
            currentAnimator.finishedCurrentAnimation = false;
        }
    }
 

    void update_agro() {
        std::shared_ptr<Entity> player = EntityManager::GetInstance()->GetEntities("Player")[0];
        auto playerPos = player->GetComponent<CTransform>()->Position;
        auto pR = player->GetComponent<CCollider>()->radius;

        auto etc = this->entity->GetComponent<CTransform>();
        float eX = etc->Position.x;
        float eY = etc->Position.y;

        float plX = playerPos.x;
        float plY = playerPos.y;

        float xDiff = plX - eX;
        float yDiff = plY - eY;

        float distance = std::sqrt(xDiff * xDiff + yDiff * yDiff);
        float velX = 0;
        float velY = 0;
           
        EnemyAnimationType direction = EnemyAnimationType::LOOKING_LEFT;

        if (distance < pR + _agroRadius) {
            state.SetAgro();
           
        }
        else {
            state.SetIdle();

        }

        if (state.state_agro) {
          
            if (distance < pR) {
              
                etc->Velocity.x += 0;
                etc->Velocity.y += 0;
                state.SetAttack();

                if (xDiff >= 0) {
                    direction = EnemyAnimationType::LOOKING_RIGHT;
                }
                else {
                    direction = EnemyAnimationType::LOOKING_LEFT;
                }

            }
            else {
           
                if (xDiff >= 0) {
                    velX += ENEMY_SPEED;
                    direction = EnemyAnimationType::LOOKING_RIGHT;
                   
                }
                if (xDiff < 0 ) {
                    velX -= ENEMY_SPEED;
                    direction = EnemyAnimationType::LOOKING_LEFT;
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
        
        this->currentAnimator.sprite.setPosition(etc->Position.x, etc->Position.y);
        
    };

public:
    Minotaur() : BaseEnemyType("Minotaur", 150, 100, 20) { 
        this->currentAnimator.ScaleToNxN(128, 128);
        this->entity->AddComponent<CCollider>(currentAnimator.frameWidth / 2);
       
    }

    std::shared_ptr<Entity> GetEntityInstance() override {
        return this->entity;
    }

   

    void Update() override {
        update_animator();
        update_agro();
        update_damage_output();
       
    }

    void SetCurrentAnimator(EnemyAnimationType animatorType) override {
        switch (animatorType)
        {
        case EnemyAnimationType::IDLE:
            this->currentAnimator = animatorData.idle;
          
            break;
        case EnemyAnimationType::RUN:
            this->currentAnimator = animatorData.run;
           
            break;
        case EnemyAnimationType::ATTACK:
            this->currentAnimator = animatorData.attack;
            break;
        case EnemyAnimationType::LOOKING_LEFT:
            currentAnimator.ScaleToNxN(-128, 128);
            break;
        case EnemyAnimationType::LOOKING_RIGHT:
            currentAnimator.ScaleToNxN(128, 128);
            break;
        default:
            break;
        }
    };

    CAnimator& GetCurrentAnimator() override {
        return this->currentAnimator;
    };

    void PlayCurrentAnimator(float dt) override {
         currentAnimator.Play(dt);
    };

    ~Minotaur() override {};
};

class Enemy {
private:
    std::shared_ptr<BaseEnemyType> enemyType;

public:
    Enemy(EnemyTypes type, Core::Physics::Vec2D pos) {
        switch (type) {
        case EnemyTypes::ENEMY_MINOTAUR:
            enemyType = std::make_shared<Minotaur>();
            break;
            
        default:
            throw std::runtime_error("Unknown enemy type");
        }
        enemyType->GetEntityInstance()->GetComponent<CTransform>()->Position = pos;
    }

    void Update() {
        enemyType->Update();
    }


    CAnimator& GetCurrentAnimator() const {
        return this->enemyType->GetCurrentAnimator();
    };

    ~Enemy() = default;
};
