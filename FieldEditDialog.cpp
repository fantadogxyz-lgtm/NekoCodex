#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif


#include "FieldEditDialog.h"
#include "ui_FieldEditDialog.h"

FieldEditDialog::FieldEditDialog(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FieldEditDialog())
{
    ui->setupUi(this);

    initComboboxes();

    setStyleSheet(R"(
        QCheckBox {
            border: none;
            background-color: transparent;
            
        }
        QCheckBox::indicator {
            width: 10px;
            height: 10px;
            border: 2px solid #d0b0eb;
            border-radius: 2px;
            background-color: #f1e0ff;
        }
        QCheckBox::indicator:checked {
            border-color: #647d6f;
            background-color: #ceeddc;
        }

        QRadioButton {
            border: none;
            background-color: transparent;
            spacing: 8px;
        }

        QRadioButton::indicator {
            width: 14px;  /* чуть больше для радио, т.к. круглому нужно больше места */
            height: 14px;
    
            /* Радио - это КРУГ, а не квадрат */
            border: 2px solid #d0b0eb;
            border-radius: 9px;  /* половина от (width + border) */
            background-color: #f1e0ff;
        }

        QRadioButton::indicator:checked {
            border-color: #647d6f;
            background-color: #ceeddc;
        }
    )");

    connect(ui->comboBoxType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FieldEditDialog::onTypeChanged);
    connect(ui->comboBoxStringCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FieldEditDialog::onStringCategoryChanged);
    connect(ui->buttonSave, &QPushButton::clicked, this, &FieldEditDialog::onSaveClicked);
    connect(ui->buttonCancel, &QPushButton::clicked, this, &FieldEditDialog::onCancelClicked);

    m_currentUuid = QUuid::createUuid().toString();

    onTypeChanged(0);
}

FieldEditDialog::~FieldEditDialog()
{
	delete ui;
}

void FieldEditDialog::initComboboxes()
{
    ui->comboBoxType->addItem("UUID", static_cast<int>(JsonDataType::UUID));
    ui->comboBoxType->addItem("ID", static_cast<int>(JsonDataType::ID));
    //число 
    ui->comboBoxType->addItem(QString::fromUtf8("\xD0\xA7\xD0\xB8\xD1\x81\xD0\xBB\xD0\xBE\x20\x28\x4C\x6F\x6E\x67\x29"), static_cast<int>(JsonDataType::NUMBER));
    //строк лит
    ui->comboBoxType->addItem(QString::fromUtf8("\xD0\xA1\xD1\x82\xD1\x80\xD0\xBE\xD0\xBA\xD0\xBE\xD0\xB2\xD1\x8B\xD0\xB9\x20\xD0\xBB\xD0\xB8\xD1\x82\xD0\xB5\xD1\x80\xD0\xB0\xD0\xBB"), static_cast<int>(JsonDataType::STRING_LITERAL));
    //булево
    ui->comboBoxType->addItem(QString::fromUtf8("\xD0\x91\xD1\x83\xD0\xBB\xD0\xB5\xD0\xB2\xD0\xBE\x20\x28\x54\x72\x75\x65\x2F\x46\x61\x6C\x73\x65\x29"), static_cast<int>(JsonDataType::BOOLEAN));
    // Имя
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\x98\xD0\xBC\xD1\x8F"), static_cast<int>(StringCategory::NAME)); 
    // Фамилия
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\xA4\xD0\xB0\xD0\xBC\xD0\xB8\xD0\xBB\xD0\xB8\xD1\x8F"), static_cast<int>(StringCategory::SURNAME));
    // Никнейм
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\x9D\xD0\xB8\xD0\xBA\xD0\xBD\xD0\xB5\xD0\xB9\xD0\xBC"), static_cast<int>(StringCategory::NICKNAME));
    // Почта
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\x9F\xD0\xBE\xD1\x87\xD1\x82\xD0\xB0"), static_cast<int>(StringCategory::EMAIL)); 
    // Случайное слово
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\xA1\xD0\xBB\xD1\x83\xD1\x87\xD0\xB0\xD0\xB9\xD0\xBD\xD0\xBE\xD0\xB5\x20\xD1\x81\xD0\xBB\xD0\xBE\xD0\xB2\xD0\xBE"), static_cast<int>(StringCategory::RANDOM_WORD)); 
    // Текст (Lorem Ipsum)
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\xA2\xD0\xB5\xD0\xBA\xD1\x81\xD1\x82\x20\x28\x4C\x6F\x72\x65\x6D\x20\x49\x70\x73\x75\x6D\x29"), static_cast<int>(StringCategory::LOREM_TEXT)); 
    //пароль
    ui->comboBoxStringCategory->addItem(QString::fromUtf8("\xD0\x9F\xD0\xB0\xD1\x80\xD0\xBE\xD0\xBB\xD1\x8C"), static_cast<int>(StringCategory::PASSWORD)); 
    
    // Русский
    ui->comboBoxStringLanguage->addItem(QString::fromUtf8("\xD0\xA0\xD1\x83\xD1\x81\xD1\x81\xD0\xBA\xD0\xB8\xD0\xB9"), false);
    ui->comboBoxStringLanguage->addItem("English", true);
}

