#ifndef NETWORKTREEMODEL_H
#define NETWORKTREEMODEL_H

#include <QStandardItemModel>
#include "networkinfo.h"
#include <queue>
#include <cmath>
#include <QDebug>
#include "sort_qvector_helper.h"

/* Класс представляет
 * собой древовидную структуру данных
 * для работы с сетевой моделью
 *            net
 *          /     \
    subnet1        subnet2 */


enum NetworkTreeModelError {__ERROR_WRITE_IN_BIN_FILE__, __ERROR_READ_OF_BIN_FILE__};       // Исключения вызываемые классом

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

    // ================ функции преобразования NetworkInfo в QString и обратно ======================
    static QString netInfoToString(const NetworkInfo &net_info);
    static NetworkInfo stringToNetInfo(const QString &data);
    //===============================================================================================

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
    void makeBusyNode(QStandardItem *node, unsigned int &busy_hosts);                          // сделать подсеть с занятыми хостами
    void makeBusyNodes(const QVector<unsigned int> &vals);                                     // сделать подсети с занятыми хостами
    //=====================================================================================================================

signals:
    void needExpandItem(const QModelIndex&);                                                  // сигнализирует представлению о необходимости
                                                                                              // раскрыть элемент с указанным индексом

    void fileReaded();                                                                        // сигнализирует о том, что файл прочитан
    void makedBusyNodes();                                                                    // сигнал о том, что подсети сделаны
    void notMakedBusyNodes(const QVector<unsigned int> &);                                    // сигнал о том, какие подсети не могут быть сделаны
};

#endif // NETWORKTREEMODEL_H
