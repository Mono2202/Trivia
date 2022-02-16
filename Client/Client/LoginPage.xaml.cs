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
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;

        public LoginPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
        }

        private void LoginButtonClick(object sender, RoutedEventArgs e)
        {
            // Condition: empty fields
            if (txtUsername.Text == "" || txtPassword.Password == "")
            {
                m_errorMessageBox.Show("Some Fields are Empty!", "ERROR");
                return;
            }

            // Sending LoginRequest to server:
            LoginRequest request = new LoginRequest { Username = txtUsername.Text, Password = txtPassword.Password };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Moving to the main menu page:
            m_infoMessageBox.Show("Login Succesful!", "INFO");
            m_mainWindow.Content = new MainMenuPage();
        }

        private void SignupWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the signup page:
            m_mainWindow.Content = new SignupPage();
        }
    }
}
