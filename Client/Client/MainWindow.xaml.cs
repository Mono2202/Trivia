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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        // Fields:
        private Communicator m_communicator;

        // C'tor:
        public MainWindow()
        {
            // Initializing the component:
            InitializeComponent();
        }

        private void WindowLoaded(object sender, RoutedEventArgs e)
        {

            // Sharing variables:
            App.Current.Properties["m_mainWindow"] = this;
            App.Current.Properties["m_errorMessageBox"] = new AmRoMessageBox
            {
                Background = "#FF0000",
                TextColor = "#000000",
                IconColor = "#3399ff",
                RippleEffectColor = "#000000",
                ClickEffectColor = "#FFFFFF",
                ShowMessageWithEffect = true,
                EffectArea = this,
                ParentWindow = this
            };
            App.Current.Properties["m_infoMessageBox"] = new AmRoMessageBox
            {
                Background = "#00FF00",
                TextColor = "#000000",
                IconColor = "#3399ff",
                RippleEffectColor = "#000000",
                ClickEffectColor = "#FFFFFF",
                ShowMessageWithEffect = true,
                EffectArea = this,
                ParentWindow = this
            };

            // Creating the socket:
            m_communicator = new Communicator();

            // Sharing the socket:
            App.Current.Properties["m_communicator"] = m_communicator;

            // Setting the starting page:
            this.Content = new LoginPage();
        }
    }
}
