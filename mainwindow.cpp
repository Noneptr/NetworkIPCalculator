#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new NetworkTreeModel(this);

    connect(ui->treeView, SIGNAL(expanded(const QModelIndex &)), model, SLOT(splitNetworkItem(const QModelIndex &)));
    connect(ui->treeView, SIGNAL(collapsed(const QModelIndex &)), model, SLOT(mergeNetworkItem(const QModelIndex &)));
    connect(model, SIGNAL(needExpandItem(const QModelIndex &)), ui->treeView, SLOT(expand(const QModelIndex &)));
    connect(ui->treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(setBusyNode(const QModelIndex &)));
    connect(model, SIGNAL(notMakedBusyNodes(const QVector<unsigned int> &)), this, SLOT(displayStatusMakedBusyNodes(const QVector<unsigned int> &)));
    connect(model, &NetworkTreeModel::fileWrited, [&]{ui->statusbar->setStyleSheet(color_message);
                                                      ui->statusbar->showMessage("Файл успешно сохранён...", 3000);});
    connect(model, &NetworkTreeModel::fileReaded, [&]{ui->statusbar->setStyleSheet(color_message);
                                                      ui->statusbar->showMessage("Файл успешно открыт...", 3000);});
//    connect(model, SIGNAL(searchIsActive()), this, SLOT(displayStatusSearch()));
//    connect(model, SIGNAL(fileReadActive()), this, SLOT(displayStatusFileReadActive()));
//    connect(model, SIGNAL(fileWriteActive()), this, SLOT(displayStatusFileWriteActive()));
//    connect(model, SIGNAL(expandAllExistActive()), this, SLOT(displayStatusModelExpanded()));

    ui->treeView->setModel(model);
    ui->treeView->setIndentation(75);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);           // запрет на редактирование содержимого узлов дерева
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setBusyNode(const QModelIndex &index)
{
    NetInputDialog dialog("Введите значение занятых хостов: ", "Изменить", "Отмена", "Занять\\Освободить сеть", this);

    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            unsigned int count = dialog.resultInput().toUInt();
            model->userMakeBusyNode(index, count);
            ui->treeView->collapse(index);
        }
        catch (NetworkTreeModelError &error)
        {
            if (error == __ERROR_USER_MAKE_BUSY_NODE__)
            {
                ui->statusbar->setStyleSheet(color_error);
                ui->statusbar->showMessage("Ошибка!!! Введено хостов больше, чем имеется в подсети!!!", 2500);
            }
        }
    }
}

