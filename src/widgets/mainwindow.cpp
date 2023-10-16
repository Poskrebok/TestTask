#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags):QMainWindow(parent,flags)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MainWindow"));
    this->resize(800, 600);

    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    this->setCentralWidget(centralwidget);

    menubar = new QMenuBar(this);
    this->setMenuBar(menubar);
    QMenu *menu = new QMenu(QString("Фаил..."),menubar);
    menubar->addAction(menu->menuAction());
    QAction *exit = new QAction(QString("Выход..."),this);
    QAction *addMember = new QAction("Добавить нового человека...");
    QAction *addFamily = new QAction("Добавить новую семью...");
    QAction *update = new QAction("Обновить");
    menu->addAction(exit);
    menu->addAction(addMember);
    menu->addAction(addFamily);
    menu->addAction(update);

    statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);


    QHBoxLayout *mainlo = new QHBoxLayout(centralwidget);
    this->centralWidget()->setLayout(mainlo);

    QTreeView *membersTable = new QTreeView(centralwidget);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(membersTable->sizePolicy().hasHeightForWidth());
    membersTable->setSizePolicy(sizePolicy);
    mainlo->addWidget(membersTable);

    QTabWidget *tabWidget  = new QTabWidget(centralwidget);
    mainlo->addWidget(tabWidget);

    QTableView *operationsTable = new QTableView(tabWidget);
    tabWidget->addTab(operationsTable, QString("Таблица операций"));
    FamilyModel *familyModel = new FamilyModel(operationsTable);
    operationsTable->setModel(familyModel);

    QChartView *chartView = new QChartView(tabWidget);
    tabWidget->addTab(chartView,QString("Графики"));

    maindb = new Maindb();

    connect(exit,&QAction::triggered,this,&MainWindow::close);
    connect(addMember,&QAction::triggered,this,&MainWindow::addMember);
    connect(addFamily,&QAction::triggered,this,&MainWindow::addFamily);
    connect(update,&QAction::triggered,maindb,&Maindb::recivieFamilyList);

    connect(this,&MainWindow::sendMemberToDb,maindb,&Maindb::addMember);
    connect(this,&MainWindow::sendFamilyToDb,maindb,&Maindb::addFamily);

    connect(maindb,&Maindb::sendFamilyList,this,&MainWindow::readDB);
}


MainWindow::~MainWindow()
{
    delete maindb;
}

void MainWindow::addMember()
{
    MemberDialog *dialog = new MemberDialog(this);
    if(dialog->exec() == 1)
    {
        auto data = dialog->getData();
        emit sendMemberToDb(data.fname,data.mname,data.lname,data.bdate,data.gender);
    }
    delete dialog;
}

void MainWindow::addFamily()
{
    FamilyDialog *dialog = new FamilyDialog(this);
    if(dialog->exec() == 1)
    {
        auto data = dialog->getData();
        emit sendFamilyToDb(data.name);
    }
    delete dialog;
}

void MainWindow::readDB(QList<Family>)
{

}
