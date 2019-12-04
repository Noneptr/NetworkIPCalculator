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

    connect(ui->treeView, &NetworkTreeView::doubleClicked,
            [&](const QModelIndex &index)
    {
        QStandardItem *parent = static_cast<QStandardItem*>(index.internalPointer());
        QString data = parent->child(index.row())->text();
        for(QString &s : data.split("\n"))
        {
            qDebug() << s;
        }
        qDebug() << endl;

        NetInputDialog dialog("Введите количество занятых: ", "Изменить", "Отмена", this);

        if (dialog.exec() == QDialog::Accepted)
        {
            try
            {
                model->userMakeBusyNode(index, 14);
            }
            catch (NetworkTreeModelError &error)
            {
                if (error == __ERROR_USER_MAKE_BUSY_NODE__)
                qDebug() << "No maked!!!" << endl;
            }
        }
    });                                                                         // реакция на попытку редактировать узел

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
