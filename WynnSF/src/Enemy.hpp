#pragma once
#include "../core/ECS.hpp"
#include "../core/Components/CHealth.hpp"

/*
	How will Enemys work?
	ultimately we want a table of all the enemys that we can loop through and check if they are within the players agro range, if they are then we set agro to true,
	if agro is true, we can deviate the enemy's position to step closer to the player, if the players collision radius is within the enemys collision radius, the player can attack the enemy
	and vice versa. 
	if the enemy is not agro we can give it a default movement path via a vector of weights where each index is a direction of that path.
	e.g..
	 l  r  u  d
	[1, 5, 1, 2]
	
	the enemy will move 1 unit to the left, then 5 units to the right, followed by 1 unit up, then finally 2 units down.
	keep repeating this 

	depending on the scene the enemies will change, therefore we should clear the enemies and initialize them on scene load.

*/

class Enemy {

private:
	std::shared_ptr<Entity> entity = nullptr;
	std::string name = "";
	bool _agro = false;
	float _agroRadius = 0;
	
public:
	Enemy(const std::string& name, float agroR, int maxHP) {
		this->name = name;
		this->entity = EntityManager::GetInstance()->AddEntity("Enemy"); //this is a table entry for all the enemys of this type
		this->entity->AddComponent<CHealth>(maxHP); //subtract hp by determining if the enemy is hit, if hit, subtract the weapon's hit damage from the enemy's health
		_agro = false;
		_agroRadius = agroR;
	};

	void Update() {
		//check first if player is within the agro radius
		//if the player is within the radius set agro to true
		//then update the enemys positioning to move closer to the player
		//then we check if the players collision is within the enemys collision
		//if true then player can attack the enemy and vice versa
		//each enemy should hold an hitpoint and hurtpoints field.
	};

	void Init() {};
	void Render() {};

	~Enemy() {
		this->entity->DestroyEntity();
	};

};