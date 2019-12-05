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

private:
    Ui::MainWindow *ui;
    NetworkTreeModel * model;
};
#endif // MAINWINDOW_H
