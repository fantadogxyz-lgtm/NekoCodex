#include "datagenerator.h"

DataGenerator::DataGenerator(QWidget *parent)
	: QWidget(parent), ui(new Ui::DataGenerator), dataGeneratorSettings(new DataGeneratorSettings(this))
{
	qDebug() << "starts DataGenerator";

	qDebug() << "2. before ui->setupUi";
    if (!ui) {
        qDebug() << "CRITICAL: ui is nullptr!";
        return;
    }

    ui->setupUi(this);
    qDebug() << "3. after ui->setupUi";
    if (!dataGeneratorSettings) {
        dataGeneratorSettings = new DataGeneratorSettings(this);
    }

    ui->scrollArea->setWidget(dataGeneratorSettings);
    ui->scrollArea->setWidgetResizable(true);
    
    dataGeneratorSettings->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dataGeneratorSettings->setMinimumHeight(1030);   




    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &DataGenerator::onDeleteClicked);
    connect(ui->pushButtonGenerateDate, &QPushButton::clicked, this, &DataGenerator::onGenerateDateClicked);
    connect(ui->pushButtonGenerateEmail, &QPushButton::clicked, this, &DataGenerator::onGenerateEmailClicked);
    connect(ui->pushButtonGenerateName, &QPushButton::clicked, this, &DataGenerator::onGenerateNameClicked);
    connect(ui->pushButtonGenerateRandomWord, &QPushButton::clicked, this, &DataGenerator::onGenerateRandomWordClicked);
    connect(ui->pushButtonGeneratePasswordNotEasy, &QPushButton::clicked, this, &DataGenerator::onGeneratePasswordNotEasyClicked);
    connect(ui->pushButtonGeneratePhone, &QPushButton::clicked, this, &DataGenerator::onGeneratePhoneClicked);
    connect(ui->pushButtonGenerateSurname, &QPushButton::clicked, this, &DataGenerator::onGenerateSurnameClicked);
    connect(ui->pushButtonGenerateUsername, &QPushButton::clicked, this, &DataGenerator::onGenerateUsernameClicked);
}

DataGenerator::~DataGenerator()
{
}

void DataGenerator::onDeleteClicked()
{
    ui->plainTextEditOutput->clear();
}
void DataGenerator::onGenerateDateClicked()
{
    QDate minDate = dataGeneratorSettings->getDateMin();
    QDate maxDate = dataGeneratorSettings->getDateMax();

    if (minDate > maxDate) {
        std::swap(minDate, maxDate);
    }

    int daysBetween = minDate.daysTo(maxDate);

    int randomDaysOffset = QRandomGenerator::global()->bounded(daysBetween + 1);

    QDate generatedDate = minDate.addDays(randomDaysOffset);

    QString formattedDate = "";

    if (dataGeneratorSettings->isDateFormat1()) {
        formattedDate = generatedDate.toString("yyyy-MM-dd");
    }
    else if (dataGeneratorSettings->isDateFormat2()) {
     
        formattedDate = generatedDate.toString("dd.MM.yyyy");
    }
    else if (dataGeneratorSettings->isDateFormat3()) {
       
        formattedDate = generatedDate.toString("dd/MM/yyyy");
    }
    else {
     
        formattedDate = generatedDate.toString("dd.MM.yyyy");
    }

  
    if (!formattedDate.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(formattedDate);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating date");
    }
}




void DataGenerator::onGenerateEmailClicked()
{

    int minLen = dataGeneratorSettings->getEmailMinLength();
    int maxLen = dataGeneratorSettings->getEmailMaxLength();

    if (minLen > maxLen) std::swap(minLen, maxLen);

    if (minLen < 5) minLen = 5;
    if (maxLen > 35) maxLen = 35;

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    QString domain = "@gmail.com"; 
    if (dataGeneratorSettings->isEmailYandex()) {
        domain = "@yandex.by";
    }
    else if (dataGeneratorSettings->isEmailMailru()) {
        domain = "@mail.ru";
    }

    QString localPart = "";
    int remainingLen = targetLen;

    QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    QString digits = "0123456789";

    while (remainingLen > 0) {
        int maxChunk = std::min(remainingLen, 11);
        int chunkLen = QRandomGenerator::global()->bounded(1, maxChunk + 1);

        enum ChunkType { NAME, SURNAME, RANDOM_LETTERS, NUMBERS, RANDOM_WORDS };
        QList<ChunkType> availableTypes;

        if (chunkLen >= 2) {
            availableTypes << NAME << SURNAME;
        }

        availableTypes << RANDOM_LETTERS;
        availableTypes << RANDOM_WORDS;

        if (chunkLen <= 3) {
            availableTypes << NUMBERS;
        }

        int typeIndex = QRandomGenerator::global()->bounded(availableTypes.size());
        ChunkType selectedType = availableTypes[typeIndex];

        QString chunkString = "";

        switch (selectedType) {
        case NAME: {
            QString path = QString(":/res/words/names/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case SURNAME: {
            QString path = QString(":/res/words/surnames/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_WORDS: {
            QString path = QString(":/res/words/randomwords/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_LETTERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(alphabet.length());
                chunkString += alphabet[idx];
            }
            break;
        }
        case NUMBERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(digits.length());
                chunkString += digits[idx];
            }
            break;
        }
        }

        localPart += chunkString;
        remainingLen -= chunkLen;
    }

    QString fullEmail = localPart + domain;

    if (!fullEmail.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(fullEmail);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating email");
    }
}
void DataGenerator::onGenerateNameClicked()
{
    bool isEng = dataGeneratorSettings->isNameEnglish();
    int minLen = dataGeneratorSettings->getNameMinLength();
    int maxLen = dataGeneratorSettings->getNameMaxLength();

    if (minLen > maxLen) std::swap(minLen, maxLen);

    if (minLen < 2) minLen = 2;
    if (maxLen > 11) maxLen = 11;

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    QString langFolder = isEng ? "en" : "ru";
    QString filePath = QString(":/res/words/names/%1/%2.txt").arg(langFolder).arg(targetLen);

    QString generatedName = getRandomWordFromFile(filePath);
    if (!generatedName.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(generatedName);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error");
    }
}


