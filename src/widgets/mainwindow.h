#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "QMainWindow"
#include "QWidget"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QFormLayout"
#include "QTableView"
#include "qtreeview.h"


#include "maindb.h"
#include "familymodel.h"
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
    FamilyModel *model;

    QTreeView *membersTable;
    QTableView *operationsTable;
private slots:
    void addMember();
    void addFamily();
    void reciveModel(AbstractTreeItem *);

    void onContextMenuFamilyControl(const QPoint &pos);
    void onContextMenuOperationsControl(const QPoint &pos);
signals:
    void sendMemberToDb(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    void sendFamilyToDb(QString name);

    void createConnection(quint16 idMember,quint16 idFamily);
    void breakConnection(quint16 idMember,quint16 idFamily);

    void updatebd();
};

#endif // MAINWINDOW_H
