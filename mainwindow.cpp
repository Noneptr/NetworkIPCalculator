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
    connect(ui->treeView, &NetworkTreeView::doubleClicked, this, &MainWindow::setBusyNode);         // реакция на попытку редактировать узел

    connect(model, &NetworkTreeModel::notMakedBusyNodes,
            [](const QVector<unsigned int> &v)
    {
        qDebug() << "{ ";
        for (unsigned e: v)
        {
            qDebug() << e << ";";
        }
        qDebug() << "}";
    });                                                                         // реакция на не выделенные подсети

    model->setHorizontalHeaderLabels({QString("")});
    model->createNetworkRoot(IPrecord(192, 168, 0, 0), NetMask(24));

    ui->treeView->setModel(model);
    ui->treeView->setIndentation(75);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрет на редактирование содержимого узлов дерева

    QVector<unsigned int> v = {60, 30, 8, 12, 18};
//    QVector<unsigned int> v = {62, 62, 62, 30, 2, 2, 3};
    model->makeBusyNodes(v);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setBusyNode(const QModelIndex index)
{
    NetInputDialog dialog("Введите значение занятых хостов: ", "Изменить", "Отмена", "Занять\\Освободить сеть", this);

    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            unsigned int count = dialog.resultInput().toUInt();
            model->userMakeBusyNode(index, count);
            ui->treeView->collapse(index);
        }
        catch (NetworkTreeModelError &error)
        {
            if (error == __ERROR_USER_MAKE_BUSY_NODE__)
            {
                ui->statusbar->setStyleSheet("color: rgba(178, 34, 34);");
                ui->statusbar->showMessage("Ошибка!!! Введено хостов больше, чем имеется в подсети!!!", 2500);
            }
        }
    }
}
