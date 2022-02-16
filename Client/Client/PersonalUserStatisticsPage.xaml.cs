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
    /// Interaction logic for PersonalUserStatisticsPage.xaml
    /// </summary>
    public partial class PersonalUserStatisticsPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;

        public PersonalUserStatisticsPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the statistics window:
            m_mainWindow.Content = new StatisticsPage();
        }

        private void LoadStats(object sender, RoutedEventArgs e)
        {
            // Sending GetPersonalStatsRequest to server:
            GetPersonalStatsRequest request = new GetPersonalStatsRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                MessageBox.Show(eResponse.Message);
                return;
            }

            // Deserializing the personal stats message:
            GetPersonalStatsResponse response = Deserializer.DeserializeResponse<GetPersonalStatsResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            string[] stats = response.Statistics.Split(new[] { ", " }, StringSplitOptions.None);

            // Setting the stats values in the labels:
            if (stats.Length == 4)
            {
                lblTotalGames.Content = stats[0];
                lblTotalAnswers.Content = stats[1];
                lblCorrectAnswers.Content = stats[2];
                lblAvgAnswerTime.Content = stats[3].Substring(0, 4);
            }
        }
    }
}
