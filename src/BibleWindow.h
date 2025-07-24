#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>     // ✅ Required for QPushButton
#include <QComboBox>       // ✅ Required for QComboBox
#include <QSpinBox>        // ✅ Required for QSpinBox
#include <QLineEdit>      // ✅ Required for QLineEdit
#include "BibleDatabase.h"

class BibleWindow : public QWidget {
    Q_OBJECT

public:
    BibleWindow(QWidget* parent = nullptr);
    ~BibleWindow();

private slots:
    void loadChapter();
    void toggleFavorite();
    void onBookChanged(int index);
    void onChapterChanged(int value);
    void onSearchTextChanged();
    void toggleTheme();

private:
    BibleDatabase db;
    QStringList bibleBooks;
    QList<int> chapters;
    QListWidget* verseList;
    QPushButton* favoriteButton;
    QComboBox* bookSelector;
    QComboBox* chapterSelector;
    QLineEdit* searchEdit; // ✅ Required for search functionality
    QPushButton* searchButton; // ✅ Required for search button
    QPushButton* themeToggleButton;
    bool darkModeEnabled = false;



    int currentBook = 0;
    int currentChapter = 1;
};
