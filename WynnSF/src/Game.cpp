#include "Game.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Components/CCollider.hpp"
#include "../core/Components/CTransform.hpp"


//TO-DO make chat log relative to player pos

Game::Game(const std::string & title) {
	_running = true;
	_window.create(sf::VideoMode::VideoMode(WINDOW_W, WINDOW_H), title, sf::Style::Titlebar | sf::Style::Close);
	_window.setFramerateLimit(60);
	_cam.setSize(WINDOW_W, WINDOW_H);

	spawn_player();
	this->_enemyManager = std::make_unique<EnemyManager>(&this->_window);
	_sceneManager = std::make_unique<SceneManager>(_enemyManager.get(), &this->_window, _player.get());
	_sceneManager->SetScene(Scenes::SCENE_MENU);

	_questData = std::make_shared<QuestDB>(_player.get());
	_questBook = std::make_unique<QuestBook>(_questData.get(), &this->_window);

	this->_gui = std::make_unique<GUIManager>(&_window);


};


void Game::Run() {

	while (_running) {
		_window.clear();
	
		s_user_input();
		s_update();
		
		if (_sceneManager->GetCurrentScene()->GetID() == Scenes::SCENE_QUIT) {
			_running = false;
			_window.close();
		}
		//handle game events
		if (_sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && _sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION) {
			s_collider();
			s_movement();
			
		}


		s_renderer();
		EntityManager::GetInstance()->Update();
		_window.display();

	}
};


void Game::s_user_input() {
	sf::Event e;

	while (_window.pollEvent(e)) {
		if (e.type == sf::Event::EventType::Closed) {
			_window.close();
			_running = false;
		}


		if (_sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && _sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION) {
			_player->HandleInput(&e);
			_gui->HandleEvents(&e);
			_questBook->HandleEvents();
			
		}
		


		_sceneManager->HandleEvents(&e);
	}

};

void Game::s_movement() {
	if (_questBook->IsOpen() || _sceneManager->IsTransitioning()) return;

	_player->HandleMovement();
};

void Game::update_cam() {
	_cam.setCenter(_player->GetPos().x, _player->GetPos().y);
	_window.setView(_cam);
}

void Game::s_update() {
	
	if (_sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && _sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION ) {
		
		if (!_questBook->IsOpen() ) {
			_player->Update();	
			update_cam();
			_gui->Update();
			//put gui->s_update in here for gui transition effect
		}	
		_questData->Update();
		_questBook->Update();

		if (!_sceneManager->IsTransitioning()) {
			_enemyManager->Update();
		}
		
	
	}
	_sceneManager->Update();
}
/*
	since we are using a circle collider, the collision system will work based positioning of each circle collider when collision occurs

	refer to diagrams/collision.png


*/

void Game::check_collision(Entity& e, EntityVec& ev) {
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


void Game::s_collider() {
	EntityVec tlTiles = _sceneManager->GetCurrentScene()->GetTopLayer();
	EntityVec enemies = EntityManager::GetInstance()->GetEntities("Enemy");
	if (enemies.size() > 0) {

		for (auto& enemy : enemies) {
			check_collision(*enemy, enemies);
			
		}
		
	}
	check_collision(*_player->GetEntityInstance(), enemies);
	check_collision(*_player->GetEntityInstance(), tlTiles);
};


//TEST

//move this to scenemanager
void Game::spawn_player() {
	
	float spawnX = WINDOW_W / 2 + 50;
	float spawnY = WINDOW_H / 2 + 100;

	_player = std::make_shared<Player>(spawnX, spawnY);

};

void Game::s_renderer() {
	_sceneManager->RenderScene();

	if (_sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && _sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION && !_questBook->IsOpen()) {
		if (!_sceneManager->IsTransitioning()) {
			_gui->Render();
		}
		if (!_sceneManager->IsTransitioning()) {
			_player->Render(this->_window);
			_enemyManager->Render();
		}
		

	}

	
	
	if (_sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_MENU && _sceneManager->GetCurrentScene()->GetID() != Scenes::SCENE_KIT_SELECTION && !_sceneManager->IsTransitioning()) {

		_questBook->Render();
	}

	
};
