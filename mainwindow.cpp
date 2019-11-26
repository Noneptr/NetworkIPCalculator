#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    QStandardItem * parentItem = model->invisibleRootItem();
    model->setHorizontalHeaderLabels({QString("")});
    for (int i = 0; i < 5; i++)
    {
        QStandardItem *pitemi = new QStandardItem(QString("item %0").arg(i));
        for (int j = 0; j < 3; j++)
        {
            QStandardItem *pitemj = new QStandardItem(QString("item %0 %1").arg(i).arg(j));
            for (int k = 0; k < 6; k++)
            {
                QStandardItem *pitemk = new QStandardItem(QString("item %0 %1 %2").arg(i).arg(j).arg(k));
                for (int h = 0; h < 2; h++)
                {
                    QStandardItem *pitemh = new QStandardItem(QString("item %0 %1 %2 %3").arg(i).arg(j).arg(k).arg(h));
                    for (int u = 0; u < 4; u++)
                    {
                        QStandardItem *pitemu = new QStandardItem(QString("item %0 %1 %2 %3 %4").arg(i).arg(j).arg(k).arg(h).arg(u));
                        pitemh->appendRow(pitemu);
                    }
                    pitemk->appendRow(pitemh);
                }
                pitemj->appendRow(pitemk);
            }
            pitemi->appendRow(pitemj);
        }
        parentItem->appendRow(pitemi);
    }
//    for (int i =0; i < 4; i++)
//    {
//        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
//        parentItem->appendRow(item);
//        parentItem = item;
//    }
    ui->treeView->setModel(model);
    QModelIndex pm = model->index(1, 0);
    QModelIndex m = model->index(0, 0, pm);
    QString text = m.data(Qt::DisplayRole).toString();
    qDebug() << text << endl;
    ui->treeView->collapseAll();
    ui->treeView->expandAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

