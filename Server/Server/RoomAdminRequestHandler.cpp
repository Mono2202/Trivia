#include "RoomAdminRequestHandler.h"
#include "RequestHandlerFactory.h"

// C'tor:

RoomAdminRequestHandler::RoomAdminRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : RoomRequestHandler(room, user, roomManager, handlerFactory)
{}

// Virtual Methods:

/*
Checking whether the request is relevant or not
Input : request - the request info
Output: true	- relevant request
        false	- otherwise
*/
bool RoomAdminRequestHandler::isRequestRelevant(RequestInfo request)
{
    // Condition: relevant request
    if (request.buffer[0] >= CLOSE_ROOM_REQUEST && request.buffer[0] <= GET_ROOM_STATE_REQUEST) {
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
RequestResult RoomAdminRequestHandler::handleRequest(RequestInfo request)
{
    switch (request.buffer[0])
    {
    case CLOSE_ROOM_REQUEST:        return closeRoom(request);
    case START_GAME_REQUEST:        return startGame(request);
    case GET_ROOM_STATE_REQUEST:    return getRoomState(request);
    }
}

// Private Methods:

/*
closing room
Input : request - the request info
Output: result  - the request result
*/
RequestResult RoomAdminRequestHandler::closeRoom(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Creating Response:
    m_roomManager.deleteRoom(m_room.getRoomData().id);
    CloseRoomResponse response = { SUCCESS_STATUS };
    
    // Creating result;
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
    return result;
}

/*
starting game
Input : request - the request info
Output: result  - the request result
*/
RequestResult RoomAdminRequestHandler::startGame(RequestInfo request)
{
    // Inits:
    RequestResult result;

    // Creating Response:
    m_roomManager.getRoom(m_room.getRoomData().id)->setIsActive(GAME_ACTIVE);
    Game* game = m_handlerFactory.getGameManager().createGame(*m_roomManager.getRoom(m_room.getRoomData().id));
    StartGameResponse response = { SUCCESS_STATUS };
    
    // Creating result;
    result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
    result.newHandler = m_handlerFactory.createGameRequestHandler(m_user, game);
    return result;
}