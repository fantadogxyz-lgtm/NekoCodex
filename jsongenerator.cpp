#include "jsongenerator.h"
#include "ui_jsongenerator.h"

JsonGenerator::JsonGenerator(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::JsonGenerator())
{
    ui->setupUi(this);
    setupScrollLayout();

    connect(ui->createFieldBut, &QPushButton::clicked, this, &JsonGenerator::onCreateFieldClicked);
    connect(ui->pushButtonGenereteText, &QPushButton::clicked, this, &JsonGenerator::onGenerateTextClicked);
    connect(ui->pushButtonGenereteFile, &QPushButton::clicked, this, &JsonGenerator::onGenerateFileClicked);
}

JsonGenerator::~JsonGenerator()
{
    delete ui;
}

void JsonGenerator::setupScrollLayout()
{
    QWidget* container = ui->scrollArea->widget();

    if (container->layout()) {
        delete container->layout();
    }

    m_scrollLayout = new QVBoxLayout(container);

 
    m_scrollLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

   
    m_scrollLayout->setSpacing(10); 
    m_scrollLayout->setContentsMargins(5, 10, 5, 10);
}

void JsonGenerator::onCreateFieldClicked()
{
    FieldEditDialog* dialog = new FieldEditDialog(nullptr);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    connect(dialog, &FieldEditDialog::saveRequested, this, &JsonGenerator::onSaveFieldRequested);
    connect(dialog, &FieldEditDialog::saveRequested, dialog, &QWidget::close);
    connect(dialog, &FieldEditDialog::cancelRequested, dialog, &QWidget::close);

    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();
}

void JsonGenerator::onEditFieldRequested(const FieldConfig& config)
{
    FieldEditDialog* dialog = new FieldEditDialog(nullptr);
    dialog->setAttribute(Qt::WA_DeleteOnClose);

   
    dialog->setFieldConfig(config);

    connect(dialog, &FieldEditDialog::saveRequested, this, &JsonGenerator::onSaveFieldRequested);
    connect(dialog, &FieldEditDialog::saveRequested, dialog, &QWidget::close);
    connect(dialog, &FieldEditDialog::cancelRequested, dialog, &QWidget::close);

    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->show();
}

void JsonGenerator::onSaveFieldRequested(const FieldConfig& config)
{
   
    auto it = std::find_if(m_fields.begin(), m_fields.end(), [&config](const FieldConfig& f) {
        return f.uuid == config.uuid;
        });

    if (it != m_fields.end()) {
  
        *it = config;


        auto uiIt = std::find_if(m_uiRows.begin(), m_uiRows.end(), [&config](const FieldUiRow& r) {
            return r.uuid == config.uuid;
            });
        if (uiIt != m_uiRows.end()) {
            uiIt->rowWidget->setData(config);
        }
    }
    else {
      
        m_fields.append(config);
        addFieldToUiList(config);
    }
}

void JsonGenerator::addFieldToUiList(const FieldConfig& config)
{
    FieldRowWidget* rowWidget = new FieldRowWidget(config, this);
    rowWidget->setFixedSize(FIELD_WIDTH, FIELD_HEIGHT);

  
    connect(rowWidget, &FieldRowWidget::deleteRequested, this, &JsonGenerator::onDeleteFieldClicked);
    connect(rowWidget, &FieldRowWidget::editRequested, this, &JsonGenerator::onEditFieldRequested);

    m_scrollLayout->addWidget(rowWidget);

    FieldUiRow row;
    row.uuid = config.uuid;
    row.rowWidget = rowWidget;
    m_uiRows.append(row);
}

void JsonGenerator::onDeleteFieldClicked(const QString& uuid)
{

    m_fields.erase(std::remove_if(m_fields.begin(), m_fields.end(), [&uuid](const FieldConfig& f) {
        return f.uuid == uuid;
        }), m_fields.end());


    auto it = std::find_if(m_uiRows.begin(), m_uiRows.end(), [&uuid](const FieldUiRow& r) {
        return r.uuid == uuid;
        });

    if (it != m_uiRows.end()) {
        m_scrollLayout->removeWidget(it->rowWidget);
        it->rowWidget->deleteLater();
        m_uiRows.erase(it);
    }
}

void JsonGenerator::clearUiList()
{
    for (auto& row : m_uiRows) {
        row.rowWidget->deleteLater();
    }
    m_uiRows.clear();
    m_fields.clear();
}

QString JsonGenerator::getRandomWordFromFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "";

    QTextStream in(&file);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8);
#else
    in.setCodec("UTF-8");
#endif

    QStringList words;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) words.append(line);
    }
    file.close();

    if (words.isEmpty()) return "";
    return words[QRandomGenerator::global()->bounded(words.size())];
}

