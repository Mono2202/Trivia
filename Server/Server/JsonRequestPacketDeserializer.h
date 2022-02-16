#pragma once

// Includes:
#include "json.hpp"
#include <sstream>
#include <string>
#include <iomanip>

// General Defines:
#define INT_SIZE 32
#define BYTE_SIZE 8
#define BINARY_BASE 2
#define DATA_LENGTH_BYTES 4
#define DATA_STARTING_BYTE 1

// Using:
using std::string;
using std::vector;
using json = nlohmann::json;

// Typedef:
typedef vector<unsigned char> Buffer;

// LoginRequest Struct:
typedef struct LoginRequest {
	string username;
	string password;
} LoginRequest;

// SignupRequest Struct:
typedef struct SignupRequest {
	string username;
	string password;
	string email;
	string address;
	string phoneNumber;
	string birthDate;
} SignupRequest;

// GetPlayersInRoomRequest Struct:
typedef struct GetPlayersInRoomRequest {
	unsigned int roomID;
} GetPlayersInRoomRequest;

// JoinRoomRequest Struct:
typedef struct JoinRoomRequest {
	unsigned int roomID;
} JoinRoomRequest;

// CreateRoomRequest Struct:
typedef struct CreateRoomRequest {
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
	unsigned int isHeadToHead;
} CreateRoomRequest;

// SubmitAnswerRequest Struct:
typedef struct SubmitAnswerRequest {
	unsigned int answerID;
	unsigned int timeSpent;
} SubmitAnswerRequest;

// AddQuestionRequest Struct:
typedef struct AddQuestionRequest {
	string question;
	string correctAnswer;
	string wrongAnswer1;
	string wrongAnswer2;
	string wrongAnswer3;
};

// JsonRequestPacketDeserializer Class:
class JsonRequestPacketDeserializer
{
public:
	// Static Methods:
	static LoginRequest deserializeLoginRequest(Buffer& buffer);
	static SignupRequest deserializeSignupRequest(Buffer& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(Buffer& buffer);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(Buffer& buffer);
	static AddQuestionRequest deserializeAddQuestionRequest(Buffer& buffer);
};
