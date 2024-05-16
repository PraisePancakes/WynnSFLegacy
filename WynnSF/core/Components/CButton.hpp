#pragma once
#include "../ECS.hpp"
#include <functional>


class CButton : public Component {

	sf::RectangleShape initialRect;
	bool hovered = false;

public:
	sf::RectangleShape buttonRect;
	CButton(sf::RectangleShape btnRect, sf::Vector2f pos, sf::Color outline, sf::Color fill) : Component(GenComponentTypeID<CButton>()) {
		this->buttonRect = btnRect;
		buttonRect.setOutlineThickness(1);
		buttonRect.setOutlineColor(outline);
		buttonRect.setFillColor(fill);
		buttonRect.setPosition(pos);

		initialRect = buttonRect;
	};

	CButton() : Component(GenComponentTypeID<CButton>()) {};

	CButton(const CButton& other) : Component(GenComponentTypeID<CButton>()){
		this->buttonRect = other.buttonRect;
		this->hovered = other.hovered;
	};

	CButton(sf::RectangleShape btnRect) : Component(GenComponentTypeID<CButton>()) {
		this->buttonRect = btnRect;
	};
	

	void OnHover(sf::RenderWindow* ctx, std::function<void()> callback) {
		sf::Mouse m;

		float mx = m.getPosition(*ctx).x;
		float my = m.getPosition(*ctx).y;

	
		if (mx >= buttonRect.getPosition().x && mx <= buttonRect.getPosition().x + buttonRect.getGlobalBounds().width &&
			my >= buttonRect.getPosition().y && my <= buttonRect.getPosition().y + buttonRect.getGlobalBounds().height) {
			if (!hovered) {
				hovered = true;
				callback(); 
			}
		}
		else {
			if (hovered) {
				hovered = false;
			}
		}

		if (!hovered) {
			this->buttonRect = initialRect;
		}

	
	}

	void OnClick(sf::RenderWindow* ctx, std::function<void()> callback) {
		sf::Mouse m;

		float mx = m.getPosition(*ctx).x;
		float my = m.getPosition(*ctx).y;
		
		if (mx >= buttonRect.getPosition().x && mx <= buttonRect.getPosition().x + buttonRect.getGlobalBounds().width) {
			if (my >= buttonRect.getPosition().y && my <= buttonRect.getPosition().y + buttonRect.getGlobalBounds().height) {
				if(m.isButtonPressed(sf::Mouse::Left)) {
					callback();
				}	

		}
			
			
	}

}


};