#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemView>
#include "networktreemodel.h"
#include "networktreeview.h"
#include "netinputdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "rfcs_stds.h"
#include "netconfirmdialog.h"
#include <QDebug>


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

    void confirmMergeNetwork(const QModelIndex &index);

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
};
#endif // MAINWINDOW_H
