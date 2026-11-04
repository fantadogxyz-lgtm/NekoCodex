#pragma once

#include <QWidget>
#include "FieldEditDialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class FieldRowWidget; };
QT_END_NAMESPACE

class FieldRowWidget : public QWidget
{
	Q_OBJECT
public:
    explicit FieldRowWidget(const FieldConfig& config, QWidget* parent = nullptr);
    ~FieldRowWidget();

    void setData(const FieldConfig& config);

    QString getUuid() const;

signals:
    void deleteRequested(const QString& uuid);
    void editRequested(const FieldConfig& config);

private slots:
    void onEditClicked();
    void onDeleteClicked();

private:
    Ui::FieldRowWidget* ui;
    FieldConfig m_config; 
};
