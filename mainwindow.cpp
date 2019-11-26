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
    ui->treeView->resizeColumnToContents(0);

    NetMask mask(IPrecord(255, 255, 248, 0));
    IPrecord ip(95, 189, 76, 115);
    IPrecord wildcard = IPrecord(255, 255, 255, 255) - mask.mask();
    IPrecord network = ip & mask.mask();
    IPrecord direct_broadcast = network + wildcard;
    IPrecord host_min = network + IPrecord(0, 0, 0, 1);
    IPrecord host_max = direct_broadcast - IPrecord(0, 0, 0, 1);

    qDebug() << "Adress: " << ip.toQString();                       // ip - адрес
    qDebug() << "Bitmask: " << mask.countBits();                    // количество бит в маске
    qDebug() << "Netmask: " << mask.mask().toQString();             // маска подсети
    qDebug() << "Wildcard: " << wildcard.toQString();               // обратная маска подсети
    qDebug() << "Network: " << network.toQString();                 // адрес сети
    qDebug() << "Broadcast: " << direct_broadcast.toQString();      // широковещательный адрес сети
    qDebug() << "Hostmin: " << host_min.toQString();                // адрес первого хоста
    qDebug() << "Hostmax: " << host_max.toQString();                // адрес последнего хоста
    qDebug() << "Hosts: " << mask.countHosts();                     // количество хостов
}

MainWindow::~MainWindow()
{
    delete ui;
}

