#pragma once
#include "QuestBook.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Components/CText.hpp"
#include "../core/Components/CButton.hpp"

	void QuestBook::update_questbook_sprite_pos() {
		sf::View view = _ctx->getView();
		std::shared_ptr<Entity> book = EntityManager::GetInstance()->GetEntities("QuestBook")[0];

		auto sc = book->GetComponent<CSprite>();

		const sf::Vector2f center = view.getCenter();
		const sf::Vector2f topLeft((center.x - (view.getSize().x / 2)) + SPRITE_BOOK_PADDING, (center.y - (view.getSize().y / 2)) + SPRITE_BOOK_PADDING);
		const sf::Vector2f bookSpriteOffset(32, 32);

		sc->sprite.setPosition(topLeft.x + bookSpriteOffset.x, topLeft.y + bookSpriteOffset.y);
	}

	void QuestBook::update_return_button_pos() {
		sf::View view = _ctx->getView();
		std::shared_ptr<Entity> close = EntityManager::GetInstance()->GetEntities("QuestBook-Close")[0];

		auto sc = close->GetComponent<CSprite>();

		const sf::Vector2f center = view.getCenter();
		const sf::Vector2f returnSpriteOffset(32, 32);
		const sf::Vector2f topLeft(((center.x - (view.getSize().x / 2)) + SPRITE_RETURN_PADDING) + returnSpriteOffset.x, ((center.y - (view.getSize().y / 2)) + SPRITE_RETURN_PADDING) + returnSpriteOffset.y);

		sc->sprite.setPosition(topLeft.x, topLeft.y);
	}




	void QuestBook::update_quest_title_pos() {
		auto entityVec = EntityManager::GetInstance()->GetEntities("Quests");

		sf::View view = _ctx->getView();

		int padY = 0;
		for (auto& e : entityVec) {
			auto txt = e->GetComponent<CText>();
			txt->text.setPosition(view.getCenter().x - (view.getSize().x / 2) + 50, (view.getCenter().y - (view.getSize().y / 2) + padY) + 100);
			padY += 100;
		}
	}

	void QuestBook::update_quest_description_pos() {
		auto questVec = EntityManager::GetInstance()->GetEntities("Quests");
		auto descVec = EntityManager::GetInstance()->GetEntities("Quests-Description");

		sf::View view = _ctx->getView();
		size_t _iter = 0;
		int padY = 0;
		int padX = 0;

		for (auto& e : descVec) {

			auto desctxt = e->GetComponent<CText>();
			auto& quest = questVec[_iter];
			auto titletxt = quest->GetComponent<CText>();

			int titleX = titletxt->text.getGlobalBounds().getPosition().x;
			int titleY = titletxt->text.getGlobalBounds().getPosition().y;

			padY = titleY + 50;
			padX = titleX + 20;

			desctxt->text.setPosition(padX, padY);


			_iter++;
		}


	}

	void QuestBook::update_quest_title_status() {
		auto entityVec = EntityManager::GetInstance()->GetEntities("Quests");


		size_t questIdx = 0;

		for (auto& q : this->_db->GetTable()) {
			auto txt = entityVec[questIdx]->GetComponent<CText>();

			if (q.second->state.isCompleted()) {
				txt->text.setFillColor(sf::Color::Green);
			}
			else if (q.second->state.isInProgress()) {
				txt->text.setFillColor(sf::Color::Yellow);
			}
			else if (q.second->state.isLocked()) {
				txt->text.setFillColor(sf::Color::Red);
			}

			questIdx++;
		}


	}

	void QuestBook::update_quest_page_pos() {
		std::shared_ptr<Entity> page = EntityManager::GetInstance()->GetEntities("QuestBook-Page")[0];
		auto pageSc = page->GetComponent<CSprite>();
		sf::View view = _ctx->getView();

		pageSc->sprite.setPosition(view.getCenter().x, view.getCenter().y);


	}

	void QuestBook::update_quest_description() {
		std::map<int, Quest*> table = _db->GetTable();
		auto descVec = EntityManager::GetInstance()->GetEntities("Quests-Description");
		size_t _iter = 0;

		for (auto& q : table) {

			auto& descriptionE = descVec[_iter];

			auto descTxt = descriptionE->GetComponent<CText>();

			descTxt->text.setString(q.second->GetDescription());

			descTxt->text.setFillColor(sf::Color::Black);

			_iter++;
		}


	}

	void QuestBook::update_quest_page_title_pos() {

		sf::View view = _ctx->getView();
		std::shared_ptr<Entity> titleE = EntityManager::GetInstance()->GetEntities("QuestBook-Title")[0];

		std::shared_ptr<CText> txtC = titleE->GetComponent<CText>();

		txtC->text.setPosition(view.getCenter().x - (view.getSize().x / 2) + 100, (view.getCenter().y - (view.getSize().y / 2) + 20));

	}

	void QuestBook::update_questbook_opened() {

		update_quest_page_title_pos();
		update_return_button_pos();
		update_quest_title_pos();
		update_quest_description_pos();
		update_quest_title_status();
		update_quest_description();
		update_quest_page_pos();


		//check if quest is completed, make text green, check if text is in progress, make text yellow, and if locked, make red and display the min lvl required next to it


	}



	void QuestBook::init_title() {
		std::shared_ptr<Entity> title = EntityManager::GetInstance()->AddEntity("QuestBook-Title");
		auto titleTxt = title->AddComponent<CText>("Quest Log", "src/Assets/Fonts/RingBearer.TTF", 48, 0, 0, false);
		titleTxt->text.setFillColor(sf::Color::Black);
	}

	void QuestBook::init_sprites() {
		std::shared_ptr<Entity> book = EntityManager::GetInstance()->AddEntity("QuestBook");
		book->AddComponent<CSprite>("src/Assets/UI/UI_QuestBook.png", sf::IntRect(0, 0, 128, 117), 64, 64);
		book->AddComponent<CButton>(sf::RectangleShape(sf::Vector2f(64, 64)), sf::Vector2f(SPRITE_BOOK_PADDING, SPRITE_BOOK_PADDING), sf::Color::Transparent, sf::Color::Transparent);
		std::shared_ptr<Entity> close = EntityManager::GetInstance()->AddEntity("QuestBook-Close");
		close->AddComponent<CSprite>("src/Assets/UI/UI_Back.png", sf::IntRect(0, 0, 100, 100), 64, 64);
		close->AddComponent<CButton>(sf::RectangleShape(sf::Vector2f(64, 64)), sf::Vector2f(SPRITE_BOOK_PADDING, SPRITE_BOOK_PADDING), sf::Color::Transparent, sf::Color::Transparent);

		std::shared_ptr<Entity> page = EntityManager::GetInstance()->AddEntity("QuestBook-Page");
		auto sc = page->AddComponent<CSprite>("src/Assets/UI/UI_QuestPage.jpg", sf::IntRect(0, 0, 4500, 6000), this->_ctx->getSize().x, this->_ctx->getSize().y);
		const sf::Color pageColor{ 180, 180, 180, 255 };
		sc->sprite.setColor(pageColor);
	}

	void QuestBook::init_ui_quests() {
		std::map<int, Quest*> table = _db->GetTable();

		for (auto& quest : table) {
			auto questE = EntityManager::GetInstance()->AddEntity("Quests");
			auto descriptionE = EntityManager::GetInstance()->AddEntity("Quests-Description");
			auto descTxt = descriptionE->AddComponent<CText>("", font_path, 24, 0, 0, false);
			questE->AddComponent<CText>(quest.second->GetTitle(), font_path, 36, 0, 0, false);

			descTxt->text.setFillColor(sf::Color::Black);
		}
	}


	QuestBook::QuestBook(QuestDB* db, sf::RenderWindow* ctx) {
		this->_db = db;
		this->_ctx = ctx;
		init_sprites();
		init_ui_quests();
		init_title();

	};



	bool QuestBook::IsOpen() const {
		return _opened;
	}

	void QuestBook::HandleEvents() {
		std::shared_ptr<Entity> book = EntityManager::GetInstance()->GetEntities("QuestBook")[0];
		std::shared_ptr<Entity> close = EntityManager::GetInstance()->GetEntities("QuestBook-Close")[0];

		auto openBtn = book->GetComponent<CButton>();
		auto closeBtn = close->GetComponent<CButton>();

		if (!_opened) {
			openBtn->OnClick(_ctx, [this]() {
				this->_opened = true;
				});
		}
		else {
			closeBtn->OnClick(_ctx, [this]() {

				this->_opened = false;
				});
		}


	}


	void QuestBook::Update() {

		if (_opened) {
			update_questbook_opened();
		}
		else {
			update_questbook_sprite_pos();
		}

		//update the books sprite positioning to be top left of the view at all times unless opened



	}

	void QuestBook::Render() {
		std::shared_ptr<Entity> book = EntityManager::GetInstance()->GetEntities("QuestBook")[0];
		std::shared_ptr<Entity> close = EntityManager::GetInstance()->GetEntities("QuestBook-Close")[0];
		std::shared_ptr<Entity> title = EntityManager::GetInstance()->GetEntities("QuestBook-Title")[0];

		auto bookSc = book->GetComponent<CSprite>();
		auto closeSc = close->GetComponent<CSprite>();
		auto questVector = EntityManager::GetInstance()->GetEntities("Quests");
		auto questDescVector = EntityManager::GetInstance()->GetEntities("Quests-Description");
		std::shared_ptr<Entity> page = EntityManager::GetInstance()->GetEntities("QuestBook-Page")[0];
		auto pageSc = page->GetComponent<CSprite>();
		auto titleTxt = title->GetComponent<CText>();


		if (_opened) {
			//render book open relative to view
			_ctx->draw(pageSc->sprite);
			_ctx->draw(closeSc->sprite);
			_ctx->draw(titleTxt->text);
			for (auto& e : questVector) {
				auto txt = e->GetComponent<CText>();
				_ctx->draw(txt->text);
			}

			for (auto& d : questDescVector) {
				auto txt = d->GetComponent<CText>();
				_ctx->draw(txt->text);
			}
		}
		else {
			//render book sprite relative to view
			_ctx->draw(bookSc->sprite);

		}

	};