void FieldEditDialog::onTypeChanged(int index)
{
    ui->stackedWidget->setCurrentIndex(index);

    JsonDataType type = static_cast<JsonDataType>(ui->comboBoxType->currentData().toInt());
    if (type == JsonDataType::STRING_LITERAL) {
        onStringCategoryChanged(ui->comboBoxStringCategory->currentIndex());
    }
}

void FieldEditDialog::onStringCategoryChanged(int index)
{
    StringCategory category = static_cast<StringCategory>(ui->comboBoxStringCategory->currentData().toInt());
    updateUiForStringCategory(category);
}

void FieldEditDialog::updateUiForStringCategory(StringCategory category)
{
   
    bool isLorem = (category == StringCategory::LOREM_TEXT);
    bool isPassword = (category == StringCategory::PASSWORD);
    bool isNickname = (category == StringCategory::NICKNAME);
    bool isEmail = (category == StringCategory::EMAIL);

    ui->frame_2->setVisible(isLorem);

    bool langVisible = !(isPassword || isNickname);
    ui->comboBoxStringLanguage->setVisible(langVisible);

    bool uniqueVisible = !(isLorem || isPassword);
    ui->checkBoxStringUnique->setVisible(uniqueVisible);

    if (isLorem) {
        ui->comboBoxStringLanguage->setEnabled(true);
    }
    else {
       
        if (isNickname || isEmail || isPassword) {
            ui->comboBoxStringLanguage->setCurrentIndex(1);
            ui->comboBoxStringLanguage->setEnabled(false);
            ui->comboBoxStringLanguage->setVisible(false);
        }
        else {
            ui->comboBoxStringLanguage->setEnabled(true);
        }
    }
}

FieldConfig FieldEditDialog::getFieldConfig() const
{
    FieldConfig config;
    config.uuid = m_currentUuid;
    config.fieldName = ui->lineEditFieldName->text().trimmed();
    config.isNullable = ui->checkBoxNullable->isChecked();
    config.type = static_cast<JsonDataType>(ui->comboBoxType->currentData().toInt());

    switch (config.type)
    {
    case JsonDataType::UUID:
        config.data = QVariant::fromValue(UuidConfig());
        break;
    case JsonDataType::ID: {
        IdConfig idCfg;
        idCfg.startId = ui->spinBoxStartId->value();
        config.data = QVariant::fromValue(idCfg);
        break;
    }
    case JsonDataType::NUMBER: {
        NumberConfig numCfg;
        numCfg.min = ui->spinBoxNumMin->value();
        numCfg.max = ui->spinBoxNumMax->value();
        numCfg.isUnique = ui->checkBoxNumUnique->isChecked();
        config.data = QVariant::fromValue(numCfg);
        break;
    }
    case JsonDataType::STRING_LITERAL: {
        StringConfig strCfg;
        strCfg.category = static_cast<StringCategory>(ui->comboBoxStringCategory->currentData().toInt());
        strCfg.isEnglish = ui->comboBoxStringLanguage->currentData().toBool();
        strCfg.isUnique = ui->checkBoxStringUnique->isChecked();
        strCfg.textMinLen = ui->spinBoxLoremMin->value();
        strCfg.textMaxLen = ui->spinBoxLoremMax->value();
        config.data = QVariant::fromValue(strCfg);
        break;
    }
    case JsonDataType::BOOLEAN: {
        BoolConfig boolCfg;
        if (ui->radioBoolOnlyTrue->isChecked()) boolCfg.mode = BoolMode::ONLY_TRUE;
        else if (ui->radioBoolOnlyFalse->isChecked()) boolCfg.mode = BoolMode::ONLY_FALSE;
        else boolCfg.mode = BoolMode::TRUE_FALSE;
        config.data = QVariant::fromValue(boolCfg);
        break;
    }
    }
    return config;
}

