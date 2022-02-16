using System;
using System.Collections.Generic;
using System.Linq;
using System.Media;
using System.Net;
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
    /// Interaction logic for GamePage.xaml
    /// </summary>
    public partial class GamePage : Page
    {
        // Fields:
        MainWindow m_mainWindow;
        Communicator m_communicator;
        private Button[] buttonArr;
        private Brush[] brushArr;
        private int amountOfQuestions;
        private int timeForQuestion;
        private int currentQuestion;
        private int correctAnswers;
        private System.Windows.Threading.DispatcherTimer dispatcherTimer;
        private int correctAnswerID;
        private bool isAnswered;
        private SoundPlayer musicPlayer;

        public GamePage()
        {
            // Inits:
            InitializeComponent();
            m_mainWindow = (MainWindow)App.Current.Properties["m_mainWindow"];
            m_communicator = (Communicator)App.Current.Properties["m_communicator"];

            // Creating the button array:
            buttonArr = new Button[] { this.btnAnswer0, this.btnAnswer1, this.btnAnswer2, this.btnAnswer3 };
            brushArr = new Brush[] { this.btnAnswer0.Background, this.btnAnswer1.Background, this.btnAnswer2.Background, this.btnAnswer3.Background };

            // Getting the fields:
            amountOfQuestions = (int)App.Current.Properties["amountOfQuestions"];
            timeForQuestion = (int)App.Current.Properties["timeForQuestion"];
            lblTimeLeft.Content = "Time Left: " + timeForQuestion.ToString();
            currentQuestion = 1;
            correctAnswers = 0;

            // Timer init:
            dispatcherTimer = new System.Windows.Threading.DispatcherTimer();
            dispatcherTimer.Tick += dispatcherTimer_Tick;
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();

            // Starting the music:
            //System.IO.Stream str = Properties.Resources.triviaMusic;
            //musicPlayer = new SoundPlayer(str);
            //musicPlayer.Play();
        }

        private void LoadedGame(object sender, RoutedEventArgs e) { //musicPlayer.Play(); 
            NextQuestion(); }

        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            // Changing the time left label:
            int timeLeft = int.Parse(lblTimeLeft.Content.ToString().Substring(11)) - 1;
            lblTimeLeft.Content = "Time Left: " + timeLeft.ToString();

            // Condition: time over
            if ((string)lblTimeLeft.Content == "Time Left: 0")
            {
                if (!isAnswered)
                {
                    // Sending SubmitAnswerRequest to server:
                    SubmitAnswerRequest request = new SubmitAnswerRequest { AnswerID = 4, TimeSpent = timeForQuestion };
                    m_communicator.Write(Serializer.SerializeRequest(request));
                    string serverMessage = m_communicator.Read();

                    // Condition: error response
                    if (serverMessage[0] == Deserializer.ERROR_RESPONSE) { return; }

                    // Setting the flag:
                    isAnswered = true;

                    // Showing the correct answer:
                    dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
                    dispatcherTimer.Start();
                    ShowCorrectAnswer();
                }

                else
                {
                    // Moving to the next question:
                    currentQuestion++;
                    NextQuestion();
                }  
            }
        }

        private void AnswerCheck(object sender, RoutedEventArgs e)
        {
            // Checking whether the user answered the question already:
            if (isAnswered)
                return;
            isAnswered = true;

            // Sending SubmitAnswerRequest to server:
            int index = (sender as Button).Name.ToString()[9] - '0';
            int timeLeft = int.Parse(lblTimeLeft.Content.ToString().Substring(11));
            SubmitAnswerRequest request = new SubmitAnswerRequest { AnswerID = index, TimeSpent = timeForQuestion - timeLeft };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE) { return; }

            // Checking whether the answer is right
            SubmitAnswerResponse response = Deserializer.DeserializeResponse<SubmitAnswerResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            correctAnswerID = response.CorrectAnswerID;
            if (index == response.CorrectAnswerID)
            {
                correctAnswers++;
            }
            ShowCorrectAnswer();
        }

        private void NextQuestion()
        {
            // Last question reached:
            if (currentQuestion > amountOfQuestions)
            {
                dispatcherTimer.Stop();
                musicPlayer.Stop();
                m_mainWindow.Content = new GameResultsPage();
                return;
            }

            // Sending GetQuestionRequest to server:
            GetQuestionRequest request = new GetQuestionRequest { };
            m_communicator.Write(Serializer.SerializeRequest(request));
            string serverMessage = m_communicator.Read();

            // Condition: error response
            if (serverMessage[0] == Deserializer.ERROR_RESPONSE) { return; }

            // Deserializing the response:
            GetQuestionResponse response = Deserializer.DeserializeResponse<GetQuestionResponse>(serverMessage.Substring(5, serverMessage.IndexOf('}') - 4));
            string[] answersArr = response.Answers.Split(new[] { ",&" }, StringSplitOptions.None);

            // TODO: Update on each new question:
            for (int i = 0; i < answersArr.Length; i++)
            {
                buttonArr[i].Content = WebUtility.HtmlDecode(answersArr[i].Split(new[] { "&&&" }, StringSplitOptions.None)[1]);
            }
            txtQuestion.Text = WebUtility.HtmlDecode(response.Question);
            lblQuestionCount.Content = "Question: " + currentQuestion.ToString() + "/" + amountOfQuestions.ToString();
            lblTimeLeft.Content = "Time Left: " + timeForQuestion.ToString();

            // Resetting the timer:
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
            dispatcherTimer.Start();
            ShowNormalAnswers();
            isAnswered = false;
        }
        public void ShowCorrectAnswer()
        {
            lblCorrectAnswers.Content = "Correct Answers: " + correctAnswers.ToString();
            lblTimeLeft.Content = "Time Left: 2";

            for (int i = 0; i < buttonArr.Length; i++)
            {
                if (i != correctAnswerID)
                {
                    buttonArr[i].Background = Brushes.Gray;
                }
            }
        }

        public void ShowNormalAnswers()
        {
            for (int i = 0; i < buttonArr.Length; i++)
            {
                if (i != correctAnswerID)
                {
                    buttonArr[i].Background = brushArr[i];
                }
            }
        }
    }
}
