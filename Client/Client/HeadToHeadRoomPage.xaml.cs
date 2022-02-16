using AmRoMessageDialog;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Client
{
    /// <summary>
    /// Interaction logic for Page1.xaml
    /// </summary>
    public partial class HeadToHeadRoomPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;
        private System.Windows.Threading.DispatcherTimer dispatcherTimer;
        private BackgroundWorker autoRefreshWorker = new BackgroundWorker();
        private bool isAdmin;

        public HeadToHeadRoomPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
            isAdmin = false;
            btnCloseRoom.Visibility = Visibility.Hidden;

            // Setting the background worker:
            autoRefreshWorker.WorkerSupportsCancellation = true;
            autoRefreshWorker.WorkerReportsProgress = true;
            autoRefreshWorker.DoWork += autoRefreshWorkerDoWork;
            autoRefreshWorker.ProgressChanged += autoRefreshWorkerProgressChanged;
            autoRefreshWorker.RunWorkerCompleted += autoRefreshWorkerRunWorkerCompleted;

            // Timer init:
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
        }

        private void autoRefreshWorkerDoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                if (autoRefreshWorker.CancellationPending)
                {
                    e.Cancel = true;
                    return;
                }

                autoRefreshWorker.ReportProgress(0);
                Thread.Sleep(1000);
            }
        }

        private void autoRefreshWorkerProgressChanged(object sender, ProgressChangedEventArgs e) { GetRoomState(); }

        private void autoRefreshWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) { }

        private void LoadHeadToHead(object sender, RoutedEventArgs e)
        {

            // Check if there is already a HeadToHead Room  (request)
            GetHeadToHeadRoomRequest request = new GetHeadToHeadRoomRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Getting the response:
            GetHeadToHeadRoomResponse response = Deserializer.DeserializeResponse<GetHeadToHeadRoomResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            string roomName = response.Room.Split(new[] { "&&&" }, StringSplitOptions.None)[0];
            int roomId = int.Parse(response.Room.Split(new[] { "&&&" }, StringSplitOptions.None)[1]);

            // Condition: room exists, player joins
            if (!roomName.Equals(""))
            {
                App.Current.Properties["m_roomName"] = roomName;

                // Joining room:
                JoinRoomRequest joinRequest = new JoinRoomRequest { RoomID = roomId };
                m_communicator.Write(Serializer.SerializeRequest(joinRequest));
                string joinServerMessage = m_communicator.Read();

                // Condition: error response
                if (joinServerMessage[0] == Deserializer.ERROR_RESPONSE)
                {
                    ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(joinServerMessage.Substring(5, joinServerMessage.IndexOf('}') - 4));
                    m_errorMessageBox.Show(eResponse.Message, "ERROR");
                    return;
                }

                // Starting the timer:
                GetRoomState();
                dispatcherTimer.Start();
            }

            // Condition: room creation
            else
            {
                // Sending CreateRoomRequest to server:
                CreateRoomRequest createRequest = new CreateRoomRequest
                {
                    RoomName = "Head To Head",
                    MaxUsers = 2,
                    QuestionCount = 5,
                    AnswerTimeout = 15,
                    isHeadToHead = 1
                };

                m_communicator.Write(Serializer.SerializeRequest(createRequest));
                string createServerMsg = m_communicator.Read();

                // Condition: error response
                if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
                {
                    ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                    m_errorMessageBox.Show(eResponse.Message, "ERROR");
                    return;
                }

                // Sharing the room's info:
                App.Current.Properties["m_roomName"] = "Head To Head";
                isAdmin = true;
                btnCloseRoom.Visibility = Visibility.Visible;

                // Start background worker:
                autoRefreshWorker.RunWorkerAsync();
            }
        }

        private void GetRoomState()
        {
            // Sending GetRoomStateRequest to server:
            GetRoomStateRequest request = new GetRoomStateRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                // Cancels the background worker:
                autoRefreshWorker.CancelAsync();
                return;
            }

            // Getting the users list:
            GetRoomStateResponse response = Deserializer.DeserializeResponse<GetRoomStateResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));

            // Sharing the fields:
            App.Current.Properties["amountOfQuestions"] = response.QuestionCount;
            App.Current.Properties["timeForQuestion"] = response.AnswerTimeout;

            // Condition: game has begun
            if (response.Players.Split(new[] { ", " }, StringSplitOptions.None).Length == 2)
            {
                // Condition: admin start game
                if (isAdmin)
                {
                    // Sending StartGameRequest to server:
                    StartGameRequest startGameRequest = new StartGameRequest { };
                    m_communicator.Write(Serializer.SerializeRequest(startGameRequest));
                    string startGameResponse = m_communicator.Read();
                    btnCloseRoom.Visibility = Visibility.Hidden;
                }

                // Starting the timer:
                dispatcherTimer.Start();
                lblUserJoined.Content = "User Joined!";

                // Cancels the background worker:
                autoRefreshWorker.CancelAsync();
            }
        }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            lblTimer.Content = (int.Parse((string)lblTimer.Content) - 1).ToString();

            if ((string)lblTimer.Content == "4" && !isAdmin)
            {
                // Sending StartGameRequest to server:
                StartGameRequest startGameRequest = new StartGameRequest { };
                m_communicator.Write(Serializer.SerializeRequest(startGameRequest));
                string startGameResponse = m_communicator.Read();
            }

            if ((string)lblTimer.Content == "0")
            {
                // Stopping the timer:
                dispatcherTimer.Stop();

                // Moving to the game page:
                m_mainWindow.Content = new GamePage();
                return;
            }
        }

        private void CloseRoomButtonClick(object sender, RoutedEventArgs e)
        {
            // Sending CloseRoomRequest to server:
            CloseRoomRequest request = new CloseRoomRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                MessageBox.Show(eResponse.Message);
                return;
            }

            // Cancels the background worker:
            autoRefreshWorker.CancelAsync();

            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
