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


void MainWindow::alignColumnsInTreeView()
{
    ui->treeView->resizeColumnToContents(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

