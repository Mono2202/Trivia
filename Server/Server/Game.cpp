#include "Game.h"

// C'tor:

Game::Game(const vector<Question>& questions, const vector<string>& players, int id) 
{
	// Questions:
	m_questions = questions;
	m_id = id;
	m_currentPlayers = players.size();

	// Players map:
	GameData data = { questions.front(), 0, 0, 0, 0 };

	for (int i = 0; i < players.size(); i++)
	{
		LoggedUser newUser(players[i]);
		m_players.emplace(newUser, data);
	}
}

// Class Methods:

/*
Getting question for certain user
Input:  the user
Output: the question
*/
Question Game::getQuestionForUser(LoggedUser& user) const
{
	if (m_players.find(user) == m_players.end())
		throw std::exception("Player was not found\n");
	return m_players.at(user).currentQuestion;
}

/*
Removing a player
Input:  the user
Output: < None >
*/
void Game::removePlayer(LoggedUser& user)
{
	if (m_players.find(user) == m_players.end())
		throw std::exception("Player was not found\n");
	m_players.erase(user);
}

/*
Submitting an answer
Input:  user submitting,
		user's answer
Output: < None >
*/
void Game::submitAnswer(LoggedUser& user, string& answer, int timeSpent)
{
	if (m_players.find(user) == m_players.end())
		throw std::exception("Player was not found\n");

	// Updating answer stats:
	if (answer == m_players.at(user).currentQuestion.getCorrectAnswer())
	{
		m_players.at(user).correctAnswerCount++;
	}
	else
	{
		m_players.at(user).wrongAnswerCount++;
	}

	m_players.at(user).averageAnswerTime += timeSpent;

	// Updating question
	if (m_players.at(user).questionIndex + 1 < m_questions.size())
	{
		m_players.at(user).questionIndex++;
		m_players.at(user).currentQuestion = m_questions[m_players.at(user).questionIndex];
	}
}

// Getters:

/*
Getting the players(with results)
Input: < None >
Output: the players-results map
*/
map<LoggedUser, GameData>& Game::getPlayers()
{
	return m_players;
}

/*
Getting the questions vector
Input: < None >
Output: the questions vector
*/
vector<Question> Game::getQuestions() const
{
	return m_questions;
}

/*
Getting the current players
Input: < None >
Output: the current players
*/
int Game::getCurrentPlayers() const
{
	return m_currentPlayers;
}

/*
Getting the id
Input: < None >
Output: the id
*/
int Game::getId() const
{
	return m_id;
}


// Setter:

void Game::setCurrentPlayers(int currentPlayers)
{
	m_currentPlayers = currentPlayers;
}

void Game::setFinalQuestion(LoggedUser& user)
{
	m_players.at(user).currentQuestion = m_questions.back();
}
