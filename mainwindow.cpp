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

    NetMask mask(IPrecord(255, 192, 0, 0));
    IPrecord ip(95, 189, 76, 115);
    IPrecord wildcard = IPrecord(255, 255, 255, 255) - mask.mask();
    IPrecord network = ip & mask.mask();
    IPrecord direct_broadcast = network + wildcard;
    IPrecord host_min = network + 1;
    IPrecord host_max = direct_broadcast - 1;

    qDebug() << "Adress: " << ip.toQString();                       // ip - адрес
    qDebug() << "Bitmask: " << mask.countBits();                    // количество бит в маске
    qDebug() << "Netmask: " << mask.mask().toQString();             // маска подсети
    qDebug() << "Wildcard: " << wildcard.toQString();               // обратная маска подсети
    qDebug() << "Network: " << network.toQString();                 // адрес сети
    qDebug() << "Broadcast: " << direct_broadcast.toQString();      // широковещательный адрес сети
    qDebug() << "Hostmin: " << host_min.toQString();                // адрес первого хоста
    qDebug() << "Hostmax: " << host_max.toQString();                // адрес последнего хоста
    qDebug() << "Hosts: " << mask.countHosts();                     // количество хостов

//    IPrecord ip1 = ip;
//    NetMask mask1(mask.countBits() + 1);
//    IPrecord wildcard1 = IPrecord(255, 255, 255, 255) - mask1.mask();
//    IPrecord network1 = ip & mask1.mask();
//    IPrecord direct_broadcast1 = network1 + wildcard1;
//    IPrecord host_min1 = network1 + IPrecord(0, 0, 0, 1);
//    IPrecord host_max1 = direct_broadcast1 - IPrecord(0, 0, 0, 1);

//    qDebug() << "Adress1: " << ip1.toQString();                       // ip - адрес
//    qDebug() << "Bitmask1: " << mask1.countBits();                    // количество бит в маске
//    qDebug() << "Netmask1: " << mask1.mask().toQString();             // маска подсети
//    qDebug() << "Wildcard1: " << wildcard1.toQString();               // обратная маска подсети
//    qDebug() << "Network1: " << network1.toQString();                 // адрес сети
//    qDebug() << "Broadcast1: " << direct_broadcast1.toQString();      // широковещательный адрес сети
//    qDebug() << "Hostmin1: " << host_min1.toQString();                // адрес первого хоста
//    qDebug() << "Hostmax1: " << host_max1.toQString();                // адрес последнего хоста
//    qDebug() << "Hosts1: " << mask1.countHosts();                     // количество хостов

//    IPrecord ip2 = network1 + ;
//    NetMask mask1(mask.countBits() + 1);
//    IPrecord wildcard1 = IPrecord(255, 255, 255, 255) - mask1.mask();
//    IPrecord network1 = ip & mask1.mask();
//    IPrecord direct_broadcast1 = network1 + wildcard1;
//    IPrecord host_min1 = network1 + IPrecord(0, 0, 0, 1);
//    IPrecord host_max1 = direct_broadcast1 - IPrecord(0, 0, 0, 1);

//    qDebug() << "Adress2: " << ip.toQString();                       // ip - адрес
//    qDebug() << "Bitmask2: " << mask.countBits();                    // количество бит в маске
//    qDebug() << "Netmask2: " << mask.mask().toQString();             // маска подсети
//    qDebug() << "Wildcard2: " << wildcard.toQString();               // обратная маска подсети
//    qDebug() << "Network2: " << network.toQString();                 // адрес сети
//    qDebug() << "Broadcast2: " << direct_broadcast.toQString();      // широковещательный адрес сети
//    qDebug() << "Hostmin2: " << host_min.toQString();                // адрес первого хоста
//    qDebug() << "Hostmax2: " << host_max.toQString();                // адрес последнего хоста
//    qDebug() << "Hosts2: " << mask.countHosts();                     // количество хостов
}

MainWindow::~MainWindow()
{
    delete ui;
}

