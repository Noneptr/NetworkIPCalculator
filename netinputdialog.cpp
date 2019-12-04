#include "netinputdialog.h"

NetInputDialog::NetInputDialog(const QString &message, const QString &btOkText,
                               const QString &btCancelText,
                               QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    QBoxLayout *hlayout = new QHBoxLayout(this);
    QBoxLayout *vlayout = new QVBoxLayout(this);
    __edit = new QLineEdit(this);
    QLabel *label = new QLabel(message, this);
    QPushButton *btOk = new QPushButton(btOkText, this);
    connect(btOk, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *btCancel = new QPushButton(btCancelText, this);
    connect(btCancel, SIGNAL(clicked()), this, SLOT(reject()));
    hlayout->addWidget(label);
    hlayout->addWidget(__edit);
    hlayout->addWidget(btOk);
    hlayout->addWidget(btCancel);
//    vlayout->addWidget(label);
//    vlayout->addWidget(__edit);
//    vlayout->addLayout(hlayout);

    this->setLayout(hlayout);
}


QString NetInputDialog::resultInput() const
{
    return __edit->text();
}


NetInputDialog::~NetInputDialog()
{
}
