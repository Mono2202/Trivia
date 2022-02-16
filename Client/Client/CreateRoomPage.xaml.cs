using AmRoMessageDialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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
    /// Interaction logic for CreateRoomPage.xaml
    /// </summary>
    public partial class CreateRoomPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;

        public CreateRoomPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
        }

        bool IsAllDigits(string s) => s.All(char.IsDigit);

        private void CreateRoomButtonClick(object sender, RoutedEventArgs e)
        {
            // Condition: one of the fields is empty
            if (txtRoomName.Text == "" || txtNumberOfPlayers.Text == "" || txtNumberOfQuestions.Text == "" || txtTimeForQuestion.Text == "" ||
                !IsAllDigits(txtNumberOfPlayers.Text) || !IsAllDigits(txtNumberOfQuestions.Text) || !IsAllDigits(txtTimeForQuestion.Text) ||
                txtNumberOfPlayers.Text == "0" || txtNumberOfQuestions.Text == "0" || txtTimeForQuestion.Text == "0")
            {
                m_errorMessageBox.Show("Invalid input...", "ERROR");
                return;
            }

            // Condition: too many questions
            if (IsAllDigits(txtNumberOfQuestions.Text) && int.Parse(txtNumberOfQuestions.Text) > 100)
            {
                m_errorMessageBox.Show("Too many questions...", "ERROR");
                return;
            }

            // Condition: too many players
            if (IsAllDigits(txtNumberOfPlayers.Text) && int.Parse(txtNumberOfPlayers.Text) > 16)
            {
                m_errorMessageBox.Show("Too many players...", "ERROR");
                return;
            }

            // Condition: too much time 
            if (IsAllDigits(txtTimeForQuestion.Text) && int.Parse(txtTimeForQuestion.Text) > 30)
            {
                m_errorMessageBox.Show("Too much time...", "ERROR");
                return;
            }

            // Sending CreateRoomRequest to server:
            CreateRoomRequest request = new CreateRoomRequest
            {
                RoomName = txtRoomName.Text,
                MaxUsers = int.Parse(txtNumberOfPlayers.Text),
                QuestionCount = int.Parse(txtNumberOfQuestions.Text),
                AnswerTimeout = int.Parse(txtTimeForQuestion.Text),
                isHeadToHead = 0
            };

            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Sharing the room's info:
            App.Current.Properties["m_roomName"] = txtRoomName.Text;
            App.Current.Properties["m_admin"] = true;

            // Moving to the room info page:
            m_infoMessageBox.Show("Room Created Successfully!", "INFO");
            m_mainWindow.Content = new RoomInfoPage();
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
