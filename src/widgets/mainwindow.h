#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QWidget"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QFormLayout"
#include "QTableView"

#include "maindb.h"
#include "familyview.h"
#include "dialogs.h"
#include "plotcontroller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();
private:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    Maindb *maindb;
private slots:
    void addMember();
    void addFamily();
    void readDB(QList<Family>);
signals:
    void sendMemberToDb(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    void sendFamilyToDb(QString name);
};

#endif // MAINWINDOW_H
