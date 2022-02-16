#pragma once

// Includes:
#include "IDatabase.h"
#include "Game.h"
#include "Room.h"

// GameManager Class:
class GameManager
{
public:
	// Static C'tor:
	static GameManager* getInstance(IDatabase* database);

	// D'tor:
	~GameManager();

	// Methods:
	Game* createGame(Room& room);
	Game* getGame(int ID);
	void deleteGame(int ID);
	void addStats(const string& username, float averageTime, int correctAnswers, int totalAnswers);

private:
	// Private C'tor:
	GameManager(IDatabase* database);

	// Fields:
	static GameManager* m_gameManagerInstance;
	IDatabase* m_database;
	vector<Game*> m_games;
};

