#pragma once

// Includes:
#include "IRequestHandler.h"
#include "RoomManager.h"

// Class Declaration:
class RequestHandlerFactory;
class RoomMemberRequestHandler;
class RoomAdminRequestHandler;

class RoomRequestHandler : public IRequestHandler
{
public:
	// C'tor & D'tor:
	RoomRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	~RoomRequestHandler() = default;

	// Pure Virtual Methods:
	virtual bool isRequestRelevant(RequestInfo request) = 0;
	virtual RequestResult handleRequest(RequestInfo request) = 0;

protected:
	// Fields:
	Room m_room;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	// Protected Methods:
	RequestResult getRoomState(RequestInfo request);
};