#ifndef NETINPUTDIALOG_H
#define NETINPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>


class NetInputDialog: public QDialog
{
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *okBt;
    QPushButton *cancelBt;
public:
    NetInputDialog(const QString &message, const QString &btOkText,
                   const QString &btCancelText, const QString &winTitle,
                   QWidget *parent = 0, Qt::WindowFlags f = 0);

    QString resultInput() const;

    ~NetInputDialog();
};

#endif // NETINPUTDIALOG_H
