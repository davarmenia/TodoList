#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->createUI(QStringList() << "Name" << "Bio" << "Date" << "State" << "FileName");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Метод для настройки интерфейса,
 * в данном методе будет выполняться заполнение QTableWidget
 * записями из таблицы
 * */
void MainWindow::createUI(const QStringList &headers)
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    // hide file path
    ui->tableWidget->hideColumn(4);
    ui->tableWidget->setColumnWidth(0,this->width()/8);
    ui->tableWidget->setColumnWidth(1,this->width()/3);
    ui->tableWidget->setColumnWidth(2,this->width()/8);
    ui->tableWidget->setColumnWidth(3,this->width()/8);

    fill_list();

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(10);

    add = new QPushButton("Add item", this);
    mark = new QPushButton("Mark as Done", this);
    update = new QPushButton("Update bio", this);
    remove = new QPushButton("Delete item", this);

    vbox->setSpacing(3);
    vbox->addStretch(1);
    vbox->addWidget(add);
    vbox->addWidget(mark);
    vbox->addWidget(update);
    vbox->addWidget(remove);
    vbox->addStretch(1);


    connect(add, &QPushButton::clicked, this, &MainWindow::AddItem);
    connect(mark, &QPushButton::clicked, this, &MainWindow::MardAsDone);
    connect(update, &QPushButton::clicked, this, &MainWindow::UpdateBio);
    connect(remove, &QPushButton::clicked, this, &MainWindow::RemoveItem);

    ui->frame_2->setLayout(vbox);



    QVBoxLayout *vboxf = new QVBoxLayout();
    le1 = new QLineEdit(this);
    le1->setMaximumWidth(150);
    QLabel *label_hint = new QLabel(this);
    label_hint->setText("Filter text:");
    vboxf->setSpacing(10);
    filter_state = new QPushButton("Filter State", this);
    filter_date = new QPushButton("Filter By Date", this);
    filter_name = new QPushButton("Filter By Name", this);
    filter_bio = new QPushButton("Filter By Bio", this);

    vboxf->setSpacing(3);
    vboxf->addStretch(1);
    vboxf->addWidget(label_hint);
    vboxf->addWidget(le1);
    vboxf->addStretch(1);
    vboxf->addWidget(filter_state);
    vboxf->addWidget(filter_date);
    vboxf->addWidget(filter_name);
    vboxf->addWidget(filter_bio);
    vboxf->addStretch(1);

    connect(filter_state, &QPushButton::clicked, this, &MainWindow::FilterByState);
    connect(filter_date, &QPushButton::clicked, this, &MainWindow::FilterByDate);
    connect(filter_name, &QPushButton::clicked, this, &MainWindow::FilterByName);
    connect(filter_bio, &QPushButton::clicked, this, &MainWindow::FilterByBio);

    ui->frame->setLayout(vboxf);
}

void MainWindow::AddItem()
{
    qInfo() << "additem";
    //Dialog new_item;
    Dialog *dialog = new Dialog(this);
    //new_item.setModal(true);
    //new_item.exec();
    dialog->exec();
    ui->tableWidget->setRowCount(0);
    fill_list();
}

void MainWindow::MardAsDone()
{
    //qInfo() << "markasdone";
    //if(ui->tableWidget->currentRow() == -1)
    //{
    //    QMessageBox::warning(this,tr("Unselected"), tr("Please select one of the items to mark it as completed"));
    //    return;
    //}
    //QString value = ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text();
    //QByteArray fileData;
    //QFile file(value);
    //file.open(QIODevice::ReadWrite); // open for read and write
    //fileData = file.readAll(); // read all the data into the byte array
    //QString text(fileData); // add to text string for easy string replace

    //text.replace(QString("Undone"), QString("Done  ")); // replace text in string

    //file.seek(0); // go to the beginning of the file
    //file.write(text.toUtf8()); // write the new text back to the file

    //file.close(); // close the file handle.

    ////ui->tableWidget->clear();
    //ui->tableWidget->setRowCount(0);
    //fill_list();


    if(ui->tableWidget->currentRow() == -1)
    {
        QMessageBox::warning(this,tr("Unselected"), tr("Please select one of the items to mark it as completed"));
        return;
    }
    QString value = ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text();
    QFile file(value);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString s;
    QTextStream t(&file);
    while(!t.atEnd())
    {
        QString line = t.readLine();
            if(!line.contains("status|"))
                s.append(line + "\n");
            else{
                QString text = "status|Done";
                s.append(text);
        }
    }

    file.resize(0);
    t << s;
    file.close();

    ui->tableWidget->setRowCount(0);
    fill_list();
}

