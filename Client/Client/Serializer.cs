using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    // LoginRequest Class:
    class LoginRequest
    {
        public string Username { get; set; }
        public string Password { get; set; }
    }

    // SignupRequest Class:
    class SignupRequest
    {
        public string Username { get; set; }
        public string Password { get; set; }
        public string Email { get; set; }
        public string Address { get; set; }
        public string PhoneNumber { get; set; }
        public string BirthDate { get; set; }
    }

    // LogoutRequest Class:
    class LogoutRequest { }

    // GetRoomsRequest Class:
    class GetRoomsRequest { }

    // GetPlayersInRoomRequest Class:
    class GetPlayersInRoomRequest
    {
        public int RoomID { get; set; }
    }

    // GetHighScoreRequest Class:
    class GetHighScoreRequest { }

    // GetPersonalStatsRequest Class:
    class GetPersonalStatsRequest { }

    // JoinRoomRequest Class:
    class JoinRoomRequest
    {
        public int RoomID { get; set; }
    }

    // CreateRoomRequest Class:
    class CreateRoomRequest
    {
        public string RoomName { get; set; }
        public int MaxUsers { get; set; }
        public int QuestionCount { get; set; }
        public int AnswerTimeout { get; set; }
        public int isHeadToHead { get; set; }
    }

    // CloseRoomRequest Class:
    class CloseRoomRequest { }

    // StartGameRequest Class:
    class StartGameRequest { }

    // GetRoomStateRequest Class:
    class GetRoomStateRequest { }

    // GetHeadToHeadRoomRequest Class:
    class GetHeadToHeadRoomRequest { }

    // LeaveRoomRequest Class:
    class LeaveRoomRequest { }

    // LeaveGameRequest Class:
    class LeaveGameRequest { }

    // GetQuestionRequest Class:
    class GetQuestionRequest { }

    // SubmitAnswerRequest Class:
    class SubmitAnswerRequest
    {
        public int AnswerID { get; set; }
        public int TimeSpent { get; set; }
    }

    // GetGameResultRequest Class:
    class GetGameResultRequest { }

    // AddQuestionRequest Class:
    class AddQuestionRequest
    {
        public string Question { get; set; }
        public string CorrectAnswer { get; set; }
        public string WrongAnswer1 { get; set; }
        public string WrongAnswer2 { get; set; }
        public string WrongAnswer3 { get; set; }
    }

    static class Serializer
    {
        // Constants:
        private const int LOGIN_REQUEST = 101;
        private const int SIGNUP_REQUEST = 102;
        private const int LOGOUT_REQUEST = 103;
        private const int GET_ROOMS_REQUEST = 104;
        private const int GET_PLAYERS_IN_ROOM_REQUEST = 105;
        private const int GET_HIGH_SCORE_REQUEST = 106;
        private const int GET_PERSONAL_STATS_REQUEST = 107;
        private const int JOIN_ROOM_REQUEST = 108;
        private const int CREATE_ROOM_REQUEST = 109;
        private const int CLOSE_ROOM_REQUEST = 110;
        private const int START_GAME_REQUEST = 111;
        private const int GET_ROOM_STATE_REQUEST = 112;
        private const int LEAVE_ROOM_REQUEST = 113;
        private const int LEAVE_GAME_REQUEST = 114;
        private const int GET_QUESTION_REQUEST = 115;
        private const int SUBMIT_ANSWER_REQUEST = 116;
        private const int GET_GAME_RESULT_REQUEST = 117;
        private const int ADD_QUESTION_REQUEST = 118;
        private const int GET_HEAD_TO_HEAD_ROOM_REQUEST = 119;

        // Methods:

        public static string SerializeRequest(LoginRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(LOGIN_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(SignupRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(SIGNUP_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(LogoutRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(LOGOUT_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetRoomsRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_ROOMS_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetPlayersInRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_PLAYERS_IN_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetHighScoreRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_HIGH_SCORE_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetPersonalStatsRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_PERSONAL_STATS_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(JoinRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(JOIN_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(CreateRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(CREATE_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(CloseRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(CLOSE_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(StartGameRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(START_GAME_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetRoomStateRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_ROOM_STATE_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(LeaveRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(LEAVE_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(LeaveGameRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(LEAVE_GAME_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetQuestionRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_QUESTION_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(SubmitAnswerRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(SUBMIT_ANSWER_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetGameResultRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_GAME_RESULT_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(AddQuestionRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(ADD_QUESTION_REQUEST, JsonConvert.SerializeObject(request));
        }

        public static string SerializeRequest(GetHeadToHeadRoomRequest request)
        {
            // Returning the buffer:
            return BuildBuffer(GET_HEAD_TO_HEAD_ROOM_REQUEST, JsonConvert.SerializeObject(request));
        }

        private static string BuildBuffer(int requestCode, string data)
        {
            // Inits:
            string buffer = "";
            
            // Getting the length string:
            byte[] byteLength = BitConverter.GetBytes(data.Length);
            string stringLength = System.Text.Encoding.Default.GetString(byteLength);

            // Building the buffer:
            buffer += (char)requestCode;
            buffer += stringLength;
            buffer += data;

            return buffer;
        }
    }
}
