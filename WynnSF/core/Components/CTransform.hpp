#pragma once
#include "../ECS.hpp"
#include "../Physics/Vec2D.hpp"


class CTransform : public Component<CTransform> {
public:

	Core::Physics::Vec2D Position = { 0.f, 0.f };
	Core::Physics::Vec2D Velocity = { 0.f, 0.f };
	float Angle = 0.f;
	

	CTransform(Core::Physics::Vec2D pos, Core::Physics::Vec2D vel, float ang)
		: Position(pos), Velocity(vel), Angle(ang) {
	
	};

	CTransform() {};
};