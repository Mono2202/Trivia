#include "GameManager.h"
GameManager* GameManager::m_gameManagerInstance = nullptr;

// C'tors:

GameManager::GameManager(IDatabase* database) : m_database(database) {	}

GameManager* GameManager::getInstance(IDatabase* database)
{
	if (m_gameManagerInstance == nullptr) {
		m_gameManagerInstance = new GameManager(database);
	}

	return m_gameManagerInstance;
}


// D'tor:

GameManager::~GameManager()
{
	for (int i = 0; i < m_games.size(); i++) {

		try {
			delete m_games[i];
		} catch (...) {  }
	
		m_games.erase(m_games.begin() + i);
	}

	delete m_gameManagerInstance;
}


// Methods:

/*
Creating a new Game.
Input: Room to create game for.
Output: the game.
*/
Game* GameManager::createGame(Room& room)
{
	// Inits:
	vector<Question> questions = m_database->getQuestions(room.getRoomData().numOfQuestionsInGame);
	vector<string> players = room.getAllUsers();
	
	// Creating Game:
	Game* newGame = new Game(questions, players, room.getRoomData().id);

	// Adding Game:
	m_games.push_back(newGame);

	return newGame;
}

/*
Getting a game by ID
Input: the id
Output: the game
*/
Game* GameManager::getGame(int ID)
{
	for (int i = 0; i < m_games.size(); i++)
	{
		if (m_games[i]->getId() == ID)
		{
			return m_games[i];
		}
	}

	throw std::exception("Game not found\n");
}

/*
Deletes the game
Input : ID - the id
Output: < None >
*/
void GameManager::deleteGame(int ID)
{
	for (int i = 0; i < m_games.size(); i++)
	{
		if (m_games[i]->getId() == ID)
		{
			delete m_games[i];
			m_games.erase(m_games.begin() + i);
		}
	}
}

/*
Adding stats
Input : < None >
Output: username	   - the username
		averageTime	   - the avg time
		correctAnswers - amount of correct answers
		totalAnswers   - total answers count
*/
void GameManager::addStats(const string& username, float averageTime, int correctAnswers, int totalAnswers)
{
	m_database->addStatistics(username, averageTime, correctAnswers, totalAnswers);
}
