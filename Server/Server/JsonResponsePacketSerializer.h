#pragma once

// Includes:
#include "json.hpp"
#include <sstream>
#include <string>
#include <iomanip>
#include "Room.h"

// Response Codes Enum:
enum ResponseCode
{
	ERROR_RESPONSE = 200,
	LOGIN_RESPONSE,
	SIGNUP_RESPONSE, 
	LOGOUT_RESPONSE,
	GET_ROOMS_RESPONSE, 
	GET_PLAYERS_IN_ROOM_RESPONSE, 
	GET_HIGH_SCORE_RESPONSE, 
	GET_PERSONAL_STATS_RESPONSE, 
	JOIN_ROOM_RESPONSE, 
	CREATE_ROOM_RESPONSE, 
	CLOSE_ROOM_RESPONSE, 
	START_GAME_RESPONSE, 
	GET_ROOM_STATE_RESPONSE, 
	LEAVE_ROOM_RESPONSE, 
	LEAVE_GAME_RESPONSE, 
	GET_QUESTION_RESPONSE, 
	SUBMIT_ANSWER_RESPONSE, 
	GET_GAME_RESULT_RESPONSE,
	ADD_QUESTION_RESPONSE,
	GET_HEAD_TO_HEAD_ROOM_RESPONSE
};

// General Defines:
#define INT_SIZE 32
#define BYTE_SIZE 8
#define BINARY_BASE 2
#define DATA_LENGTH_BYTES 4

// Using:
using std::string;
using std::vector;
using std::map;
using json = nlohmann::json;

// Typedef:
typedef vector<unsigned char> Buffer;

// PlayerResults Struct:
typedef struct PlayerResults {
	string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;

	bool operator<(const PlayerResults& a) const
	{
		return correctAnswerCount > a.correctAnswerCount;
	}
} PlayerResults;

// ErrorResponse Struct:
typedef struct ErrorResponse {
	string message;
} ErrorResponse;

// LoginResponse Struct:
typedef struct LoginResponse {
	unsigned int status;
} LoginResponse;

// SignupResponse Struct:
typedef struct SignupResponse {
	unsigned int status;
} SignupResponse;

// LogoutResponse Struct:
typedef struct LogoutResponse {
	unsigned int status;
} LogoutResponse;

// GetRoomsResponse Struct:
typedef struct GetRoomsResponse {
	unsigned int status;
	vector<RoomData> rooms;
} GetRoomsResponse;

// GetPlayersInRoomResponse Struct:
typedef struct GetPlayersInRoomResponse {
	vector<string> players;
} GetPlayersInRoomResponse;

// GetHighScoreResponse Struct:
typedef struct GetHighScoreResponse {
	unsigned int status;
	vector<string> statistics;
} GetHighScoreResponse;

// GetPersonalStatsResponse Struct:
typedef struct GetPersonalStatsResponse {
	unsigned int status;
	vector<string> statistics;
} GetPersonalStatsResponse;

// JoinRoomResponse Struct:
typedef struct JoinRoomResponse {
	unsigned int status;
} JoinRoomResponse;

// CreateRoomResponse Struct:
typedef struct CreateRoomResponse {
	unsigned int status;
} CreateRoomResponse;

// CloseRoomResponse Struct:
typedef struct CloseRoomResponse {
	unsigned int status;
} CloseRoomResponse;

// StartGameResponse Struct:
typedef struct StartGameResponse {
	unsigned int status;
} StartGameResponse;

// GetRoomStateResponse Struct:
typedef struct GetRoomStateResponse {
	unsigned int status;
	bool hasGameBegun;
	vector<string> players;
	unsigned int questionCount;
	int answerTimeout;
} GetRoomStateResponse;

// LeaveRoomResponse Struct:
typedef struct LeaveRoomResponse {
	unsigned int status;
} LeaveRoomResponse;

// LeaveGameResponse Struct:
typedef struct LeaveGameResponse {
	unsigned int status;
} LeaveGameResponse;

// GetQuestionResponse Struct:
typedef struct GetQuestionResponse {
	unsigned int status;
	string question;
	map<unsigned int, string> answers;
} GetQuestionResponse;

// SubmitAnswerResponse Struct:
typedef struct SubmitAnswerResponse {
	unsigned int status;
	unsigned int correctAnswerID;
} SubmitAnswerResponse;

// GetGameResultsResponse Struct:
typedef struct GetGameResultsResponse {
	unsigned int status;
	vector<PlayerResults> results;
} GetGameResultsResponse;

// AddQuestionResponse Struct:
typedef struct AddQuestionResponse {
	unsigned int status;
} AddQuestionResponse;

// GetHeadToHeadRoomResponse Struct:
typedef struct GetHeadToHeadRoomResponse {
	unsigned int status;
	RoomData room;
} GetHeadToHeadRoomResponse;

// JsonResponsePacketSerializer Class:
class JsonResponsePacketSerializer
{
public:
	// Static Methods:
	static Buffer serializeResponse(const ErrorResponse& response);
	static Buffer serializeResponse(const LoginResponse& response);
	static Buffer serializeResponse(const SignupResponse& response);
	static Buffer serializeResponse(const LogoutResponse& response);
	static Buffer serializeResponse(const GetRoomsResponse& response);
	static Buffer serializeResponse(const GetPlayersInRoomResponse& response);
	static Buffer serializeResponse(const GetHighScoreResponse& response);
	static Buffer serializeResponse(const GetPersonalStatsResponse& response);
	static Buffer serializeResponse(const JoinRoomResponse& response);
	static Buffer serializeResponse(const CreateRoomResponse& response);
	static Buffer serializeResponse(const CloseRoomResponse& response);
	static Buffer serializeResponse(const StartGameResponse& response);
	static Buffer serializeResponse(const GetRoomStateResponse& response);
	static Buffer serializeResponse(const LeaveRoomResponse& response);
	static Buffer serializeResponse(const LeaveGameResponse& response);
	static Buffer serializeResponse(const GetQuestionResponse& response);
	static Buffer serializeResponse(const SubmitAnswerResponse& response);
	static Buffer serializeResponse(const GetGameResultsResponse& response);
	static Buffer serializeResponse(const AddQuestionResponse& response);
	static Buffer serializeResponse(const GetHeadToHeadRoomResponse& response);

private:
	// Private Static Methods:
	static string intToBinary(const int& num);
	static void insertDataLength(Buffer& buffer, string& dataLength);
	static void insertData(Buffer& buffer, string& data);
};
