#include "mainwindow.h"
#include "quizstate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , currentQuestionIndex(0)
    , score(0)
    , quizStarted(false)
{
    setupUI();
    createQuestions();
    setState(new QuestionState());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    quizTime.setHMS(0, 5, 0); // 5 minutes for quiz
    elapsedTime.setHMS(0, 0, 0);

    showWelcome();
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    timerLabel = new QLabel();
    timerLabel->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(timerLabel);

    questionLabel = new QLabel();
    questionLabel->setWordWrap(true);
    questionLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(questionLabel);

    answersGroup = new QGroupBox("Відповіді");
    answersLayout = new QVBoxLayout(answersGroup);

    for(int i = 0; i < 4; ++i) {
        QRadioButton* radio = new QRadioButton();
        radioButtons.append(radio);
        answersLayout->addWidget(radio);
    }

    mainLayout->addWidget(answersGroup);
    answersGroup->hide(); // Спочатку ховаємо варіанти відповідей

    nextButton = new QPushButton("Почати тест");
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextQuestion);
    mainLayout->addWidget(nextButton);
}

void MainWindow::showWelcome()
{
    questionLabel->setText("Ласкаво просимо до тесту!\n\n"
                           "У вас буде 5 хвилин на проходження тесту.\n"
                           "Тест складається з " + QString::number(questions.size()) +
                           " питань.\n\nНатисніть 'Почати тест' коли будете готові.");
    answersGroup->hide();
}

void MainWindow::nextQuestion()
{
    if (!quizStarted) {
        // Початок тесту
        quizStarted = true;
        timer->start(1000);
        nextButton->setText("Наступне питання");
        answersGroup->show();
        updateQuestion();
        return;
    }

    // Перевіряємо відповідь на поточне питання
    checkAnswer();

    currentState->handleNext(this);
}

void MainWindow::checkAnswer()
{
    if (currentQuestionIndex < questions.size()) {
        for(auto radio : radioButtons) {
            if(radio->isChecked() &&
                radio == radioButtons[questions[currentQuestionIndex].correctAnswer]) {
                score++;
                break;
            }
        }
    }
}

void MainWindow::updateQuestion()
{
    if(currentQuestionIndex < questions.size()) {
        questionLabel->setText("Питання " + QString::number(currentQuestionIndex + 1) +
                               " з " + QString::number(questions.size()) + ":\n\n" +
                               questions[currentQuestionIndex].text);
        for(int i = 0; i < radioButtons.size(); ++i) {
            radioButtons[i]->setText(questions[currentQuestionIndex].answers[i]);
            radioButtons[i]->setChecked(false);
        }
    }
}

void MainWindow::createQuestions()
{
    questions = {
        {"Яка столиця України?",
         {"Київ", "Харків", "Львів", "Одеса"},
         0},
        {"Скільки планет у Сонячній системі?",
         {"7", "8", "9", "10"},
         1},
        {"Який найбільший материк?",
         {"Північна Америка", "Південна Америка", "Африка", "Євразія"},
         3}
    };
}

void MainWindow::showResults()
{
    timer->stop();
    questionLabel->setText(QString("Тест завершено!\n\n"
                                   "Ваш результат: %1 з %2\n"
                                   "Витрачений час: %3")
                               .arg(score-1)
                               .arg(questions.size())
                               .arg(elapsedTime.toString("mm:ss")));
    answersGroup->hide();
    nextButton->hide();
}

void MainWindow::setState(QuizState* state)
{
    // If there’s a currently active state, delete it or transition out
    delete currentState;

    // Update the current state to the new state
    currentState = state;
}

MainWindow::~MainWindow()
{
    delete timer;           // Clean up dynamically allocated objects
    delete currentState;
    // Other potential clean-ups, if any
}

void MainWindow::updateTimer()
{
    elapsedTime = elapsedTime.addSecs(1);
    quizTime = quizTime.addSecs(-1);
    timerLabel->setText("Залишилось часу: " + quizTime.toString("mm:ss"));

    if(quizTime == QTime(0, 0)) {
        setState(new ResultState());
    }
}
