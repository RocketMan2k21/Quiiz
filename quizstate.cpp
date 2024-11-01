#include "quizstate.h"
#include "mainwindow.h"

void QuestionState::handleNext(MainWindow* window)
{
    // Використовуємо нові методи доступу
    if(window->getCurrentQuestionIndex() < window->getQuestionsCount() - 1) {
        window->incrementQuestionIndex();
        window->updateQuestion();
    } else {
        window->setState(new ResultState());
    }
}

void ResultState::handleNext(MainWindow* window)
{
    window->showResults();
}
