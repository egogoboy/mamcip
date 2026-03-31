#include "ui.hpp"

#include <QtWidgets>
#include <iostream>

#include "encoding.cpp"

UI::UI(size_t height, size_t width) {
    resize(width, height);
    setWindowTitle("Шифрование методами подстановки");

    _window = new QWidget(this);
    setCentralWidget(_window);

    initLayout();
    initMenuBar();
    initValidator();
    initWidgets();

    show();
}

void UI::initLayout() {
    _layout = new QVBoxLayout(_window);
    _layout->setAlignment(Qt::AlignTop);
    setLayout(_layout);
}

void UI::resetLayout() {
    while (!_lines.empty()) {
        _layout->removeWidget(&_lines.top());
        _lines.pop();
    }

    _lines.emplace(_window);
    _lines.top().setValidator(_input_text_validator);
    _layout->addWidget(&_lines.top());
}

void UI::initValidator() {
    QRegularExpression rx("(?(?=[А-Я_])[^Й]|^$){0,30}");
    _input_text_validator = new QRegularExpressionValidator(rx, _window);
    _key_validator = new QIntValidator(0, 1000, _window);
}

void UI::initWidgets() {
    _key_input_dialog = new QInputDialog(_window);
    _key_input_dialog->setWindowFlag(Qt::WindowStaysOnTopHint);
    _key_input_dialog->setLabelText("Введите ключ шифрования");
    _key_input_dialog->setInputMode(QInputDialog::TextInput);
    _key_input_dialog->setOkButtonText("OK");
    _key_input_dialog->setCancelButtonText("ESC");

    connect(_key_input_dialog, &QDialog::accepted, this, [&]() {
        _lines.top().setReadOnly(true);
        QString text = _lines.top().text();
        _lines.emplace(_window);
        _lines.top().setReadOnly(true);
        _lines.top().setText(_cur_method(text, askKey()));
        _layout->addWidget(&_lines.top());
    });
}

void UI::initMenuBar() {
    std::vector<QAction*> notImplementedActions;
    notImplementedActions.reserve(6);
    QAction* connectedAction = nullptr;
    menuBar()->setNativeMenuBar(false);

    QMenu* fileMenu = menuBar()->addMenu(QObject::tr("Файл"));
    connectedAction = fileMenu->addAction("Создать");
    connect(connectedAction, &QAction::triggered, this,
            &UI::createTemporaryFile);
    connectedAction = fileMenu->addAction("Открыть");
    connect(connectedAction, &QAction::triggered, this, &UI::openFile);
    _save_action = fileMenu->addAction("Сохранить");
    connect(_save_action, &QAction::triggered, this, &UI::saveFile);
    connectedAction = fileMenu->addAction("Сохранить как");
    connect(connectedAction, &QAction::triggered, this, &UI::saveFileAs);
    QAction* nonInteractiveAction = fileMenu->addAction("");
    nonInteractiveAction->setEnabled(false);
    connectedAction = fileMenu->addAction("Выход");
    connect(connectedAction, &QAction::triggered, this, &QApplication::quit);

    _encrypt_menu = menuBar()->addMenu(QObject::tr("Зашифровать"));
    connectedAction = _encrypt_menu->addAction("Моноалфавитная");
    connect(connectedAction, &QAction::triggered, this, &UI::encode);
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Гомофоническая"));
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Полиалфавитная"));
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Полиграммная"));
    _encrypt_menu->setEnabled(false);

    _decrypt_menu = menuBar()->addMenu(QObject::tr("Дешифровать"));
    connectedAction = _decrypt_menu->addAction("Моноалфавитная");
    connect(connectedAction, &QAction::triggered, this, &UI::decode);
    notImplementedActions.emplace_back(
        _decrypt_menu->addAction("Гомофоническая"));
    notImplementedActions.emplace_back(
        _decrypt_menu->addAction("Полиалфавитная"));
    notImplementedActions.emplace_back(
        _decrypt_menu->addAction("Полиграммная"));
    _decrypt_menu->setEnabled(false);

    QMenu* infoMenu = menuBar()->addMenu(QObject::tr("Справка"));
    connectedAction = infoMenu->addAction("О программе");
    connect(connectedAction, &QAction::triggered, this, &UI::showAbout);
    connectedAction = infoMenu->addAction("Помощь");
    connect(connectedAction, &QAction::triggered, this, &UI::showHelp);

    connectedAction = menuBar()->addAction(QObject::tr("Выход"));
    connect(connectedAction, &QAction::triggered, this, &QApplication::quit);

    for (QAction* action : notImplementedActions) {
        connect(action, &QAction::triggered, this,
                &UI::showNotImplementedWarning);
    }
}

int UI::askKey() {
    QString key = _key_input_dialog->textValue();

    int pos = 0;
    if (_key_validator->validate(key, pos)) {
        _key_input_dialog->hide();
    } else {
        QMessageBox* wrong_message = new QMessageBox(_window);
        wrong_message->setModal(true);
        wrong_message->setAttribute(Qt::WA_DeleteOnClose);
        wrong_message->setInformativeText("Значение ключа неправильное");
        wrong_message->show();
        wrong_message->setIcon(QMessageBox::Critical);
        _key_input_dialog->show();
    }

    return key.toInt();
}

void UI::encode() {
    _cur_method = encoding::encode;
    _key_input_dialog->setTextValue("");
    _key_input_dialog->show();
}

void UI::decode() {
    _cur_method = encoding::decode;
    _key_input_dialog->setTextValue("");
    _key_input_dialog->show();
}

void UI::openFile() {
    _current_file = QFileDialog::getOpenFileName(this, tr("Открыть файл"), ".",
                                                 tr("Text Files (*.txt)"));
    _file.setFileName(_current_file);
    _encrypt_menu->setEnabled(true);
    _decrypt_menu->setEnabled(true);
    _save_action->setEnabled(true);

    resetLayout();
    readContentFromFile();
}

void UI::readContentFromFile() {
    if (!_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::information(this, tr("Не удалось открыть файл"),
                                 _file.errorString());
        return;
    }

    QString input_text;
    QTextStream in(&_file);
    in >> input_text;
    _lines.top().setText(input_text);
    _file.close();
}

void UI::createTemporaryFile() {
    resetLayout();
    _save_action->setEnabled(false);
    _encrypt_menu->setEnabled(true);
    _decrypt_menu->setEnabled(true);
}

void UI::saveFileAs() {
    _file.setFileName(QFileDialog::getSaveFileName(
        _window, tr("Сохранить как"), "Новый файл.txt", tr("All Files (*)")));
    saveFile();
}

void UI::saveFile() {
    if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Не удалось открыть файл"),
                                 _file.errorString());
        return;
    }

    QTextStream out(&_file);
    out << _lines.top().text();
    _file.close();
}

void UI::showNotImplementedWarning() {
    QMessageBox* warn = new QMessageBox(_window);
    warn->setIcon(QMessageBox::Warning);
    warn->setStandardButtons(QMessageBox::NoButton);
    warn->setInformativeText("Указанный метод не реализован");

    QPushButton* cancel_button =
        warn->addButton(QMessageBox::StandardButton::Cancel);
    cancel_button->hide();
    warn->setEscapeButton(cancel_button);

    warn->show();
}

void UI::showAbout() {
    QMessageBox::information(this, "О программе", ABOUT_MESSAGE);
}

void UI::showHelp() { QMessageBox::information(this, "Помощь", HELP_MESSAGE); }
