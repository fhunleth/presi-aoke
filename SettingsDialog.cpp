#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "Settings.h"
#include <QFileDialog>

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    settings_(settings)
{
    ui->setupUi(this);

    ui->slideImagePathLineEdit->setText(settings_->slidePath());
    ui->slidesPerTurnSpinBox->setValue(settings_->slidesPerTurn());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    settings_->setSlidePath(ui->slideImagePathLineEdit->text());
    settings_->setSlidesPerTurn(ui->slidesPerTurnSpinBox->value());

    QDialog::accept();
}

void SettingsDialog::on_slideImageBrowseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Slide Path"),
                                                    ui->slideImagePathLineEdit->text(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        ui->slideImagePathLineEdit->setText(dir);
}
