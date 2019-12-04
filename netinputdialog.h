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
    QLineEdit *__edit;
public:
    NetInputDialog(const QString &message, const QString &btOkText, const QString &btCancelText,
                   QWidget *parent = 0, Qt::WindowFlags f = 0);

    QString resultInput() const;

    ~NetInputDialog();
};

#endif // NETINPUTDIALOG_H
