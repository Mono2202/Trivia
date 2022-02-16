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
    /// Interaction logic for JoinRoomPage.xaml
    /// </summary>
    public partial class JoinRoomPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;
        private BackgroundWorker autoRefreshWorker = new BackgroundWorker();

        public JoinRoomPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];

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

        private void autoRefreshWorkerProgressChanged(object sender, ProgressChangedEventArgs e) { UpdateRoomsFunc(); }

        private void autoRefreshWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {  }

        private void UpdateRooms(object sender, RoutedEventArgs e) { UpdateRoomsFunc(); }

        private void JoinRoomButtonClick(object sender, RoutedEventArgs e)
        {
            // Condition: no room was selected
            if (boxRooms.SelectedItem == null)
            {
                m_errorMessageBox.Show("No Room was Selected...", "ERROR");
                return;
            }

            // Sending GetRoomsRequest to server:
            GetRoomsRequest request = new GetRoomsRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Getting the room's ID:
            GetRoomsResponse response = Deserializer.DeserializeResponse<GetRoomsResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            ListBoxItem selectedItem = (ListBoxItem)boxRooms.SelectedItem;
            string m_roomName = selectedItem.Content.ToString();
            string substringRooms = response.Rooms.Substring(response.Rooms.IndexOf(m_roomName));
            substringRooms = substringRooms.Split(new[] { "&&&" }, StringSplitOptions.None)[1];
            substringRooms = substringRooms.Split(',')[0];
            int roomId = int.Parse(substringRooms);

            // Sending JoinRoomRequest to server:
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

            // Sharing the room's info:
            App.Current.Properties["m_roomName"] = m_roomName;
            App.Current.Properties["m_admin"] = false;

            // Stopping the auto refresher:
            autoRefreshWorker.CancelAsync();

            // Moving to the room info page:
            m_mainWindow.Content = new RoomInfoPage();
        }

        private void UpdateRoomsFunc()
        {
            // Sending GetRoomsRequest to server:
            GetRoomsRequest request = new GetRoomsRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Getting the rooms:
            GetRoomsResponse response = Deserializer.DeserializeResponse<GetRoomsResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            string[] rooms = response.Rooms.Split(new[] { ", " }, StringSplitOptions.None);
            boxRooms.Items.Clear();

            // Adding the rooms to the list:
            for (int i = 0; i < rooms.Length; i++)
            {
                ListBoxItem itm = new ListBoxItem();
                itm.Content = rooms[i].Split(new[] { "&&&" }, StringSplitOptions.None)[0];

                if (!String.IsNullOrEmpty(itm.Content.ToString()))
                {
                    boxRooms.Items.Add(itm);
                }
            }
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Stopping the auto refresher:
            autoRefreshWorker.CancelAsync();

            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
