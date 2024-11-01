#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>
#include <QTime>
#include <QTimer>

class QuizState;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class QuizState;
    friend class QuestionState;
    friend class ResultState;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setState(QuizState* state);
    void updateQuestion();
    void showResults();
    void showWelcome();

    int getCurrentQuestionIndex() const { return currentQuestionIndex; }
    void incrementQuestionIndex() { currentQuestionIndex++; }
    int getQuestionsCount() const { return questions.size(); }

private slots:
    void nextQuestion();
    void updateTimer();

private:
    void setupUI();
    void createQuestions();
    void checkAnswer();

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *questionLabel;
    QGroupBox *answersGroup;
    QVBoxLayout *answersLayout;
    QPushButton *nextButton;
    QVector<QRadioButton*> radioButtons;

    QLabel *timerLabel;
    QTimer *timer;
    QTime quizTime;
    QTime elapsedTime;

    struct Question {
        QString text;
        QVector<QString> answers;
        int correctAnswer;
    };

    QVector<Question> questions;
    int currentQuestionIndex;
    int score;
    bool quizStarted;

    QuizState* currentState;
};

#endif
