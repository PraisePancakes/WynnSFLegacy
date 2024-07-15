#include "GUIManager.hpp"



	void GUIManager::_initHealthBarUI() {
		
		healthBarContainer.setFillColor(sf::Color::Black);

		healthBarContainer.setSize(sf::Vector2f(200, 20));

		healthBar.setSize(sf::Vector2f(200, 20));
		healthBar.setFillColor(sf::Color::Green);

	}


	GUIManager::GUIManager(sf::RenderWindow* ctx) {
		this->ctx = ctx;
		//get players max health. set health container to be a set size with health bars e.g..
		// [|| || || || || ||    ] where || is a health bar etc..

		_initHealthBarUI();



	};

	void GUIManager::updateHealthBarUI() {


		sf::View view = ctx->getView();
		const sf::Vector2f center = view.getCenter();


		const std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
		const auto healthTxtC = healthE->GetComponent<CText>();
		const auto healthC = healthE->GetComponent<CHealth>();

		if (healthC->CurrHp <= 0) {
			healthC->CurrHp = 0;
		}
		const int scaledHP = healthC->CurrHp << 1;


		const sf::Vector2f topRight(((center.x + (view.getSize().x / 2)) - healthBarContainer.getSize().x) - 25, (center.y - (view.getSize().y / 2)) + healthBarContainer.getSize().y);

		healthBarContainer.setPosition(topRight.x, topRight.y);
		healthBar.setPosition(healthBarContainer.getPosition().x, healthBarContainer.getPosition().y);
		healthBar.setSize(sf::Vector2f(scaledHP, 20));

		const int YELLOW_MIN = 70 << 1;
		const int YELLOW_MAX = 50 << 1;
		const int RED_MAX = 50;

		if (scaledHP <= YELLOW_MIN && scaledHP > YELLOW_MAX) {
			healthBar.setFillColor(sf::Color::Yellow);
		}
		else if (scaledHP <= RED_MAX) {
			healthBar.setFillColor(sf::Color::Red);
		}

		healthTxtC->text.setPosition(healthBarContainer.getPosition().x - 28, healthBarContainer.getPosition().y - 5);
		healthTxtC->text.setString(std::to_string(healthC->CurrHp));
	}

	void GUIManager::renderHealthBarUI() {
		const std::shared_ptr<Entity> healthE = EntityManager::GetInstance()->GetEntities("Player-Health")[0];
		const auto healthTxtC = healthE->GetComponent<CText>();


		ctx->draw(healthBarContainer);
		ctx->draw(healthBar);
		ctx->draw(healthTxtC->text);
	}

	void GUIManager::renderChatUI() const {
		GUI::GlobalChatManager::GetInstance().Render(ctx);
	}

	void GUIManager::updateChatUI() const {
		GUI::GlobalChatManager::GetInstance().Update(ctx);
	}

	void GUIManager::Update() {
		if (ctx) {
			updateHealthBarUI();
			updateChatUI();
		}
		else
			std::cout << "window ctx not initialzied for updating in GUIManager" << std::endl;
	};

	void GUIManager::Render() {

		if (ctx) {
			renderChatUI();
			renderHealthBarUI();
		}
		else
			std::cout << "window ctx not initialzied for rendering in GUIManager" << std::endl;


	};

	void GUIManager::HandleEvents(sf::Event* e) {
		if (ctx)
			GUI::GlobalChatManager::GetInstance().HandleScrollEvent(e, ctx);
		else
			std::cout << "window ctx not initialzied for handling events in GUIManager" << std::endl;

	};


	GUIManager::~GUIManager() {};
