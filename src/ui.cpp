#include "ui.hpp"

#include <QtWidgets>

UI::UI(size_t height, size_t width) {
    resize(width, height);
    setWindowTitle("Шифрование методами подстановки");

    _window = new QWidget(this);
    setCentralWidget(_window);
    initMenuBar();
    initValidator();
    initLayout();

    show();
}

void UI::initLayout() {
    _input_line = new QLineEdit(_window);
    _input_line->setValidator(_input_text_validator);
}

void UI::initValidator() {
    QRegularExpression rx("(?(?=[А-Я_])[^Й]|^$){0,30}");
    _input_text_validator = new QRegularExpressionValidator(rx, _window);
    _key_validator = new QIntValidator(0, 1000, _window);
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
    fileMenu->addAction("Сохранить");
    fileMenu->addAction("Сохранить как");
    QAction* nonInteractiveAction = fileMenu->addAction("");
    nonInteractiveAction->setEnabled(false);
    connectedAction = fileMenu->addAction("Выход");
    connect(connectedAction, &QAction::triggered, this, &QApplication::quit);

    _encrypt_menu = menuBar()->addMenu(QObject::tr("Зашифровать"));
    connectedAction = _encrypt_menu->addAction("Моноалфавитная");
    connect(connectedAction, &QAction::triggered, this, &UI::askKey);
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Гомофоническая"));
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Полиалфавитная"));
    notImplementedActions.emplace_back(
        _encrypt_menu->addAction("Полиграммная"));
    _encrypt_menu->setEnabled(false);

    _decrypt_menu = menuBar()->addMenu(QObject::tr("Дешифровать"));
    _decrypt_menu->addAction("Моноалфавитная");
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

void UI::askKey() {
    bool ok;
    _key_input_dialog = new QInputDialog(_window);
    _key_input_dialog->setCancelButtonText("Отмена");

    QString key =
        QInputDialog::getText(this, "Ключ", "", QLineEdit::Normal, "", &ok);

    int pos = 0;

    ok = ok && _key_validator->validate(key, pos) == QIntValidator::Acceptable;

    QMessageBox* msg;
    if (ok && !key.isEmpty()) {
        QMessageBox::information(this, "Key", key);
    } else {
        QMessageBox::warning(this, "", "Значение ключа неправильное");
    }
}

void UI::openFile() {
    _current_file = QFileDialog::getOpenFileName(this, tr("Открыть файл"), ".",
                                                 tr("Text Files (*.txt)"));
    _encrypt_menu->setEnabled(true);
    _decrypt_menu->setEnabled(true);
}

void UI::createTemporaryFile() {
    _current_file = "Новый файл";
    _encrypt_menu->setEnabled(true);
    _decrypt_menu->setEnabled(true);
}

void UI::showNotImplementedWarning() {
    QMessageBox::warning(this, "", "Указанный метод не реализован");
}

void UI::showAbout() {
    QMessageBox::information(this, "О программе", ABOUT_MESSAGE);
}

void UI::showHelp() { QMessageBox::information(this, "Помощь", HELP_MESSAGE); }
