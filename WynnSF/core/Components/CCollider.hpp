#pragma once
#include "../ECS.hpp"
#include <SFML/Graphics.hpp>

class CCollider : public Component<CCollider> {
public:
	float radius = 0.f;
	sf::CircleShape circle;
	CCollider() {};

	CCollider(float r) {
		this->radius = r;
		this->circle.setRadius(r);
		this->circle.setOutlineColor(sf::Color::Red);
		this->circle.setOutlineThickness(2.f);
		this->circle.setFillColor(sf::Color::Transparent);
	};

};