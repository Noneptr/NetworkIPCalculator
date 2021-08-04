#ifndef NETCONFIRMDIALOG_H
#define NETCONFIRMDIALOG_H


#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>



class NetConfirmDialog: public QDialog
{
private:
    QLabel *label;
    QPushButton *okBt;
    QPushButton *cancelBt;
public:
    NetConfirmDialog(const QString &message, const QString &btOkText,
                   const QString &btCancelText, const QString &winTitle,
                   QWidget *parent = 0, Qt::WindowFlags f = 0);

    ~NetConfirmDialog();
};

#endif // NETCONFIRMDIALOG_H
