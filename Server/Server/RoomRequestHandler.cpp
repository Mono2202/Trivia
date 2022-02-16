#include "RoomRequestHandler.h"
#include "RequestHandlerFactory.h"

// C'tor:

RoomRequestHandler::RoomRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory) : m_room(room), m_user(user),
m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{}

// Protected Methods:

/*
Getting Room State
Input : request - the closeRoom request
Output: result  - the request result
*/
RequestResult RoomRequestHandler::getRoomState(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Updating room:
    m_room = *(m_roomManager.getRoom(m_room.getRoomData().id));
    RoomData data = m_room.getRoomData();

    // Creating response:
    GetRoomStateResponse response;
    response.answerTimeout = data.timePerQuestion;
    response.questionCount = data.numOfQuestionsInGame;
    response.hasGameBegun = data.isActive == GAME_ACTIVE;
    response.players = m_room.getAllUsers();
    response.status = SUCCESS_STATUS;

    // Creating result:
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = nullptr;
    return result;
}