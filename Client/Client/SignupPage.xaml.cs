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
    /// Interaction logic for SignupPage.xaml
    /// </summary>
    public partial class SignupPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        AmRoMessageBox m_errorMessageBox;
        AmRoMessageBox m_infoMessageBox;

        public SignupPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
            m_errorMessageBox = (AmRoMessageBox)App.Current.Properties["m_errorMessageBox"];
            m_infoMessageBox = (AmRoMessageBox)App.Current.Properties["m_infoMessageBox"];
        }

        private void SignupButtonClick(object sender, RoutedEventArgs e)
        {
            // Condition: empty fields
            if (txtUsername.Text == "" || txtPassword.Password == "" || txtEmail.Text == "" ||
                txtStreet.Text == "" || txtApt.Text == "" || txtCity.Text == "" || txtPhoneNumber.Text == "" ||
                txtDay.Text == "" || txtMonth.Text == "" || txtYear.Text == "")
            {
                m_errorMessageBox.Show("Some Fields are Empty...", "ERROR");
                return;
            }

            // Sending SignupRequest to server:
            SignupRequest request = new SignupRequest { Username = txtUsername.Text, Password = txtPassword.Password, Email = txtEmail.Text, 
                Address = txtStreet.Text + "-" + txtApt.Text + "-" + txtCity.Text, PhoneNumber = txtPhoneNumber.Text,
                BirthDate = txtDay.Text + "." + txtMonth.Text + "." + txtYear.Text };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE)
            {
                ErrorResponse eResponse = Deserializer.DeserializeResponse<ErrorResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
                m_errorMessageBox.Show(eResponse.Message, "ERROR");
                return;
            }

            // Moving to the login page:
            m_infoMessageBox.Show("Account Created Successfully!", "INFO");
            m_mainWindow.Content = new LoginPage();
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the login page:
            m_mainWindow.Content = new LoginPage();
        }
    }
}
