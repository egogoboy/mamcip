#include "ui.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    UI ui(400, 600);
    return app.exec();
}
