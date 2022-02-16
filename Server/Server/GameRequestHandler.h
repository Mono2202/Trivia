#pragma once

// Includes:
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"

// Class Declaration:
class RequestHandlerFactory;

// GameRequestHandler Class
class GameRequestHandler : public IRequestHandler
{
public:
	// C'tors & D'tors:
	GameRequestHandler(Game* game, LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& handlerFactory);
	~GameRequestHandler() = default;

	// Virtual Methods:
	virtual bool isRequestRelevant(RequestInfo request);
	virtual RequestResult handleRequest(RequestInfo request);

private:
	// Fields:
	Game* m_game;
	LoggedUser m_user;
	GameManager& m_gameManager;
	RequestHandlerFactory& m_handlerFactory;

	// Private Methods:
	RequestResult getQuestion(RequestInfo request);
	RequestResult submitAnswer(RequestInfo request);
	RequestResult getGameResults(RequestInfo request);
	RequestResult leaveGame(RequestInfo request);
};

