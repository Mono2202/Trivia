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
    /// Interaction logic for StatisticsPage.xaml
    /// </summary>
    public partial class StatisticsPage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;

        public StatisticsPage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];
        }

        private void PersonalUserStatisticsWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the personal user statistics page:
            m_mainWindow.Content = new PersonalUserStatisticsPage();
        }

        private void HighScoresWindowButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the high scores page:
            m_mainWindow.Content = new HighScoresPage();
        }

        private void BackButtonClick(object sender, RoutedEventArgs e)
        {
            // Moving to the main menu page:
            m_mainWindow.Content = new MainMenuPage();
        }
    }
}
