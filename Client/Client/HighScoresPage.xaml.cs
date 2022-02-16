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
    /// Interaction logic for HighScoresPage.xaml
    /// </summary>
    public partial class HighScoresPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;

        public HighScoresPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];

            // Sending GetHighScoreRequest to server:
            GetHighScoreRequest request = new GetHighScoreRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                MessageBox.Show(eResponse.Message);
                return;
            }

            // Deserializing the highscores message:
            GetHighScoreResponse response = Deserializer.DeserializeResponse<GetHighScoreResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            string[] playerStats = response.HighScores.Split(',');

            // Setting the highscores:

            if (playerStats.Length > 0)
            {
                lblUser1.Content = "1. " + playerStats[0];
            }

            if (playerStats.Length > 1)
            {
                lblUser2.Content = "2. " + playerStats[1];
            }

            if (playerStats.Length > 2)
            {
                lblUser3.Content = "3. " + playerStats[2];
            }

            if (playerStats.Length > 3)
            {
                lblUser4.Content = "4. " + playerStats[3];
            }

            if (playerStats.Length > 4)
            {
                lblUser5.Content = "5. " + playerStats[4];
            }
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the statistics page:
            m_mainWindow.Content = new StatisticsPage();
        }
    }
}