void MainWindow::on_action_create_triggered()
{
    NetInputDialog dialog("Введите адрес сети с маской: ", "Создать", "Отмена", "Создать новую сеть", this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString data = dialog.resultInput();
        if (data.indexOf("/") != -1)
        {
            QStringList list = data.split("/");
            try
            {
                IPrecord adress(list[0]);
                unsigned short bits = list[1].toUShort();
                try
                {
                    if (bits > 0 && bits <= 30)
                    {
                        NetMask mask(bits);

                        IPrecord net_address = adress & mask.mask();

                        if (__check_rfc__::check_rfc5737(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC5737 192.0.2.0/24, 198.51.100.0/24, 203.0.113.0/24 зарезервированы!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc1918(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC1918 10.0.0.0/8, 172.16.0.0/12,  192.168.0.0/16 зарезервированы!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc3068(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC3068 192.88.99.0/24 зарезервирован!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc3927(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC3927 169.254.0.0/16 зарезервирован!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc1122(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC1122 0.0.0.0/8, 127.0.0.0/8, 240.0.0.0/4 зарезервированы!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc2544(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC2544 198.18.0.0/15 зарезервирован!!!", 4000);
                        }
                        else if (__check_rfc__::check_rfc3171(net_address))
                        {
                            ui->statusbar->setStyleSheet(color_error);
                            ui->statusbar->showMessage("Согласно RFC3171 224.0.0.0/4 зарезервирован!!!", 4000);
                        }
                        else
                        {
                            model->createNetworkRoot(adress, mask);
                        }
                    }
                    else
                    {
                        ui->statusbar->setStyleSheet(color_error);
                        ui->statusbar->showMessage("Некорректная маска корня дерева!!! Необходимо: 0 < mask < 31", 2500);
                    }
                }
                catch(NetMaskError &err_mask)
                {
                    if (err_mask == InvalidCountBitsError)
                    {
                        ui->statusbar->setStyleSheet(color_error);
                        ui->statusbar->showMessage("Некорректная маска подсети!!!", 2500);
                    }
                }
            }
            catch(IPrecordError &err_adress)
            {
                ui->statusbar->setStyleSheet(color_error);
                ui->statusbar->showMessage("Некорректный IP адрес!!!", 2500);
            }
        }
        else
        {
            ui->statusbar->setStyleSheet(color_error);
            ui->statusbar->showMessage("Неверно указан префикс маски!!!", 2500);
        }
    }
}

void MainWindow::on_action_search_triggered()
{
    NetInputDialog dialog("Введите данные поиска: ", "Искать", "Отмена", "Поиск элемента", this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString key = dialog.resultInput();
        QVector<QModelIndex> find_indexs;
        try
        {
            find_indexs = model->findNodes(key);
            ui->treeView->selectionModel()->clear();
            if (find_indexs.size() > 0)
            {
                for (const QModelIndex &index: find_indexs)
                {
                    ui->treeView->selectionModel()->select(index, QItemSelectionModel::Select | QItemSelectionModel::Rows);
                }
                ui->statusbar->setStyleSheet(color_message);
                ui->statusbar->showMessage("Поиск завершён...", 2500);
            }
            else
            {
                ui->statusbar->setStyleSheet(color_message);
                ui->statusbar->showMessage("Элемент не найден!!!", 2500);
            }
        }
        catch (NetworkTreeModelError &error)
        {
            if (error == __ERROR_NETWORK_TREE_IS_EMPTY__)
            {
                ui->statusbar->setStyleSheet(color_error);
                ui->statusbar->showMessage("Сетевое дерево не создано!!!", 2500);
            }
        }
    }
}


void MainWindow::displayStatusSearch()
{
    ui->statusbar->setStyleSheet(color_message);
    ui->statusbar->showMessage("Поиск...", 1000);
}


void MainWindow::displayStatusMakedBusyNodes(const QVector<unsigned int> &vals)
{
    QString text = "Подсети со следующими размерами не могут быть выделены!!! ( ";
    for (int i = 0; i < vals.size(); i++)
    {
        text += QString::number(vals[i]);
        if (i != vals.size() - 1)
        {
            text += ", ";
        }
    }
    text += " )";
    ui->statusbar->setStyleSheet(color_error);
    ui->statusbar->showMessage(text, 3000);
}


void MainWindow::displayStatusFileReadActive()
{
    ui->statusbar->setStyleSheet(color_message);
    ui->statusbar->showMessage("Открытие...", 1000);
}

void MainWindow::displayStatusFileWriteActive()
{
    ui->statusbar->setStyleSheet(color_message);
    ui->statusbar->showMessage("Сохранение...", 1000);
}


void MainWindow::displayStatusModelExpanded()
{
    ui->statusbar->setStyleSheet(color_message);
    ui->statusbar->showMessage("Идёт обработка...", 1000);
}


void MainWindow::on_action_clear_tree_triggered()
{
    model->clear();
}

void MainWindow::on_action_split_triggered()
{
    NetInputDialog dialog("Введите размеры подсетей(через пробел): ", "Разделить", "Отмена", "Разделить на подсети", this);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString data = dialog.resultInput();
        QVector <unsigned int> v;
        if (data.indexOf(" ") != -1)
        {
            QStringList list = data.split(" ");
            for (QString &e: list)
            {
                v.append(e.toUInt());
            }
        }
        else
        {
            v.append(data.toUInt());
        }

        if (v.indexOf(0) == -1)
        {
            try
            {
                model->makeBusyNodes(v);
                if (ui->statusbar->currentMessage() == "")
                {
                    ui->statusbar->setStyleSheet(color_message);
                    ui->statusbar->showMessage("Сеть успешно разбита", 2500);
                }
            }
            catch(NetworkTreeModelError &error)
            {
                if (error == __ERROR_NETWORK_TREE_IS_EMPTY__)
                {
                    ui->statusbar->setStyleSheet(color_error);
                    ui->statusbar->showMessage("Сетевое дерево не создано!!!", 2500);
                }
            }
        }
        else
        {
            ui->statusbar->setStyleSheet(color_error);
            ui->statusbar->showMessage("Некорректное значение размера подсети!!!", 2500);
        }
    }
}

void MainWindow::on_action_exit_triggered()
{
    this->close();
}

void MainWindow::on_action_open_triggered()
{
    QString extention_file = "(*" + __file_extention__ +")";
    QString filenameO = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "/home/",
                                                     tr(extention_file.toStdString().c_str()));
    model->setFilename(filenameO);
    if (model->filename() != "")
    {
        try
        {
            model->readNetworkOfFile();
        }
        catch (NetworkTreeModelError &error)
        {
            if (error == __ERROR_READ_OF_BIN_FILE__)
            {
                ui->statusbar->setStyleSheet(color_error);
                ui->statusbar->showMessage("Ошибка открытия файла!!!", 2500);
            }
        }
    }
}

void MainWindow::on_action_help_triggered()
{
    QMessageBox::information(this, "Справка",\
    "IP калькулятор - инструмент, помощник для проектирования сетей на сетевом уровне.",\
                                  QMessageBox::Ok);
}

void MainWindow::on_action_save_triggered()
{
    QString extention_file = "(*" + __file_extention__ +")";
    QString filenameS = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),\
                       "/home/my_project", tr(extention_file.toStdString().c_str()));

    model->setFilename(filenameS);

    if (model->filename() != "")
    {
        try
        {
            model->writeNetworkInFile();
        }
        catch (NetworkTreeModelError &error)
        {
            if (error == __ERROR_WRITE_IN_BIN_FILE__)
            {
                ui->statusbar->setStyleSheet(color_error);
                ui->statusbar->showMessage("Ошибка сохранения файла!!!", 2500);
            }
        }
    }
}
