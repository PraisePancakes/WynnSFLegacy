#include "SceneManager.hpp"
#include "../core/Utils/Utils.hpp"

static std::string getSceneName(SCENES id) {
	switch (id) {
	case SCENES::SCENE_RAGNI:
		return "Ragni";
	case SCENES::SCENE_DETLAS:
		return "Detlas";
	case SCENES::SCENE_PIGMANS:
		return "Pigmen's Ravines";
	case SCENES::SCENE_ALMUJ:
		return "Almuj";
	default: return "";
	}
};

std::string SceneManager::getSceneFilePath(SCENES id) {
		
		switch (id) {
		case SCENES::SCENE_RAGNI:
			return "src/Data/Scenes/ragni.txt";
			
		case SCENES::SCENE_DETLAS:
			return "";
		case SCENES::SCENE_PIGMANS:
			return "src/Data/Scenes/pigmans.txt";
		case SCENES::SCENE_ALMUJ:
			return "";
		default:
			return "";
	
		}

	
	};

	void SceneManager::init_table() {
		for (SCENES scene = (SCENES)0; scene < SCENES::SCENE_QUIT; scene = static_cast<SCENES>((size_t)scene + 1)) {
			std::shared_ptr<Scene> s = std::make_shared<Scene>(scene, getSceneFilePath(scene));

			_sceneTable.push_back(s);
		}
	};

	void SceneManager::handle_menu_event() {
		if (_currentSceneToProcess == SCENES::SCENE_MENU) {
			int event = _menu->GetMenuEvents();
			switch (event) {
			case 1:
				_currentSceneToProcess = SCENES::SCENE_KIT_SELECTION;
				break;
			case 2:
				_currentSceneToProcess = SCENES::SCENE_QUIT;
				break;


			}

		}
	}


	void SceneManager::update_transitioning_player() {
		std::vector<Entrance> entranceV = this->_sceneTable[(int)_currentSceneToProcess]->GetEntranceVector();
		Core::Physics::Vec2D plPos = _player->GetPos();

		for (size_t i = 0; i < entranceV.size(); i++) {
			if (plPos.x >= entranceV[i].pos.x && plPos.x <= entranceV[i].pos.x + entranceV[i].size.x) {
				if (plPos.y >= entranceV[i].pos.y && plPos.y <= entranceV[i].pos.y + entranceV[i].size.y) {
					SCENES targetScene = _currentSceneToProcess;
					std::vector<Entrance> targetEntrance = {};
					switch (entranceV[i].side) {
					case SCENE_SIDES::SIDE_LEFT:
					{
						targetScene = this->_sceneTable[(int)_currentSceneToProcess]->GetExternals()->left;
						targetEntrance = this->_sceneTable[(int)targetScene]->GetEntranceVector();
						for (auto& e : targetEntrance) {
							if (e.side == SCENE_SIDES::SIDE_RIGHT) {
								_player->SetPos(e.pos.x - 256, e.pos.y);
							}
						}

					}
						
						
						break;
					case SCENE_SIDES::SIDE_RIGHT:
					{
						targetScene = this->_sceneTable[(int)_currentSceneToProcess]->GetExternals()->right;
						targetEntrance = this->_sceneTable[(int)targetScene]->GetEntranceVector();
						for (auto& e : targetEntrance) {
							if (e.side == SCENE_SIDES::SIDE_LEFT) {
								_player->SetPos(e.pos.x + 256, e.pos.y);
							}
						}
					}
					
						break;
					case SCENE_SIDES::SIDE_TOP:
					{
						targetScene = this->_sceneTable[(int)_currentSceneToProcess]->GetExternals()->top;
						targetEntrance = this->_sceneTable[(int)targetScene]->GetEntranceVector();
						for (auto& e : targetEntrance) {
							if (e.side == SCENE_SIDES::SIDE_BOTTOM) {
								_player->SetPos(e.pos.x, e.pos.y - 256);
							}
						}
					}
									
						break;
					case SCENE_SIDES::SIDE_BOTTOM:
					{
						targetScene = this->_sceneTable[(int)_currentSceneToProcess]->GetExternals()->bottom;
						targetEntrance = this->_sceneTable[(int)targetScene]->GetEntranceVector();
						for (auto& e : targetEntrance) {
							if (e.side == SCENE_SIDES::SIDE_TOP) {
								_player->SetPos(e.pos.x, e.pos.y + 256);
							}
						}
					}
									
						break;
					default:
						break;
					}
					SetScene(targetScene);
					break;
				}
			}
		}
	}

	void SceneManager::init_transition() {
		sf::View v = _ctx->getView();
		sf::Vector2f size = { v.getSize().x , v.getSize().y  };
		this->_transititionOverlay = std::make_shared<sf::RectangleShape>(size);
		_transititionOverlay->setFillColor(sf::Color::Transparent);
		
	};

	void SceneManager::update_transitioning_pos() {
		sf::View v = _ctx->getView();
		sf::Vector2f topLeft = { v.getCenter().x - (v.getSize().x / 2) , v.getCenter().y - (v.getSize().y / 2)};
		this->_transititionOverlay->setPosition(topLeft);
	};

	void SceneManager::update_transitioning_animation() {

			sf::Color color = _transititionOverlay->getFillColor();
			static bool flag = false;
			static int __iter_count = 0;
			const int animation_step = 10;
			
			if (!flag) {
				
				color.a += animation_step;
				__iter_count += animation_step;
				_transititionOverlay->setFillColor(color);
				if (__iter_count >= 255) {
					flag = true;
				}
			}
			else if (flag) {
				color.a -= animation_step;
				__iter_count -= animation_step;
				_transititionOverlay->setFillColor(color);
				if (__iter_count <= 0) {
					_isTransitioning = false;
					flag = false;
				}
			}

		}

	

	void SceneManager::update_transitioning() {
		if (_isTransitioning) {
			_player->Disable();
			update_transitioning_animation();
			update_transitioning_pos();

		}
		else {
			update_transitioning_player();
			_player->Enable();
		}
		
	}

	void SceneManager::Update() {
		if (_currentSceneToProcess == SCENES::SCENE_MENU || _currentSceneToProcess == SCENES::SCENE_KIT_SELECTION || _currentSceneToProcess == SCENES::SCENE_QUIT) {
			return;
		}
		update_transitioning();
		update_introduction();
	}

	 void SceneManager::update_introduction_color() {
		sf::Color currentColor = this->_currentIntroText->text.getFillColor();
		currentColor.a -= .001;
		_currentIntroText->text.setFillColor(currentColor);
	}

	void SceneManager::init_introduction() {
		

		_currentIntroText = nullptr;
		sf::View view = _ctx->getView();
		sf::Vector2f center(view.getCenter().x, view.getCenter().y);
		const std::string sceneName = getSceneName(_currentSceneToProcess);
		this->_currentIntroText = std::make_shared<CText>("Welcome to " + sceneName + "!", "src/Assets/Fonts/RingBearer.TTF", 72, 0, 0, true);
	
	}

	void SceneManager::update_introduction() {
		
			if (this->_currentIntroText != nullptr) {
				update_introduction_color();
				update_introduction_pos();
			}
		
		
	};

	void SceneManager::update_introduction_pos() {
		sf::View view = _ctx->getView();
		sf::Vector2f pos(view.getCenter().x, view.getCenter().y - 200);
		std::string str = _currentIntroText->text.getString();
		
		this->_currentIntroText->text.setPosition(pos.x, pos.y);
		
	}
	

	SceneManager::SceneManager(EnemyManager* _em, sf::RenderWindow* _ctx, Player* _player) {
		this->_ctx = _ctx;
		this->_player = _player;
		this->_kitSelection = std::make_unique<KitSelection>(_ctx);
		_menu = std::make_unique<Menu>(_ctx);
		this->_em = _em;
		init_table();
		
		
	};

	void SceneManager::handle_kit_selection_event() {
		int type = _kitSelection->HandleEvents();

		switch (type) {
		case 0:

			_player->SetKit(KitTypes::KIT_ARCHER);
			SetScene(SCENES::SCENE_RAGNI);
			break;
		case 1:
			_player->SetKit(KitTypes::KIT_ASSASSIN);
			SetScene(SCENES::SCENE_RAGNI);
			break;
		case 2:
			_player->SetKit(KitTypes::KIT_WARRIOR);
			SetScene(SCENES::SCENE_RAGNI);
			break;
		case 3:
			_player->SetKit(KitTypes::KIT_WIZARD);
			SetScene(SCENES::SCENE_RAGNI);
			break;
		default:

			break;
		}
		
	}


	void SceneManager::HandleEvents(sf::Event* e) {
		if (_currentSceneToProcess == SCENES::SCENE_MENU) {
			handle_menu_event();
		}
		else if(_currentSceneToProcess == SCENES::SCENE_KIT_SELECTION) {
			//handle scene events if there are any
			handle_kit_selection_event();

		  }
	
	};

	void SceneManager::SetScene(SCENES scene) {
		_em->ClearEnemies();
		this->_currentSceneToProcess = scene;

		if (_currentSceneToProcess == SCENES::SCENE_MENU || _currentSceneToProcess == SCENES::SCENE_KIT_SELECTION || _currentSceneToProcess == SCENES::SCENE_QUIT) {
			return;
		}
		/*
			@TEST
			multiple enemy spawns with 150 radius and 100 hp.
			convert all similar traits to derive from a enemy type enum :
			e.g..

			type {
				ENEMY_MINOTAUR,
				ENEMY_WITCH,
				etc..
			}

			from that enum type we can plug it into AddEnemy().

			instead of passing all these arguments we can create a data set of these arguments for specified enemy type

			ENEMY_MINOTAUR data set => 
			animator path : "src/Assets/Sprites/Enemy/Minotaur.png", 
			animator src rect : sf::IntRect(0, 0, 100, 100), 
			enemy name : "Minotaur", 
			agro radius : 150, 
			hp : 100, 
			pos : Core::Physics::Vec2D(300, 300)

			convert sprite to animator and implement animation states refer to kit module for better understanding

		
		*/
		_em->AddEnemy(ENEMY_TYPES::ENEMY_MINOTAUR, Core::Physics::Vec2D(300, 300));
		_em->AddEnemy(ENEMY_TYPES::ENEMY_MINOTAUR, Core::Physics::Vec2D(600, 300));
		_em->AddEnemy(ENEMY_TYPES::ENEMY_MINOTAUR, Core::Physics::Vec2D(100, 300));
		_isTransitioning = true;
		init_introduction();
		init_transition();
		//log welcome
	}

	void SceneManager::render_introduction() {		
		_ctx->draw(this->_currentIntroText->text);
	}

	void SceneManager::RenderScene() {
		if (this->_currentSceneToProcess == SCENES::SCENE_MENU) {
			_menu->Render();
			return;
		}
		else if (this->_currentSceneToProcess == SCENES::SCENE_KIT_SELECTION) {
			_kitSelection->Render();
		}
		else {
			
			_sceneTable[(int)_currentSceneToProcess]->RenderScene(_ctx);
			if (_isTransitioning) {
				_ctx->draw(*this->_transititionOverlay);
			}
			
			render_introduction();
		}

	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene() const {
		return _sceneTable[(int)_currentSceneToProcess];
	}


	SceneManager::~SceneManager() {};