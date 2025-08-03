#include "BibleWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QCoreApplication>
#include <QPushButton>
#include <QApplication>

BibleWindow::BibleWindow(QWidget* parent)
    : QWidget(parent),
      db("/usr/share/bible/bible.db")
{
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Failed to open Bible database.");
        return;
    }

    verseList = new QListWidget(this);
    favoriteButton = new QPushButton("Toggle Favorite", this);
    favoriteButton->setEnabled(false); // Initially disabled until a verse is selected
    bookSelector = new QComboBox(this);
    chapterSelector = new QComboBox(this);
    searchEdit = new QLineEdit(this); // ✅ Required for search functionality
    searchButton = new QPushButton("Search", this); // ✅ Required for search button

    bibleBooks = {
    "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
    "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel",
    "1 Kings", "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra",
    "Nehemiah", "Esther", "Job", "Psalms", "Proverbs",
    "Ecclesiastes", "Song of Solomon", "Isaiah", "Jeremiah", "Lamentations",
    "Ezekiel", "Daniel", "Hosea", "Joel", "Amos",
    "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
    "Zephaniah", "Haggai", "Zechariah", "Malachi",
    "Matthew", "Mark", "Luke", "John", "Acts",
    "Romans", "1 Corinthians", "2 Corinthians", "Galatians", "Ephesians",
    "Philippians", "Colossians", "1 Thessalonians", "2 Thessalonians",
    "1 Timothy", "2 Timothy", "Titus", "Philemon", "Hebrews",
    "James", "1 Peter", "2 Peter", "1 John", "2 John",
    "3 John", "Jude", "Revelation"
};
    bookSelector->addItems(bibleBooks);

    chapters =  {50, 40, 27, 36, 34, 24, 21, 4, 31, 24, 22, 25, 29, 36, 10, 13, 10, 42, 150, 31, 12, 8, 66, 52, 5, 48, 12, 14, 3, 9, 1, 4, 7, 3, 3, 3, 2, 14, 4,
            28, 16, 24, 21, 28, 16, 16, 13, 6, 6, 4, 4, 5, 3, 6, 4, 3, 1, 13, 5, 5, 3, 5, 1, 1, 1, 22};

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(bookSelector);
    layout->addWidget(chapterSelector);
    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->setSpacing(8);
    searchLayout->setContentsMargins(0, 0, 0, 0);
    themeToggleButton = new QPushButton("☀️ Light Mode", this);
    layout->addWidget(themeToggleButton);

    connect(themeToggleButton, &QPushButton::clicked, this, &BibleWindow::toggleTheme);


    // Make the button compact
    searchButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    // Add the horizontal layout to the main layout
    layout->addLayout(searchLayout);
    layout->addWidget(verseList);
    layout->addWidget(favoriteButton);

    connect(favoriteButton, &QPushButton::clicked, this, &BibleWindow::toggleFavorite);
    connect(bookSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BibleWindow::onBookChanged);
    connect(chapterSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BibleWindow::onChapterChanged);
    connect(verseList, &QListWidget::itemSelectionChanged, this, [this]() {
    favoriteButton->setEnabled(verseList->currentItem() != nullptr);
    });
    connect(searchButton, &QPushButton::clicked, this, &BibleWindow::onSearchTextChanged);



    loadChapter();
}

void BibleWindow::onChapterChanged(int value) {
    currentChapter = value+1;
    loadChapter();
}
void BibleWindow::onBookChanged(int index) {
    chapterSelector->clear();
    int numberOfChapters = chapters[index];
    for (int i = 1; i <= numberOfChapters; ++i) {
    chapterSelector->addItem(QString::number(i));
}

}


void BibleWindow::loadChapter() {
    verseList->clear();
    auto verses = db.getVerses(currentBook, currentChapter);
    for (const auto& verse : verses) {
        QString text = QString("%1. %2 %3")
            .arg(verse["verse"].toInt())
            .arg(verse["content"].toString())
            .arg(verse["favorite"].toInt() ? "★" : "");
        verseList->addItem(text);
    }
}
void BibleWindow::onSearchTextChanged() {
    QString keyword = searchEdit->text().trimmed();
    if (keyword.isEmpty()) {
        return;
    }
    verseList->clear();
    auto verses = db.searchVerses(keyword);
    for (const auto& verse : verses) {
        QString text = QString("%1 %2:%3 %4")
            .arg(bibleBooks[verse["book"].toInt()])
            .arg(verse["chapter"].toInt())
            .arg(verse["verse"].toInt())        
            .arg(verse["content"].toString());
        verseList->addItem(text);
    }
}
void BibleWindow::toggleTheme() {
    darkModeEnabled = !darkModeEnabled;

    auto app = qobject_cast<QApplication*>(qApp);  // ✅ cast from QCoreApplication*

    if (!app) {
        qDebug() << "Failed to cast qApp to QApplication";
        return;
    }

    if (darkModeEnabled) {
        app->setStyleSheet(R"(
            QWidget { background-color: #121212; color: #e0e0e0; }
            QPushButton { background-color: #1e1e1e; color: #ffffff; border: 1px solid #333; }
            QListWidget { background-color: #1e1e1e; color: #ffffff; }
            QLineEdit { background-color: #1e1e1e; color: #ffffff; }
            QComboBox { background-color: #1e1e1e; color: #ffffff; }
            QSpinBox { background-color: #1e1e1e; color: #ffffff; }
        )");
        themeToggleButton->setText("☀️ Light Mode");
    } else {
        app->setStyleSheet("");  // Reset to default
        themeToggleButton->setText("🌙 Dark Mode");
    }
}


void BibleWindow::toggleFavorite() {
    auto item = verseList->currentItem();
    if (!item) return;

    int verseNum = item->text().split(".")[0].toInt();
    bool isFav = item->text().contains("★");
    db.markFavorite(currentBook, currentChapter, verseNum, !isFav);
    loadChapter();
}
BibleWindow::~BibleWindow() = default; // Use default destructor
