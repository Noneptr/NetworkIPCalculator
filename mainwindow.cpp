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
    connect(model, SIGNAL(needExpandItem(const QModelIndex &)), ui->treeView, SLOT(expand(const QModelIndex &)));

    model->setHorizontalHeaderLabels({QString("")});
//    model->createNetworkRoot(IPrecord(192, 168, 0, 0), NetMask(24));
    model->insertIntoNetwork(NetworkInfo(IPrecord(192, 168, 0, 0), NetMask(24)));
    model->insertIntoNetwork(NetworkInfo(IPrecord(192, 168, 0, 128), NetMask(25)));
    model->insertIntoNetwork(NetworkInfo(IPrecord(192, 168, 0, 128), NetMask(26)));
    model->insertIntoNetwork(NetworkInfo(IPrecord(192, 168, 0, 192), NetMask(26)));
    model->insertIntoNetwork(NetworkInfo(IPrecord(192, 168, 0, 0), NetMask(25)));
    model->setFilename("my_net");

    ui->treeView->setModel(model);
    ui->treeView->setIndentation(75);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрет на редактирование содержимого узлов дерева

//    for (QString &s: NetworkTreeModel::netInfoToString(NetworkInfo(IPrecord(192, 168, 0, 0), NetMask(31))).split('\n'))
//    {
//        qDebug() << s;
//    }
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


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    model->expandAllExist();
//    model->writeNetworkInFile();
}

void MainWindow::on_pushButton_2_clicked()
{
    model->expandAllExist();
//    model->readNetworkOfFile();
}
