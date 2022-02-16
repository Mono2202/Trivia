using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    // ErrorResponse Class:
    class ErrorResponse
    {
        public string Message { get; set; }
    }

    // LoginResponse Class:
    class LoginResponse
    {
        public int Status { get; set; }
    }

    // SignupResponse Class:
    class SignupResponse
    {
        public int Status { get; set; }
    }

    // LogoutResponse Class:
    class LogoutResponse
    {
        public int Status { get; set; }
    }

    // GetRoomsResponse Class:
    class GetRoomsResponse
    {
        public int Status { get; set; }
        public string Rooms { get; set; }
    }

    // GetPlayersInRoomResponse Class:
    class GetPlayersInRoomResponse
    {
        public string PlayersInRoom { get; set; }
    }

    // GetHighScoreResponse Class:
    class GetHighScoreResponse
    {
        public int Status { get; set; }
        public string HighScores { get; set; }
    }

    // GetPersonalStatsResponse Class:
    class GetPersonalStatsResponse
    {
        public int Status { get; set; }
        public string Statistics { get; set; }
    }

    // JoinRoomResponse Class:
    class JoinRoomResponse
    {
        public int Status { get; set; }
    }

    // CreateRoomResponse Class:
    class CreateRoomResponse
    {
        public int Status { get; set; }
    }

    // CloseRoomResponse Class:
    class CloseRoomResponse
    {
        public int Status { get; set; }
    }

    // StartGameResponse Class:
    class StartGameResponse
    {
        public int Status { get; set; }
    }

    // GetRoomStateResponse Class:
    class GetRoomStateResponse
    {
        public int Status { get; set; }
        public bool HasGameBegun { get; set; }
        public string Players { get; set; }
        public int QuestionCount { get; set; }
        public int AnswerTimeout { get; set; }
    }

    // GetHeadToHeadRoomResponse Class
    class GetHeadToHeadRoomResponse
    {
        public int Status { get; set; }
        public string Room { get; set; }
    }
    // LeaveRoomResponse Class:
    class LeaveRoomResponse
    {
        public int Status { get; set; }
    }

    // LeaveGameResponse Class:
    class LeaveGameResponse
    {
        public int Status { get; set; }
    }

    // GetQuestionResponse Class:
    class GetQuestionResponse
    {
        public int Status { get; set; }
        public string Question { get; set; }
        public string Answers { get; set; }
    }

    // SubmitAnswerResponse Class:
    class SubmitAnswerResponse
    {
        public int Status { get; set; }
        public int CorrectAnswerID { get; set; }
    }

    // GetGameResultsResponse Class:
    class GetGameResultsResponse
    {
        public int Status { get; set; }
        public string Results { get; set; }
    }

    class Deserializer
    {
        // Constants:
        public const int ERROR_RESPONSE = 200;


        // Methods:

        /*
         * Deserializing a Response
         */
        public static T DeserializeResponse<T>(string buffer)
        {
            // Building the response:
            return JsonConvert.DeserializeObject<T>(buffer);
        }
    }
}
