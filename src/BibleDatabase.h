#pragma once
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QVariantMap>

class BibleDatabase {
public:
    BibleDatabase(const QString& dbPath);
    bool open();
    QVector<QVariantMap> getVerses(int book, int chapter);
    void markFavorite(int book, int chapter, int verse, bool favorite);
    QVector<QVariantMap> searchVerses(QString text);



private:
    QSqlDatabase db;
    QString path;
};
