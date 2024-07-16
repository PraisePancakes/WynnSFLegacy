#include "GUIManager.hpp"



	void GUIManager::init_health_bar_ui() {
		
		_healthBarContainer.setFillColor(sf::Color::Black);

		_healthBarContainer.setSize(sf::Vector2f(200, 20));

		_healthBar.setSize(sf::Vector2f(200, 20));
		_healthBar.setFillColor(sf::Color::Green);

	}


	GUIManager::GUIManager(sf::RenderWindow* _ctx) {
		this->_ctx = _ctx;
		//get players max health. set health container to be a set size with health bars e.g..
		// [|| || || || || ||    ] where || is a health bar etc..

		init_health_bar_ui();



	};

	void GUIManager::update_health_bar_ui() {


		sf::View view = _ctx->getView();
		const sf::Vector2f center = view.getCenter();


		const std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
		const auto healthTxtC = healthE->GetComponent<CText>();
		const auto healthC = healthE->GetComponent<CHealth>();

		if (healthC->CurrHp <= 0) {
			healthC->CurrHp = 0;
		}
		const int scaledHP = healthC->CurrHp << 1;


		const sf::Vector2f topRight(((center.x + (view.getSize().x / 2)) - _healthBarContainer.getSize().x) - 25, (center.y - (view.getSize().y / 2)) + _healthBarContainer.getSize().y);

		_healthBarContainer.setPosition(topRight.x, topRight.y);
		_healthBar.setPosition(_healthBarContainer.getPosition().x, _healthBarContainer.getPosition().y);
		_healthBar.setSize(sf::Vector2f(scaledHP, 20));

		const int YELLOW_MIN = 70 << 1;
		const int YELLOW_MAX = 50 << 1;
		const int RED_MAX = 50;

		if (scaledHP <= YELLOW_MIN && scaledHP > YELLOW_MAX) {
			_healthBar.setFillColor(sf::Color::Yellow);
		}
		else if (scaledHP <= RED_MAX) {
			_healthBar.setFillColor(sf::Color::Red);
		}

		healthTxtC->text.setPosition(_healthBarContainer.getPosition().x - 28, _healthBarContainer.getPosition().y - 5);
		healthTxtC->text.setString(std::to_string(healthC->CurrHp));
	}

	void GUIManager::render_health_bar_ui() {
		const std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
		const auto healthTxtC = healthE->GetComponent<CText>();


		_ctx->draw(_healthBarContainer);
		_ctx->draw(_healthBar);
		_ctx->draw(healthTxtC->text);
	}

	void GUIManager::render_chat_ui() const {
		GUI::GlobalChatManager::GetInstance().Render(_ctx);
	}

	void GUIManager::update_chat_ui() const {
		GUI::GlobalChatManager::GetInstance().Update(_ctx);
	}

	void GUIManager::Update() {
		if (_ctx) {
			update_health_bar_ui();
			update_chat_ui();
		}
		else
			std::cout << "window _ctx not initialzied for updating in GUIManager" << std::endl;
	};

	void GUIManager::Render() {

		if (_ctx) {
			render_chat_ui();
			render_health_bar_ui();
		}
		else
			std::cout << "window _ctx not initialzied for rendering in GUIManager" << std::endl;


	};

	void GUIManager::HandleEvents(sf::Event* e) {
		if (_ctx)
			GUI::GlobalChatManager::GetInstance().HandleScrollEvent(e, _ctx);
		else
			std::cout << "window _ctx not initialzied for handling events in GUIManager" << std::endl;

	};


	GUIManager::~GUIManager() {};
