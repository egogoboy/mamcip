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
    void createFile();
    void saveFile();
    void saveFileAs();
    void readContentFromFile();
    void enableWorkspace();
    void enableHelp();

   private:
    class CustomTextEdit : public QTextEdit {
       public:
        CustomTextEdit(QWidget* parent) : QTextEdit(parent) {}

       protected:
        void keyPressEvent(QKeyEvent* event) override {
            if (event->key() == Qt::Key_Backspace) {
                QTextEdit::keyPressEvent(event);
            }

            if (event->text().contains(QRegExp("[^А-ИК-Я_\b]"))) {
                return;
            }

            if (toPlainText().size() >= 30) {
                return;
            }

            QTextEdit::keyPressEvent(event);
        }
    };

    void initMenuBar();
    void initLayout();
    void resetWorkspace();
    void initValidator();
    void initWidgets();
    void validateInputText();
    std::optional<int> askKey();

    QWidget* _window;
    QStackedLayout* _layout;
    CustomTextEdit* _workspace;

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

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const QString HELP_MESSAGE = R"(
1. Ввести открытый текст с клавиатуры или загрузить из файла
2. Выбрать метод зашифрования/расшифрования в выпадающем меню  
3. Ввести целое положительное значение ключа, используемого при зашифровании/расшифровании
)";
};
