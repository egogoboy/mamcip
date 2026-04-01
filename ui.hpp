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
    void onWorkspaceEdit();

   private:
    class CustomTextEdit : public QTextEdit {
       public:
        explicit CustomTextEdit(QWidget* parent = nullptr)
            : QTextEdit(parent) {}

       protected:
        void keyPressEvent(QKeyEvent* event) override {
            if (event->key() == Qt::Key_Backspace) {
                QTextEdit::keyPressEvent(event);
                return;
            }

            if (event->text().contains(QRegExp("[^А-ИК-Я_\b]")) ||
                toPlainText().size() >= 30) {
                return;
            }

            QTextEdit::keyPressEvent(event);
        }
    };

    class HelpWindow : public QWidget {
       public:
        explicit HelpWindow(QWidget* parent = nullptr) : QWidget(parent) {
            setWindowTitle("Помощь");
            resize(600, 400);

            auto* layout = new QVBoxLayout(this);

            auto* textEdit = new QPlainTextEdit(this);
            textEdit->setReadOnly(true);

            textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

            textEdit->setPlainText(HELP_MESSAGE);

            layout->addWidget(textEdit);
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

    std::unique_ptr<HelpWindow> _help_window;

    QString (*_cur_method)(const QString&, int);

    QFile _file;

    bool _file_opened = false;
    std::stack<QString> _lines;

    const QString ABOUT_MESSAGE = R"(
Шифрование методами подстановки

Шифрование моноалфавитным методом

Судаков Е.Ю., ИПБ-23)";

    const static inline QString HELP_MESSAGE = R"(
Инструкция по работе с программой.

При запуске поле ввода заблокировано, для начала работы возможны два варианта действий:
1. нажать «Файл» - «Создать» и ввести строку, удовлетворяющую следующим условиям:
- строка содержит только прописные русские буквы и символы ‘_’, заменяющие пробел;
- максимальная длина введенной строки - 30 символов.
2. Нажать «Файл» - «Открыть», что дает возможность открыть уже существующий файл.

Алгоритм шифрования может быть применен при выборе ключа шифрования. Ключом шифра является целое положительное число.

Зашифрование:
1. ввести открытый текст с клавиатуры или загрузить из файла;
2. выбрать метод зашифрования в выпадающем меню, нажмите «Зашифровать» - «Полиграммная»;
3. ввести значение ключа, нажать «ОК»;
4. результат появится в новой строке.

Расшифрования:
1. ввести шифротекст или загрузить из файла;
2. выбрать метод расшифрования в выпадающем меню, нажмите «Расшифровать» - «Полиграммная»;  
3. ввести значение ключа, нажать «ОК»;
4. Результат появится в новой строке

В меню «Файл» представлены следующие опции:
1. «Создать» - очистка поля и предоставление доступа ввода текста;
2. «Открыть» - загрузка текста из уже существующего файла с расширением .txt;
3. «Сохранить» - сохранение последней непустой строки в файл, доступна только при предварительном открытии файла;
4. «Сохранить как» - сохранение последней строчки в новый файл.

Описание моноалфавитного метода.

При моноалфавитной замене каждой букве алфавита открытого текста ставится в соответствие одна буква закрытого текста из этого же алфавита. 
Уравнение зашифрования имеет вид:
yi = (xi + k)mod n, где:
– yi – код  i –го символа закрытого текста;
– xi – код  i –го символа открытого текста;
– k – константа, являющаяся смещением и равная значению ключа;
– n = 32 – степень подстановки.
                     
Уравнение расшифрования соответственно равно:
xi = (n  + yi – k)mod n
)";
};
