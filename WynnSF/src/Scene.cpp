#include "Scene.hpp"



	void Scene::init_externals() {
		switch (_id) {
		case SCENES::SCENE_MENU:
			break;
		case SCENES::SCENE_KIT_SELECTION:
			break;
		case SCENES::SCENE_RAGNI:
			_externalScenes->InitExternalScenes(SCENES::SCENE_PIGMANS, SCENES::SCENE_PIGMANS, SCENES::SCENE_PIGMANS, SCENES::SCENE_PIGMANS);
			break;
		case SCENES::SCENE_PIGMANS:
			_externalScenes->InitExternalScenes(SCENES::SCENE_RAGNI, SCENES::SCENE_RAGNI, SCENES::SCENE_RAGNI, SCENES::SCENE_RAGNI);
			break;
		case SCENES::SCENE_DETLAS:
			break;
		case SCENES::SCENE_ALMUJ:
			break;
		case SCENES::SCENE_QUIT:
			break;
		default:
			break;
		}
	}

	void Scene::parse_scene_data() {
		std::ifstream file(_scenePath);
		if (!file.is_open()) {
			std::cerr << "Unable to open file: " << _scenePath << std::endl;
			return;
		}

		std::string line;
		std::string b;
		std::string t;
		bool onB = false;
		bool onT = false;



		while (std::getline(file, line)) {
			if (line == " ") {
				continue;
			}

			if (line == "b") {
				onB = true;
				onT = false;
				continue;
			}

			if (line == "t") {
				onT = true;
				onB = false;
				continue;
			}

			if (onB) {
				b += line;
			}
			else {
				t += line;
			}
		}

		std::vector<int> row;
		for (size_t i = 0; i < b.length(); i++) {
			if (b[i] == ' ') {
				continue;
			}
			if (b[i] == 'e') {
				_scenebl.push_back(row);
				row.clear();
			}
			else {
				if (b[i + 1] != ' ') {
					std::string numStr;
					while (b[i] != ' ') {
						numStr += b[i];
						i++;
					}
					int num = std::stoi(numStr);
					row.push_back(num);
				}
				else {
					row.push_back(b[i] - '0');
				}
			}
		}


		for (size_t i = 0; i < t.length(); i++) {
			if (t[i] == ' ') {
				continue;
			}

			if (t[i] == 'e') {
				_scenetl.push_back(row);
				row.clear();
			}
			else {
				if (t[i + 1] != ' ') {
					std::string numStr;
					while (t[i] != ' ') {
						numStr += t[i];
						i++;
					}
					int num = std::stoi(numStr);
					row.push_back(num);
				}
				else {
					row.push_back(t[i] - '0');
				}


			}
		}

		// TO DO fix bug where side is 0 in scenemanagers update
		for (size_t r = 0; r < _scenetl.size(); r++) {
			for (size_t c = 0; c < _scenetl[r].size(); c++) {
				if (_scenetl[r][c] == 0) {
					sf::Vector2f pos(c * 128, r * 128);
					sf::Vector2f size(128, 128);

					if (r == 0) {
						_entrances.push_back({ SCENE_SIDES::SIDE_TOP, pos, size });
						continue;
					}
					else if (r == _scenetl.size() - 1) {
						_entrances.push_back({ SCENE_SIDES::SIDE_BOTTOM, pos, size });
						continue;
					}
					else if (c == 0) {

						_entrances.push_back({ SCENE_SIDES::SIDE_LEFT, pos, size });
						continue;

					}
					else if (c == _scenetl[r].size() - 1) {

						_entrances.push_back({ SCENE_SIDES::SIDE_RIGHT, pos, size });
						continue;
					}



				}
			}
		}





		file.close();


	}



	void Scene::load_scene_base_layer() {
		for (size_t i = 0; i < _scenebl.size(); i++) {

			for (size_t j = 0; j < _scenebl[i].size(); j++) {
				switch (_scenebl[j][i]) {
				case 0:
				{
					auto grassTile = EntityManager::GetInstance()->AddEntity(_sceneBLUniqueKey);
					auto sc = grassTile->AddComponent<CSprite>(_blTiles->TXGrassSetPath, _blTiles->grass, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 1:
				{
					auto flowerTile = EntityManager::GetInstance()->AddEntity(_sceneBLUniqueKey);

					auto sc = flowerTile->AddComponent<CSprite>(_blTiles->TXGrassSetPath, _blTiles->flower, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 2:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(_sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(_blTiles->TXGrassSetPath, _blTiles->stone_path, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);

				}
				break;
				case 3:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(_sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(_blTiles->TXGrassSetPath, _blTiles->stone_path2, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 4:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(_sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(_blTiles->TXStoneGroundSetPath, _blTiles->stone_ground, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;

				}
			}
		}
	}

	void Scene::load_scene_top_layer() {
		for (size_t i = 0; i < _scenetl.size(); i++) {
			for (size_t j = 0; j < _scenetl[i].size(); j++) {
				switch (_scenetl[j][i]) {

				case 1:
				{
					auto leftWall = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = leftWall->AddComponent<CSprite>(_tlTiles->TXWallSetPath, _tlTiles->leftWall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					leftWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 2:
				{
					auto rightWall = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = rightWall->AddComponent<CSprite>(_tlTiles->TXWallSetPath, _tlTiles->rightWall, 128, 128);
					sc->sprite.setPosition((i * 128) + 50, j * 128);
					rightWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);


				}
				break;

				case 4:
				{
					auto wallTile = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = wallTile->AddComponent<CSprite>(_tlTiles->TXWallSetPath, _tlTiles->wall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					wallTile->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 5:
				{
					auto cornerWall = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = cornerWall->AddComponent<CSprite>(_tlTiles->TXWallSetPath, _tlTiles->cornerWall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					cornerWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 6:
				{
					auto grassPatchTile = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = grassPatchTile->AddComponent<CSprite>(_tlTiles->TXPlantSetPath, _tlTiles->grass_patch, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 7:
				{
					auto tree1 = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = tree1->AddComponent<CSprite>(_tlTiles->TXPlantSetPath, _tlTiles->tree1, 256, 256);
					sc->sprite.setPosition(i * 128, j * 128);
					tree1->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 8:
				{
					auto bushSmall = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = bushSmall->AddComponent<CSprite>(_tlTiles->TXPlantSetPath, _tlTiles->bushSmall, 64, 64);
					sc->sprite.setPosition(i * 128, j * 128);
					bushSmall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 9:
				{
					auto bushBig = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = bushBig->AddComponent<CSprite>(_tlTiles->TXPlantSetPath, _tlTiles->bushBig, 75, 75);
					sc->sprite.setPosition(i * 128, j * 128);
					bushBig->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				case 10:
				{
					auto house1 = EntityManager::GetInstance()->AddEntity(_sceneTLUniqueKey);
					auto sc = house1->AddComponent<CSprite>(_tlTiles->TXSVSet, _tlTiles->house1, 256, 256);
					sc->sprite.setPosition(i * 128, j * 128);
					house1->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);

				}
				break;
				default:
					break;
				}
			}
		}
	}



	void Scene::load_scene() {
		load_scene_base_layer();
		load_scene_top_layer();
	}






	Scene::Scene(SCENES sceneId, const std::string& scenePath) {
		this->_id = sceneId;
		this->_scenePath = scenePath;
		this->_sceneBLUniqueKey = scenePath + "bl";
		this->_sceneTLUniqueKey = scenePath + "tl";
		_blTiles = std::make_unique<TXBaseLayerTiles>();
		_tlTiles = std::make_unique<TXTopLayerTiles>();

		_externalScenes = std::make_shared<ExternalScenes>();

		init_externals();
		parse_scene_data();
		load_scene();


	};


	std::vector<Entrance> Scene::GetEntranceVector() const {
		return _entrances;
	}

	void Scene::RenderScene(sf::RenderWindow* ctx) const {
		//render base layer
		if (this->_id == SCENES::SCENE_MENU || this->_id == SCENES::SCENE_KIT_SELECTION || this->_id == SCENES::SCENE_QUIT) {
			return;
		}

		auto _blTiles = EntityManager::GetInstance()->GetEntities(_sceneBLUniqueKey);
		for (auto& blt : _blTiles) { //im hungry can u tell
			ctx->draw(blt->GetComponent<CSprite>()->sprite);
		}

		//render top layer
		auto _tlTiles = EntityManager::GetInstance()->GetEntities(_sceneTLUniqueKey);

		for (auto& tlt : _tlTiles) {
			ctx->draw(tlt->GetComponent<CSprite>()->sprite);

		}

	}

	std::shared_ptr<ExternalScenes> Scene::GetExternals() const {
		return this->_externalScenes;
	}

	sf::Vector2f Scene::GetSize() const {
		return { (float)this->_scenetl[0].size() * 128, (float)this->_scenetl.size() * 128 };
	}

	SCENES Scene::GetID() const {
		return _id;
	}

	EntityVec Scene::GetTopLayer() const {
		return EntityManager::GetInstance()->GetEntities(_sceneTLUniqueKey);
	}

	void Scene::CleanScene() {
		auto _blTiles = EntityManager::GetInstance()->GetEntities(_sceneBLUniqueKey);

		for (auto& blt : _blTiles) {
			blt->DestroyEntity();
		}

		auto tiles = EntityManager::GetInstance()->GetEntities(_sceneTLUniqueKey);

		for (auto& t : tiles) {
			t->DestroyEntity();
		}

		_scenetl.clear();
		_scenebl.clear();
	}


	Scene::~Scene() {
		CleanScene();

	};


