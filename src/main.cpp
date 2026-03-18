#include "ui.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    UI ui(1000, 800);
    return app.exec();
}
