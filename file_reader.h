#ifndef FILE_READER_H
#define FILE_READER_H
#include <QString>
#include <QVector>
#include <QFile>
#include <QPair>
#include <QStringList>
#include <QTextStream>

class File_reader
{
public:
    File_reader();

   QVector<QString> readAllLines(const QString &fileName);

   QVector<QPair<QString, QString>> readAllArgumentsInfile(const QString &fileName);

   QString readArgumentInFile(const QString &fileName, const QString &argument);

};

#endif // FILE_READER_H
