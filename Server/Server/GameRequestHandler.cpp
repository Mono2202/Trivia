#include "GameRequestHandler.h"

// C'tors:

GameRequestHandler::GameRequestHandler(Game* game, LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& handlerFactory) :
	m_game(game), m_user(user), m_gameManager(gameManager), m_handlerFactory(handlerFactory) {  }


// Virtual Methods:

/*
Checking whether the request is relevant or not
Input : request - the request info
Output: true	- relevant request
        false	- otherwise
*/
bool GameRequestHandler::isRequestRelevant(RequestInfo request)
{
    // Condition: relevant request
    if (request.buffer[0] >= LEAVE_GAME_REQUEST && request.buffer[0] <= GET_GAME_RESULT_REQUEST) {
        return true;
    }

    // Condition: irrelevant request
    return false;
}

/*
Handling the request
Input : request - the request info
Output: result  - the request result
*/
RequestResult GameRequestHandler::handleRequest(RequestInfo request)
{
    switch (request.buffer[0])
    {
    case LEAVE_GAME_REQUEST:        return leaveGame(request);
    case GET_QUESTION_REQUEST:      return getQuestion(request);
    case SUBMIT_ANSWER_REQUEST:     return submitAnswer(request);
    case GET_GAME_RESULT_REQUEST:   return getGameResults(request);
    }
}


// Private Methods:

/*
Getting a question for a user
Input : request - the getQuestion request
Output: result  - the request result
*/
RequestResult GameRequestHandler::getQuestion(RequestInfo request)
{
    // Inits:
    RequestResult result;
    int i = 0;

    // Creating Response:
    Question question = m_gameManager.getGame(m_game->getId())->getQuestionForUser(m_user);
    vector<string> answers = question.getPossibleAnswers();
    map<unsigned int, string> mappedAnswers;
    
    for (i = 0; i < answers.size(); i++)
    {
        mappedAnswers.emplace(i, answers[i]);
    }

    GetQuestionResponse response = {SUCCESS_STATUS, question.getQuestion(), mappedAnswers};

    // Creating result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Submitting an answer
Input : request - the submit answer request
Output: result  - the request result
*/
RequestResult GameRequestHandler::submitAnswer(RequestInfo request)
{
    // Inits:
    RequestResult result;
    SubmitAnswerRequest submitRequest = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(request.buffer);
    int i = 0;
    int ansIndex = 0;

    // Creating Response:
    Question question = m_gameManager.getGame(m_game->getId())->getQuestionForUser(m_user);
    string correctAnswer = question.getCorrectAnswer();
    vector<string> answers = question.getPossibleAnswers();
    
    // Getting correct answer index
    for (i = 0; i < answers.size(); i++)
    {
        if (answers[i] == correctAnswer)
        {
            ansIndex = i;
        }
    }

    // Condition: wrong answer
    if (submitRequest.answerID == 4) {
        submitRequest.answerID = (ansIndex + 1) % 4;
    }

    m_game->submitAnswer(m_user, answers[submitRequest.answerID], submitRequest.timeSpent);
    SubmitAnswerResponse response = {SUCCESS_STATUS, ansIndex};

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Getting game results(for players)
Input : request - the getGameResult request
Output: result  - the request result
*/
RequestResult GameRequestHandler::getGameResults(RequestInfo request)
{
    // Inits:
    RequestResult result;
    result.newHandler = nullptr;

    // Creating Response:
    vector<PlayerResults> results;
    int responseStatus = SUCCESS_STATUS;

    map<LoggedUser, GameData> mappedData = m_game->getPlayers();
    for (auto& data : mappedData)
    {
        if (data.second.currentQuestion.getQuestion() != m_game->getQuestions().back().getQuestion())
        {
            responseStatus = 0;
        }
        PlayerResults newPlayerResults = {data.first.getUsername(), data.second.correctAnswerCount,
            data.second.wrongAnswerCount, data.second.averageAnswerTime};
        results.push_back(newPlayerResults);
    }

    // Sorting the vector:
    std::sort(results.begin(), results.end());

    // Creating Result:
    GetGameResultsResponse response = { responseStatus, results };
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    
    return result;
}

/*
Leaving the game
Input : request - the leaveGame request
Output: result  - the request result
*/
RequestResult GameRequestHandler::leaveGame(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Adding the user's stats:
    map<LoggedUser, GameData> mappedData = m_game->getPlayers();
    for (auto& data : mappedData)
    {
        if (data.first.getUsername() == m_user.getUsername() && !data.second.statsAdded && data.second.wrongAnswerCount + data.second.correctAnswerCount == m_game->getQuestions().size())
        {
            m_gameManager.addStats(m_user.getUsername(), data.second.averageAnswerTime / (float)(data.second.wrongAnswerCount + data.second.correctAnswerCount),
                data.second.correctAnswerCount, data.second.wrongAnswerCount + data.second.correctAnswerCount);
            data.second.statsAdded = true;
        }
    }

    // Condition: last player to leave
    if (m_gameManager.getGame(m_game->getId())->getCurrentPlayers() <= 1)
    {
        for (auto& data : mappedData)
        {
            // Removing the player from the game:
            LoggedUser currentUser(data.first.getUsername());
            m_gameManager.getGame(m_game->getId())->removePlayer(currentUser);
        }

        m_handlerFactory.getRoomManager().deleteRoom(m_game->getId());
        m_gameManager.deleteGame(m_game->getId());
    }
    
    else
    {
        // Lowering the counter:
        m_gameManager.getGame(m_game->getId())->setCurrentPlayers(m_gameManager.getGame(m_game->getId())->getCurrentPlayers() - 1);
        
        // Crash handling:
        m_gameManager.getGame(m_game->getId())->setFinalQuestion(m_user);
    }

    // Creating Response:
    LeaveGameResponse response = { SUCCESS_STATUS };

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    return result;
}
