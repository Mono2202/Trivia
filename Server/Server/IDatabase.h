#pragma once

// Includes:
#include <string>
#include <vector>
#include "Question.h"

#include <algorithm> 

// Using:
using std::string;
using std::vector;


// IDatabase Class:
class IDatabase
{
public:
	// D'tors:
	virtual ~IDatabase() = default;

	// DB Functions:
	virtual bool open() = 0;
	virtual void close() = 0;


	/* Users Table */

	// Queries:
	virtual bool doesUserExist(const string& username) = 0;
	virtual bool doesPasswordMatch(const string& username, const string& password) = 0;

	// Actions:
	virtual void addNewUser(const string& username, const string& password, const string& email, 
		const string& address, const string& phoneNumber, const string& birthDate) = 0;


	/* Questions Table */

	// Queries:
	virtual vector<Question> getQuestions(const int amount) = 0;

	// Actions:
	virtual void addNewQuestions(const int amount) = 0;
	virtual void addUserQuestion(Question question) = 0;

	/* Statistics Table */
	
	// Queries:
	virtual float getPlayerAverageTime(const string& username) = 0;
	virtual int getNumOfCorrectAnswers(const string& username) = 0;
	virtual int getNumOfTotalAnswers(const string& username) = 0;
	virtual int getNumOfPlayerGames(const string& username) = 0;
	virtual vector<string> getHighScores() = 0;
	virtual void addStatistics(const string& username, float averageTime, int correctAnswers, int totalAnswers) = 0;
};
