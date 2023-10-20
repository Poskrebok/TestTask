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
    QTableView *incomeOperationsTable;
    QTableView *outcomeOperationsTable;

private slots:
    void addMember();
    void addFamily();
    void reciveModel(AbstractTreeItem *);

    void onContextMenuFamilyControl(const QPoint &pos);
    void onContextMenuIncomeOperationsControl(const QPoint &pos);
    void onContextMenuOutcomeOperationsControl(const QPoint &pos);

    void onEntityClicked(const QModelIndex &index);
    void incomeOperationsModelHandler(QSqlQueryModel* model);
    void outcomeOperationsModelHandler(QSqlQueryModel* model);
signals:
    void sendMemberToDb(QString fname, QString mname, QString lname, QDateTime bdate, QString gender);
    void sendFamilyToDb(QString name);

    void createConnection(quint16 idMember,quint16 idFamily);
    void breakConnection(quint16 idMember,quint16 idFamily);

    void createOperationIncomeRecord(quint16 idMember, QDateTime dt, int income, QString comment, quint16 idSource);
    void changeOperationIncomeRecord(quint16 id, QDateTime dt, int income, QString comment, quint16 idSource);
    void createOperationOutcomeRecord(quint16 idMember,QDateTime dt, int spendings, QString comment,quint16 idGoods);;
    void changeOperationOutcomeRecord(quint16 id, QDateTime dt, int spendings, QString comment, quint16 idGoods);

    void createGoodsCategory(QString name);
    void removeGoodsCategory(quint16 id);
    void createProduct(QString name, quint16 id);
    void removeProduct(quint16 id);

    void toReciveIncomeOperationsByMember(quint16 id);
    void toReciveIncomeOperationsByFamily(quint16 id);
    void toReciveOutcomeOperationsByMember(quint16 id);
    void toReciveOutcomeOperationsByFamily(quint16 id);

    void updatebd();
};

#endif // MAINWINDOW_H