void MainWindow::UpdateBio()
{
    qInfo() << "updatebio";
    if(ui->tableWidget->currentRow() == -1)
    {
        QMessageBox::warning(this,tr("Unselected"), tr("Please select one of the items to menage the bio"));
        return;
    }
    bool status = false;
    if (ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text() == "Done")
    {
        status = true;
    }
    Dialog *dialog = new Dialog(this, true, ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(), ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(), ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(), status, ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text());
    //new_item.setModal(true);
    //new_item.exec();
    dialog->exec();

    //ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    fill_list();
}

void MainWindow::RemoveItem()
{
    qInfo() << "removeitem";
    qInfo() << ui->tableWidget->currentRow();
    if(ui->tableWidget->currentRow() < 0)
    {
        QMessageBox::warning(this,tr("Unselected"), tr("You need to select one of the elements to delete"));
        return;
    }
    QString value = ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text();
    if(QFileInfo::exists(value))
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Deleting item", "Are you sure you want to delete the current task?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No)
            return;
        QFile file (value);
        file.remove();
        ui->tableWidget->setRowCount(0);
        fill_list();
    }else{
        QMessageBox::critical(this,tr("Error removing item"), tr("File doesn't exsist"));
    }
}

void MainWindow::FilterByState()
{
    filtering(3);
}

void MainWindow::FilterByDate()
{
    filtering(2);
}

void MainWindow::FilterByName()
{
    filtering(0);
}

void MainWindow::FilterByBio()
{
    filtering(1);
}

void MainWindow::filtering(int colNumber)
{
    QString filter = le1->text();
    for( int i = 0; i < ui->tableWidget->rowCount(); ++i )
    {
        bool match = false;
        QTableWidgetItem *item = ui->tableWidget->item( i, colNumber );
        qInfo() << item->text();
        if( item->text().contains(filter) )
        {
            match = true;
        }
        ui->tableWidget->setRowHidden( i, !match );
    }
}

void MainWindow::fill_list(){

    QString dir_path = QDir::currentPath() + "/list_items/";
    QDir dir(dir_path);
    int i = 0;
    for(const QFileInfo &file : dir.entryInfoList(QDir::Files))
    {
        QFile fl(file.filePath());

        if(!fl.open(QIODevice::ReadOnly)) {
            QMessageBox::information(0, "error", fl.errorString());
        }

        QTextStream in(&fl);

        ui->tableWidget->insertRow(i);
        QTableWidgetItem *item = new QTableWidgetItem();

        while(!in.atEnd()) {
            QString line = in.readLine();
            //QStringList fields = line.split(",");
            //model->appendRow(fields);
            qInfo() << line;

            // Создаём элемент, который будет выполнять роль чекбокса

            QStringList pieces = line.split( "|" );
            QString neededWord = pieces.value( pieces.length() - 1 );

            if (line.startsWith("name")){
                ui->tableWidget->setItem(i,0, new QTableWidgetItem(neededWord));
            }else if (line.startsWith("bio")){
                ui->tableWidget->setItem(i,1, new QTableWidgetItem(neededWord));
            }else if (line.startsWith("date")){
                ui->tableWidget->setItem(i,2, new QTableWidgetItem(neededWord));
            }else if (line.startsWith("status")){
                ui->tableWidget->setItem(i,3, new QTableWidgetItem(neededWord));
            }
        }
        ui->tableWidget->setItem(i,4, new QTableWidgetItem(file.filePath()));
        i++;
        fl.close();
    }
}
