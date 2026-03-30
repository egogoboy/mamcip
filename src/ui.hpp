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
    void openFile();
    void createTemporaryFile();

   private:
    void initMenuBar();
    void initLayout();
    void initValidator();
    void initWidgets();

    QWidget* _window;
    QValidator* _input_text_validator;
    QValidator* _key_validator;
    QLineEdit* _input_line;
    QInputDialog* _key_input_dialog;
    QMenu* _encrypt_menu;
    QMenu* _decrypt_menu;

    QString _current_file;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const QString HELP_MESSAGE = R"(
Not implemented
    )";
};
