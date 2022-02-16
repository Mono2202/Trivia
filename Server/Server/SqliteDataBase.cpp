#include "SqliteDataBase.h"
SqliteDataBase* SqliteDataBase::m_sqliteDataBaseInstance = nullptr;

// C'tor:

SqliteDataBase* SqliteDataBase::getInstance()
{
	if (m_sqliteDataBaseInstance == nullptr) {
		m_sqliteDataBaseInstance = new SqliteDataBase();
	}

	return m_sqliteDataBaseInstance;
}


// D'tor:

SqliteDataBase::~SqliteDataBase()
{
	delete m_sqliteDataBaseInstance;
}


// DB Functions:

/*
Opening the DB
Input : < None >
Output: true  - opening the DB was successfull
		false - otherwise
*/
bool SqliteDataBase::open()
{
	// Inits:
	string query = "CREATE TABLE USERS(username TEXT NOT NULL PRIMARY KEY, password TEXT NOT NULL, email TEXT NOT NULL, \
					address TEXT NOT NULL, phoneNumber TEXT NOT NULL, birthDate TEXT NOT NULL);\n\
					CREATE TABLE QUESTIONS(question TEXT NOT NULL, wrong_answer_1 TEXT NOT NULL, wrong_answer_2 TEXT NOT NULL,\
					wrong_answer_3 TEXT NOT NULL, correct_answer TEXT NOT NULL);\
					CREATE TABLE STATISTICS( username TEXT NOT NULL PRIMARY KEY,\
					totalGames INTEGER NOT NULL, correctAns INTEGER NOT NULL, totalAns INTEGER NOT NULL, avgAns REAL NOT NULL);";
	string dbFile = "TriviaDB.db";
	char* errMsg = NULL;
	int res = _access(dbFile.c_str(), 0);

	// Condition: opening the DB failed
	if (sqlite3_open(dbFile.c_str(), &m_db) != SQLITE_OK)
	{
		m_db = nullptr;
		std::cerr << "DB Connection Failed!\n";
		return false;
	}

	// Condition: DB file doesn't exist
	if (res == DB_DOESNT_EXIST && sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg))
	{
		std::cerr << "SQL ERROR: " << errMsg << "\n";
		close();
		return false;
	}

	// Condition: DB opened successfully
	return true;
}

