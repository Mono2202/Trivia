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
    /// Interaction logic for GameResultsPage.xaml
    /// </summary>
    public partial class GameResultsPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        private BackgroundWorker autoRefreshWorker = new BackgroundWorker();
        private GetGameResultsResponse gameResults;

        public GameResultsPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];

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
                Thread.Sleep(3000);
            }
        }

        private void autoRefreshWorkerProgressChanged(object sender, ProgressChangedEventArgs e) { UpdateGetResults(); }

        private void autoRefreshWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            // Sleep:
            Thread.Sleep(2000);

            // Sending GetGameResultRequest to server:
            GetGameResultRequest request = new GetGameResultRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();
            gameResults = Deserializer.DeserializeResponse<GetGameResultsResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));

            // Showing the game results:
            lblResults.Content = "";
            string[] results = gameResults.Results.Split(new[] { ",&" }, StringSplitOptions.None);
            for (int i = 0; i < results.Length; i++)
            {
                // Getting the current user's results:
                string[] currentResults = results[i].Split(new[] { "&&&" }, StringSplitOptions.None);

                // Showing the current player's results:
                lblResults.Content += "Username: " + currentResults[0] + "     Score: " + currentResults[1] + "\n";
            }

            // Showing the Back button:
            btnBack.Visibility = Visibility.Visible;
        }

        private void GetResults(object sender, RoutedEventArgs e) { UpdateGetResults(); }

        private void UpdateGetResults()
        {
            // Sending GetGameResultRequest to server:
            GetGameResultRequest request = new GetGameResultRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                lblResults.Content = "Waiting for game to end...";
                return;
            }

            // Condition: game didn't end
            gameResults = Deserializer.DeserializeResponse<GetGameResultsResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            if (gameResults.Status == 0)
            {
                lblResults.Content = "Waiting for game to end...";
                return;
            }

            // Cancelling the background worker:
            autoRefreshWorker.CancelAsync();
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Sending LeaveGameRequest to server:
            LeaveGameRequest request = new LeaveGameRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
