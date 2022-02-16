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
    /// Interaction logic for AddQuestionPage.xaml
    /// </summary>
    public partial class AddQuestionPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;

        public AddQuestionPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
        }

        private void AddQuestionClick(object sender, RoutedEventArgs e)
        {
            // Condition: one of the fields is empty
            if (txtQuestion.Text == "" || txtCorrectAnswer.Text == "" || txtWrongAnswer1.Text == "" || txtWrongAnswer2.Text == "" || txtWrongAnswer3.Text == "")
            {
                m_errorMessageBox.Show("Invalid input...", "ERROR");
                return;
            }

            // Sending AddQuestionRequest to server:
            AddQuestionRequest request = new AddQuestionRequest
            {
                Question = txtQuestion.Text,
                CorrectAnswer = txtCorrectAnswer.Text,
                WrongAnswer1 = txtWrongAnswer1.Text,
                WrongAnswer2 = txtWrongAnswer2.Text,
                WrongAnswer3 = txtWrongAnswer3.Text
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
            
            // Moving to the room info page:
            m_infoMessageBox.Show("Question Added Successfully!", "INFO");
            m_mainWindow.Content = new MainMenuPage();
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
