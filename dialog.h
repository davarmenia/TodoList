#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr, bool update = false, QString name = "", QString bio = "", QString date = "", bool status = false, QString file_name = "");
    ~Dialog();

private slots:
    void on_TaskStatus_stateChanged(int arg1);

    void on_btnAccept_clicked();

    void on_btnCancel_clicked();
    void setCurrentDate();

private:
    Ui::Dialog *ui;
    bool update_d = false;
    QString file_dir;
};

#endif // DIALOG_H
