#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>
#include "iprecord.h"
#include "netmask.h"
#include <QAbstractItemView>
#include "networkinfo.h"
#include "networktreemodel.h"
#include "networktreeview.h"
#include <queue>
#include "netinputdialog.h"
#include <QFileDialog>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setBusyNode(const QModelIndex &index);                 // установить значение занятости в узле

    void displayStatusSearch();

    void displayStatusMakedBusyNodes(const QVector<unsigned int> &vals);

    void displayStatusFileReadActive();

    void displayStatusFileWriteActive();

    void displayStatusModelExpanded();

    void on_action_create_triggered();

    void on_action_search_triggered();

    void on_action_clear_tree_triggered();

    void on_action_split_triggered();

    void on_action_exit_triggered();

    void on_action_open_triggered();

    void on_action_help_triggered();

    void on_action_save_triggered();

private:
    Ui::MainWindow *ui;
    NetworkTreeModel * model;
    QString color_error = "color: rgba(178, 34, 34);";
    QString color_message = "color: rgba(0, 0, 0);";

    //=========== Согласно RFC 5737 приведенные адреса зарезервированы для документации =========================
    /* Три блока адресов IPv4, предназначенных для использования в документации.
       Сетевым операторам следует добавить эти блоки адресов в число немаршрутизируемых,
       а при использовании пакетных фильтров следует добавить эти адреса в списки фильтрации.
       Блоки адресов не предназначены для локального использования и фильтры могут
       применяться как в локальном, так и в публичном контексте.

       Блоки адресов для документации:
            192.0.2.0/24 (TEST-NET-1),
            198.51.100.0/24 (TEST-NET-2),
            203.0.113.0/24 (TEST-NET-3)*/
    QVector <IPrecord> RFC_5737 = {IPrecord(192, 0, 2, 0),
                                  IPrecord(198, 51, 100, 0),
                                  IPrecord(203, 0, 113, 0)};
    NetMask RFC_5737_m = NetMask(24);
    //===========================================================================================================

    //=========== Согласно RFC 1918 приведенные адреса являются частными =======================================
    // Блоки частных(серых) адресов:
    // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
    // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
    // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
    QVector<IPrecord> RFC_1918 = {IPrecord(10, 0, 0, 0),
                                  IPrecord(172, 16, 0, 0),
                                  IPrecord(192, 168, 0, 0)};
    QVector<NetMask> RFC_1918_m = {NetMask(8),
                                   NetMask(12),
                                   NetMask(16)};
    //===========================================================================================================

    //=========== Согласно RFC 3068 =======================================
    QVector<IPrecord> RFC_3068 = {IPrecord(10, 0, 0, 0),
                                  IPrecord(172, 16, 0, 0),
                                  IPrecord(192, 168, 0, 0)};
    QVector<NetMask> RFC_3068_m = {NetMask(8),
                                   NetMask(12),
                                   NetMask(16)};
    //===========================================================================================================
};
#endif // MAINWINDOW_H