void FieldEditDialog::setFieldConfig(const FieldConfig& config)
{
    m_currentUuid = config.uuid;
    ui->lineEditFieldName->setText(config.fieldName);
    ui->checkBoxNullable->setChecked(config.isNullable);

    for (int i = 0; i < ui->comboBoxType->count(); ++i) {
        if (ui->comboBoxType->itemData(i).toInt() == static_cast<int>(config.type)) {
            ui->comboBoxType->setCurrentIndex(i);
            break;
        }
    }

    switch (config.type)
    {
    case JsonDataType::UUID:
        break;
    case JsonDataType::ID: {
        IdConfig idCfg = config.data.value<IdConfig>();
        ui->spinBoxStartId->setValue(idCfg.startId);
        break;
    }
    case JsonDataType::NUMBER: {
        NumberConfig numCfg = config.data.value<NumberConfig>();
        ui->spinBoxNumMin->setValue(numCfg.min);
        ui->spinBoxNumMax->setValue(numCfg.max);
        ui->checkBoxNumUnique->setChecked(numCfg.isUnique);
        break;
    }
    case JsonDataType::STRING_LITERAL: {
        StringConfig strCfg = config.data.value<StringConfig>();
        for (int i = 0; i < ui->comboBoxStringCategory->count(); ++i) {
            if (ui->comboBoxStringCategory->itemData(i).toInt() == static_cast<int>(strCfg.category)) {
                ui->comboBoxStringCategory->setCurrentIndex(i);
                break;
            }
        }
        int langIdx = strCfg.isEnglish ? 1 : 0;
        ui->comboBoxStringLanguage->setCurrentIndex(langIdx);
        ui->checkBoxStringUnique->setChecked(strCfg.isUnique);
        ui->spinBoxLoremMin->setValue(strCfg.textMinLen);
        ui->spinBoxLoremMax->setValue(strCfg.textMaxLen);
        break;
    }
    case JsonDataType::BOOLEAN: {
        BoolConfig boolCfg = config.data.value<BoolConfig>();
        if (boolCfg.mode == BoolMode::ONLY_TRUE) ui->radioBoolOnlyTrue->setChecked(true);
        else if (boolCfg.mode == BoolMode::ONLY_FALSE) ui->radioBoolOnlyFalse->setChecked(true);
        else ui->radioBoolBoth->setChecked(true);
        break;
    }
    }
}

void FieldEditDialog::onSaveClicked()
{
    if (ui->lineEditFieldName->text().trimmed().isEmpty()) {

        QMessageBox msgBox;
        msgBox.setWindowTitle("\xD0\x9E\xD1\x88\xD0\xB8\xD0\xB1\xD0\xBA\xD0\xB0");
        msgBox.setText("\xD0\x9F\xD0\xBE\xD0\xB6\xD0\xB0\xD0\xBB\xD1\x83\xD0\xB9\xD1\x81\xD1\x82\xD0\xB0\x2C\x20\xD0\xB2\xD0\xB2\xD0\xB5\xD0\xB4\xD0\xB8\xD1\x82\xD0\xB5\x20\xD0\xB8\xD0\xBC\xD1\x8F\x20\xD0\xBF\xD0\xBE\xD0\xBB\xD1\x8F\x20\xD0\xB2\x20\x4A\x53\x4F\x4E\x21");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("QMessageBox { background-color: white; }"
            "QMessageBox QLabel { color: rgb(85, 85, 127); }"
            "QPushButton { "
            "    background-color: rgb(85, 85, 127); "
            "    color: white; "
            "}");

        msgBox.exec();
        return;

    }

    emit saveRequested(getFieldConfig());
}

void FieldEditDialog::onCancelClicked()
{
    emit cancelRequested();
}