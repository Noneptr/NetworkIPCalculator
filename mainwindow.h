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
    void setBusyNode(const QModelIndex index);                 // установить значение занятости в узле

    void on_action_create_triggered();

private:
    Ui::MainWindow *ui;
    NetworkTreeModel * model;
    QString color_error = "color: rgba(178, 34, 34);";
    QString color_message = "color: rgba(0, 0, 0);";

    //=========== Согласно RFC 5737 приведенные адреса зарезервированы для документации =========================
    QVector <IPrecord> RFC_5737 = {IPrecord(192, 0, 2, 0),
                                  IPrecord(198, 51, 100, 0),
                                  IPrecord(203, 0, 113, 0)};
    NetMask RFC_5737_mask = NetMask(24);
    //===========================================================================================================

    //=========== Согласно RFC 1918 приведенные адреса являются частными =======================================
    // 10.0.0.0        -   10.255.255.255  (10/8 prefix)
    // 172.16.0.0      -   172.31.255.255  (172.16/12 prefix)
    // 192.168.0.0     -   192.168.255.255 (192.168/16 prefix)
    //===========================================================================================================
};
#endif // MAINWINDOW_H