QString JsonGenerator::generateRandomValue(const FieldConfig& config) const
{
    if (config.isNullable && (QRandomGenerator::global()->bounded(100) < 10)) {
        return "null";
    }

    switch (config.type)
    {
    case JsonDataType::UUID:
        return QUuid::createUuid().toString(QUuid::WithoutBraces);

    case JsonDataType::ID:
        return "0";

    case JsonDataType::NUMBER: {
        NumberConfig numCfg = config.data.value<NumberConfig>();
        long long minVal = numCfg.min;
        long long maxVal = numCfg.max;
        if (minVal > maxVal) std::swap(minVal, maxVal);
        return QString::number(QRandomGenerator::global()->bounded(minVal, maxVal + 1));
    }
    case JsonDataType::STRING_LITERAL: {
        StringConfig strCfg = config.data.value<StringConfig>();
        bool isEng = strCfg.isEnglish;
        QString lang = isEng ? "en" : "ru";

        switch (strCfg.category)
        {
        case StringCategory::NAME: {
            int len = QRandomGenerator::global()->bounded(3, 9);
            return getRandomWordFromFile(QString(":/res/words/names/%1/%2.txt").arg(lang).arg(len));
        }
        case StringCategory::SURNAME: {
            int len = QRandomGenerator::global()->bounded(4, 10);
            return getRandomWordFromFile(QString(":/res/words/surnames/%1/%2.txt").arg(lang).arg(len));
        }
        case StringCategory::NICKNAME: {
           
            QString part = (QRandomGenerator::global()->bounded(4) != 0)
                ? getRandomWordFromFile(QString(":/res/words/randomwords/%1/5.txt").arg(lang))
                : "user";

            QString suffix = "";
            int numLen = QRandomGenerator::global()->bounded(1, 4); 
            for (int i = 0; i < numLen; ++i) suffix += QString::number(QRandomGenerator::global()->bounded(10));

            QString nick = part + suffix;
            if (strCfg.isUnique) nick += QString::number(QRandomGenerator::global()->bounded(10, 99));
            return nick.toLower();
        }
        case StringCategory::EMAIL: {
            QStringList nameParts;

            nameParts << getRandomWordFromFile(QString(":/res/words/names/%1/5.txt").arg(lang)).toLower();

            if (QRandomGenerator::global()->bounded(2) == 0) {
                nameParts << "_" << getRandomWordFromFile(QString(":/res/words/surnames/%1/6.txt").arg(lang)).toLower();
            }

            if (QRandomGenerator::global()->bounded(2) == 0) {
                nameParts << QString::number(QRandomGenerator::global()->bounded(10, 999));
            }
            else {
                QString alpha = "xyz";
                nameParts << alpha[QRandomGenerator::global()->bounded(alpha.length())];
            }

            QStringList domains = { "@gmail.com", "@yandex.ru", "@mail.ru", "@outlook.com" };
            return nameParts.join("") + domains[QRandomGenerator::global()->bounded(domains.size())];
        }
        case StringCategory::RANDOM_WORD: {
            int len = QRandomGenerator::global()->bounded(4, 9);
            return getRandomWordFromFile(QString(":/res/words/randomwords/%1/%2.txt").arg(lang).arg(len)).toLower();
        }
        case StringCategory::LOREM_TEXT: {
            int minLen = strCfg.textMinLen;
            int maxLen = strCfg.textMaxLen;
            if (minLen > maxLen) std::swap(minLen, maxLen);
            int wordsCount = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

            QStringList loremList;
            for (int i = 0; i < wordsCount; ++i) {
                int len = QRandomGenerator::global()->bounded(3, 8);
                QString w = getRandomWordFromFile(QString(":/res/words/randomwords/%1/%2.txt").arg(lang).arg(len));
                if (!w.isEmpty()) loremList.append(w.toLower());
            }
            return loremList.join(" ");
        }
        case StringCategory::PASSWORD: {
            int targetLen = QRandomGenerator::global()->bounded(8, 26);
            QString pool = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!#_-";
            QString password = "";
            for (int i = 0; i < targetLen; ++i) {
                password += pool[QRandomGenerator::global()->bounded(pool.length())];
            }
            return password;
        }
        }
        break;
    }
    case JsonDataType::BOOLEAN: {
        BoolConfig boolCfg = config.data.value<BoolConfig>();
        if (boolCfg.mode == BoolMode::ONLY_TRUE) return "true";
        if (boolCfg.mode == BoolMode::ONLY_FALSE) return "false";
        return (QRandomGenerator::global()->bounded(2) == 0) ? "true" : "false";
    }
    }
    return "";
}

