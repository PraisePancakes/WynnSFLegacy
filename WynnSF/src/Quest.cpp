#pragma once
#include "Quest.hpp"

	Quest::Quest(const std::string& title, QUESTID id, const int required) : _title(title), _id(id), requiredLevel(required) {};


	std::string Quest::GetTitle() const {
		return _title;
	}

	std::string Quest::GetDescription() const {
		if (_currIdxToProcess < _processes.size() && !state.locked) {
			return _processes[_currIdxToProcess]->desc;
		}
		else if (state.locked) {
			const std::string lockedMessage = "Unlock at level" + std::to_string(requiredLevel);
			return lockedMessage;
		}
		else if (state.completed)
			return "Completed";
	}

	void Quest::Update() {
		if (_currIdxToProcess < _processes.size() && _processes[_currIdxToProcess]->isSuccessful()) {
			_currIdxToProcess++;
		}
	}

	void Quest::AddProcess(const std::string& description, ProcessType callback) {
		std::cout << _processes.size();
		_processes.push_back(new Process(description, callback));
	}

	bool Quest::processed() {
		return _currIdxToProcess >= _processes.size();
	}

	Quest::~Quest() {
		for (size_t i = 0; i < _processes.size(); i++) {
			delete _processes[i];
		}
	};


;


	void QuestDB::init_quests() {

		_table[(int)QUESTID::QUEST_A_NEW_BEGINNING] = new Quest("A new Beginning", QUESTID::QUEST_A_NEW_BEGINNING, 0);
		auto q1 = _table[(int)QUESTID::QUEST_A_NEW_BEGINNING];
		q1->AddProcess("Explore a little bit, harness your surroundings", [this]() -> bool {
			//(first step of the quest to process)

			static size_t __func_calls = 0;

			static Core::Physics::Vec2D plStartingPos;

			if (__func_calls == 0) {
				plStartingPos = _player->GetPos();

				__func_calls++;
			}

			Core::Physics::Vec2D plCurrentPos = _player->GetPos();

			float x1 = plStartingPos.x;
			float x2 = plCurrentPos.x;
			float y1 = plStartingPos.y;
			float y2 = plCurrentPos.y;
			float dx = x2 - x1;
			float dy = y2 - y1;

			float distance = std::sqrt((dx)*dx + (dy)*dy);


			if (distance >= 500) {
				return true;
			}



			return false;
			});



		_table[(int)QUESTID::QUEST_ENZANS_BROTHER] = new Quest("Enzan's Brother", QUESTID::QUEST_ENZANS_BROTHER, 1);
		auto q2 = _table[(int)QUESTID::QUEST_ENZANS_BROTHER];
		q2->AddProcess("Talk to Enzan close to the exit of Ragni", [this]() -> bool {


			return false;
			});

		q2->AddProcess("test", [this]()->bool {
			//some process
			return false;
			});



	};




	QuestDB::QuestDB(Player* player) {
		this->_player = player;
		init_quests();
	};

	void QuestDB::Update() {

		for (auto& pair : _table) {
			if (pair.second) {


				if (_player->currentLevel >= pair.second->requiredLevel && !pair.second->state.isCompleted()) {

					pair.second->state.setInProgress();

				}

				if (pair.second->state.isInProgress() && !pair.second->state.isCompleted()) {
					pair.second->Update();
					if (pair.second->processed()) {
						std::cout << pair.second->GetTitle();
						pair.second->state.complete();
					}
				}
			}


		}


	}

	[[nodiscard]] std::map<int, Quest*>& QuestDB::GetTable() {
		return _table;
	};

	


