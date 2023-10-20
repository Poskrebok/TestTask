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

    QVBoxLayout *layout = new QVBoxLayout(tabWidget);
    QWidget *widget = new QWidget(tabWidget);
    widget->setLayout(layout);
    incomeOperationsTable = new QTableView(tabWidget);
    outcomeOperationsTable = new QTableView(tabWidget);
    layout->addWidget(incomeOperationsTable);
    layout->addWidget(outcomeOperationsTable);
    tabWidget->addTab(widget, QString("Таблица операций"));
    maindb = new Maindb();


    QThread *thread = new QThread();
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);
    connect(thread,&QThread::started,maindb,&Maindb::initDB);
    thread->start();
    maindb->moveToThread(thread);

    connect(exit,&QAction::triggered,this,&MainWindow::close);
    connect(addMember,&QAction::triggered,this,&MainWindow::addMember);
    connect(addFamily,&QAction::triggered,this,&MainWindow::addFamily);

    connect(this,&MainWindow::sendMemberToDb,maindb,&Maindb::addMember);
    connect(this,&MainWindow::sendFamilyToDb,maindb,&Maindb::addFamily);

    membersTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(membersTable, &QTreeView::customContextMenuRequested,this,&MainWindow::onContextMenuFamilyControl);
    connect(membersTable,&QTreeView::clicked,this,&MainWindow::onEntityClicked);

    incomeOperationsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(incomeOperationsTable, &QTreeView::customContextMenuRequested,this,&MainWindow::onContextMenuIncomeOperationsControl);
    outcomeOperationsTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(outcomeOperationsTable, &QTreeView::customContextMenuRequested,this,&MainWindow::onContextMenuOutcomeOperationsControl);

    connect(maindb,&Maindb::sendIncomeOperations,this,&MainWindow::incomeOperationsModelHandler);
    connect(maindb,&Maindb::sendOutcomeOperations,this,&MainWindow::outcomeOperationsModelHandler);
    
    connect(update,&QAction::triggered,maindb,&Maindb::reciveFamilyModel);
    connect(this,&MainWindow::updatebd,maindb,&Maindb::reciveFamilyModel);
    connect(this,&MainWindow::updatebd,this,[=](){
        onEntityClicked(membersTable->currentIndex());
    });
    connect(maindb,&Maindb::sendModel,this,&MainWindow::reciveModel);

    connect(this,&MainWindow::toReciveIncomeOperationsByMember,maindb,&Maindb::operationsIncomeByMember);
    connect(this,&MainWindow::toReciveIncomeOperationsByFamily,maindb,&Maindb::operationsIncomeByFamily);
    connect(this,&MainWindow::toReciveOutcomeOperationsByMember,maindb,&Maindb::operationsOutcomeByMember);
    connect(this,&MainWindow::toReciveOutcomeOperationsByFamily,maindb,&Maindb::operationsOutcomeByFamily);

    connect(this,&MainWindow::createConnection,maindb,&Maindb::addConnection);
    connect(this,&MainWindow::breakConnection,maindb,&Maindb::removeConnection);

    connect(this,&MainWindow::createOperationIncomeRecord,maindb,&Maindb::addIncomeOperation);
    connect(this,&MainWindow::createOperationOutcomeRecord,maindb,&Maindb::addOutcomeOperation);
    connect(this,&MainWindow::changeOperationIncomeRecord,maindb,&Maindb::changeIncomeOperation);
    connect(this,&MainWindow::changeOperationOutcomeRecord,maindb,&Maindb::changeOutcomeOperation);
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

void MainWindow::onContextMenuIncomeOperationsControl(const QPoint &pos)
{
    QMenu menu;
    QModelIndex index = incomeOperationsTable->indexAt(pos);
    QModelIndex memberIndex = membersTable->currentIndex();
    AbstractTreeItem *entity = static_cast<AbstractTreeItem*>(memberIndex.internalPointer());
    MemberTreeItem *member = dynamic_cast<MemberTreeItem*>(entity);
    if(member == nullptr)
        return;
    if(!index.isValid())
    {
        menu.addAction("Добавить запись о доходах", this, [=](){
            IncomeOperationDialog dialog;
            if(dialog.exec())
            {
                auto id = member->getItemData().id;
                emit createOperationIncomeRecord(id, dialog.getDT(),dialog.getIncome(),dialog.getComment(),dialog.getSourceId());
            }
        });
    }
    emit updatebd();
    menu.exec(QCursor::pos());
}

void MainWindow::onContextMenuOutcomeOperationsControl(const QPoint &pos)
{
    QMenu menu;
    QModelIndex index = outcomeOperationsTable->indexAt(pos);
    QModelIndex memberIndex = membersTable->currentIndex();
    AbstractTreeItem *entity = static_cast<AbstractTreeItem*>(memberIndex.internalPointer());
    MemberTreeItem *member = dynamic_cast<MemberTreeItem*>(entity);
    if(member == nullptr)
        return;
    if(!index.isValid())
    {
        menu.addAction("Добавить запись о расходах", this, [=](){
            OutcomeOperationDialog dialog;
            if(dialog.exec())
            {
                auto id = member->getItemData().id;
                emit createOperationOutcomeRecord(id, dialog.getDT(),dialog.getOutcome(),dialog.getComment(),dialog.getGoodsId());
            }
        });
    }
    emit updatebd();
    menu.exec(QCursor::pos());
}

void MainWindow::onEntityClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    auto data = static_cast<AbstractTreeItem*>(index.internalPointer());
    auto family = dynamic_cast<FamilyTreeItem*>(data);
    auto member = dynamic_cast<MemberTreeItem*>(data);
    if(family == nullptr)
    {
        emit toReciveIncomeOperationsByMember(member->getItemData().id);
        emit toReciveOutcomeOperationsByMember(member->getItemData().id);
    }
    if(member == nullptr)
    {
        emit toReciveIncomeOperationsByFamily(family->getItemData().id);
        emit toReciveOutcomeOperationsByFamily(family->getItemData().id);
    }

}

void MainWindow::incomeOperationsModelHandler(QSqlQueryModel *model)
{
    auto toDelete = incomeOperationsTable->model();
    incomeOperationsTable->setModel(model);
    delete toDelete;
}

void MainWindow::outcomeOperationsModelHandler(QSqlQueryModel *model)
{
    auto toDelete = outcomeOperationsTable->model();
    outcomeOperationsTable->setModel(model);
    delete toDelete;
}
