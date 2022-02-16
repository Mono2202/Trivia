#include "MenuRequestHandler.h"

int MenuRequestHandler::currId = 0;

// C'tors:

MenuRequestHandler::MenuRequestHandler(RoomManager& roomManager, StatisticsManager& statisticsManager, LoggedUser& user, RequestHandlerFactory& handlerFactory) :
    m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_user(user) , m_handlerFactory(handlerFactory) { }


// Virtual Methods:

/*
Checking whether the request is relevant or not
Input : request - the request info
Output: true	- relevant request
        false	- otherwise
*/
bool MenuRequestHandler::isRequestRelevant(RequestInfo request)
{
    // Condition: relevant request
    if ((request.buffer[0] >= LOGOUT_REQUEST && request.buffer[0] <= CREATE_ROOM_REQUEST) || request.buffer[0] == ADD_QUESTION_REQUEST || request.buffer[0] == GET_HEAD_TO_HEAD_ROOM_REQUEST) {
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
RequestResult MenuRequestHandler::handleRequest(RequestInfo request)
{
    switch (request.buffer[0])
    {
    case LOGOUT_REQUEST:                return signout(request);   
    case GET_ROOMS_REQUEST:             return getRooms(request); 
    case GET_PLAYERS_IN_ROOM_REQUEST:   return getPlayersInRoom(request);  
    case GET_HIGH_SCORE_REQUEST:        return getHighScore(request);  
    case GET_PERSONAL_STATS_REQUEST:    return getPersonalStats(request); 
    case JOIN_ROOM_REQUEST:             return joinRoom(request);
    case CREATE_ROOM_REQUEST:           return createRoom(request);
    case ADD_QUESTION_REQUEST:          return addQuestion(request);
    case GET_HEAD_TO_HEAD_ROOM_REQUEST: return getHeadToHeadRoom(request);
    }
}

// Private Methods:

/*
Signing-out from an account
Input : request - the signout request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::signout(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Creating Response:
    m_handlerFactory.getLoginManager().logout(m_user.getUsername());
    LogoutResponse response = { SUCCESS_STATUS };

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createLoginRequestHandler();
    return result;
}

/*
Getting rooms
Input : request - the get rooms request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::getRooms(RequestInfo request)
{
    // Inits:
    RequestResult result;
    GetRoomsResponse response = { SUCCESS_STATUS, m_roomManager.getRooms() };

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Getting the players in a room
Input : request - the get players in room request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::getPlayersInRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;
    GetPlayersInRoomRequest getPlayersRequest = JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(request.buffer);

    // Creating Response:
    GetPlayersInRoomResponse response;
    response.players = m_roomManager.getRoom(getPlayersRequest.roomID)->getAllUsers();
    
    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Getting personal statistics
Input : request - the get personal stats request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::getPersonalStats(RequestInfo request)
{
    // Inits:
    RequestResult result;
    GetPersonalStatsResponse response;
    
    // Creating Response:
    response.status = SUCCESS_STATUS;
    response.statistics = m_statisticsManager.getUserStatistics(m_user.getUsername());

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Getting highscores
Input : request - the get highscores request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::getHighScore(RequestInfo request)
{
    // Inits:
    RequestResult result;
    GetHighScoreResponse response;

    // Creating Response:
    response.status = SUCCESS_STATUS;
    response.statistics = m_statisticsManager.getHighScore();

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Joining a room
Input : request - the join room request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::joinRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;
    JoinRoomRequest joinRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(request.buffer);

    // Creating Response:
    JoinRoomResponse response;
    m_roomManager.getRoom(joinRequest.roomID)->addUser(m_user);
    response.status = SUCCESS_STATUS;

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createRoomMemberRequestHandler(m_user, *m_roomManager.getRoom(joinRequest.roomID));
    return result;
}

/*
Creating a room
Input : request - the create room request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::createRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;
    CreateRoomRequest createRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(request.buffer);

    // Creating Response:
    CreateRoomResponse response;
    RoomData data;
    data.id = currId++;
    data.name = createRequest.roomName;
    data.maxPlayers = createRequest.maxUsers;
    data.numOfQuestionsInGame = createRequest.questionCount;
    data.timePerQuestion = createRequest.answerTimeout;
    data.isActive = 0;
    data.isHeadToHead = createRequest.isHeadToHead != 0;
    if (data.isHeadToHead) {
        data.name += std::to_string(data.id);
    }
    m_roomManager.createRoom(m_user, data);
    response.status = SUCCESS_STATUS;

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createRoomAdminRequestHandler(m_user, *m_roomManager.getRoom(data.id));
    return result;
}

/*
Adds a question to the DB
Input : request - the add question request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::addQuestion(RequestInfo request)
{
    // Inits:
    RequestResult result;
    AddQuestionRequest addQuestionRequest = JsonRequestPacketDeserializer::deserializeAddQuestionRequest(request.buffer);

    // Creating Response:
    AddQuestionResponse response;
    response.status = SUCCESS_STATUS;

    // Adding the question:
    vector<string> answers;
    answers.push_back(addQuestionRequest.correctAnswer);
    answers.push_back(addQuestionRequest.wrongAnswer1);
    answers.push_back(addQuestionRequest.wrongAnswer2);
    answers.push_back(addQuestionRequest.wrongAnswer3);
    Question newQuestion(addQuestionRequest.question, addQuestionRequest.correctAnswer, answers);
    m_statisticsManager.addUserQuestion(newQuestion);

    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}

/*
Getting a head to head room
Input : request - the head to head room request
Output: result  - the request result
*/
RequestResult MenuRequestHandler::getHeadToHeadRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;
    GetHeadToHeadRoomResponse response = { SUCCESS_STATUS, m_roomManager.getHeadToHeadRoom() };
    
    // Creating Result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}
