#pragma once

// Includes:
#include "RoomRequestHandler.h"

class RequestHandlerFactory;
class RoomRequestHandler;
class RoomMemberRequestHandler : public RoomRequestHandler
{
public:

	// C'tor & D'tor:
	RoomMemberRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	~RoomMemberRequestHandler() = default;

	// Virtual Methods:
	virtual bool isRequestRelevant(RequestInfo request);
	virtual RequestResult handleRequest(RequestInfo request);

private:
	// Private Methods:
	RequestResult leaveRoom(RequestInfo request);
	RequestResult startGame(RequestInfo request);
};