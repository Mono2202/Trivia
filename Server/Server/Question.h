#pragma once

// Includes:
#include <vector>
#include <string>

// Using:
using std::vector;
using std::string;

class Question
{
public:
	// C'tors & D'tors:
	Question(const string& question, const string& correctAnswer, const vector<string> possibleAnswers);
	~Question() = default;

	// Methods:
	string getQuestion() const;
	vector<string> getPossibleAnswers() const;
	string getCorrectAnswer() const;
private:
	string m_question;
	string m_correctAnswer;
	vector<string> m_possibleAnswers;
};

