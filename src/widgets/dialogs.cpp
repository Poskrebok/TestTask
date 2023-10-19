#include "dialogs.h"
#include "qdialogbuttonbox.h"
#include "qpushbutton.h"

MemberDialog::MemberDialog(QWidget *parent):QDialog(parent)
{
    QFormLayout *layout = new QFormLayout(this);

    fnameEdit = new QLineEdit(this);
    mnameEdit = new QLineEdit(this);
    lnameEdit = new QLineEdit(this);
    dtEdit = new QDateTimeEdit(this);
    genderEdit = new QLineEdit(this);

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    QPushButton *btnAccept = new QPushButton(btnBox);
    btnAccept->setText(QString("Сохранить изменения"));

    QPushButton *btnReject = new QPushButton(btnBox);
    btnReject->setText(QString("Отмена"));

    btnBox->addButton(btnAccept,QDialogButtonBox::AcceptRole);
    btnBox->addButton(btnReject,QDialogButtonBox::RejectRole);

    layout->addRow(QString("Имя"),fnameEdit);
    layout->addRow(QString("Отчество"),mnameEdit);
    layout->addRow(QString("Фамилия"),lnameEdit);
    layout->addRow(QString("Дата/Время"),dtEdit);
    layout->addRow(QString("Пол"),genderEdit);

    layout->addWidget(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted,this,&MemberDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected,this,&MemberDialog::reject);

    this->setLayout(layout);

}

MemberDialog::~MemberDialog()
{

}

void MemberDialog::setMember(Member member)
{
    fnameEdit->setText(member.fname);
    mnameEdit->setText(member.mname);
    lnameEdit->setText(member.lname);
    dtEdit->setDateTime(member.bdate);
    genderEdit->setText(member.gender);
}

Member MemberDialog::getMember()
{
    Member member;
    member.bdate = dtEdit->dateTime();
    member.fname = fnameEdit->text();
    member.lname = lnameEdit->text();
    member.mname = mnameEdit->text();
    member.gender = genderEdit->text();
    return member;
}

FamilyDialog::FamilyDialog(QWidget *parent):QDialog(parent)
{
    QFormLayout *layout = new QFormLayout(this);
    name = new QLineEdit(this);

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    QPushButton *btnAccept = new QPushButton(btnBox);
    btnAccept->setText(QString("Сохранить изменения"));

    QPushButton *btnReject = new QPushButton(btnBox);
    btnReject->setText(QString("Отмена"));

    btnBox->addButton(btnAccept,QDialogButtonBox::AcceptRole);
    btnBox->addButton(btnReject,QDialogButtonBox::RejectRole);

    connect(btnBox, &QDialogButtonBox::accepted,this,&MemberDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected,this,&MemberDialog::reject);

    layout->addRow(QString("Название семьи"),name);
    layout->addWidget(btnBox);
    this->setLayout(layout);
}

FamilyDialog::~FamilyDialog()
{

}

void FamilyDialog::setData(Family family)
{
    name->setText(family.name);
}

Family FamilyDialog::getData()
{
    Family family;
    family.name = name->text();
    return family;
}

OperationDialog::OperationDialog(QWidget *parent)
{

}

OperationDialog::~OperationDialog()
{

}