void DataGenerator::onGenerateRandomWordClicked()
{
    int minLen = dataGeneratorSettings->getRandomWordMinLength();
    int maxLen = dataGeneratorSettings->getRandomWordMaxLength();

    if (minLen > maxLen) std::swap(minLen, maxLen);

    if (minLen < 3) minLen = 3;
    if (maxLen > 12) maxLen = 12;

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    bool isEng = dataGeneratorSettings->isWordEnglish();
    QString langFolder = isEng ? "en" : "ru";
    QString filePath = QString(":/res/words/randomwords/%1/%2.txt").arg(langFolder).arg(targetLen);

    QString generatedWord = getRandomWordFromFile(filePath).toLower();

    if (!generatedWord.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(generatedWord);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating random word");
    }
}


void DataGenerator::onGeneratePasswordNotEasyClicked()
{
    int minLen = dataGeneratorSettings->getPasswordMinLength();
    int maxLen = dataGeneratorSettings->getPasswordMaxLength();

    if (minLen > maxLen) std::swap(minLen, maxLen);

    if (minLen < 4) minLen = 4;
    if (maxLen > 100) maxLen = 100;

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    QString password = "";
    int remainingLen = targetLen;

    QString alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString digits = "0123456789";
    QString specialSymbols = "?!#_-";

    bool onlyLetters = dataGeneratorSettings->isPasswordOnlyLetters();
    bool onlyNumbers = dataGeneratorSettings->isPasswordOnlyNumbers();
    bool useSymbols = dataGeneratorSettings->isPasswordSpecificSymbols();

    while (remainingLen > 0) {
        int maxChunk = std::min(remainingLen, 11);
        int chunkLen = QRandomGenerator::global()->bounded(1, maxChunk + 1);

        enum ChunkType { NAME, SURNAME, RANDOM_LETTERS, NUMBERS, SYMBOLS, RANDOM_WORDS };
        QList<ChunkType> availableTypes;

        if (onlyNumbers) {
            availableTypes << NUMBERS;
        }
        else if (onlyLetters) {
            availableTypes << RANDOM_LETTERS;

            if (chunkLen >= 3) {
                availableTypes << NAME << SURNAME << RANDOM_WORDS;
            }
        }
        else {
            availableTypes << RANDOM_LETTERS << NUMBERS;
            if (chunkLen >= 3) {
                availableTypes << NAME << SURNAME << RANDOM_WORDS;
            }
        }

        if (useSymbols) {
            availableTypes << SYMBOLS;
        }

        if (availableTypes.isEmpty()) {
            availableTypes << RANDOM_LETTERS;
        }

        int typeIndex = QRandomGenerator::global()->bounded(availableTypes.size());
        ChunkType selectedType = availableTypes[typeIndex];

        QString chunkString = "";

        switch (selectedType) {
        case NAME: {
            QString path = QString(":/res/words/names/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case SURNAME: {
            QString path = QString(":/res/words/surnames/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_WORDS: {
            QString path = QString(":/res/words/randomwords/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_LETTERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(alphabet.length());
                chunkString += alphabet[idx];
            }
            break;
        }
        case NUMBERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(digits.length());
                chunkString += digits[idx];
            }
            break;
        }
        case SYMBOLS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(specialSymbols.length());
                chunkString += specialSymbols[idx];
            }
            break;
        }
        }

        password += chunkString;
        remainingLen -= chunkLen;
    }


    if (!password.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(password);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating password");
    }
}
void DataGenerator::onGeneratePhoneClicked()
{
    QString randomDigits = "";
    for (int i = 0; i < 7; ++i) {
        int digit = QRandomGenerator::global()->bounded(10);
        randomDigits += QString::number(digit);
    }

    QString finalPhoneNumber = "";

    if (dataGeneratorSettings->isPhoneFormat1()) {
        finalPhoneNumber = "37529" + randomDigits;
    }
    else if (dataGeneratorSettings->isPhoneFormat2()) {
        finalPhoneNumber = "+375 (29) " + randomDigits;
    }
    else if (dataGeneratorSettings->isPhoneFormat3()) {
        finalPhoneNumber = "8029" + randomDigits;
    }
    else if (dataGeneratorSettings->isPhoneFormat4()) {
        finalPhoneNumber = "8 (029) " + randomDigits;
    }
    else {

        finalPhoneNumber = "+375 (29) " + randomDigits;
    }


    if (!finalPhoneNumber.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(finalPhoneNumber);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating phone number");
    }
}
void DataGenerator::onGenerateSurnameClicked()
{
    bool isEng = dataGeneratorSettings->isSurnameEnglish();
    int minLen = dataGeneratorSettings->getSurnameMinLength();
    int maxLen = dataGeneratorSettings->getSurnameMaxLength();

    if (minLen > maxLen) std::swap(minLen, maxLen);

    if (minLen < 2) minLen = 2;
    if (maxLen > 12) maxLen = 12; 

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    QString langFolder = isEng ? "en" : "ru";
    QString filePath = QString(":/res/words/surnames/%1/%2.txt").arg(langFolder).arg(targetLen);

    QString generatedSurname = getRandomWordFromFile(filePath);
    if (!generatedSurname.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(generatedSurname);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error");
    }
}
void DataGenerator::onGenerateUsernameClicked()
{
    int minLen = dataGeneratorSettings->getUsernameMinLength();
    int maxLen = dataGeneratorSettings->getUsesnameMaxLength();
    bool useNumbs = dataGeneratorSettings->isUsernameNumbers();
    bool useSymbols = dataGeneratorSettings->isUsernameSymbols();

    if (minLen > maxLen) std::swap(minLen, maxLen);
    if (minLen < 4) minLen = 4;
    if (maxLen > 60) maxLen = 60;

    int targetLen = QRandomGenerator::global()->bounded(minLen, maxLen + 1);

    QString username = "";
    int remainingLen = targetLen;
    int totalDigitsGenerated = 0;

    QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    QString specialSymbols = "?!#_-";
    QString digits = "0123456789";

    while (remainingLen > 0) {
    
        int maxChunk = std::min(remainingLen, 11);
        int chunkLen = QRandomGenerator::global()->bounded(1, maxChunk + 1);


        enum ChunkType { NAME, SURNAME, RANDOM_LETTERS, NUMBERS, SYMBOLS, RANDOM_WORDS };
        QList<ChunkType> availableTypes;

        if (chunkLen >= 2) {
            availableTypes << NAME << SURNAME;
        }

        availableTypes << RANDOM_LETTERS;
        availableTypes << RANDOM_WORDS;

        if (useNumbs && (totalDigitsGenerated + chunkLen <= 6) && chunkLen <= 4) {
            availableTypes << NUMBERS;
        }

        if (useSymbols && chunkLen <= 2) {
            availableTypes << SYMBOLS;
        }

        int typeIndex = QRandomGenerator::global()->bounded(availableTypes.size());
        ChunkType selectedType = availableTypes[typeIndex];

        QString chunkString = "";

        switch (selectedType) {
        case NAME: {
            QString path = QString(":/res/words/names/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case SURNAME: {
            QString path = QString(":/res/words/surnames/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_WORDS: {
            QString path = QString(":/res/words/randomwords/en/%1.txt").arg(chunkLen);
            chunkString = getRandomWordFromFile(path).toLower();
            break;
        }
        case RANDOM_LETTERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(alphabet.length());
                chunkString += alphabet[idx];
            }
            break;
        }
        case NUMBERS: {
            for (int i = 0; i < chunkLen; ++i) {
                int idx = QRandomGenerator::global()->bounded(digits.length());
                chunkString += digits[idx];
            }
            totalDigitsGenerated += chunkLen; 
            break;
        }
        case SYMBOLS: {
            for (int i = 0; i < chunkLen; ++i) {
                int absolutePos = username.length() + chunkString.length();

                QString allowedSymbols = specialSymbols;

                if (absolutePos == 0 || absolutePos == targetLen - 1) {
                    allowedSymbols = "?!"; 
                }

                int idx = QRandomGenerator::global()->bounded(allowedSymbols.length());
                chunkString += allowedSymbols[idx];
            }
            break;
        }
        }

        username += chunkString;
        remainingLen -= chunkLen;

        
    }

    if (!username.isEmpty()) {
        ui->plainTextEditOutput->appendPlainText(username);
    }
    else {
        ui->plainTextEditOutput->appendPlainText("Error generating username");
    }
}

QString DataGenerator::getRandomWordFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error with fle:" << filePath;
        return "";
    }

    QTextStream in(&file);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    in.setEncoding(QStringConverter::Utf8); // Äëÿ Qt6
#else
    in.setCodec("UTF-8");                   // Äëÿ Qt5
#endif

    QStringList words;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            words.append(line);
        }
    }
    file.close();

    if (words.isEmpty()) {
        qDebug() << "Error: emty file" << filePath;
        return "";
    }

    int randomIndex = QRandomGenerator::global()->bounded(words.size());
    return words[randomIndex];
}
