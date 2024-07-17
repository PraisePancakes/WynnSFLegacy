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
#include "../core/Components/CAnimator.hpp"
#include "../core/Components/CText.hpp"

constexpr float ENEMY_SPEED = 1;

enum class ENEMY_TYPES {
    ENEMY_MINOTAUR,
};


enum class ENEMY_ANIMATION_TYPES {
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

//TO DO : FIX ENEMY HEALTH BAR UI

class BaseEnemyType {
protected:
    std::shared_ptr<Entity> _entity = nullptr;
    std::shared_ptr<Entity> _healthEntity = nullptr;
    std::string _name;
    bool _agro = false;
    float _agroRadius = 100;
    unsigned short int _damageOutput = 0;
    sf::RectangleShape _healthBarContainer;
    sf::RectangleShape _healthBarFill;
    sf::Text _nameRenderable;
    sf::Font _enemyUiFont;
    
public:
    BaseEnemyType(const std::string& name, float health, float agroRadius, unsigned short int damage);

    
    virtual void SetCurrentAnimator(ENEMY_ANIMATION_TYPES animatorType) = 0;
    virtual std::shared_ptr<Entity> GetEntityInstance() = 0;
    virtual void Update() = 0;
    virtual CAnimator& GetCurrentAnimator() = 0;
    virtual void PlayCurrentAnimator(float dt) = 0;
    virtual void Render(sf::RenderWindow* ctx) = 0;


    void DestroyEnemy();

    const std::string GetName() const;

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

    MinotaurAnimatorData _animatorData;
    CAnimator _currentAnimator = _animatorData.idle;
    ENEMY_ANIMATION_TYPES _currentAnimationType = ENEMY_ANIMATION_TYPES::IDLE;
    EnemyState _state;

    void update_healthbar_position();
    void update_healthbar_fill();
    void update_healthbar_text();
    void update_healthbar_ui();
    void update_animator();
    void update_damage_output();
    void update_agro();


    void Render(sf::RenderWindow* ctx) override;

public:
    Minotaur();

    std::shared_ptr<Entity> GetEntityInstance() override;
    void Update() override;
    void SetCurrentAnimator(ENEMY_ANIMATION_TYPES animatorType) override;
    CAnimator& GetCurrentAnimator() override;
    void PlayCurrentAnimator(float dt) override;

    ~Minotaur() override;
};

class Enemy {
private:
    std::shared_ptr<BaseEnemyType> _enemyType;

public:
    Enemy(ENEMY_TYPES type, Core::Physics::Vec2D pos);
    void Update();
    void Render(sf::RenderWindow* ctx);

    CAnimator& GetCurrentAnimator() const;

    ~Enemy();
};
