#ifndef DIALOGS_H
#define DIALOGS_H

#include <QObject>
#include <QDialog>

#include "QFormLayout"
#include "QHBoxLayout"
#include "qdatetimeedit.h"
#include "qlineedit.h"
#include "common.h"
#include <qcombobox.h>
#include <QSpinBox>

class MemberDialog: public QDialog
{
    Q_OBJECT
public:
    MemberDialog(QWidget *parent = 0);
    ~MemberDialog();

    void setMember(Member member);

    Member getMember();
private:
    QLineEdit *fnameEdit;
    QLineEdit *mnameEdit;
    QLineEdit *lnameEdit;
    QDateTimeEdit *dtEdit;
    QLineEdit *genderEdit;
};

class FamilyDialog: public QDialog
{
    Q_OBJECT
public:
    FamilyDialog(QWidget *parent = 0);
    ~FamilyDialog();

    void setData(Family family);
    Family getData();
private:
    QLineEdit *name;
};

class IncomeOperationDialog:public QDialog
{
    Q_OBJECT
public:
    IncomeOperationDialog(QWidget *parent = nullptr);
    ~IncomeOperationDialog(){}

    int getIncome(){return income->value();}
    QDateTime getDT(){return dtEdit->dateTime();}
    QString getComment(){return comment->text();}
    quint16 getSourceId(){return source;}
private:
    QSpinBox *income;
    QDateTimeEdit *dtEdit;
    QLineEdit *comment;
    quint16 source;
};

class OutcomeOperationDialog:public QDialog
{
    Q_OBJECT
public:
    OutcomeOperationDialog(QWidget *parent = nullptr);
    ~OutcomeOperationDialog(){}

    int getOutcome(){return outcome->value();}
    QDateTime getDT(){return dtEdit->dateTime();}
    QString getComment(){return comment->text();}
    quint16 getGoodsId(){return goodsId;}
private:
    QSpinBox *outcome;
    QDateTimeEdit *dtEdit;
    QLineEdit *comment;
    quint16 goodsId;
};



#endif // DIALOGS_H
