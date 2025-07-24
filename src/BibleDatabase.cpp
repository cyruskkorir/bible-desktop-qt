#include "BibleDatabase.h"
#include <QDebug>
#include <QSqlError>

BibleDatabase::BibleDatabase(const QString& dbPath) : path(dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
}


bool BibleDatabase::open() {
    if (!db.open()) {
        qDebug() << "DB Error:" << db.lastError().text();
        return false;
    }
    return true;
}


QVector<QVariantMap> BibleDatabase::getVerses(int book, int chapter) {
    QVector<QVariantMap> verses;
    QSqlQuery query;
    query.prepare("SELECT verse, content, favorite FROM bible WHERE book = ? AND chapter = ?");
    query.addBindValue(book);
    query.addBindValue(chapter);
    query.exec();

    while (query.next()) {
        QVariantMap verse;
        verse["verse"] = query.value(0);
        verse["content"] = query.value(1);
        verse["favorite"] = query.value(2);
        verses.append(verse);
    }
    return verses;
}
//implement search verses
QVector<QVariantMap> BibleDatabase::searchVerses(QString text){
    QVector<QVariantMap> verses;
    QSqlQuery query;
    query.prepare("SELECT book, chapter, verse, content, favorite FROM bible WHERE content LIKE ?");
    query.addBindValue("%"+text+"%");
    query.exec();

    while (query.next()) {
        QVariantMap verse;
        verse["book"] = query.value(0);
        verse["chapter"] = query.value(1);
        verse["verse"] = query.value(2);
        verse["content"] = query.value(3);
        verse["favorite"] = query.value(4);
        verses.append(verse);
    }
    return verses;
}

void BibleDatabase::markFavorite(int book, int chapter, int verse, bool favorite) {
    QSqlQuery query;
    query.prepare("UPDATE bible SET favorite = ? WHERE book = ? AND chapter = ? AND verse = ?");
    query.addBindValue(favorite ? 1 : 0);
    query.addBindValue(book);
    query.addBindValue(chapter);
    query.addBindValue(verse);
    query.exec();
}
