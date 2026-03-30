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
    void initLayout();
    void initValidator();

    QWidget* _window;
    QValidator* _input_text_validator;
    QValidator* _key_validator;
    QLineEdit* _input_line;
    QInputDialog* _key_input_dialog;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const QString HELP_MESSAGE = R"(
Not implemented
    )";
};
