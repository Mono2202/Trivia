#include "Question.h"

// C'tor:
Question::Question(const string& question, const string& correctAnswer, const vector<string> possibleAnswers) : 
	m_question(question), m_correctAnswer(correctAnswer), m_possibleAnswers(possibleAnswers)
{
}


// Class Methods:

/*
Getting the question string
Input: <None>
Output: the question
*/
string Question::getQuestion() const
{
	return m_question;
}

/*
Getting the correct answer string
Input: <None>
Output: the correct answer
*/
string Question::getCorrectAnswer() const
{
	return m_correctAnswer;
}

/*
Getting the possible answers
Input: <None>
Output: the correct answer
*/
vector<string> Question::getPossibleAnswers() const
{
	return m_possibleAnswers;
}