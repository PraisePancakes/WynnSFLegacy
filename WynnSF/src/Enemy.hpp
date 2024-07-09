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

class BaseEnemyType {
protected:
    std::shared_ptr<Entity> entity = nullptr;
    std::string name;
    bool _agro = false;
    float _agroRadius = 100;

public:
    BaseEnemyType(const std::string& name, const std::string& spritePath, float health, float agroRadius)
        : name(name), _agroRadius(agroRadius) {
        this->entity = EntityManager::GetInstance()->AddEntity("Enemy");
        this->entity->AddComponent<CHealth>(health);
        this->entity->AddComponent<CTransform>(Core::Physics::Vec2D(0, 0), Core::Physics::Vec2D(0, 0), 0);
        this->entity->AddComponent<CSprite>(spritePath, sf::IntRect(0, 0, 100, 100), 64, 64);
    }

    virtual std::shared_ptr<Entity> GetEntityInstance() = 0;
    virtual void Update() = 0;

    const std::string GetName() const {
        return this->name;
    }

    virtual ~BaseEnemyType() {
        this->entity->DestroyEntity();
    }
};

class Minotaur : public BaseEnemyType {
public:
    Minotaur() : BaseEnemyType("Minotaur", "src/Assets/Sprites/Enemy/Minotaur.png", 150, 100) {}

    std::shared_ptr<Entity> GetEntityInstance() override {
        return this->entity;
    }

    void Update() override {
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

        if (distance < pR + _agroRadius) {
            _agro = true;
        }
        else {
            _agro = false;
        }

        if (_agro) {
            Core::Physics::Vec2D direction(xDiff, yDiff);
            direction.Normalize();
            Core::Physics::Vec2D velocity = direction * ENEMY_SPEED;
            etc->Position.x += velocity.x;
            etc->Position.y += velocity.y;
        }

        auto sc = this->entity->GetComponent<CSprite>();
        sc->sprite.setPosition(etc->Position.x, etc->Position.y);
    }

    ~Minotaur() override {}
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

    sf::Sprite& GetSprite() const {
        return enemyType->GetEntityInstance()->GetComponent<CSprite>()->sprite;
    }

    ~Enemy() = default;
};
