#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFrame>
#include <QtWidgets>
#include <qtablewidget.h>
#include <QMessageBox>

/* My includes */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void AddItem();
    void MardAsDone();
    void UpdateBio();
    void RemoveItem();

    void FilterByState();
    void FilterByDate();
    void FilterByName();
    void FilterByBio();
private:
    Ui::MainWindow  *ui;
    QMenu *viewMenu;
    QLineEdit *le1;

    QPushButton *add;
    QPushButton *mark;
    QPushButton *update;
    QPushButton *remove;

    QPushButton *filter_state;
    QPushButton *filter_date;
    QPushButton *filter_name;
    QPushButton *filter_bio;

private:
    /* Метод для настройки интерфейса,
     * в данном методе будет выполняться заполнение QTableWidget
     * записями из таблицы
     * */
    void createUI(const QStringList &headers);

    void fill_list();
    void filtering(int colNumber);
    void MakeChangeInFile(int caller);
};

#endif // MAINWINDOW_H
