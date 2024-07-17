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

enum class SCENES {
	SCENE_MENU,
	SCENE_KIT_SELECTION,
	SCENE_RAGNI,
	SCENE_PIGMANS,
	SCENE_DETLAS,
	SCENE_ALMUJ,

	SCENE_QUIT,
};


enum class SCENE_SIDES {
	SIDE_NONE,
	SIDE_LEFT,
	SIDE_RIGHT,
	SIDE_TOP,
	SIDE_BOTTOM
};

struct Entrance {
	SCENE_SIDES side = SCENE_SIDES::SIDE_NONE;
	sf::Vector2f pos{ 0, 0 };
	sf::Vector2f size{ 0, 0 };

	Entrance() {};
	Entrance(SCENE_SIDES side, sf::Vector2f p, sf::Vector2f size) {
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
	SCENES left = SCENES::SCENE_MENU;
	SCENES right = SCENES::SCENE_MENU;
	SCENES top = SCENES::SCENE_MENU;
	SCENES bottom = SCENES::SCENE_MENU;


	ExternalScenes() {};

	void InitExternalScenes(SCENES l, SCENES r, SCENES t, SCENES b) {
		this->left = l;
		this->right = r;
		this->top = t;
		this->bottom = b;
	};
	~ExternalScenes() {};
};



class Scene {
	SCENES _id;
	std::shared_ptr<ExternalScenes> _externalScenes;
	std::string _scenePath = "";
	

	std::string _sceneBLUniqueKey = "";
	std::string _sceneTLUniqueKey = "";



	std::vector<std::vector<int>> _scenetl = {};
	std::vector<std::vector<int>> _scenebl = {};

	std::unique_ptr<TXBaseLayerTiles> _blTiles	{ nullptr };
	std::unique_ptr<TXTopLayerTiles> _tlTiles	{ nullptr };

	std::vector<Entrance> _entrances;

	
	void init_externals();
	void parse_scene_data();
	void load_scene_base_layer();
	void load_scene_top_layer();
	void load_scene();


	

public:
	Scene(SCENES sceneId, const std::string& scenePath);

	std::vector<Entrance> GetEntranceVector() const;

	void RenderScene(sf::RenderWindow* ctx) const;
	
	std::shared_ptr<ExternalScenes> GetExternals() const;

	sf::Vector2f GetSize() const;

	SCENES GetID() const;

	EntityVec GetTopLayer() const;

	void CleanScene();


	~Scene();

};
