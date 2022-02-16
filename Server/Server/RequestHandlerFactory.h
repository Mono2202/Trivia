#pragma once
// Includes:
#include "IDatabase.h"
#include "LoginManager.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "GameRequestHandler.h"

// Class Declaration:
class LoginRequestHandler;
class MenuRequestHandler;
class RoomRequestHandler;
class RoomAdminRequestHandler;
class RoomMemberRequestHandler;
class GameRequestHandler;

// RequestHandlerFactory Class:
class RequestHandlerFactory
{
public:
    // C'tors & D'tors:
    RequestHandlerFactory(const LoginManager& loginManager, IDatabase* database, const RoomManager& roomManager, const StatisticsManager& statisticsManager, const GameManager& gameManager);
    ~RequestHandlerFactory();

    // Methods:
    LoginRequestHandler* createLoginRequestHandler();
    MenuRequestHandler* createMenuRequestHandler(LoggedUser& user);
    RoomAdminRequestHandler* createRoomAdminRequestHandler(LoggedUser& user, Room& room);
    RoomMemberRequestHandler* createRoomMemberRequestHandler(LoggedUser& user, Room& room);
    GameRequestHandler* createGameRequestHandler(LoggedUser& user, Game* game);
    LoginManager& getLoginManager();
    RoomManager& getRoomManager();
    StatisticsManager& getStatisticsManager();
    GameManager& getGameManager();

private:
    // Fields:
    LoginManager m_loginManager;
    IDatabase* m_database;
    RoomManager m_roomManager;
    StatisticsManager m_statisticsManager;
    GameManager m_gameManager;
};