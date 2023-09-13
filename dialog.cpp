#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent, bool update, QString name, QString bio, QString date, bool status, QString file_name) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->TaskName->setText(name);
    ui->TaskBio->setPlainText(bio);

    if (update){
        update_d = update;
        ui->btnAccept->setText("Update Bio");

        QStringList datatime = date.split( " " );
        ui->TaskDate->setDate(QDate::fromString(datatime[0],"dd/MM/yyyy"));
        ui->TaskDate->setTime(QTime::fromString(datatime[1],"HH:mm"));

        ui->TaskName->setEnabled(false);
        ui->TaskDate->setEnabled(false);
        ui->TaskStatus->setEnabled(false);
    }else{
        setCurrentDate();
    }

    ui->TaskStatus->setChecked(status);
    file_dir = file_name;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_TaskStatus_stateChanged(int arg1)
{
    if (ui->TaskStatus->isChecked())
        ui->TaskStatus->setText("Done");
    else
        ui->TaskStatus->setText("Undone");
}


void Dialog::on_btnAccept_clicked()
{
    if (ui->TaskName->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Unselected"), tr("Please enter the name for new task"));
        return;
    }
    if (ui->TaskBio->toPlainText().isEmpty())
    {
        QMessageBox::warning(this,tr("Unselected"), tr("Please add bio for new task"));
        return;
    }

    QString new_file_name = "list_items/" + ui->TaskName->text() + ".txt";
    int file_name_count = 1;
    while(QFile::exists(new_file_name)){
        if (file_name_count == 1)
            new_file_name.remove(new_file_name.length() - 4,4);
        else
            new_file_name.remove(new_file_name.length() - 6,6);
        new_file_name = new_file_name + "_" + QString::number(file_name_count) + ".txt";
        file_name_count ++;
    }

    if (!update_d){
        QFile f(new_file_name);
        bool ok = f.open(QIODevice::WriteOnly);

        if(!ok) {
            qDebug() << "open file error!";
            return;
        }

        QTextStream stream(&f);
        stream << "name|" << ui->TaskName->text() << "\n";
        stream << "bio|" << ui->TaskBio->toPlainText() << "\n";
        stream << "date|" << ui->TaskDate->text() << "\n";
        stream << "status|" << ui->TaskStatus->text();
        f.close();
    }else{
        QFile file(file_dir);
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        QString s;
        QTextStream t(&file);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(!line.contains("bio|"))
                s.append(line + "\n");
            else{
                QString text = "bio|" + ui->TaskBio->toPlainText() + "\n";
                s.append(text);
            }
        }

        file.resize(0);
        t << s;
        file.close();
    }
    Dialog::accept();
}


void Dialog::on_btnCancel_clicked()
{
    Dialog::reject();
}

void Dialog::setCurrentDate()
{
    ui->TaskDate->setDate(QDateTime::currentDateTime().date());
    ui->TaskDate->setTime(QDateTime::currentDateTime().time());
}
