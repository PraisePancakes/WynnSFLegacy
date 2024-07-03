#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "../core/Manager/EntityManager.hpp"
#include "../core/Components/CSprite.hpp"
#include "../core/Components/CCollider.hpp"
#include <queue>
#include <utility>

enum class Scenes {
	SCENE_MENU,
	SCENE_KIT_SELECTION,
	SCENE_RAGNI,
	SCENE_PIGMANS,
	SCENE_DETLAS,
	SCENE_ALMUJ,

	SCENE_QUIT,
};


enum class Side {
	SIDE_NONE,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_TOP,
	SIDE_BOTTOM
};

struct Entrance {
	Side side = Side::SIDE_NONE;
	sf::Vector2f pos{ 0, 0 };
	sf::Vector2f size{ 0, 0 };

	Entrance() {};
	Entrance(Side side, sf::Vector2f p, sf::Vector2f size) {
		this->side = side;
		this->pos = p;
		this->size = size;
	}


	~Entrance() {};
};

const static std::string padtxBasePath = "src/Assets/Tiles/PATDTx/";
const static std::string svrtxBasePath = "src/Assets/Tiles/SVRTx/";

struct TXTopLayerTiles {
	const std::string TXWallSetPath = padtxBasePath + "TXWallSet.png";
	const std::string TXPlantSetPath = padtxBasePath + "TXPlantSet.png";
	const std::string TXSVSet = svrtxBasePath + "TXSvrSet.png";


	//Plant tiles
	const sf::IntRect grass_patch = { 0, 384, 128, 128 };
	const sf::IntRect tree1 = { 0, 0, 159, 159 };
	const sf::IntRect bushSmall = { 37,195, 25, 25 };
	const sf::IntRect bushBig = { 212, 185, 49, 45 };
	//Wall tiles
	const sf::IntRect wall = { 32, 192, 128, 64 };
	const sf::IntRect cornerWall = { 384, 64, 64, 96 };
	const sf::IntRect leftWall = { 288, 31, 13, 96 };
	const sf::IntRect rightWall = { 344, 31, 13, 96 };

	//Structure tiles
	const sf::IntRect house1 = { 0, 1026, 144, 159 };

};



struct TXBaseLayerTiles {

	const std::string TXGrassSetPath = padtxBasePath + "TXGrassSet.png";
	const std::string TXStoneGroundSetPath = padtxBasePath + "TXStoneGroundSet.png";

	//Ground tiles
	const sf::IntRect grass = { 0, 0, 128, 128 };
	const sf::IntRect flower = { 128, 0, 128, 128 };
	const sf::IntRect stone_path = { 0, 128, 128, 128 };
	const sf::IntRect stone_path2 = { 128, 128, 128, 128 };
	const sf::IntRect stone_ground = { 160, 0, 95, 95 };


};

struct  ExternalScenes {
	Scenes left = Scenes::SCENE_MENU;
	Scenes right = Scenes::SCENE_MENU;
	Scenes top = Scenes::SCENE_MENU;
	Scenes bottom = Scenes::SCENE_MENU;


	ExternalScenes() {};

	void InitExternalScenes(Scenes l, Scenes r, Scenes t, Scenes b) {
		this->left = l;
		this->right = r;
		this->top = t;
		this->bottom = b;
	};
	~ExternalScenes() {};
};



class Scene {
	Scenes m_id;
	std::shared_ptr<ExternalScenes> externalScenes;
	std::string scenePath = "";
	

	std::string sceneBLUniqueKey = "";
	std::string sceneTLUniqueKey = "";



	std::vector<std::vector<int>> scenetl = {};
	std::vector<std::vector<int>> scenebl = {};

	std::unique_ptr<TXBaseLayerTiles> blTiles	{ nullptr };
	std::unique_ptr<TXTopLayerTiles> tlTiles	{ nullptr };

	std::vector<Entrance> entrances;

	


