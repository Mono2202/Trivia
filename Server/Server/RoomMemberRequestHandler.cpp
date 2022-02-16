#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
// C'tor:

RoomMemberRequestHandler::RoomMemberRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : RoomRequestHandler(room, user, roomManager, handlerFactory)
{}

// Virtual Methods:

/*
Checking whether the request is relevant or not
Input : request - the request info
Output: true	- relevant request
        false	- otherwise
*/
bool RoomMemberRequestHandler::isRequestRelevant(RequestInfo request)
{
    // Condition: relevant request
    if (request.buffer[0] >= START_GAME_REQUEST && request.buffer[0] <= LEAVE_ROOM_REQUEST) {
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
RequestResult RoomMemberRequestHandler::handleRequest(RequestInfo request)
{
    switch (request.buffer[0])
    {
    case LEAVE_ROOM_REQUEST:        return leaveRoom(request);
    case GET_ROOM_STATE_REQUEST:    return getRoomState(request);
    case START_GAME_REQUEST:        return startGame(request);
    }
}

/*
leaving room
Input : request - the request info
Output: result  - the request result
*/
RequestResult RoomMemberRequestHandler::leaveRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Creating Response:
    try {
        m_roomManager.getRoom(m_room.getRoomData().id)->removeUser(m_user);
    } catch (...) { }

    LeaveRoomResponse response = { SUCCESS_STATUS };

    // Creating result;
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    return result;
}


/*
setting user to be in game
Input : request - the request info
Output: result  - the request result
*/
RequestResult RoomMemberRequestHandler::startGame(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Creating response:
    Game* game = m_handlerFactory.getGameManager().getGame(m_room.getRoomData().id);
    StartGameResponse response = { SUCCESS_STATUS };

    // Creating result;
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, game);
    return result;
}