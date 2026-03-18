#pragma once

#include <QtWidgets>
#include <cstdlib>

class UI : public QMainWindow {
    Q_OBJECT

   public:
    UI(size_t height, size_t width);

   private slots:
    void showNotImplementedWarning();
    void showAbout();
    void showHelp();
    void askKey();

   private:
    void initMenuBar();

    const QString ABOUT_MESSAGE = R"(
Программа "Шифрование методами подстановки"

Назначение программы: программа предназначена для ознакомления с шифрованием методами подстановки

Автор: Судаков Егор, группа ИПБ-23)";

    const QString HELP_MESSAGE = R"(
Not implemented
    )";
};
