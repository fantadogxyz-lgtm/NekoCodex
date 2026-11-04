#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QJsonDocument>
#include <QRandomGenerator>
#include <QUuid>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <algorithm>

#include "FieldEditDialog.h"
#include "fieldrowwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class JsonGenerator; }
QT_END_NAMESPACE


struct FieldUiRow {
    QString uuid;
    FieldRowWidget* rowWidget;
};

class JsonGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit JsonGenerator(QWidget* parent = nullptr);
    ~JsonGenerator();

private slots:
    void onCreateFieldClicked();                          
    void onEditFieldRequested(const FieldConfig& config); 
    void onSaveFieldRequested(const FieldConfig& config); 
    void onDeleteFieldClicked(const QString& uuid);      
    void onGenerateTextClicked();
    void onGenerateFileClicked();

private:
    void setupScrollLayout();
    void addFieldToUiList(const FieldConfig& config);
    void clearUiList();

    QJsonDocument generateJson() const;
    QString generateRandomValue(const FieldConfig& config) const;
    QString getRandomWordFromFile(const QString& filePath) const;

    Ui::JsonGenerator* ui;
    QVBoxLayout* m_scrollLayout;          
    QVector<FieldConfig> m_fields;
    QVector<FieldUiRow> m_uiRows;

   
    static constexpr int FIELD_WIDTH = 370;
    static constexpr int FIELD_HEIGHT = 110;
};