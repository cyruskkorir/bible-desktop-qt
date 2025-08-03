#include <QApplication>
#include "BibleWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    BibleWindow window;
    window.setWindowTitle("Bible App");
    window.setWindowIcon(QIcon(":/icons/bible.png"));
    window.showMaximized();
    //set dark mode
    app.setStyleSheet(R"(
            QWidget { background-color: #121212; color: #e0e0e0; }
            QPushButton { background-color: #1e1e1e; color: #ffffff; border: 1px solid #333; }
            QListWidget { background-color: #1e1e1e; color: #ffffff; }
            QLineEdit { background-color: #1e1e1e; color: #ffffff; }
            QComboBox { background-color: #1e1e1e; color: #ffffff; }
            QSpinBox { background-color: #1e1e1e; color: #ffffff; }
        )");
    return app.exec();
}
    