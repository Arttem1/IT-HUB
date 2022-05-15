#include "note.h"
#include<QMouseEvent>
#include<QPainter>
#include "mainwindow.h"
#include <QLabel>



int Note::NoteSize = -1;

Note::Note(QWidget *parent) : QWidget(parent)
{
    myText = "Text";
    myBackGroundColor = Qt::white;
    setFixedSize(200,100);

    myVBoxLayout= new QVBoxLayout();
    this->setLayout(myVBoxLayout);

    myDeleteButton = new QPushButton();
    myDeleteButton->setText("X");
    myDeleteButton->setMaximumHeight(10);
    myDeleteButton->setMaximumWidth(10);
    myDeleteButton->resize(10,10);

    myDeleteButton->setStyleSheet("QPushButton {background-color: red; color: black;}");
    //#A3C1DA

    myVBoxLayout->addWidget(myDeleteButton, Qt::AlignAbsolute);

    myQLabel = new QLabel(myText);
    myQLabel->setAlignment(Qt::AlignHCenter);
    myVBoxLayout->addWidget(myQLabel);
    myQLabel->setWordWrap(true);

    QSpacerItem* VBoxLayoutSpacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);
    myVBoxLayout->addSpacerItem(VBoxLayoutSpacer);

     connect(myDeleteButton, SIGNAL (released()), this, SLOT(DeleteNote()));
     connect(this, SIGNAL (DeleteThisNote(Note* )), parent, SLOT (DeleteNote(Note* )));

    connect(this, SIGNAL (Clicked(Note*)), parent, SLOT (AddNote(Note* )));

    Note::NoteSize++;
}

void Note::setText(const QString& theQString)
{
    myText = theQString;
    emit TextChanged();
}

void Note::DeleteNote()
{
    emit DeleteThisNote(this);
}


void Note::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit Clicked(this);
}
void Note::paintEvent(QPaintEvent *event)
{
    QPainter aPainter(this);
    aPainter.setRenderHint(QPainter::Antialiasing);
    //aPainter.save();

    aPainter.setBrush(myBackGroundColor);
    aPainter.drawRect(rect());

    if (myText.length() > 70)
    {
        QString aString = myText.mid(0, 70) + "...";
        myQLabel->setText(aString);
    }
    else
    {
        myQLabel->setText(myText);
    }

}


void Note::SetAllFields(const QString& theString)
{
    myId = Note::NoteSize;
    myText = theString;
}

Note::~Note(){
    delete myDeleteButton;
    myDeleteButton = nullptr;
    NoteSize--;
}
