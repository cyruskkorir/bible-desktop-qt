#include <QApplication>
#include "BibleWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BibleWindow window;
    window.setWindowTitle("Bible App");
    window.setWindowIcon(QIcon(":/icons/bible.png"));
    window.showMaximized();
    return app.exec();
}
