#include "ui.hpp"

#include <QtWidgets>

UI::UI(size_t height, size_t width) {
    resize(width, height);
    setWindowTitle("Шифрование методами подстановки");

    initMenuBar();

    show();
}

void UI::initMenuBar() {
    std::vector<QAction*> notImplementedActions;
    notImplementedActions.reserve(6);
    QAction* connectedAction = nullptr;
    menuBar()->setNativeMenuBar(false);

    QMenu* fileMenu = menuBar()->addMenu(QObject::tr("Файл"));
    fileMenu->addAction("Создать");
    fileMenu->addAction("Открыть");
    fileMenu->addAction("Сохранить");
    fileMenu->addAction("Сохранить как");
    QAction* nonInteractiveAction = fileMenu->addAction("");
    nonInteractiveAction->setEnabled(false);
    connectedAction = fileMenu->addAction("Выход");
    connect(connectedAction, &QAction::triggered, this, &QApplication::quit);

    QMenu* encryptMenu = menuBar()->addMenu(QObject::tr("Зашифровать"));
    connectedAction = encryptMenu->addAction("Моноалфавитная");
    connect(connectedAction, &QAction::triggered, this, &UI::askKey);
    notImplementedActions.emplace_back(
        encryptMenu->addAction("Гомофоническая"));
    notImplementedActions.emplace_back(
        encryptMenu->addAction("Полиалфавитная"));
    notImplementedActions.emplace_back(encryptMenu->addAction("Полиграммная"));
    encryptMenu->setEnabled(false);

    QMenu* decryptMenu = menuBar()->addMenu(QObject::tr("Дешифровать"));
    decryptMenu->addAction("Моноалфавитная");
    notImplementedActions.emplace_back(
        decryptMenu->addAction("Гомофоническая"));
    notImplementedActions.emplace_back(
        decryptMenu->addAction("Полиалфавитная"));
    notImplementedActions.emplace_back(decryptMenu->addAction("Полиграммная"));
    decryptMenu->setEnabled(false);
    decryptMenu->setStyleSheet("color: red");

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

    QString key =
        QInputDialog::getText(this, "Ключ", "", QLineEdit::Normal, "", &ok);

    if (ok && !key.isEmpty()) {
        QMessageBox::information(this, "Key", key);
    }
}

void UI::showNotImplementedWarning() {
    QMessageBox::warning(this, "", "Указанный метод не реализован");
}

void UI::showAbout() {
    QMessageBox::information(this, "О программе", ABOUT_MESSAGE);
}

void UI::showHelp() { QMessageBox::information(this, "Помощь", HELP_MESSAGE); }
