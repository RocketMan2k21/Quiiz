#ifndef QUIZSTATE_H
#define QUIZSTATE_H

class MainWindow;

class QuizState {
public:
    virtual ~QuizState() {}
    virtual void handleNext(MainWindow* window) = 0;
};

class QuestionState : public QuizState {
public:
    void handleNext(MainWindow* window) override;
};

class ResultState : public QuizState {
public:
    void handleNext(MainWindow* window) override;
};

#endif // QUIZSTATE_H
