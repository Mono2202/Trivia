#pragma once

// Includes:
#include "LoggedUser.h"
#include <vector>

// Defines:
#define GAME_ACTIVE 1

// Using:
using std::string;
using std::vector;

// RoomData Struct:
typedef struct RoomData{
	unsigned int id;
	string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
	bool isHeadToHead;
} RoomData;

// Room Class:
class Room
{
public:
	// C'tors & D'tors:
	Room() = default;
	Room(const RoomData& data);
	~Room() = default;

	// Methods:
	void setIsActive(unsigned int flag);
	void addUser(LoggedUser user);
	void removeUser(LoggedUser user);
	vector<string> getAllUsers() const;
	RoomData getRoomData() const;

private:
	// Fields:
	RoomData m_metadata;
	vector<LoggedUser> m_users;
};
