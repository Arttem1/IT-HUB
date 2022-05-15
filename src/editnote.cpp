#include "editnote.h"
#include "ui_editnote.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QDebug>

editNote::editNote(Note* theNote, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::editNote)
{
    connect(this, SIGNAL(NoteCreated(Note*)), parent, SLOT(Update(Note*)));
    ui->setupUi(this);

    if (theNote != nullptr)
    {
        myNote = theNote;
        mytxtEdit = new QTextEdit();
        mytxtEdit->setText(myNote->Text());
    }
    else
    {
        myNote = new Note(parent);
        mytxtEdit = new QTextEdit();
        myNote->setId(Note::NoteSize);
    }

    ui->verticalLayout->addWidget(mytxtEdit);

    QHBoxLayout* hBoxLt1= new QHBoxLayout();
    ui->verticalLayout->addLayout(hBoxLt1);

    QPushButton* pushB1 = new QPushButton();
    pushB1->setText("Cancel");
    hBoxLt1->addWidget(pushB1);
    connect(pushB1, SIGNAL (released()), this, SLOT (close()));

    connect(this, SIGNAL (TheWindowIsClosing()), parent, SLOT (ClearMyShowWidget()));

    QPushButton* pushB2 = new QPushButton();
    pushB2->setText("Save");
    hBoxLt1->addWidget(pushB2);
    connect(pushB2, SIGNAL (released()), this, SLOT (save()));
}

editNote::~editNote()
{
    delete ui;
}

void editNote::save()
{
   myNote->setText(mytxtEdit->toPlainText());
   emit NoteCreated(myNote);
   this->close();
}

void editNote::closeEvent(QCloseEvent *event)
{
    if (myNote->Id() == -1)
        delete myNote;

    emit TheWindowIsClosing();
    event->accept();
}
