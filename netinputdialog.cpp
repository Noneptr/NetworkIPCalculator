#include "netinputdialog.h"

NetInputDialog::NetInputDialog(const QString &message, const QString &btOkText,
                               const QString &btCancelText, const QString &winTitle,
                               QWidget *parent, Qt::WindowFlags f)
    :QDialog(parent, f)
{
    QFont font_label = QFont("MS Shell Dlg 2", 10, 10);
    QFont font_edit_line = QFont("MS Shell Dlg 2", 10, 9);
    QFont font_bts = QFont("MS Shell Dlg 2", 10, 7);


    label = new QLabel(message, this);
    label->setFont(font_label);

    lineEdit = new QLineEdit(this);
    label->setBuddy(lineEdit);
    lineEdit->setFont(font_edit_line);

    okBt = new QPushButton(btOkText);
    okBt->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    okBt->setFont(font_bts);

    cancelBt = new QPushButton(btCancelText);
    cancelBt->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    cancelBt->setFont(font_bts);

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
