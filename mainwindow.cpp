#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new NetworkTreeModel(this);

    connect(ui->treeView, SIGNAL(expanded(const QModelIndex &)), model, SLOT(splitNetworkItem(const QModelIndex &)));
    connect(ui->treeView, SIGNAL(collapsed(const QModelIndex &)), model, SLOT(mergeNetworkItem(const QModelIndex &)));
    connect(ui->treeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(alignColumnsInTreeView()));

    model->setHorizontalHeaderLabels({QString("")});
    model->createNetworkRoot(IPrecord(192, 168, 0, 0), NetMask(24));
    ui->treeView->setModel(model);
    ui->treeView->setIndentation(100);
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрет на редактирование дерева
}


//void MainWindow::createSubNet(int count)
//{
//    QStandardItem *rootItem = model->invisibleRootItem();
//    std::queue<QStandardItem *> nodes;
//    QStandardItem *curr;
//    NetworkInfo curr_netinfo;
//    nodes.push(rootItem);
//    while (!nodes.empty())
//    {
//        curr = nodes.front();
//        nodes.pop();


//        for (int i = 0; i < curr->rowCount(); i++)
//        {
//            nodes.push(curr->child(i));
//        }

//        if (curr == rootItem || curr->text() == NetworkTreeModel::__emptySign__)
//        {
//            continue;
//        }

//        QString data = curr->text();

//        QStringList list = data.split("\n");
//        IPrecord curr_ip(list[0].split(" ")[1]);                                     // выделить из данных адрес сети
//        NetMask curr_mask(list[2].split(" ")[1]);                                    // выделить из данных маску сети
//        curr_netinfo.calculateNet(curr_ip, curr_mask);

//        //if (coun <= )
//    }
//}


void MainWindow::alignColumnsInTreeView()
{
    ui->treeView->resizeColumnToContents(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

