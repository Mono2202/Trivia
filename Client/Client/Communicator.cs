using AmRoMessageDialog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Client
{
    class Communicator
    {
        // Fields:
        MainWindow m_mainWindow;
        AmRoMessageBox m_errorMessageBox;
        private NetworkStream m_socket;

        // Constants:
        private const string IP = "127.0.0.1";
        private const int PORT = 54321;
        private const int SIZE = 4096;


        // C'tor:

        public Communicator()
        {
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];

            // Creating the socket with the server:
            try
            {
                TcpClient client = new TcpClient();
                IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(IP), PORT);
                client.Connect(serverEndPoint);
                m_socket = client.GetStream();
            }

            catch
            {
                m_errorMessageBox.Show("Server not Running...", "ERROR");
                m_mainWindow.Close();
            }
        }


        // Methods:

        /*
         * Sends a message to the server
         * Input : message - the client's message
         * Output: < None >
         */
        public void Write(string message)
        {
            // Sending the message to the server:
            try
            {
                byte[] buffer = new ASCIIEncoding().GetBytes(message);
                m_socket.Write(buffer, 0, buffer.Length);
                m_socket.Flush();
            }
            
            catch
            {
                m_errorMessageBox.Show("Server not Running...", "ERROR");
                m_mainWindow.Close();
            }
        }

        /*
         * Reads a message from the server
         * Input : < None >
         * Output: the server's message
         */
        public string Read()
        {
            // Reading the message from the server:
            try
            {
                byte[] buffer = new byte[SIZE];
                m_socket.Read(buffer, 0, SIZE);
                return System.Text.Encoding.Default.GetString(buffer);
            }

            catch
            {
                m_errorMessageBox.Show("Server not Running...", "ERROR");
                m_mainWindow.Close();
                return "";
            }
        }
    }
}
