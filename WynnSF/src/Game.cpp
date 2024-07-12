#include "Game.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Components/CCollider.hpp"
#include "../core/Components/CTransform.hpp"


//TO-DO make chat log relative to player pos

Game::Game(const std::string & title) {
	m_running = true;
	m_Window.create(sf::VideoMode::VideoMode(WINDOW_W, WINDOW_H), title, sf::Style::Titlebar | sf::Style::Close);
	m_Window.setFramerateLimit(60);
	m_Cam.setSize(WINDOW_W, WINDOW_H);

	spawnPlayer();
	this->m_EnemyManager = std::make_unique<EnemyManager>(&this->m_Window);
	m_SceneManager = std::make_unique<SceneManager>(m_EnemyManager.get(), &this->m_Window, m_Player.get());
	m_SceneManager->SetScene(Scenes::SCENE_MENU);

	m_QuestData = std::make_shared<QuestDB>(m_Player.get());
	m_QuestBook = std::make_unique<QuestBook>(m_QuestData.get(), &this->m_Window);

	this->m_Gui = std::make_unique<GUIManager>(&m_Window);


};


void Game::Run() {

	while (m_running) {
		m_Window.clear();
	
		sUserInput();
		sUpdate();
		
		if (m_SceneManager->GetCurrentScene()->GetID() == Scenes::SCENE_QUIT) {
			m_running = false;
			m_Window.close();
		}
		//handle game events
		if (m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION) {
			sCollider();
			sMovement();
			
		}


		sRenderer();
		EntityManager::GetInstance()->Update();
		m_Window.display();

	}
};


void Game::sUserInput() {
	sf::Event e;

	while (m_Window.pollEvent(e)) {
		if (e.type == sf::Event::EventType::Closed) {
			m_Window.close();
			m_running = false;
		}


		if (m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION) {
			m_Player->HandleInput(&e);
			m_Gui->HandleEvents(&e);
			m_QuestBook->HandleEvents();
			
		}
		


		m_SceneManager->HandleEvents(&e);
	}

};

void Game::sMovement() {
	if (m_QuestBook->IsOpen() || m_SceneManager->IsTransitioning()) return;

	m_Player->HandleMovement();
};

void Game::updateCam() {
	m_Cam.setCenter(m_Player->GetPos().x, m_Player->GetPos().y);
	m_Window.setView(m_Cam);
}

void Game::sUpdate() {
	
	if (m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION ) {
		
		if (!m_QuestBook->IsOpen() ) {
			m_Player->Update();	
			updateCam();
			m_Gui->Update();
			//put gui->supdate in here for gui transition effect
		}	
		m_QuestData->Update();
		m_QuestBook->Update();

		if (!m_SceneManager->IsTransitioning()) {
			m_EnemyManager->Update();
		}
		
	
	}
	m_SceneManager->Update();
}
/*
	since we are using a circle collider, the collision system will work based positioning of each circle collider when collision occurs

	refer to diagrams/collision.png


*/

void Game::checkCollision(Entity& e, EntityVec& ev) {
	float ex = e.GetComponent<CTransform>()->Position.x;
	float ey = e.GetComponent<CTransform>()->Position.y;

	for (auto& entity : ev) {
		if (*entity == e) {
			continue;
		}

		if (entity->HasComponent<CCollider>()) {
			float entityX = 0.f;
			float entityY = 0.f;

			if (entity->HasComponent<CSprite>()) { //top layer tiles
				entityX = entity->GetComponent<CSprite>()->sprite.getPosition().x;
				entityY = entity->GetComponent<CSprite>()->sprite.getPosition().y;
			}
			else if (entity->HasComponent<CTransform>()) { //other entities that arent tiles
				entityX = entity->GetComponent<CTransform>()->Position.x;
				entityY = entity->GetComponent<CTransform>()->Position.y;
			}

			float xDiff = ex - entityX;
			float yDiff = ey - entityY;

			float distance = std::sqrt(xDiff * xDiff + yDiff * yDiff);
			float er = e.GetComponent<CCollider>()->radius;
			float entityR = entity->GetComponent<CCollider>()->radius;

			if (distance < er + entityR) {
				auto tc = e.GetComponent<CTransform>();

				if (xDiff < 0 && std::abs(xDiff) > std::abs(yDiff)) {
					tc->Position.x -= std::abs(e.GetComponent<CTransform>()->Velocity.x);
				}
				else if (xDiff > 0 && std::abs(xDiff) > std::abs(yDiff)) {
					tc->Position.x += std::abs(e.GetComponent<CTransform>()->Velocity.x);
				}
				else if (yDiff < 0 && std::abs(yDiff) > std::abs(xDiff)) {
					tc->Position.y -= std::abs(e.GetComponent<CTransform>()->Velocity.y);
				}
				else if (yDiff > 0 && std::abs(yDiff) > std::abs(xDiff)) {
					tc->Position.y += std::abs(e.GetComponent<CTransform>()->Velocity.y);
				}
			}
		}
	}
}


void Game::sCollider() {
	EntityVec tlTiles = m_SceneManager->GetCurrentScene()->GetTopLayer();
	EntityVec enemies = EntityManager::GetInstance()->GetEntities("Enemy");
	if (enemies.size() > 0) {

		for (auto& enemy : enemies) {
			checkCollision(*enemy, enemies);
			
		}
		
	}
	checkCollision(*m_Player->GetEntityInstance(), enemies);
	checkCollision(*m_Player->GetEntityInstance(), tlTiles);
};


//TEST

//move this to scenemanager
void Game::spawnPlayer() {
	
	float spawnX = WINDOW_W / 2 + 50;
	float spawnY = WINDOW_H / 2 + 100;

	m_Player = std::make_shared<Player>(spawnX, spawnY);

};

void Game::sRenderer() {
	m_SceneManager->RenderScene();

	if (m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION && !m_QuestBook->IsOpen()) {
		if (!m_SceneManager->IsTransitioning()) {
			m_Gui->Render();
		}
		if (!m_SceneManager->IsTransitioning()) {
			m_Player->Render(this->m_Window);
			m_EnemyManager->Render();
		}
		

	}

	
	
	if (m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && m_SceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION && !m_SceneManager->IsTransitioning()) {

		m_QuestBook->Render();
	}

	
};
