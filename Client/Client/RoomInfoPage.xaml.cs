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
    /// Interaction logic for RoomInfoPage.xaml
    /// </summary>
    public partial class RoomInfoPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;
        private BackgroundWorker autoRefreshWorker = new BackgroundWorker();
        private bool manualLeave = false;

        public RoomInfoPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];

            // Getting the user's premissions:
            bool isAdmin = (bool)App.Current.Properties["m_admin"];

            // Condition: user is an admin
            if (isAdmin)
            {
                btnCloseRoom.Visibility = Visibility.Visible;
                btnStartGame.Visibility = Visibility.Visible;
            }

            // Condition: normal user
            else
            {
                btnLeaveRoom.Visibility = Visibility.Visible;
            }

            // Setting the background worker:
            autoRefreshWorker.WorkerSupportsCancellation = true;
            autoRefreshWorker.WorkerReportsProgress = true;
            autoRefreshWorker.DoWork += autoRefreshWorkerDoWork;
            autoRefreshWorker.ProgressChanged += autoRefreshWorkerProgressChanged;
            autoRefreshWorker.RunWorkerCompleted += autoRefreshWorkerRunWorkerCompleted;
            autoRefreshWorker.RunWorkerAsync();
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

        private void autoRefreshWorkerProgressChanged(object sender, ProgressChangedEventArgs e) { UpdateUsersFunc(); }

        private void autoRefreshWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // Condition: normal user
            if (!(bool)App.Current.Properties["m_admin"] && !manualLeave)
            {
                // Sending LeaveRoomRequest to server:
                LeaveRoomRequest request = new LeaveRoomRequest { };
                m_communicator.Write(Serializer.SerializeRequest(request));
                string serverMessage = m_communicator.Read();

                // Moving to the main menu page:
                MessageBox.Show("Room is Closed");
                m_mainWindow.Content = new MainMenuPage();
            }
        }

        private void UpdateUsers(object sender, RoutedEventArgs e) { UpdateUsersFunc(); }

        private void UpdateUsersFunc()
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
            GetRoomStateResponse responseUsers = Deserializer.DeserializeResponse<GetRoomStateResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));

            // Sharing the fields:
            App.Current.Properties["amountOfQuestions"] = responseUsers.QuestionCount;
            App.Current.Properties["timeForQuestion"] = responseUsers.AnswerTimeout;

            // Condition: game has begun
            if (responseUsers.HasGameBegun && !(bool)App.Current.Properties["m_admin"])
            {
                // Setting the flag:
                manualLeave = true;

                // Sending StartGameRequest to server:
                StartGameRequest startGameRequest = new StartGameRequest { };
                m_communicator.Write(Serializer.SerializeRequest(startGameRequest));
                string startGameResponse = m_communicator.Read();

                // Cancels the background worker:
                autoRefreshWorker.CancelAsync();

                // Moving to the game page:
                m_mainWindow.Content = new GamePage();
                return;
            }
            
            // Getting the users list:
            string[] users = responseUsers.Players.Split(new[] { ", " }, StringSplitOptions.None);
            string formattedUsers = "";

            // Showing the users list:
            for (int i = 0; i < users.Length; i++)
            {
                // Condition: admin user
                if (i == 0)
                {
                    formattedUsers += users[i] + " - Admin\n";
                }

                else
                {
                    formattedUsers += users[i] + '\n';
                }
            }
            lblRoom.Content = App.Current.Properties["m_roomName"];
            txtUsers.Text = formattedUsers;
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

        private void StartGameButtonClick(object sender, RoutedEventArgs e)
        {
            // Sending StartGameRequest to server:
            StartGameRequest request = new StartGameRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                MessageBox.Show(eResponse.Message);
                return;
            }

            // Setting the flag:
            manualLeave = true;

            // Cancels the background worker:
            autoRefreshWorker.CancelAsync();

            // Moving to the game page:
            m_mainWindow.Content = new GamePage();
        }

        private void LeaveRoomButtonClick(object sender, RoutedEventArgs e)
        {
            // Sending LeaveRoomRequest to server:
            LeaveRoomRequest request = new LeaveRoomRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                MessageBox.Show(eResponse.Message);
                return;
            }

            // Setting the flag:
            manualLeave = true;

            // Cancels the background worker:
            autoRefreshWorker.CancelAsync();

            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
