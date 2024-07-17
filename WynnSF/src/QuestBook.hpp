#pragma once
#include "Quest.hpp"
#include "../core/Manager/EntityManager.hpp"


static constexpr float SPRITE_BOOK_PADDING = 10;
static constexpr float SPRITE_RETURN_PADDING = 10;
static const std::string font_path = "src/Assets/Fonts/RingBearer.TTF";

class QuestBook {
	QuestDB* _db{ nullptr };
	bool _opened = false;
	sf::RenderWindow* _ctx{ nullptr };
	



	void update_questbook_sprite_pos();
	void update_return_button_pos();
	void update_quest_title_pos();
	void update_quest_description_pos();
	void update_quest_title_status();
	void update_quest_page_pos();
	void update_quest_description();
	void update_quest_page_title_pos();
	void update_questbook_opened();
	void init_title();
	void init_sprites();
	void init_ui_quests();


public:

	QuestBook(QuestDB* db, sf::RenderWindow* ctx);

	bool IsOpen() const;
	
	void HandleEvents();


	void Update();

	void Render();

	~QuestBook() = default;

};