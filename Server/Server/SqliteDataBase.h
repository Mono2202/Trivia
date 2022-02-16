#pragma once

// Includes:
#include <iostream>
#include <time.h>
#include "IDatabase.h"
#include "sqlite3.h"
#include "io.h"

// Defines:
#define DB_DOESNT_EXIST -1


// SqliteDataBase Class:
class SqliteDataBase : public IDatabase
{
public:
	// Static C'tor:
	static SqliteDataBase* getInstance();

	// D'tors:
	virtual ~SqliteDataBase();

	// DB Functions:
	virtual bool open();
	virtual void close();


	/* Users Table */

	// Queries:
	virtual bool doesUserExist(const string& username);
	virtual bool doesPasswordMatch(const string& username, const string& password);

	// Actions:
	virtual void addNewUser(const string& username, const string& password, const string& email, 
		const string& address, const string& phoneNumber, const string& birthDate);


	/* Questions Table */

	// Queries:
	virtual vector<Question> getQuestions(const int amount);

	// Actions:
	virtual void addNewQuestions(const int amount);
	virtual void addUserQuestion(Question question);


	/* Statistics Table */

	// Queries:
	virtual float getPlayerAverageTime(const string& username);
	virtual int getNumOfCorrectAnswers(const string& username);
	virtual int getNumOfTotalAnswers(const string& username);
	virtual int getNumOfPlayerGames(const string& username);
	virtual vector<string> getHighScores();
	virtual void addStatistics(const string& username, float averageTime, int correctAnswers, int totalAnswers);

private:
	// Private C'tor:
	SqliteDataBase() = default;

	// Fields:
	static SqliteDataBase* m_sqliteDataBaseInstance;
	sqlite3* m_db;

	// Callback Functions:
	static int outputExistsCallback(void* data, int argc, char** argv, char** azColName);
	static int questionsCallback(void* data, int argc, char** argv, char** azColName);
	static int floatNumCallback(void* data, int argc, char** argv, char** azColName);
	static int intNumCallback(void* data, int argc, char** argv, char** azColName);
	static int scoreCallback(void* data, int argc, char** argv, char** azColName);
};
