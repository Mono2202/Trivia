#pragma once

// Includes:
#include "RoomRequestHandler.h"

class RequestHandlerFactory;
class RoomRequestHandler;
class RoomAdminRequestHandler : public RoomRequestHandler
{
public:

	// C'tor & D'tor:
	RoomAdminRequestHandler(Room& room, LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	~RoomAdminRequestHandler() = default;

	// Virtual Methods:
	virtual bool isRequestRelevant(RequestInfo request);
	virtual RequestResult handleRequest(RequestInfo request);

private:
	// Private Methods:
	RequestResult closeRoom(RequestInfo request);
	RequestResult startGame(RequestInfo request);
};