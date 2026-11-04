#include "FieldRowWidget.h"
#include "ui_FieldRowWidget.h"

FieldRowWidget::FieldRowWidget(const FieldConfig& config, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FieldRowWidget())
{
    ui->setupUi(this);

 
    setData(config);

    connect(ui->pushButtonEdit, &QPushButton::clicked, this, &FieldRowWidget::onEditClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &FieldRowWidget::onDeleteClicked);
}

FieldRowWidget::~FieldRowWidget()
{
	delete ui;
}

void FieldRowWidget::setData(const FieldConfig& config)
{
    m_config = config;

    ui->lineEditFieldName->setText(m_config.fieldName);

    QString typeStr = "UNKNOWN";
    if (m_config.type == JsonDataType::UUID) typeStr = "UUID";
    else if (m_config.type == JsonDataType::ID) typeStr = "ID";
    else if (m_config.type == JsonDataType::NUMBER) typeStr = "Number";
    else if (m_config.type == JsonDataType::STRING_LITERAL) typeStr = "String";
    else if (m_config.type == JsonDataType::BOOLEAN) typeStr = "Boolean";

    ui->lineEditFieldType->setText(QString("[%1]").arg(typeStr));
}

QString FieldRowWidget::getUuid() const
{
    return m_config.uuid;
}

void FieldRowWidget::onEditClicked()
{
    emit editRequested(m_config);
}

void FieldRowWidget::onDeleteClicked()
{
    emit deleteRequested(m_config.uuid);
}