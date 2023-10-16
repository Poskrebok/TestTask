#ifndef DIALOGS_H
#define DIALOGS_H

#include <QObject>
#include <QDialog>

#include "QFormLayout"
#include "QHBoxLayout"
#include "qdatetimeedit.h"
#include "qlineedit.h"
#include "common.h"


class MemberDialog: public QDialog
{
    Q_OBJECT
public:
    MemberDialog(QWidget *parent = 0);
    ~MemberDialog();

    void setData(Member member);
    Member getData();
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


#endif // DIALOGS_H
