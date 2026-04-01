#include "ui.hpp"

#include <QtWidgets>

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
    _layout = new QStackedLayout(_window);
    _workspace = new CustomTextEdit(_window);
    _layout->addWidget(_workspace);
    _workspace->setAlignment(Qt::AlignmentFlag::AlignTop);
    _workspace->setVisible(false);
    _workspace->setReadOnly(true);
    connect(_workspace, &QTextEdit::textChanged, this, &UI::onWorkspaceEdit);
    setLayout(_layout);
}

void UI::resetWorkspace() {
    while (!_lines.empty()) {
        _lines.pop();
    }

    _workspace->setText("");
    _workspace->setVisible(true);
    _workspace->setReadOnly(false);
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
    connect(connectedAction, &QAction::triggered, this, &UI::createFile);
    connectedAction = fileMenu->addAction("Открыть");
    fileMenu->addSeparator();
    connect(connectedAction, &QAction::triggered, this, &UI::openFile);
    _save_action = fileMenu->addAction("Сохранить");
    _save_action->setEnabled(false);
    connect(_save_action, &QAction::triggered, this, &UI::saveFile);
    connectedAction = fileMenu->addAction("Сохранить как");
    fileMenu->addSeparator();
    connect(connectedAction, &QAction::triggered, this, &UI::saveFileAs);
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

    _decrypt_menu = menuBar()->addMenu(QObject::tr("Расшифровать"));
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
    _show_help_action = infoMenu->addAction("Помощь");
    connect(_show_help_action, &QAction::triggered, this, &UI::showHelp);
    _show_help_action->setEnabled(false);

    connectedAction = menuBar()->addAction(QObject::tr("Выход"));
    connect(connectedAction, &QAction::triggered, this, &QApplication::quit);

    for (QAction* action : notImplementedActions) {
        connect(action, &QAction::triggered, this,
                &UI::showNotImplementedWarning);
    }
}

void UI::initWidgets() {
    _key_input_dialog = new QInputDialog(_window);
    _key_input_dialog->setWindowFlag(Qt::WindowStaysOnTopHint);
    _key_input_dialog->setWindowTitle("Ключ");
    _key_input_dialog->setLabelText("");
    _key_input_dialog->setInputMode(QInputDialog::TextInput);
    _key_input_dialog->setOkButtonText("OK");
    _key_input_dialog->setCancelButtonText("ESC");

    connect(_key_input_dialog, &QDialog::accepted, this, [&]() {
        std::optional<int> key = askKey();
        if (key.has_value()) {
            if (_lines.empty()) {
                _lines.push(_workspace->toPlainText());
            }

            _lines.emplace(_cur_method(_lines.top(), key.value()));
            _workspace->append(_lines.top());
            _workspace->setReadOnly(true);
        }
    });
}

std::optional<int> UI::askKey() {
    QString key = _key_input_dialog->textValue();

    int pos = 0;
    if (_key_validator->validate(key, pos) && !key.isEmpty()) {
        _key_input_dialog->hide();
    } else {
        _key_input_dialog->show();
        QMessageBox* wrong_message = new QMessageBox(_window);
        wrong_message->setModal(true);
        wrong_message->setAttribute(Qt::WA_DeleteOnClose);
        wrong_message->setWindowTitle("");
        wrong_message->setInformativeText("Значение ключа неправильное");
        wrong_message->show();
        wrong_message->setIcon(QMessageBox::Critical);
        return std::nullopt;
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
    _file.setFileName(QFileDialog::getOpenFileName(
        this, tr("Открыть файл"), ".", tr("Text Files (*.txt)")));

    resetWorkspace();
    readContentFromFile();
    _save_action->setEnabled(true);
}

void UI::createFile() {
    resetWorkspace();
    _save_action->setEnabled(false);
}

void UI::saveFile() {
    if (!_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Не удалось открыть файл"),
                                 _file.errorString());
        return;
    }

    QTextStream out(&_file);
    if (!_lines.empty()) {
        out << _lines.top();
    } else {
        out << _workspace->toPlainText();
    }
    _file.close();
}

void UI::saveFileAs() {
    _file.setFileName(QFileDialog::getSaveFileName(
        _window, tr("Сохранить как"), "Новый файл.txt", tr("All Files (*)")));
    saveFile();
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
    _workspace->setText(input_text);
    _file.close();
}

void UI::showNotImplementedWarning() {
    QMessageBox* warn = new QMessageBox(_window);
    warn->setIcon(QMessageBox::Warning);
    warn->setStandardButtons(QMessageBox::NoButton);
    warn->setWindowTitle("");
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

void UI::showHelp() { QMessageBox::information(this, "", HELP_MESSAGE); }

void UI::onWorkspaceEdit() {
    if (_workspace->toPlainText().isEmpty()) {
        _encrypt_menu->setEnabled(false);
        _decrypt_menu->setEnabled(false);
        _show_help_action->setEnabled(false);
    } else {
        _encrypt_menu->setEnabled(true);
        _decrypt_menu->setEnabled(true);
        _show_help_action->setEnabled(true);
    }
}
