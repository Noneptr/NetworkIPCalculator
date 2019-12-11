#include "networktreemodel.h"

QVector<QString> NetworkTreeModel::__signs__ = {"Adress: ", "BitMask: ", "Mask: ",
                                                "Wildcard: ", "Direct broadcast: ",
                                                "Host min: ", "Host max: ", "Total hosts: ",
                                                "Busy hosts: ", "Free hosts: "};

QString NetworkTreeModel::__emptySign__ = "&";


QString NetworkTreeModel::__ico_busy_net__ = ":/rec/icons/busy_network.png";


NetworkTreeModel::NetworkTreeModel(QObject *parent)
    :QStandardItemModel(parent), __filename("new_net_tree" + __file_extention__)
{
    setHorizontalHeaderLabels({QString("")});
    connect(this, SIGNAL(fileReaded()), this, SLOT(expandAllExist()));
    connect(this, SIGNAL(makedBusyNodes()), this, SLOT(expandAllExist()));
}

NetworkTreeModel::NetworkTreeModel(const QString &filename, QObject *parent)
    :QStandardItemModel(parent), __filename(filename)
{
    setHorizontalHeaderLabels({QString("")});
    connect(this, SIGNAL(fileReaded()), this, SLOT(expandAllExist()));
    connect(this, SIGNAL(makedBusyNodes()), this, SLOT(expandAllExist()));
}


QString NetworkTreeModel::netInfoToString(const NetworkInfo &net_info)
{
    QString data = "";
    data += __signs__[0] + net_info.network().toQString() + "\n" +
            __signs__[1] + QString::number(net_info.mask().countBits()) + "\n" +
            __signs__[2] + net_info.mask().mask().toQString() + "\n" +
            __signs__[3] + net_info.wildcard().toQString() + "\n" +
            __signs__[4] + net_info.directBroadcast().toQString() + "\n" +
            __signs__[5] + net_info.hostMin().toQString() + "\n" +
            __signs__[6] + net_info.hostMax().toQString() + "\n" +
            __signs__[7] + QString::number(net_info.mask().countHosts()) + "\n" +
            __signs__[8] + QString::number(net_info.busyHosts()) + "\n" +
            __signs__[9] + QString::number(net_info.mask().countHosts() - net_info.busyHosts());
    return data;
}


NetworkInfo NetworkTreeModel::stringToNetInfo(const QString &data)
{
    QStringList list = data.split("\n");
    IPrecord net_ip(list[0].split(": ")[1]);                                     // выделить из данных адрес сети
    NetMask net_mask(list[2].split(": ")[1]);                                    // выделить из данных маску сети
    unsigned int busy_hosts = list[8].split(": ")[1].toUInt();                   // выделить из данных количество занятых хостов
    NetworkInfo net_info(net_ip, net_mask);
    net_info.setBusyHosts(busy_hosts);
    return net_info;
}


