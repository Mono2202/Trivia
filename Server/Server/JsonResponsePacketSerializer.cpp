#include "JsonResponsePacketSerializer.h"

// Static Methods:

/*
JSON response serializer to error response
Input : response - the error response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Message"] = response.message;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(ERROR_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to login response
Input : response - the login response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(LOGIN_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to signup response
Input : response - the signup response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(SIGNUP_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to logout response
Input : response - the logout response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(LOGOUT_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get rooms response
Input : response - the get rooms response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", roomList = "";
	int i = 0;

	// Concatenating the rooms vector to a string:
	for (i = 0; i < response.rooms.size(); i++) {
		roomList += response.rooms[i].name + "&&&" + std::to_string(response.rooms[i].id) + ", ";
	}
	
	// Creating the data:
	data["Rooms"] = roomList.substr(0, roomList.size() - 2);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_ROOMS_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get players in room response
Input : response - the get players in room response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", playersList = "";
	int i = 0;

	// Concatenating the players vector to a string:
	for (i = 0; i < response.players.size(); i++) {
		playersList += response.players[i] + ", ";
	}

	// Creating the data:
	data["PlayersInRoom"] = playersList.substr(0, playersList.size() - 2);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_PLAYERS_IN_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get high scores response
Input : response - the get high scores response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", highScoresList = "";
	int i = 0;

	// Concatenating the highscores vector to a string:
	for (i = 0; i < response.statistics.size(); i++) {
		highScoresList += response.statistics[i] + ", ";
	}

	// Creating the data:
	data["Status"] = response.status;
	data["HighScores"] = highScoresList.substr(0, highScoresList.size() - 2);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_HIGH_SCORE_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get personal stats response
Input : response - the get personal stats response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", personalStatsList = "";
	int i = 0;

	// Concatenating the players statistics vector to a string:
	for (i = 0; i < response.statistics.size(); i++) {
		personalStatsList += response.statistics[i] + ", ";
	}

	// Creating the data:
	data["Status"] = response.status;
	data["Statistics"] = personalStatsList.substr(0, personalStatsList.size() - 2);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_PERSONAL_STATS_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to join room response
Input : response - the join room response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(JOIN_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to create room response
Input : response - the create room response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(CREATE_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to close room response
Input : response - the close room response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(CLOSE_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to start game response
Input : response - the start game response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(START_GAME_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get room state response
Input : response - the get room state response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", playersList = "";
	int i = 0;

	// Concatenating the players vector to a string:
	for (i = 0; i < response.players.size(); i++) {
		playersList += response.players[i] + ", ";
	}

	// Creating the data:
	data["Status"] = response.status;
	data["HasGameBegun"] = response.hasGameBegun;
	data["Players"] = playersList.substr(0, playersList.size() - 2);
	data["QuestionCount"] = response.questionCount;
	data["AnswerTimeout"] = response.answerTimeout;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_ROOM_STATE_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to leave room response
Input : response - the leave room response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(LEAVE_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to leave game response
Input : response - the leave game response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(LEAVE_GAME_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get question response
Input : response - the get question response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", answersList = "";
	map<unsigned int, string>::iterator it;

	// Concatenating the answers map to a string:
	for (auto const& it : response.answers)
	{
		answersList += std::to_string(it.first) + "&&&" + it.second + ",&";   // string (key)
	}

	// Creating the data:
	data["Status"] = response.status;
	data["Question"] = response.question;
	data["Answers"] = answersList.substr(0, answersList.size() - 2);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_QUESTION_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to submit answer response
Input : response - the submit answer response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	data["CorrectAnswerID"] = response.correctAnswerID;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(SUBMIT_ANSWER_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get game results response
Input : response - the get game results response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "", gameResultsList = "";
	int i = 0;

	// Concatenating the game results vector to a string:
	for (i = 0; i < response.results.size(); i++) {
		gameResultsList += response.results[i].username + "&&&" + std::to_string(response.results[i].correctAnswerCount) + "&&&" +  
			std::to_string(response.results[i].wrongAnswerCount) + "&&&" + std::to_string(response.results[i].averageAnswerTime) + ",&";
	}

	// Creating the data:
	data["Status"] = response.status;
	data["Results"] = gameResultsList.substr(0, gameResultsList.size() - 2);;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_GAME_RESULT_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to add question response
Input : response - the add question response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const AddQuestionResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";

	// Creating the data:
	data["Status"] = response.status;
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(ADD_QUESTION_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}

/*
JSON response serializer to get rooms response
Input : response - the get rooms response
Output: buffer   - the serialized response
*/
Buffer JsonResponsePacketSerializer::serializeResponse(const GetHeadToHeadRoomResponse& response)
{
	// Inits:
	Buffer buffer;
	json data;
	string strData = "", dataLength = "";
	int i = 0;

	// Creating the data:
	data["Room"] = response.room.name + "&&&" + std::to_string(response.room.id);
	strData = data.dump();

	// Getting the data's length:
	dataLength = intToBinary(strData.length());

	// Building the buffer:
	buffer.push_back(GET_HEAD_TO_HEAD_ROOM_RESPONSE);
	insertDataLength(buffer, dataLength);
	insertData(buffer, strData);

	// Returning the buffer:
	return buffer;
}


// Private Static Methods:

/*
Converting an integer to a binary string
Input : num				- the integer
Output: paddedBinaryNum - the binary string
*/
string JsonResponsePacketSerializer::intToBinary(const int& num)
{
	// Inits:
	std::stringstream binaryNum;
	std::stringstream paddedBinaryNum;
	int i = 0, k = 0;

	// Converting the integer number to a binary number:
	for (i = INT_SIZE - 1; i >= 0; i--) {
		k = num >> i;
		if (k & 1) {
			binaryNum << "1";
		} else {
			binaryNum << "0";
		}
	}

	// Padding "0" to the left:
	paddedBinaryNum << std::setw(INT_SIZE) << std::setfill('0') << binaryNum.str();

	// Returning the padded binary number:
	return paddedBinaryNum.str();
}

/*
Inserting the data length bytes (1 - 4) to the buffer
Input : buffer	   - the buffer
		dataLength - the data length string
Output: < None >
*/
void JsonResponsePacketSerializer::insertDataLength(Buffer& buffer, string& dataLength)
{
	// Inits:
	int pos = 0, i = 0;

	// Bytes 1 - 4: Data Length
	for (i = 0; i < DATA_LENGTH_BYTES; i++)
	{
		// Pushing the current data length byte:
		buffer.push_back(std::stoi(dataLength.substr(pos, BYTE_SIZE), nullptr, BINARY_BASE));

		// Raising the position:
		pos += BYTE_SIZE;
	}
}

/*
Inserting the data bytes (5 - ...) to the buffer
Input : buffer - the buffer
		data   - the data string
Output: < None >
*/
void JsonResponsePacketSerializer::insertData(Buffer& buffer, string& data)
{
	// Inits:
	int i = 0;

	// Bytes 5 - ...: Data
	for (i = 0; i < data.length(); i++) {
		// Pushing the current data char byte:
		buffer.push_back(data[i]);
	}
}
