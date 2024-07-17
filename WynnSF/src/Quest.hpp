#pragma once
#include <iostream>
#include <vector>
#include "Player.hpp"
#include <map>
#include <functional>
#include "../core/Manager/EntityManager.hpp"
/*
*		
		quests could be implemented using a vector since the quests in this game will be sequential
		each element could have a state => locked, inProgress, Completed. 
		Ultimately we want a quest data base to store the information of all these quests such as the objective and any multistep objectives.
*		Once each quest's data has been assigned we can query quests by id and pull all that information through. 



*/



/*
	maybe data base can map these ids to a proccess such as a validate or check on the player e.g bool queryProcess(Quest*  quest)
	that looks up by id and does a check on the player for what needs to be done to complete the quest.

	return _processLookUp[quest->getID()].process(player);

	say the table is like so


	QUEST_ID | POCESSES
	--------------------
	QUEST_1  |  process(player);


	this process will have the necessary checks to determine if the player has complete the quest

	process(player) {
		does player have this specific item ? return true : false;
	}




*/


enum class QUESTID {
	QUEST_INVALID = -1,
	QUEST_A_NEW_BEGINNING,
	QUEST_ENZANS_BROTHER,

	QUEST_MAX_ENUM

};

struct QuestState {
	bool locked = true;
	bool inProgress = false;
	bool completed = false;
	QuestState() {};

	void unlock() {
		this->locked = false;
		this->inProgress = true;
	};

	void lock() {
		this->locked = true;
		this->inProgress = false;
	}

	void complete() {
		completed = true;
		inProgress = false;
		locked = false;
	}


	bool isLocked() const {
		return locked;
	}

	bool isInProgress() const {
		return inProgress;
	}

	bool isCompleted() const {
		return completed;
	}

	void setInProgress() {
		locked = false;
		inProgress = true;
		completed = false;
		
	}



	~QuestState() = default;
};

using ProcessType = std::function<bool()>;

struct Process {
	std::string desc;
	ProcessType process;

	Process(const std::string& desc, ProcessType process) {
		this->desc = desc;
		this->process = process;
	}

	bool isSuccessful() const {
		return process();
	}

	~Process() {};

};

class Quest {
	/*
		implement children of quests to recursively iterate until process returns false, if false then current process equals that process, description updates 
	*/
	
	std::string _title = "";
	QUESTID _id = QUESTID::QUEST_INVALID;

	int _currIdxToProcess = 0;
	std::vector<Process*> _processes {};

public:
	QuestState state{};
	
	//create add process method
	Quest(const std::string& title, QUESTID id, const int required);

	int requiredLevel = 0;

	std::string GetTitle() const;

	std::string GetDescription() const;

	void Update();

	void AddProcess(const std::string& description, ProcessType callback);

	bool processed();

	~Quest();
};


class QuestDB {

private:
	std::map<int, Quest*> _table;
	Player* _player;
	void init_quests();

public:
	QuestDB(Player* player);
	void Update();
	[[nodiscard]] std::map<int, Quest*>& GetTable();
	~QuestDB() = default;
};