void NetworkTreeModel::insertIntoNetwork(const NetworkInfo &net_info)
{
    QStandardItem *parent = invisibleRootItem();
    if (parent->rowCount() == 0)
    {
        createNetworkRoot(net_info);
    }
    else
    {
        QStandardItem *root = parent->child(0);
        QStandardItem *curr = root;
        while (true)
        {
            NetworkInfo curr_info = stringToNetInfo(curr->text());          // получение сетевых данных о текущем узле

            IPrecord mid_host = IPrecord::uintToIPrecord(curr_info.directBroadcast().toUInt() - curr_info.wildcard().toUInt() / 2);

            if (net_info.directBroadcast() < mid_host)                      // если меньше среднего адреса сети узла
            {
                if (curr->rowCount() == 1)
                {
                    QString child0data = curr->child(0)->text();
                    if (child0data != __emptySign__)
                    {
                        NetworkInfo child_info = stringToNetInfo(child0data);
                        if (child_info.directBroadcast() < mid_host)        // если позиция левого занята
                        {
                            curr = curr->child(0);                          // то спускаемся к нему
                            continue;
                        }
                    }

                    createNetworkItem(curr, net_info);                      // создаём и присоединяем новый узел
                    if (child0data == __emptySign__)
                    {
                        curr->removeRow(0);                                 // удаления пустого символа
                    }
                    else                                                    // нарушение структуры т.к. правый узел стоит первым
                    {
                        curr->sortChildren(0);                              // восстановление структуры сыновей, путём перестановки их местами
                    }
                    break;
                }
                else if (curr->rowCount() == 2)
                {
                    curr = curr->child(0);                                  // спуститься к левому сыну
                    continue;
                }
                else
                {
                    break;                                                  // достигнут самый нижний уровень дерева, выход из вставки
                }
            }
            else
            {
                if (curr->rowCount() == 1)
                {
                    QString child0data = curr->child(0)->text();
                    if (child0data != __emptySign__)
                    {
                        NetworkInfo child_info = stringToNetInfo(child0data);
                        if (child_info.directBroadcast() >= mid_host)       // если позиция правого занята
                        {
                            curr = curr->child(0);                          // то спускаемся к нему
                            continue;
                        }
                    }

                    createNetworkItem(curr, net_info);                       // создаём и присоединяем новый узел
                    if (curr->child(0)->text() == __emptySign__)
                    {
                        curr->removeRow(0);                                  // удаление пустого символа
                    }
                    break;
                }
                else if (curr->rowCount() == 2)
                {
                    curr = curr->child(1);                                  // спуститься к правому сыну
                    continue;
                }
                else
                {
                    break;                                                  // достигнут самый нижний уровень дерева, выход из вставки
                }
            }
        }
    }
}


void NetworkTreeModel::expandAllExist()
{
    QStandardItem *parent = invisibleRootItem();
    if (parent->rowCount() > 0)
    {
        QStandardItem *root = parent->child(0);
        std::queue<QStandardItem *> nodes;
        nodes.push(root);
        QStandardItem *curr;
        while(!nodes.empty())                                           // обход дерева в ширину
        {
            emit expandAllExistActive();

            curr = nodes.front();
            nodes.pop();

            if (curr->rowCount() > 1)                                   // если у узла два сына значит его необходимо раскрыть
            {
                emit needExpandItem(curr->index());
                nodes.push(curr->child(0));
                nodes.push(curr->child(1));
            }
        }
    }
}


void NetworkTreeModel::createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info)
{
    QString data = netInfoToString(net_info);

    QStandardItem *node = new QStandardItem(data);

    unsigned short bits = net_info.mask().countBits() + 1;
    if (bits < 31)
    {
        if (net_info.busyHosts() == 0)                     // проверка на то что может ли существовать следующий узел
        {
            //================================== Проверка RFC ============================================
            if (!__check_rfc__::check_rfc(net_info.network()))
            {
                node->appendRow(new QStandardItem(__emptySign__));                              // добавление пустого символа
            }
            //===========================================================================================
        }
    }

    parent->appendRow(node);

    //================== Установка иконки если узел занятыми хостами ========================
    if (net_info.busyHosts() > 0)
    {
        node->setIcon(QIcon(QPixmap(__ico_busy_net__)));
    }
    //=======================================================================================
}


void NetworkTreeModel::createNetworkRoot(const IPrecord &ip, const NetMask &mask)
{
    this->clear();
    setHorizontalHeaderLabels({QString("")});
    QStandardItem *parentItem = this->invisibleRootItem();
    createNetworkItem(parentItem, NetworkInfo(ip, mask));
}


void NetworkTreeModel::createNetworkRoot(const NetworkInfo &net_info)
{
    this->clear();
    setHorizontalHeaderLabels({QString("")});
    QStandardItem *parentItem = this->invisibleRootItem();
    createNetworkItem(parentItem, net_info);
}

void NetworkTreeModel::splitNetworkItem(const QModelIndex &parentIndex)
{
    QStandardItem * parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());       // родитель разделяемого узла
    QStandardItem *netItem = parentItem->child(parentIndex.row());                                  // разделяемый узел
    if (netItem->rowCount() == 1)
    {
        QString data = netItem->text();
        NetworkInfo net = stringToNetInfo(data);
        NetworkInfo sub_net1(net.network(), NetMask(net.mask().countBits() + 1));                   // выделение подсетей
        NetworkInfo sub_net2(sub_net1.directBroadcast() + 1, NetMask(net.mask().countBits() + 1));

        createNetworkItem(netItem, sub_net1);                                                       // добавление подсетей в дерево
        createNetworkItem(netItem, sub_net2);
        netItem->removeRow(0);                                                                      // удаление пустого символа __emptySign__
    }
}


