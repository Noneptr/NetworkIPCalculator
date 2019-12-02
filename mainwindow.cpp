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
    model->createNetworkRoot(IPrecord(192, 168, 0, 0), NetMask(24));

    ui->treeView->setModel(model);
    ui->treeView->setIndentation(75);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрет на редактирование содержимого узлов дерева

    unsigned int val = 	60;
    model->makeBusyNode(model->invisibleRootItem()->child(0), val);

    val = 	30;
    model->makeBusyNode(model->invisibleRootItem()->child(0), val);

    val = 	18;
    model->makeBusyNode(model->invisibleRootItem()->child(0), val);

    val = 	12;
    model->makeBusyNode(model->invisibleRootItem()->child(0), val);

    val = 	8;
    model->makeBusyNode(model->invisibleRootItem()->child(0), val);

//    for (QString &s: NetworkTreeModel::netInfoToString(NetworkInfo(IPrecord(192, 168, 0, 0), NetMask(31))).split('\n'))
//    {
//        qDebug() << s;
//    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    model->writeNetworkInFile();
}

void MainWindow::on_pushButton_2_clicked()
{
    model->readNetworkOfFile();
}

void MainWindow::on_pushButton_3_clicked()
{
    model->expandAllExist();
}