	void initExternals() {
		switch (m_id) {
		case Scenes::SCENE_MENU:
			break;
		case Scenes::SCENE_KIT_SELECTION:
			break;
		case Scenes::SCENE_RAGNI:
			externalScenes->InitExternalScenes(Scenes::SCENE_PIGMANS, Scenes::SCENE_PIGMANS, Scenes::SCENE_PIGMANS, Scenes::SCENE_PIGMANS);
			break;
		case Scenes::SCENE_PIGMANS:
			externalScenes->InitExternalScenes(Scenes::SCENE_RAGNI, Scenes::SCENE_RAGNI, Scenes::SCENE_RAGNI, Scenes::SCENE_RAGNI);
			break;
		case Scenes::SCENE_DETLAS:
			break;
		case Scenes::SCENE_ALMUJ:
			break;
		case Scenes::SCENE_QUIT:
			break;
		default:
			break;
		}
	}

	void parseSceneData() {
		std::ifstream file(scenePath);
		if (!file.is_open()) {
			std::cerr << "Unable to open file: " << scenePath << std::endl;
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
				scenebl.push_back(row);
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
				scenetl.push_back(row);
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
		for (size_t r = 0; r < scenetl.size(); r++) {
			for (size_t c = 0; c < scenetl[r].size(); c++) {
				if (scenetl[r][c] == 0) {
					sf::Vector2f pos(c * 128, r * 128);
					sf::Vector2f size(128, 128);
					
					if (r == 0) {
						entrances.push_back({ Side::SIDE_TOP, pos, size });
						continue;
					}
					else if (r == scenetl.size() - 1) {
						entrances.push_back({ Side::SIDE_BOTTOM, pos, size });
						continue;
					}
					else if (c == 0) {
						
						entrances.push_back({ Side::SIDE_LEFT, pos, size });
						continue;

					}
					else if (c == scenetl[r].size() - 1) {
						
						entrances.push_back({ Side::SIDE_RIGHT, pos, size });
						continue;
					}

				

				}
			}
		}
		

	
		

		file.close();

		
	}

	

	void loadSceneBaseLayer() {
		for (size_t i = 0; i < scenebl.size(); i++) {
			
			for (size_t j = 0; j < scenebl[i].size(); j++) {
				switch (scenebl[j][i]) {
				case 0:
				{	
					auto grassTile = EntityManager::GetInstance()->AddEntity(sceneBLUniqueKey);
					auto sc = grassTile->AddComponent<CSprite>(blTiles->TXGrassSetPath, blTiles->grass, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 1:
				{
					auto flowerTile = EntityManager::GetInstance()->AddEntity(sceneBLUniqueKey);

					auto sc = flowerTile->AddComponent<CSprite>(blTiles->TXGrassSetPath, blTiles->flower, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 2:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(blTiles->TXGrassSetPath, blTiles->stone_path, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);

				}
				break;
				case 3:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(blTiles->TXGrassSetPath, blTiles->stone_path2, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 4:
				{
					auto stoneTile = EntityManager::GetInstance()->AddEntity(sceneBLUniqueKey);
					auto sc = stoneTile->AddComponent<CSprite>(blTiles->TXStoneGroundSetPath, blTiles->stone_ground, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;

				}
			}
		}
	}

	void loadSceneTopLayer() {
		for (size_t i = 0; i < scenetl.size(); i++) {
			for (size_t j = 0; j < scenetl[i].size(); j++) {
				switch (scenetl[j][i]) {

				case 1:
				{
					auto leftWall = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = leftWall->AddComponent<CSprite>(tlTiles->TXWallSetPath, tlTiles->leftWall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					leftWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 2:
				{
					auto rightWall = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = rightWall->AddComponent<CSprite>(tlTiles->TXWallSetPath, tlTiles->rightWall, 128, 128);
					sc->sprite.setPosition((i * 128) + 50, j * 128);
					rightWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
			
				}
				break;

				case 4:
				{
					auto wallTile = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = wallTile->AddComponent<CSprite>(tlTiles->TXWallSetPath, tlTiles->wall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					wallTile->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 5:
				{
					auto cornerWall = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = cornerWall->AddComponent<CSprite>(tlTiles->TXWallSetPath, tlTiles->cornerWall, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
					cornerWall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 6:
				{
					auto grassPatchTile = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = grassPatchTile->AddComponent<CSprite>(tlTiles->TXPlantSetPath, tlTiles->grass_patch, 128, 128);
					sc->sprite.setPosition(i * 128, j * 128);
				}
				break;
				case 7:
				{
					auto tree1 = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = tree1->AddComponent<CSprite>(tlTiles->TXPlantSetPath, tlTiles->tree1, 256, 256);
					sc->sprite.setPosition(i * 128, j * 128);
				 tree1->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 8:
				{
					auto bushSmall = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = bushSmall->AddComponent<CSprite>(tlTiles->TXPlantSetPath, tlTiles->bushSmall, 64, 64);
					sc->sprite.setPosition(i * 128, j * 128);
					bushSmall->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 9:
				{
					auto bushBig = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = bushBig->AddComponent<CSprite>(tlTiles->TXPlantSetPath, tlTiles->bushBig, 75, 75);
					sc->sprite.setPosition(i * 128, j * 128);
					bushBig->AddComponent<CCollider>(sc->sprite.getGlobalBounds().height / 2);
					
				}
				break;
				case 10:
				{
					auto house1 = EntityManager::GetInstance()->AddEntity(sceneTLUniqueKey);
					auto sc = house1->AddComponent<CSprite>(tlTiles->TXSVSet, tlTiles->house1, 256, 256);
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

	

	void loadScene() {
		loadSceneBaseLayer();
		loadSceneTopLayer();
	}	



	

public:
	Scene(Scenes sceneId, const std::string& scenePath) {
		this->m_id = sceneId;
		this->scenePath = scenePath;
		this->sceneBLUniqueKey = scenePath + "bl";
		this->sceneTLUniqueKey = scenePath + "tl";
		blTiles = std::make_unique<TXBaseLayerTiles>();
		tlTiles = std::make_unique<TXTopLayerTiles>();
		
		externalScenes = std::make_shared<ExternalScenes>();
		
		initExternals();
		parseSceneData();
		loadScene();
		
		
	};

	
	std::vector<Entrance> GetEntranceVector() const {
		return entrances;
	}

	void RenderScene(sf::RenderWindow* ctx) const {
		//render base layer
		if (this->m_id == Scenes::SCENE_MENU || this->m_id == Scenes::SCENE_KIT_SELECTION || this->m_id == Scenes::SCENE_QUIT) {
			return;
		}

		auto blTiles = EntityManager::GetInstance()->GetEntities(sceneBLUniqueKey);
		for (auto& blt : blTiles) { //im hungry can u tell
			ctx->draw(blt->GetComponent<CSprite>()->sprite);
		}

		//render top layer
		auto tlTiles = EntityManager::GetInstance()->GetEntities(sceneTLUniqueKey);

		for (auto& tlt : tlTiles) {
			ctx->draw(tlt->GetComponent<CSprite>()->sprite);
			
		}
	
	}
	
	std::shared_ptr<ExternalScenes> GetExternals() const {
		return this->externalScenes;
	}

	sf::Vector2f GetSize() const {
		return { (float)this->scenetl[0].size() * 128, (float)this->scenetl.size() * 128 };
	}

	Scenes GetID() const {
		return m_id;
	}

	EntityVec GetTopLayer() const {
		return EntityManager::GetInstance()->GetEntities(sceneTLUniqueKey);
	}

	void cleanScene() {
		auto blTiles = EntityManager::GetInstance()->GetEntities(sceneBLUniqueKey);

		for (auto& blt : blTiles) {
			blt->DestroyEntity();
		}

		auto tiles = EntityManager::GetInstance()->GetEntities(sceneTLUniqueKey);

		for (auto& t : tiles) {
			t->DestroyEntity();
		}

		scenetl.clear();
		scenebl.clear();
	}


	~Scene() {
		cleanScene();

	};

};