void NetworkTreeModel::mergeNetworkItem(const QModelIndex &parentIndex)
{
    QStandardItem *parentItem = static_cast<QStandardItem *>(parentIndex.internalPointer());        // родитель агрегируемого узла
    QStandardItem *netItem = parentItem->child(parentIndex.row());                                  // агрегируемый узел
    if (netItem->rowCount() == 2)
    {
        netItem->removeRows(0, netItem->rowCount());                                                // удаление дочерних узлов
        netItem->appendRow(new QStandardItem(__emptySign__));                                       // добавление пустого символа
    }
}


void NetworkTreeModel::setFilename(const QString &filename)
{
    __filename = filename;
}


void NetworkTreeModel::changeBusyHostInNode(QStandardItem *node, NetworkInfo &net_info ,unsigned int &busy_hosts)
{
    net_info.setBusyHosts(busy_hosts);
    node->setText(netInfoToString(net_info));
    node->removeRows(0, node->rowCount());
    busy_hosts = 0;

    //================== Установка иконки ===================================================
    node->setIcon(QIcon(QPixmap(__ico_busy_net__)));
    //=======================================================================================
}


void NetworkTreeModel::makeBusyNode(QStandardItem *node, unsigned int &busy_hosts)
{
    if (node && busy_hosts > 0)         /* проверка статуса выполнения задачи, busy_host = 0 - задача выполнена (выход из рекурсии)*/
    {
        NetworkInfo node_info = stringToNetInfo(node->text());              // получение сетевой информации о текущем узле

        if (node_info.busyHosts() == 0)                                     // если узел не имеет занятых хостов, то идём дальше, иначе выход
        {
            long long unsigned int e = 10 *
            static_cast<long long unsigned int>(pow(10, unsigned(QString::number(busy_hosts).count())));

            /* e - говорит нам о точности результата разности двух чисел
             * например,
                         e = 1000; val = 19.2428;
                         ans = round(val * e)/e;    // ans == 19.243
             * например,
                         e = 100; val = 19.2428;
                         ans = round(val * e)/e;    // ans == 19.24
            */

            double val = round((log2(node_info.mask().countHosts())
                        - log2(busy_hosts)) * e) / e;

            /*
             * val - разность логарифмов количества имеющихся хостов и количества необходимых для занятости
            */

            if (val >= 0)                       // случай когда кол-во необходимых для занятости хостов меньше или равно кол-ву имеющихся
            {
                if (val <= 1)                   // случай когда кол-во необходимых для занятости хостов может разместиться в узле
                {
                    if (!__check_rfc__::check_rfc(node_info.network()))
                    {
                        changeBusyHostInNode(node, node_info, busy_hosts);          // изменение состояния занятости подсети
                    }
                }
                else                            // случай когда кол-во свободных хостов сильно превышает кол-во необходимых для занятости хостов
                {
                    if (node->rowCount() == 1)              // если узел не был разделён разделить
                    {
                        splitNetworkItem(node->index());
                    }

                    makeBusyNode(node->child(0), busy_hosts);           // спуститься по левой ветви
                    makeBusyNode(node->child(1), busy_hosts);           // спуститься по правой ветви
                }
            }
        }
    }
}


void NetworkTreeModel::userMakeBusyNode(const QModelIndex &index, unsigned int busy_hosts)
{
    QStandardItem *parent = static_cast<QStandardItem*>(index.internalPointer());
    QStandardItem *netItem = parent->child(index.row());
    QString data = netItem->text();
    NetworkInfo net_info = stringToNetInfo(data);
    if (busy_hosts <= net_info.mask().countHosts())
    {
        //================================== Проверка RFC ===========================================
        if (!__check_rfc__::check_rfc_with_error(net_info.network()))
        {
            if (busy_hosts == 0)
            {
                net_info.setBusyHosts(busy_hosts);
                netItem->setText(netInfoToString(net_info));
                netItem->removeRows(0, netItem->rowCount());
                netItem->appendRow(new QStandardItem(__emptySign__));

                //================== Отчистка иконки ===================================================
                netItem->setIcon(QIcon());
                //=======================================================================================
            }
            else
            {
                changeBusyHostInNode(netItem, net_info, busy_hosts);                    // изменение состояния занятости подсети
            }
        }
        //===========================================================================================
    }
    else
    {
        throw __ERROR_USER_MAKE_BUSY_NODE__;
        /* количество занимаемых хостов должно не превышать количества имеющихся */
    }
}


