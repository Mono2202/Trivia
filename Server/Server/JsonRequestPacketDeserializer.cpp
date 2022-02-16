#include "JsonRequestPacketDeserializer.h"

// Static Methods:

/*
JSON request deserializer to login request
Input : buffer  - the login request
Output: request - the LoginRequest struct
*/
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer& buffer)
{
    // Inits:
    LoginRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the LoginRequest struct:
    request.username = jsonData["Username"];
    request.password = jsonData["Password"];

    // Returning the LoginRequest:
    return request;
}

/*
JSON request deserializer to signup request
Input : buffer  - the signup request
Output: request - the SignupRequest struct
*/
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer& buffer)
{
    // Inits:
    SignupRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the SignupRequest struct:
    request.username = jsonData["Username"];
    request.password = jsonData["Password"];
    request.email = jsonData["Email"];
    request.address = jsonData["Address"];
    request.phoneNumber = jsonData["PhoneNumber"];
    request.birthDate = jsonData["BirthDate"];

    // Returning the SignupRequest:
    return request;
}

/*
JSON request deserializer to get players in room request
Input : buffer  - the get players in room request
Output: request - the GetPlayersInRoomRequest struct
*/
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(Buffer& buffer)
{
    // Inits:
    GetPlayersInRoomRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the GetPlayersInRoomRequest struct:
    request.roomID = jsonData["RoomID"];

    // Returning the GetPlayersInRoomRequest:
    return request;
}

/*
JSON request deserializer to join room request
Input : buffer  - the join room request
Output: request - the JoinRoomRequest struct
*/
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer& buffer)
{
    // Inits:
    JoinRoomRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the JoinRoomRequest struct:
    request.roomID = jsonData["RoomID"];

    // Returning the JoinRoomRequest:
    return request;
}

/*
JSON request deserializer to create room request
Input : buffer  - the create room request
Output: request - the CreateRoomRequest struct
*/
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer& buffer)
{
    // Inits:
    CreateRoomRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the CreateRoomRequest struct:
    request.roomName = jsonData["RoomName"];
    request.maxUsers = jsonData["MaxUsers"];
    request.questionCount = jsonData["QuestionCount"];
    request.answerTimeout = jsonData["AnswerTimeout"];
    request.isHeadToHead = jsonData["isHeadToHead"];

    // Returning the CreateRoomRequest:
    return request;
}

/*
JSON request deserializer to submit answer request
Input : buffer  - the submit answer request
Output: request - the SubmitAnswerRequest struct
*/
SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(Buffer& buffer)
{
    // Inits:
    SubmitAnswerRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the SubmitAnswerRequest struct:
    request.answerID = jsonData["AnswerID"];
    request.timeSpent = jsonData["TimeSpent"];

    // Returning the SubmitAnswerRequest:
    return request;
}

/*
JSON request deserializer to add question request
Input : buffer  - the add question request
Output: request - the AddQuestionRequest struct
*/
AddQuestionRequest JsonRequestPacketDeserializer::deserializeAddQuestionRequest(Buffer& buffer)
{
    // Inits:
    AddQuestionRequest request;
    string data = "";
    int i = 0;

    // Reading the data:
    for (i = DATA_STARTING_BYTE; i < buffer.size(); i++) {
        data += buffer[i];
    }

    // Building the data JSON:
    json jsonData = json::parse(data);

    // Building the SubmitAnswerRequest struct:
    request.question = jsonData["Question"];
    request.correctAnswer = jsonData["CorrectAnswer"];
    request.wrongAnswer1 = jsonData["WrongAnswer1"];
    request.wrongAnswer2 = jsonData["WrongAnswer2"];
    request.wrongAnswer3 = jsonData["WrongAnswer3"];

    // Returning the SubmitAnswerRequest:
    return request;
}
