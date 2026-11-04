#include "File_reader.h"

File_reader::File_reader() {}

QVector<QString> File_reader::readAllLines(const QString &fileName){
    QFile qfile(fileName);

    if(!qfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return QVector<QString> (0);
    }

    QVector<QString> qvector;
    QString line = qfile.readLine();
    while(!line.isNull()){
        qvector.append(line);
        line = qfile.readLine();
    }

    qfile.close();

    return qvector;
}


QVector<QPair<QString, QString>> File_reader::readAllArgumentsInfile(const QString &fileName){
    QVector<QString> vector_of_lines = readAllLines(fileName);

    //пустое - пустой
    if (vector_of_lines.size() == 0){
        return QVector<QPair<QString, QString>>();
    }

    QVector<QPair<QString, QString>> vector_of_arguments;

    //собираем аргументы в парочки
    for(const auto &line: std::as_const( vector_of_lines)){
        //const auto &line: std::as_const( vector_of_lines) делает константну ссылку
        //сслка, с помощбю которой ничего нельзя изменить ( т е ток "чистать") и которая не копирует элемент
        if(line.indexOf(':') != -1){
            QStringList argument = line.split(":");
            vector_of_arguments.append(std::make_pair(argument[0], argument[1]));
        }
//я пока хочу, чтобы добавляло ток те аргументы, у которых есть :, в файле кшкдвч будет проверяется, и если чего-то не хватает,
        // то настройки по умолчанию сделает и файл полностью перезапишет
    }
    return vector_of_arguments;
}


QString File_reader::readArgumentInFile(const QString &fileName, const QString &name_of_argument)
{
   QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }
        
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains(':')) {
            QStringList parts = line.split(':');
            if (parts.size() >= 2 && parts[0].trimmed() == name_of_argument) {
                return parts[1].trimmed();  
            }
        }
    }
    
    return QString();

}
