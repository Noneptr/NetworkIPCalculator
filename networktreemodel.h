#ifndef NETWORKTREEMODEL_H
#define NETWORKTREEMODEL_H

#include <QStandardItemModel>
#include "networkinfo.h"
#include <queue>
#include "sort_qvector_helper.h"

/* Класс представляет
 * собой древовидную структуру данных
 * для работы с сетевой моделью
 *            net
 *          /     \
    subnet1        subnet2 */


enum NetworkTreeModelError {__ERROR_WRITE_IN_BIN_FILE__, __ERROR_READ_OF_BIN_FILE__,
                           __ERROR_USER_MAKE_BUSY_NODE__,
                           __ERROR_NETWORK_TREE_IS_EMPTY__,};                                  // Исключения вызываемые классом

const QString __file_extention__ = ".ipcalc";                                               // расширение бинарного файла с сетевой схемой

class NetworkTreeModel: public QStandardItemModel
{
    Q_OBJECT

private:
    QString __filename;                                                                     // имя файла

public:
    static QVector<QString> __signs__;
    /* Хранит в себе список подписей для элементов NetworkInfo.
       Необходимо для понятного вывода, а соответственно преобразования
       NetworkInfo в QString и обратно*/

    static QString __emptySign__;
    /* Хранит в себе пустой символ.
       Он необходим для того чтобы представление чётко понимало,
       что имеющие его элементы должны иметь возможность разделения,
       а не имеющие нет*/

    static QString __ico_busy_net__;                                                        // путь к файлу иконки занятой сети

    // ================ функции преобразования NetworkInfo в QString и обратно ======================
    static QString netInfoToString(const NetworkInfo &net_info);
    static NetworkInfo stringToNetInfo(const QString &data);
    //===============================================================================================

private:
    static void changeBusyHostInNode(QStandardItem *node, NetworkInfo &net_info, unsigned int &busy_hosts);         // изменение состояния занятости подсети

public:
    NetworkTreeModel(QObject *parent = nullptr);
    NetworkTreeModel(const QString &filename, QObject *parent = nullptr);
    virtual ~NetworkTreeModel();

public:
    void setFilename(const QString &filename);
    QString filename() const;

public slots:
    // ============================= Создание и вставка узла ===============================================================
    void insertIntoNetwork(const NetworkInfo &net_info);
    void createNetworkItem(QStandardItem *parent, const NetworkInfo &net_info);               // создать узел
    void createNetworkRoot(const IPrecord &ip, const NetMask &mask);                          // создать корневой узел сеть
    void createNetworkRoot(const NetworkInfo &net_info);                                      // создать корневой узел сеть
    //======================================================================================================================

    //============================= Разделение и слияние сети =============================================================
    void splitNetworkItem(const QModelIndex &parentIndex);                                    // разделить сеть на подсети
    void mergeNetworkItem(const QModelIndex &parentIndex);                                    // объединить подсети в сеть
    //=====================================================================================================================

    void expandAllExist();                                                                    // раскрыть все cуществующие p.s. довольно тяжёлая операция

    //===================================== Работа с файлом ===============================================================
    void writeNetworkInFile();                                                                // записать модель в файл
    void readNetworkOfFile();                                                                 // считать модель из файла
    //=====================================================================================================================

    //========================== Разделение сети по заданным параметрам ===================================================
    void makeBusyNode(QStandardItem *node, unsigned int &busy_hosts);                          // модельное выделение подсети с занятыми хостами
    void makeBusyNodes(const QVector<unsigned int> &vals);                                     // модельное выделение подсетей с занятыми хостами
    void userMakeBusyNode(const QModelIndex &index, unsigned int busy_hosts);                  // пользовательское выделение подсети с занятыми хостами
    //=====================================================================================================================

    //========================== Поиск узла в дереве ======================================================================
    QVector<QModelIndex> findNodes(const QString &key);
    //=====================================================================================================================

signals:
    void needExpandItem(const QModelIndex&);                                                  // сигнализирует представлению о необходимости
                                                                                              // раскрыть элемент с указанным индексом

    void fileReaded();                                                                        // сигнализирует о том, что файл прочитан
    void fileWrited();                                                                        // сигнализирует о том, что файл записан
    void fileReadActive();                                                                    // сигнал, файл считывается
    void fileWriteActive();                                                                   // сигнал, файл записывается
    void expandAllExistActive();                                                              // сигнал, раскрытие модели
    void makedBusyNodes();                                                                    // сигнал о том, что подсети сделаны
    void notMakedBusyNodes(const QVector<unsigned int> &);                                    // сигнал о том, какие подсети не могут быть сделаны
    void searchIsActive();                                                                    // сигнал о том, что идёт поиск
};

#endif // NETWORKTREEMODEL_H
