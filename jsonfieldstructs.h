#pragma once

#include <QString>
#include <QVariant>

enum class JsonDataType {
    UUID,
    ID,
    NUMBER,
    STRING_LITERAL,
    BOOLEAN
};

enum class StringCategory {
    NAME,         
    SURNAME,
    NICKNAME,     
    EMAIL,       
    RANDOM_WORD,  
    LOREM_TEXT,
    PASSWORD
};

enum class BoolMode {
    TRUE_FALSE, 
    ONLY_TRUE,     
    ONLY_FALSE     
};

//структуры для настроечек

struct UuidConfig {
}; 

struct IdConfig {
    int startId = 0; 
};

struct NumberConfig {
    qint64 min = 0;        
    qint64 max = 100;     
    bool isUnique = false; 
};

struct StringConfig {
    StringCategory category = StringCategory::NAME; 
    bool isEnglish = false;                         
    bool isUnique = false;  // для всех кроме текста

   //только для текста, в символах
    int textMinLen = 50;
    int textMaxLen = 300;
};

struct BoolConfig {
    BoolMode mode = BoolMode::TRUE_FALSE;
};

struct FieldConfig {
    QString uuid;
    QString fieldName;
    JsonDataType type;  
    bool isNullable = false;

    QVariant data;
};