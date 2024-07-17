#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../core/Manager/EntityManager.hpp"
#include "../core/Components/CAnimator.hpp"
#include "../core/Components/CButton.hpp"
#include "../core/Components/CText.hpp"
#include "Kit.hpp"

constexpr float SPRITE_END = 360;

class KitSelection {
	sf::RenderWindow* _ctx{ nullptr };

	void init_kit_selection_sprites();
	void init_kit_selection_buttons();

public:
	KitSelection(sf::RenderWindow* ctx);

	int HandleEvents() const;
	void Render();

	~KitSelection();
};