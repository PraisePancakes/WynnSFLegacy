#pragma once
#include "../core/ECS.hpp"
#include "../core/Components/CHealth.hpp"
#include "../core/Physics/Vec2D.hpp"
#include "../core/Components/CTransform.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Utils/Utils.hpp"
#include "../core/Components/CCollider.hpp"
#include "../core/Components/CSprite.hpp"

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
        state_agro = false;
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

    
public:
    BaseEnemyType(const std::string& name, float health, float agroRadius)
        : name(name), _agroRadius(agroRadius) {
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
};

class Minotaur : public BaseEnemyType {

    MinotaurAnimatorData animatorData;
    CAnimator currentAnimator = animatorData.idle;
    EnemyAnimationType currentAnimationType = EnemyAnimationType::IDLE;
    EnemyState state;

    void update_animator() {

        EnemyAnimationType newAnimationState = state.state_agro ? EnemyAnimationType::RUN : EnemyAnimationType::IDLE;

        if (newAnimationState != currentAnimationType) {
            SetCurrentAnimator(newAnimationState);
            currentAnimationType = newAnimationState;
          

        }
        PlayCurrentAnimator(.2f);
      
    };

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
                state.SetIdle(); // state.SetAttack();
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
    Minotaur() : BaseEnemyType("Minotaur", 150, 100) { 
        this->currentAnimator.ScaleToNxN(128, 128);
        this->entity->AddComponent<CCollider>(currentAnimator.frameWidth / 2);
       
    }

    std::shared_ptr<Entity> GetEntityInstance() override {
        return this->entity;
    }

   

    void Update() override {
        update_animator();
        update_agro();
       
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
