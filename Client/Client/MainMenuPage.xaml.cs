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
    /// Interaction logic for MainMenuPage.xaml
    /// </summary>
    public partial class MainMenuPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;

        public MainMenuPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
        }

        private void CreateRoomWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the create room page:
            m_mainWindow.Content = new CreateRoomPage();
        }

        private void  HeadToHeadWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the HeadToHead room page:
            m_mainWindow.Content = new HeadToHeadRoomPage();
        }

        private void JoinRoomWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the join room page:
            m_mainWindow.Content = new JoinRoomPage();
        }

        private void StatisticsWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the statistics page:
            m_mainWindow.Content = new StatisticsPage();
        }

        private void AddQuestionWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the add question page:
            m_mainWindow.Content = new AddQuestionPage();
        }

        private void ExitButtonClick(object sender, RoutedEventArgs e)
        {
            // Sending LogoutRequest to server:
            LogoutRequest request = new LogoutRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Exiting the program:
            m_infoMessageBox.Show("Thank You for Playing Our Game :)", "THANK YOU");
            m_mainWindow.Close();
        }
    }
}
