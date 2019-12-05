#include "netinputdialog.h"

NetInputDialog::NetInputDialog(const QString &message, const QString &btOkText,
                               const QString &btCancelText, const QString &winTitle,
                               QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    label = new QLabel(message, this);
    lineEdit = new QLineEdit(this);
    label->setBuddy(lineEdit);

    okBt = new QPushButton(btOkText);
    okBt->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

    cancelBt = new QPushButton(btCancelText);
    cancelBt->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

    connect(okBt, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelBt, SIGNAL(clicked()), this, SLOT(reject()));

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(label, 0, 0);
    mainLayout->addWidget(lineEdit, 0, 1);
    mainLayout->addWidget(okBt, 1, 0);
    mainLayout->addWidget(cancelBt, 1, 1);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setLayout(mainLayout);

    setWindowTitle(winTitle);

    this->setStyleSheet("QPushButton {"
                        "min-width: 3em;"
                        "max-width: 5em;"
                        "margin: 0.5em 2em 0.5em 2em;"
                        "border-radius: 0.25em;"
                        "background: rgb(65, 105, 225);"
                        "padding: 0.5em 0.75em 0.5em 0.75em;"
                        "}"
                        "QPushButton:hover {background: rgb(147, 112, 219);"
                        "}");
}


QString NetInputDialog::resultInput() const
{
    return lineEdit->text();
}


NetInputDialog::~NetInputDialog()
{
}
