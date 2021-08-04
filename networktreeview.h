#ifndef NETWORKTREEVIEW_H
#define NETWORKTREEVIEW_H


#include <QTreeView>
#include "netconfirmdialog.h"

class NetworkTreeView: public QTreeView
{
    Q_OBJECT
public:
    NetworkTreeView(QWidget *parent = nullptr);

    ~NetworkTreeView();

public slots:
    void alignItemsInTreeView();                            // выровнять элементы дерева
};

#endif // NETWORKTREEVIEW_H
