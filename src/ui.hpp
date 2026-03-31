#pragma once

#include <QtWidgets>
#include <cstdlib>
#include <stack>

class UI : public QMainWindow {
    Q_OBJECT

   public:
    UI(size_t height, size_t width);

   private slots:
    void showNotImplementedWarning();
    void showAbout();
    void showHelp();
    void encode();
    void decode();
    void openFile();
    void createTemporaryFile();
    void saveFile();
    void saveFileAs();
    void readContentFromFile();

   private:
    void initMenuBar();
    void initLayout();
    void resetLayout();
    void initValidator();
    void initWidgets();
    int askKey();

    QWidget* _window;
    QVBoxLayout* _layout;
    QValidator* _input_text_validator;
    QValidator* _key_validator;
    QInputDialog* _key_input_dialog;
    QMenu* _encrypt_menu;
    QMenu* _decrypt_menu;
    QString (*_cur_method)(const QString&, int);
    QFile _file;
    bool _file_opened = false;
    std::stack<QLineEdit> _lines;

    QString _current_file;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const QString HELP_MESSAGE = R"(
Not implemented
    )";
};
