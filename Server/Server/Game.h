#pragma once

// Includes:
#include <map>
#include "Question.h"
#include "LoggedUser.h"

// Using:
using std::map;

// GameData Struct:
typedef struct GameData {
	Question currentQuestion;
	unsigned int questionIndex;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
	bool statsAdded;
} GameData;

// Game Class:
class Game
{
public:
	// C'tors && D'tors:
	Game(const vector<Question>& questions, const vector<string>& players, int id);
	~Game() = default;

	// Methods:
	Question getQuestionForUser(LoggedUser& user) const;
	void submitAnswer(LoggedUser& user, string& answer, int timeSpent);
	void removePlayer(LoggedUser& user);

	// Getters:
	map<LoggedUser, GameData>& getPlayers();
	vector<Question> getQuestions() const;
	int getCurrentPlayers() const;
	int getId() const;

	// Setter:
	void setCurrentPlayers(int currentPlayers);
	void setFinalQuestion(LoggedUser& user);

private:
	// Fields:
	vector<Question> m_questions;
	map<LoggedUser, GameData> m_players;
	int m_currentPlayers;
	int m_id;
};