/*
Closing the DB
Input : < None >
Output: < None >
*/
void SqliteDataBase::close()
{
	// Closing the DB:
	if (m_db != nullptr)
	{
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}


/* Users Table */

// Queries:

/*
Checking whether a user exists
Input : username - the username
Output: found	 - whether the user exists
*/
bool SqliteDataBase::doesUserExist(const string& username)
{
	// Inits:
	string query = "SELECT * FROM USERS WHERE username='" + username + "';";
	char* errMsg = NULL;
	bool found = false;

	// Checking whether the user exists:
	if (sqlite3_exec(m_db, query.c_str(), outputExistsCallback, &found, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return found;
}

/*
Checking whether a password matches to the username
Input : username - the username
		password - the password
Output: found	 - whether the password matches to the username
*/
bool SqliteDataBase::doesPasswordMatch(const string& username, const string& password)
{
	// Inits:
	string query = "SELECT * FROM USERS WHERE username='" + username + "' AND password='" + password + "';";
	char* errMsg = NULL;
	bool found = false;

	// Checking whether the password matches to the username:
	if (sqlite3_exec(m_db, query.c_str(), outputExistsCallback, &found, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return found;
}

// Actions:

/*
Adding a new user to the DB
Input : username - the username
		password - the password
		email	 - the email
Output: < None >
*/
void SqliteDataBase::addNewUser(const string& username, const string& password, const string& email, 
	const string& address, const string& phoneNumber, const string& birthDate)
{
	// Inits:
	string query = "INSERT INTO USERS VALUES ('" + username + "', '" + password + "', '" + email + "', '" +  
		address + "', '" + phoneNumber + "', '" + birthDate +"');";
	char* errMsg = NULL;

	// Adding a new user to the DB:
	if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}
}


/* Questions Table */

// Queries:

/*
Getting a questions vector
Input : amount - questions vector's size
Output: questions - the questions vector
*/
vector<Question> SqliteDataBase::getQuestions(const int amount)
{
	// Inits:
	string query = "SELECT * FROM QUESTIONS ORDER BY RANDOM() LIMIT " + std::to_string(amount) + ";";
	vector<Question> questions;
	char* errMsg = NULL;

	// Getting the questions from the DB:
	if (sqlite3_exec(m_db, query.c_str(), questionsCallback, &questions, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the questions vector:
	return questions;
}

// Actions:

/*
Adds new questions to the DB
Input : amount - amount of new questions
Output: < None >
*/
void SqliteDataBase::addNewQuestions(const int amount)
{
	// Inits:
	string executeScript = "./QuestionAdder.py "  + std::to_string(amount);

	// Calling the question adder python script:
	system(executeScript.c_str());
}

/*
Adds a user question to the DB
Input : question - the question
Output: < None >
*/
void SqliteDataBase::addUserQuestion(Question question)
{
	// Inits:
	string query = "INSERT INTO QUESTIONS VALUES('" + question.getQuestion() + "', '" + question.getPossibleAnswers()[1] + "', '" +
		question.getPossibleAnswers()[2] + "', '" + question.getPossibleAnswers()[3] + "', '" + question.getCorrectAnswer() + "');";
	char* errMsg = NULL;

	// Getting the player's average time:
	if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}
}


/* Statistics Table */

// Queries:

/*
Getting a player's average time
Input : username - the player's username
Output: avgTime  - the player's average time
*/
float SqliteDataBase::getPlayerAverageTime(const string& username)
{
	// Inits:
	string query = "SELECT avgAns FROM STATISTICS WHERE username='" + username + "';";
	char* errMsg = NULL;
	float avgTime = 0.0;

	// Getting the player's average time:
	if (sqlite3_exec(m_db, query.c_str(), floatNumCallback, &avgTime, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return avgTime;
}

/*
Getting a player's number of correct answers
Input : username - the player's username
Output: avgTime  - the player's number of correct answers
*/
int SqliteDataBase::getNumOfCorrectAnswers(const string& username)
{
	// Inits:
	string query = "SELECT correctAns FROM STATISTICS WHERE username='" + username + "';";
	char* errMsg = NULL;
	int correctAns = 0;

	// Getting the player's number of correct answers:
	if (sqlite3_exec(m_db, query.c_str(), intNumCallback, &correctAns, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return correctAns;
}

/*
Getting a player's number of total answers
Input : username - the player's username
Output: avgTime  - the player's number of total answers
*/
int SqliteDataBase::getNumOfTotalAnswers(const string& username)
{
	// Inits:
	string query = "SELECT totalAns FROM STATISTICS WHERE username='" + username + "';";
	char* errMsg = NULL;
	int totalAns = 0;

	// Getting the player's number of total answers:
	if (sqlite3_exec(m_db, query.c_str(), intNumCallback, &totalAns, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return totalAns;
}

/*
Getting a player's number of games
Input : username - the player's username
Output: avgTime  - the player's number of games
*/
int SqliteDataBase::getNumOfPlayerGames(const string& username)
{
	// Inits:
	string query = "SELECT totalGames FROM STATISTICS WHERE username='" + username + "';";
	char* errMsg = NULL;
	int totalGames = 0;

	// Getting the player's number of games:
	if (sqlite3_exec(m_db, query.c_str(), intNumCallback, &totalGames, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return totalGames;
}

/*
Getting the highscores
Input : < None >
Output: topScores - the highscores
*/
vector<string> SqliteDataBase::getHighScores()
{
	// Inits:
	string query = "SELECT username, correctAns FROM STATISTICS ORDER BY correctAns DESC LIMIT 5;";
	char* errMsg = NULL;
	vector<string> topScores;

	// Getting the highscores:
	if (sqlite3_exec(m_db, query.c_str(), scoreCallback, &topScores, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Returning the result:
	return topScores;
}

/*
Adding stats
Input : < None >
Output: username	   - the username
		averageTime	   - the avg time
		correctAnswers - amount of correct answers
		totalAnswers   - total answers count
*/
void SqliteDataBase::addStatistics(const string& username, float averageTime, int correctAnswers, int totalAnswers)
{
	// Inits:
	string query = "SELECT username FROM STATISTICS WHERE username='" + username + "';";
	char* errMsg = NULL;
	bool doesUsernameExist = false;

	// Getting whether the user exists:
	if (sqlite3_exec(m_db, query.c_str(), outputExistsCallback, &doesUsernameExist, &errMsg)) {
		std::cerr << "SQL ERROR: " << errMsg << "\n";
	}

	// Condition: username exists
	if (doesUsernameExist)
	{
		// Inits:
		float originalAverageTime = getPlayerAverageTime(username);
		int originalCorrectAnswers = getNumOfCorrectAnswers(username);
		int originalTotalAnswers = getNumOfTotalAnswers(username);
		int originalTotalGames = getNumOfPlayerGames(username);

		// Updating:
		originalAverageTime = (originalAverageTime * originalTotalAnswers + averageTime * totalAnswers) / (float)(originalTotalAnswers + totalAnswers);
		originalCorrectAnswers += correctAnswers;
		originalTotalAnswers += totalAnswers;
		originalTotalGames++;

		// Updating the current stats:
		query = "UPDATE STATISTICS SET totalGames=" + std::to_string(originalTotalGames) + ", correctAns=" + std::to_string(originalCorrectAnswers) +
			", totalAns=" + std::to_string(originalTotalAnswers) + ", avgAns=" + std::to_string(originalAverageTime) + " WHERE username='" + username + "';";
		if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg)) {
			std::cerr << "SQL ERROR: " << errMsg << "\n";
		}
	}

	// Condition: new user
	else
	{
		query = "INSERT INTO STATISTICS VALUES('" + username + "', 1, " + std::to_string(correctAnswers) + ", " + std::to_string(totalAnswers) + ", " + std::to_string(averageTime) + ");";
		if (sqlite3_exec(m_db, query.c_str(), nullptr, nullptr, &errMsg)) {
			std::cerr << "SQL ERROR: " << errMsg << "\n";
		}
	}
}


// Callback Functions:

/*
Callback for checking whether an item exists in the DB
*/
int SqliteDataBase::outputExistsCallback(void* data, int argc, char** argv, char** azColName)
{
	// Inits:
	bool* output = static_cast<bool*>(data);

	*output = true;

	return 0;
}

/*
Callback to get questions from the QUESTIONS table
*/
int SqliteDataBase::questionsCallback(void* data, int argc, char** argv, char** azColName)
{
	// Inits:
	vector<Question>* questions = static_cast<vector<Question>*>(data);
	string question = "";
	string correctAnswer = "";
	vector<string> answers;

	// Building the current Question struct:
	if (argc == 5)
	{
		question = argv[0];
		answers.push_back(argv[1]);
		answers.push_back(argv[2]);
		answers.push_back(argv[3]);
		answers.push_back(argv[4]);
		correctAnswer = argv[4];
	}
	
	// Shuffling answers:

	std::random_shuffle(answers.begin(), answers.end());
	
	Question currQuestion(question, correctAnswer, answers);

	// Pushing the current Question to the questions vector:
	questions->push_back(currQuestion);
	return 0;
}

/*
Callback to get average time (as float) from STATISTICS table
*/
int SqliteDataBase::floatNumCallback(void* data, int argc, char** argv, char** azColName)
{
	// Inits:
	float* num = static_cast<float*>(data);

	if (argc > 0) {
		*num = std::stof(argv[0]);
	}

	return 0;
}

/*
Callback to get integer value from STATISTICS table
*/
int SqliteDataBase::intNumCallback(void* data, int argc, char** argv, char** azColName)
{
	// Inits:
	int* num = static_cast<int*>(data);

	if (argc > 0) {
		*num = std::stoi(argv[0]);
	}

	return 0;
}

/*
Callback to add score to scores vector from STATISTICS table
*/
int SqliteDataBase::scoreCallback(void* data, int argc, char** argv, char** azColName)
{
	// Inits:
	vector<string>* score = static_cast<vector<string>*>(data);

	// Adding username with score:
	if (argc > 1) {
		score->push_back(std::string(argv[0]) + " : " + std::string(argv[1]));
	}

	return 0;
}
