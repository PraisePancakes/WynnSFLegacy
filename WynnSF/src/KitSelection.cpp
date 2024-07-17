#include "KitSelection.hpp"




	void KitSelection::init_kit_selection_sprites() {
		std::shared_ptr<Entity> archerKit = EntityManager::GetInstance()->AddEntity("KitSelection");
		std::shared_ptr<Entity> assassinKit = EntityManager::GetInstance()->AddEntity("KitSelection");
		std::shared_ptr<Entity> warriorKit = EntityManager::GetInstance()->AddEntity("KitSelection");
		std::shared_ptr<Entity> wizardKit = EntityManager::GetInstance()->AddEntity("KitSelection");


		auto archerAc = archerKit->AddComponent<CAnimator>("src/Assets/Sprites/Player/Archer/Sprites/Character/Idle.png", sf::IntRect(0, 0, 100, 100), 900, 100);
		auto assassinAc = assassinKit->AddComponent<CAnimator>("src/Assets/Sprites/Player/Assassin/Sprites/Character/Idle.png", sf::IntRect(0, 0, 200, 200), 600, 200);
		auto warriorAc = warriorKit->AddComponent<CAnimator>("src/Assets/Sprites/Player/Warrior/Sprites/Character/Idle.png", sf::IntRect(0, 0, 162, 162), 1458, 162);
		auto wizardAc = wizardKit->AddComponent<CAnimator>("src/Assets/Sprites/Player/Wizard/Sprites/Character/Idle.png", sf::IntRect(0, 0, 230, 190), 925, 230);

		archerAc->ScaleToNxN(256, 256);
		assassinAc->ScaleToNxN(356, 356);
		warriorAc->ScaleToNxN(356, 356);
		wizardAc->ScaleToNxN(256, 256);

		archerAc->sprite.setPosition((float)_ctx->getSize().x / 2 + SPRITE_END, (float)_ctx->getSize().y / 2);
		assassinAc->sprite.setPosition((float)_ctx->getSize().x / 2 + SPRITE_END - 256, (float)_ctx->getSize().y / 2);
		warriorAc->sprite.setPosition((float)_ctx->getSize().x / 2 + SPRITE_END - (256 * 2), (float)_ctx->getSize().y / 2);
		wizardAc->sprite.setPosition((float)_ctx->getSize().x / 2 + SPRITE_END - (256 * 3), (float)_ctx->getSize().y / 2);

		auto archerTxt = archerKit->AddComponent<CText>("Archer", "src/Assets/Fonts/PixelFont.ttf", 24, archerAc->sprite.getPosition().x, archerAc->sprite.getPosition().y - 100, true);
		auto assassinTxt = assassinKit->AddComponent<CText>("Assassin", "src/Assets/Fonts/PixelFont.ttf", 24, assassinAc->sprite.getPosition().x, assassinAc->sprite.getPosition().y - 100, true);
		auto warriorTxt = warriorKit->AddComponent<CText>("Warrior", "src/Assets/Fonts/PixelFont.ttf", 24, warriorAc->sprite.getPosition().x, warriorAc->sprite.getPosition().y - 100, true);
		auto wizardTxt = wizardKit->AddComponent<CText>("Wizard", "src/Assets/Fonts/PixelFont.ttf", 24, wizardAc->sprite.getPosition().x, wizardAc->sprite.getPosition().y - 100, true);
	}

	void KitSelection::init_kit_selection_buttons() {
		std::shared_ptr<Entity> archerKit = EntityManager::GetInstance()->GetEntities("KitSelection")[0];
		std::shared_ptr<Entity> assassinKit = EntityManager::GetInstance()->GetEntities("KitSelection")[1];
		std::shared_ptr<Entity> warriorKit = EntityManager::GetInstance()->GetEntities("KitSelection")[2];
		std::shared_ptr<Entity> wizardKit = EntityManager::GetInstance()->GetEntities("KitSelection")[3];

		auto archerAc = archerKit->GetComponent<CAnimator>();
		auto assassinAc = assassinKit->GetComponent<CAnimator>();
		auto warriorAc = warriorKit->GetComponent<CAnimator>();
		auto wizardAc = wizardKit->GetComponent<CAnimator>();

		// Button dimensions
		const sf::Vector2f buttonSize(256, 256);

		// Calculating the new positions to center the sprite inside the button
		const sf::Vector2f archerAcPos = archerAc->sprite.getPosition() - buttonSize / 2.f;
		const sf::Vector2f assassinAcPos = assassinAc->sprite.getPosition() - buttonSize / 2.f;
		const sf::Vector2f warriorAcPos = warriorAc->sprite.getPosition() - buttonSize / 2.f;
		const sf::Vector2f wizardAcPos = wizardAc->sprite.getPosition() - buttonSize / 2.f;

		// Creating the button shapes
		const sf::RectangleShape archerAcBtnShape(buttonSize);
		const sf::RectangleShape assassinAcBtnShape(buttonSize);
		const sf::RectangleShape warriorAcBtnShape(buttonSize);
		const sf::RectangleShape wizardAcBtnShape(buttonSize);

		// Adding the buttons to the kits
		archerKit->AddComponent<CButton>(archerAcBtnShape, archerAcPos, sf::Color::White, sf::Color::Transparent);
		assassinKit->AddComponent<CButton>(assassinAcBtnShape, assassinAcPos, sf::Color::White, sf::Color::Transparent);
		warriorKit->AddComponent<CButton>(warriorAcBtnShape, warriorAcPos, sf::Color::White, sf::Color::Transparent);
		wizardKit->AddComponent<CButton>(wizardAcBtnShape, wizardAcPos, sf::Color::White, sf::Color::Transparent);
	}

	KitSelection::KitSelection(sf::RenderWindow* _ctx) {
		this->_ctx = _ctx;

		std::shared_ptr<Entity> title = EntityManager::GetInstance()->AddEntity("KitSelection-Title");
		auto titleText = title->AddComponent<CText>("Select A Kit", "src/Assets/Fonts/PixelFont.ttf", 72, (float)_ctx->getSize().x / 2, 100, true);

	
		init_kit_selection_sprites();
		init_kit_selection_buttons();


	};


	int KitSelection::HandleEvents() const {
		auto kits = EntityManager::GetInstance()->GetEntities("KitSelection");
		
		int kit_idx = -1;
		bool selected = false;

		for (int i = 0; i < kits.size(); ++i) {
			auto& kit = kits[i];
			auto ac = kit->GetComponent<CAnimator>();
			auto btn = kit->GetComponent<CButton>();

			btn->OnHover(_ctx, [btn]() {
				btn->buttonRect.setOutlineColor(sf::Color::Green);
				});

			btn->OnClick(_ctx, [this , &i, &selected, &kit_idx]() {
					
					selected = true;
					selected = true;
					kit_idx = i;
				});
		}

		if (selected) {
			
			return kit_idx;
		}
		
		return -1;

	};

	void KitSelection::Render()
	{
		auto kitSprites = EntityManager::GetInstance()->GetEntities("KitSelection");
		std::shared_ptr<Entity> title = EntityManager::GetInstance()->GetEntities("KitSelection-Title")[0];


		auto titleText = title->GetComponent<CText>();
		for (auto& kit : kitSprites) {
			auto ac = kit->GetComponent<CAnimator>();
			auto txt = kit->GetComponent<CText>();
			auto btn = kit->GetComponent<CButton>();
			_ctx->draw(btn->buttonRect);
			_ctx->draw(ac->sprite);
			_ctx->draw(txt->text);
			ac->Play(.2f);
		}

		_ctx->draw(titleText->text);
	};

	KitSelection::~KitSelection() {};