void NetworkTreeModel::makeBusyNodes(const QVector<unsigned int> &vals)
{
    QStandardItem *root = invisibleRootItem()->child(0);
    if (root)
    {
        QVector<unsigned int> vector = vals;
        __sort_qvector_helper__::selectUpSortQVector(vector);                       // сортировка полученной последовательности
        for (unsigned int &e: vector)
        {
            makeBusyNode(root, e);
        }

        emit makedBusyNodes();                                                      // узлы с занятыми хостами сделаны

        QVector<unsigned int> notMakedVector;
        for (unsigned int e: vector)
        {
            if (e != 0)
            {
                notMakedVector.append(e);
            }
        }

        if (notMakedVector.size() > 0)
        {
            emit notMakedBusyNodes(notMakedVector);                                     // сигнал говорит о том какие подсети не могут быть добавлены
        }
    }
    else
    {
        throw __ERROR_NETWORK_TREE_IS_EMPTY__;
    }
}


QString NetworkTreeModel::filename() const
{
    return __filename;
}

void NetworkTreeModel::writeNetworkInFile()
{
    FILE *file = nullptr;
    file = fopen((__filename).toStdString().c_str(), "wb");
    if (file)
    {
        QStandardItem *parent = invisibleRootItem();
        if (parent->rowCount() > 0)
        {
            QStandardItem *root = parent->child(0);
            std::queue<QStandardItem *> nodes;
            nodes.push(root);
            QStandardItem *curr;
            while(!nodes.empty())                                                // обход дерева в ширину
            {
//                emit fileWriteActive();

                curr = nodes.front();
                nodes.pop();

                if (curr->text() != __emptySign__)
                {
                    NetworkInfo net_info = stringToNetInfo(curr->text());

                    fwrite(&net_info, sizeof(net_info), 1, file);               // запись информации об узле в файл

                    for (int i = 0; i < curr->rowCount(); i++)
                    {
                        nodes.push(curr->child(i));
                    }
                }
            }
        }
        fclose(file);

        emit fileWrited();                                                      // файл записан
    }
    else
    {
        throw __ERROR_WRITE_IN_BIN_FILE__;
    }
}


void NetworkTreeModel::readNetworkOfFile()
{
    FILE *file = nullptr;
    file = fopen((__filename).toStdString().c_str(), "rb");
    if (file)
    {
        this->clear();
        NetworkInfo net_info;
        while (fread(&net_info, sizeof(net_info), 1, file) == 1)        // чтение информации об узле из файла
        {
//            emit fileReadActive();
            insertIntoNetwork(net_info);                                // вставка узла в дерево
        }

        fclose(file);

        emit fileReaded();                                              // файл прочитан
    }
    else
    {
        throw __ERROR_READ_OF_BIN_FILE__;
    }
}


QVector<QModelIndex> NetworkTreeModel::findNodes(const QString &key)
{
    QStandardItem *parent = invisibleRootItem();
    QStandardItem *root = parent->child(0);
    if (root)
    {
        QVector<QModelIndex> find_nodes;
        std::queue<QStandardItem *> nodes;
        nodes.push(root);
        QStandardItem *curr;
        while(!nodes.empty())                                                // обход дерева в ширину
        {
            emit searchIsActive();

            curr = nodes.front();
            nodes.pop();

            QString data = curr->text();
            if (data != __emptySign__)
            {
                if (data.toLower().indexOf(key.toLower()) != -1)
                {
                    find_nodes.append(curr->index());
                }

                for (int i = 0; i < curr->rowCount(); i++)
                {
                    nodes.push(curr->child(i));
                }
            }
        }
        return find_nodes;
    }
    else
    {
        throw __ERROR_NETWORK_TREE_IS_EMPTY__;
        /* поиск в пустом дереве */
    }
}


NetworkTreeModel::~NetworkTreeModel()
{
}
