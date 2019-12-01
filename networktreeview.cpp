#include "networktreeview.h"

NetworkTreeView::NetworkTreeView(QWidget *parent)
    :QTreeView(parent)
{
    connect(this, SIGNAL(expanded(const QModelIndex &)), this, SLOT(alignItemsInTreeView()));
    connect(this, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(alignItemsInTreeView()));
}


NetworkTreeView::~NetworkTreeView()
{
}


void NetworkTreeView::alignItemsInTreeView()
{
    resizeColumnToContents(0);
}
