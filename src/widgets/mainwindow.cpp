#include "mainwindow.h"
#include "QMenuBar"
#include "QAction"
#include "QStatusBar"
#include "QThread";

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

    membersTable = new QTreeView(centralwidget);
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(membersTable->sizePolicy().hasHeightForWidth());
    membersTable->setSizePolicy(sizePolicy);
    mainlo->addWidget(membersTable);
    model = new FamilyModel();
    membersTable->setModel(model);

    QTabWidget *tabWidget  = new QTabWidget(centralwidget);
    mainlo->addWidget(tabWidget);

    operationsTable = new QTableView(tabWidget);
    tabWidget->addTab(operationsTable, QString("Таблица операций"));
    FamilyModel *familyModel = new FamilyModel(operationsTable);
    operationsTable->setModel(familyModel);

//    QChartView *chartView = new QChartView(tabWidget);
//    tabWidget->addTab(chartView,QString("Графики"));
    maindb = new Maindb();


    QThread *thread = new QThread();
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::started,maindb,&Maindb::initDB);
    thread->start();
    maindb->moveToThread(thread);


    connect(thread,&QThread::finished,thread,&QThread::deleteLater);

    connect(exit,&QAction::triggered,this,&MainWindow::close);
    connect(addMember,&QAction::triggered,this,&MainWindow::addMember);
    connect(addFamily,&QAction::triggered,this,&MainWindow::addFamily);

    connect(this,&MainWindow::sendMemberToDb,maindb,&Maindb::addMember);
    connect(this,&MainWindow::sendFamilyToDb,maindb,&Maindb::addFamily);
    
    connect(update,&QAction::triggered,maindb,&Maindb::reciveFamilyModel);
    connect(this,&MainWindow::updatebd,maindb,&Maindb::reciveFamilyModel);
    connect(maindb,&Maindb::sendModel,this,&MainWindow::reciveModel);

    membersTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(membersTable, &QTreeView::customContextMenuRequested,this,&MainWindow::onContextMenuFamilyControl);


    operationsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(operationsTable, &QTreeView::customContextMenuRequested,this,&MainWindow::onContextMenuFamilyControl);

    connect(this,&MainWindow::createConnection,maindb,&Maindb::addConnection);
    connect(this,&MainWindow::breakConnection,maindb,&Maindb::removeConnection);
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
        auto data = dialog->getMember();
        emit sendMemberToDb(data.fname,data.mname,data.lname,data.bdate,data.gender);
        emit this->updatebd();
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
        emit this->updatebd();
    }
    delete dialog;
}

void MainWindow::reciveModel(AbstractTreeItem* data)
{
    model->setupModelData(data);
}

void MainWindow::onContextMenuFamilyControl(const QPoint &pos)
{
    QMenu menu;
    QModelIndex index = membersTable->indexAt(pos);
    if(index.isValid())
    {
        AbstractTreeItem *entity = static_cast<AbstractTreeItem*>(index.internalPointer());
        MemberTreeItem *member = dynamic_cast<MemberTreeItem*>(entity);
        if(member == nullptr)
            return;
        menu.addAction("Изменить", this, [=](){

            emit this->updatebd();
        });
        menu.addAction("Удалить", this, [=](){

            emit this->updatebd();
        });
        QMenu *innerMenuAdd = new QMenu(&menu);
        innerMenuAdd->setTitle(QString("Добавить в..."));
        menu.addAction(innerMenuAdd->menuAction());
        auto families = model->getFamilies();
        for(auto i : families)
        {
            innerMenuAdd->addAction(QString("%1").arg(i.name),[=](){
                emit createConnection(member->getItemData().id,i.id);
                emit this->updatebd();
            });
        }

        FamilyTreeItem  *family = dynamic_cast<FamilyTreeItem*>(member->parent());
        if(family != nullptr)
        {
            QMenu *innerMenuRemove = new QMenu(&menu);
            innerMenuRemove->setTitle(QString("Убрать из..."));
            menu.addAction(innerMenuRemove->menuAction());
            innerMenuRemove->addAction(QString("%1").arg(family->getItemData().name),[=](){
                emit breakConnection(member->getItemData().id,family->getItemData().id);
                emit this->updatebd();
            });
        }
    }

    menu.exec(QCursor::pos());
}

void MainWindow::onContextMenuOperationsControl(const QPoint &pos)
{
    QMenu menu;
    QModelIndex index = operationsTable->indexAt(pos);
    if(!index.isValid())
    {
        menu.addAction("Добавить запись о расходах", this, [=](){

        });
        menu.addAction("Добавить запись о доходах", this, [=](){

        });
    }
    if(index.isValid())
    {
        menu.addAction("Редактировать запись о расходах", this, [=](){

        });
        menu.addAction("Редактировать запись о доходах", this, [=](){

        });
    }

    menu.exec(QCursor::pos());
}
