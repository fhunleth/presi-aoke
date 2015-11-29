#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class Settings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings *settings, QWidget *parent = 0);
    ~SettingsDialog();

public slots:
    void accept();

private slots:
    void on_slideImageBrowseButton_clicked();

private:
    Ui::SettingsDialog *ui;
    Settings *settings_;
};

#endif // SETTINGSDIALOG_H
