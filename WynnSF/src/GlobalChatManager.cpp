#include "GlobalChatManager.hpp"
#include "../core/Components/CSprite.hpp"



std::mutex GUI::GlobalChatManager::_mut;
GUI::GlobalChatManager* GUI::GlobalChatManager::_pinstance{ nullptr };


GUI::GlobalChatManager::GlobalChatManager() {
	_chatBox.setSize(sf::Vector2f(500, 150));
	
	sf::Color fill(0, 0, 0, 100);
	_chatBox.setOutlineColor(sf::Color::Black);
	_chatBox.setOutlineThickness(2);

	_chatBox.setFillColor(fill);

	_chatScroller.setPointCount(10);
	_chatScroller.setRadius(4);
	_chatScroller.setFillColor(sf::Color::Red);
	_chatScroller.setOutlineColor(sf::Color::White);
	_chatScroller.setOutlineThickness(2);

	auto ui_chatLogBanner = EntityManager::GetInstance()->AddEntity("UI_ChatLogBanner");
	ui_chatLogBanner->AddComponent<CSprite>("src/Assets/UI/UI_ChatLog.png", sf::IntRect(0, 0, 645, 214), 500, 100);

};


GUI::GlobalChatManager& GUI::GlobalChatManager::GetInstance() {
	
	std::lock_guard<std::mutex> lock(_mut);

	if (_pinstance == nullptr) {
		_pinstance = new GlobalChatManager();

	}
	return *_pinstance;

};


void GUI::GlobalChatManager::Log(const std::string& str) {
		//push front

		auto logs = EntityManager::GetInstance()->GetEntities("ChatLog");
		if (logs.size() >= CHAT_LOG_MAX_SIZE) {
			std::shared_ptr<Entity> e = EntityManager::GetInstance()->GetEntities("ChatLog")[0];
			e->DestroyEntity();
			_max = _max;
		}
		else {
			_max++;
		}
		std::shared_ptr<Entity> log = EntityManager::GetInstance()->AddEntity("ChatLog");
		log->AddComponent<CText>(str, "src/Assets/Fonts/PixelFont.ttf", 24, 0, 0, false);
		
		
		_min = std::max(0, _max - 5);
};

void GUI::GlobalChatManager::Update( sf::RenderWindow* ctx) {

		sf::Vector2f viewCenter = ctx->getView().getCenter();
		sf::Vector2f viewSize = ctx->getView().getSize();
		sf::Vector2f bottomLeft(viewCenter.x - (viewSize.x / 2), viewCenter.y + viewSize.y / 2);
		_chatBox.setPosition(bottomLeft.x, bottomLeft.y - 150);
		_chatScroller.setPosition(_chatBox.getPosition().x + 495, (bottomLeft.y) - (_min * 50));
		std::shared_ptr<Entity> ui_banner_entity = EntityManager::GetInstance()->GetEntities("UI_ChatLogBanner")[0];
		auto banner = ui_banner_entity->GetComponent<CSprite>();
		banner->sprite.setPosition(bottomLeft.x + (_chatBox.getSize().x / 2), bottomLeft.y - (_chatBox.getSize().y) - 50);

		if (_max > 0) {
			for (size_t i = _min; i < _max; i++) {
				std::shared_ptr<Entity> e = EntityManager::GetInstance()->GetEntities("ChatLog")[i];
				auto txt = e->GetComponent<CText>();
				txt->text.setPosition(bottomLeft.x + 50, (bottomLeft.y - 35) - ((i - _min) * 30));
				
			}

		}
		
};

void GUI::GlobalChatManager::Render( sf::RenderWindow* ctx) {
	std::shared_ptr<Entity> ui_banner_entity = EntityManager::GetInstance()->GetEntities("UI_ChatLogBanner")[0];
	auto banner = ui_banner_entity->GetComponent<CSprite>();

	ctx->draw(banner->sprite);
	ctx->draw(_chatBox);
	ctx->draw(_chatScroller);
	for (size_t i = _min; i < _max; i++) {
		std::shared_ptr<Entity> e = EntityManager::GetInstance()->GetEntities("ChatLog")[i];
		auto txt = e->GetComponent<CText>();
		ctx->draw(txt->text);
		

	}


};

void GUI::GlobalChatManager::HandleScrollEvent(sf::Event* e, sf::RenderWindow* ctx) {

	auto logs = EntityManager::GetInstance()->GetEntities("ChatLog");
	if (logs.size() > 5) {
		sf::Vector2i mousepixelpos = sf::Mouse::getPosition(*ctx);
		sf::Vector2f mouseworldpos = ctx->mapPixelToCoords(mousepixelpos);
		
		if (_chatBox.getGlobalBounds().contains(mouseworldpos)) {
			
			if (e->type == sf::Event::MouseWheelScrolled) {
				if (e->mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					//hande sliding 
					if (e->mouseWheelScroll.delta > 0) {
						//slide right
						if (_max >= logs.size()) {
							_max = _max;
							_min = _min;
						}
						else {
							_max++;
							_min++;
						}

					}
					else if (e->mouseWheelScroll.delta < 0) {
						//slide left
						if (_min <= 0) {
							_max = _max;
							_min = _min;
						}
						else {
							_min--;
							_max--;
						}


					}



				}

			}
		}
		
		//handle drag scroll

			
		
	}

}


GUI::GlobalChatManager::~GlobalChatManager() {};