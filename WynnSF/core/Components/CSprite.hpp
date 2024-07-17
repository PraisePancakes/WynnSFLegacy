#pragma once
#include "../ECS.hpp"
#include <SFML/Graphics.hpp>


//static sprite use only, dynamic sprites (animators) used by CAnimator component
class CSprite : public Component<CSprite> {



public:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::IntRect srcRect;

	CSprite() {};
	CSprite(const std::string& spritePath, sf::IntRect srcRect, const int finalDimensionX, const int finalDimensionY) {

		if (!this->texture.loadFromFile(spritePath)) {
			std::cerr << "Error opening file at path : " << spritePath << std::endl;
		};

		sprite.setTexture(this->texture);
		sprite.setTextureRect(srcRect);
		this->srcRect = srcRect;
		
		sf::FloatRect gb = sprite.getGlobalBounds();

		
		ScaleToNxN(finalDimensionX, finalDimensionY);
		sprite.setOrigin(gb.width / 2, gb.height / 2);
	};


	void ScaleToNxN(const int nx, const int ny) {
		const int MAX_DIM_X = nx;
		const int MAX_DIM_Y = ny;

		float w = srcRect.getSize().x;
		float h = srcRect.getSize().y;
		float wScale = 0;
		float hScale = 0;

		wScale = (MAX_DIM_X / w);
		hScale = (MAX_DIM_Y / h);


		float newWidth = 1 * wScale;
		float newHeight = 1 * hScale;
		
		sprite.setScale(newWidth, newHeight);

	}

	~CSprite() = default;
};