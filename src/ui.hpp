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
    std::optional<int> askKey();

    QWidget* _window;
    QStackedLayout* _layout;
    QTextEdit* _workspace;

    QValidator* _input_text_validator;
    QValidator* _key_validator;

    QInputDialog* _key_input_dialog;

    QAction* _save_action;
    QAction* _show_help_action;

    QMenu* _encrypt_menu;
    QMenu* _decrypt_menu;

    QString (*_cur_method)(const QString&, int);

    QFile _file;

    bool _file_opened = false;
    std::stack<QString> _lines;

    QString _current_file;
    QString _help_text;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const QString HELP_MESSAGE_ENCODE = R"(
1. Ввести открытый текст с клавиатуры или загрузить из файла
2. Выбрать метод зашифрования в выпадающем меню  
3. Выбрать из выпадающего списка ключ, используемый при зашифровании

Описание моноалфавитного метода.
Каждой букве алфавита открытого текста ставится в соответствие одна буква закрытого текста из этого же алфавита.
)";
    const QString HELP_MESSAGE_DECODE = R"(
1. Ввести или загрузить шифротекст из файла
2. Выбрать метод расшифрования в выпадающем меню  
3. Выбрать из выпадающего списка ключ, используемый при расшифровании

Описание моноалфавитного метода.
Каждой букве алфавита открытого текста ставится в соответствие одна буква закрытого текста из этого же алфавита.
)";
};