QJsonDocument JsonGenerator::generateJson() const
{

    QSet<QString> fieldNames;
    for (const auto& field : m_fields) {
        if (fieldNames.contains(field.fieldName)) {
          
            QMessageBox msgBox;
            msgBox.setWindowTitle("\xD0\x9E\xD1\x88\xD0\xB8\xD0\xB1\xD0\xBA\xD0\xB0");
            msgBox.setText("\xD0\x9E\xD0\xB1\xD0\xBD\xD0\xB0\xD1\x80\xD1\x83\xD0\xB6\xD0\xB5\xD0\xBD\xD0\xBE\x20\xD0\xBF\xD0\xBE\xD0\xBB\xD0\xB5\x20\xD1\x81\x20\xD0\xBE\xD0\xB4\xD0\xB8\xD0\xBD\xD0\xB0\xD0\xBA\xD0\xBE\xD0\xB2\xD1\x8B\xD0\xBC\xD0\xB8\x20\xD0\xB8\xD0\xBC\xD0\xB5\xD0\xBD\xD0\xB0\xD0\xBC\xD0\xB8\x3A\x20 " + field.fieldName);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setStyleSheet("QMessageBox { background-color: white; }"
                "QMessageBox QLabel { color: rgb(85, 85, 127); }"
                "QPushButton { "
                "    background-color: rgb(85, 85, 127); "
                "    color: white; "
                "}");

            msgBox.exec();
            return QJsonDocument();


        }
        fieldNames.insert(field.fieldName);
    }

    QJsonArray rootArray;
    int recordsCount = ui->spinBoxNumberOfRecords->value();

    QMap<QString, int> idCounters;
    for (const auto& field : m_fields) {
        if (field.type == JsonDataType::ID) {
            IdConfig idCfg = field.data.value<IdConfig>();
            idCounters[field.uuid] = idCfg.startId;
        }
    }


    QMap<QString, QSet<QString>> usedValues;

    for (int i = 0; i < recordsCount; ++i) {
        QJsonObject recordObject;

        for (const auto& field : m_fields) {
           
            if (field.type == JsonDataType::ID) {
                int currentId = idCounters[field.uuid];
                recordObject.insert(field.fieldName, currentId);
                idCounters[field.uuid] = currentId + 1;
            }
            else {
                
                QString rawVal;
                bool isUnique = false;

               
                if (field.type == JsonDataType::NUMBER) {
                    isUnique = field.data.value<NumberConfig>().isUnique;
                }
                else if (field.type == JsonDataType::STRING_LITERAL) {
                    isUnique = field.data.value<StringConfig>().isUnique;
                }

               
                int attempts = 0;
                do {
                    rawVal = generateRandomValue(field);
                    attempts++;
                } while (isUnique && usedValues[field.uuid].contains(rawVal) && attempts < 100);

                if (isUnique) {
                    usedValues[field.uuid].insert(rawVal);
                }

               
                if (rawVal == "null") recordObject.insert(field.fieldName, QJsonValue::Null);
                else if (field.type == JsonDataType::NUMBER) recordObject.insert(field.fieldName, rawVal.toLongLong());
                else if (field.type == JsonDataType::BOOLEAN) recordObject.insert(field.fieldName, (rawVal == "true"));
                else recordObject.insert(field.fieldName, rawVal);
            }
        }
        rootArray.append(recordObject);
    }
    return QJsonDocument(rootArray);
}

void JsonGenerator::onGenerateTextClicked()
{
    if (m_fields.isEmpty()) {
        ui->plainTextEdit->setPlainText("[]");
        return;
    }
    QJsonDocument doc = generateJson();
    ui->plainTextEdit->setPlainText(doc.toJson(QJsonDocument::Indented));
}

void JsonGenerator::onGenerateFileClicked()
{
    if (m_fields.isEmpty()) return;

    QString title = QString::fromUtf8("\xD0\xA1\xD0\xBE\xD1\x85\xD1\x80\xD0\xB0\xD0\xBD\xD0\xB8\xD1\x82\xD1\x8C\x20\x4A\x53\x4F\x4E\x20\xD1\x84\xD0\xB0\xD0\xB9\xD0\xBB");
    QString fileName = QFileDialog::getSaveFileName(this, title, "", "JSON Files (*.json)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QString::fromUtf8("\xD0\x9E\xD1\x88\xD0\xB8\xD0\xB1\xD0\xBA\xD0\xB0"), QString::fromUtf8("\xD0\x9D\xD0\xB5\xD1\x81\xD0\xBE\xD0\xB7\xD0\xB4\xD0\xB0\xD1\x82\xD1\x8C\x20\xD1\x84\xD0\xB0\xD0\xB9\xD0\xBB"));
        return;
    }

    QJsonDocument doc = generateJson();
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}