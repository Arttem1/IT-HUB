#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QGridLayout>
#include <QSpacerItem>

#include <QWidget>
#include <QFrame>
#include "editnote.h"
#include "note.h"
#include <algorithm>

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myPushButton = new QPushButton();
    myPushButton->setText("Add Note");
    ui->verticalLayout->addWidget(myPushButton);

    connect(myPushButton, SIGNAL (released()), this, SLOT (AddNote()));

    myScrollArea= new QScrollArea();
    ui->verticalLayout->addWidget(myScrollArea);
    myScrollArea->setWidgetResizable(true);

    myScrollAreaWidget = new QWidget(myScrollArea);
    myScrollArea->setWidget(myScrollAreaWidget);

    myScrollAreaVBoxLayout= new QVBoxLayout();
    myScrollAreaWidget->setLayout(myScrollAreaVBoxLayout);


    myScrollAreaGridLayout = new QGridLayout();
    myScrollAreaVBoxLayout->addLayout(myScrollAreaGridLayout);
    myScrollAreaGridLayout->setSpacing(20);

    myScrollAreaVBoxLayoutSpacer = new QSpacerItem(1,1,QSizePolicy::Minimum,QSizePolicy::Expanding);
    myScrollAreaVBoxLayout->addSpacerItem(myScrollAreaVBoxLayoutSpacer);

    ReadFromFile();
    ShowNotes();
}

void MainWindow::ShowNotes()
{
    for (auto aNote : myVectorNote)
    {
        myScrollAreaGridLayout->addWidget(aNote, aNote->Id()/2 , aNote->Id()%2);
        aNote->setMinimumHeight(100);
        aNote->setMinimumWidth(100);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddNote(Note* theNote)
{
    if (!myShowWidget)
        myShowWidget = new editNote(theNote, this);
    myShowWidget->show();
}

void MainWindow::Update(Note* theNote)
{
    auto isFounded = std::find_if(myVectorNote.begin(),myVectorNote.end(),[theNote](Note* aNote){
        return theNote->Id() == aNote->Id();
    });
    if (isFounded == myVectorNote.end())
    {
        myVectorNote.push_back(theNote);
        SaveToFile(theNote);
    }
    else
    {
        UpdateNoteInFile((*isFounded));
    }

    ReadFromFile();
    ShowNotes();
}

QString GetQstringFromFile()
{
    QFile anInFile( "Notes.txt" );
    QString  aString;
    if( anInFile.open( QIODevice::ReadOnly ) ) {
        QTextStream stream( &anInFile );
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        aString =  stream.readAll();
        anInFile.close();
     }
    return aString;
}

void MainWindow::UpdateNoteInFile(Note* theNote)
{
    QString aString = GetQstringFromFile();
    int aStartIndex = 0;
    int aEndIndex   = 0;
    QString aSeparator = "----------";
    if (theNote->Id() != 0)
    {
        for(int i = 0; i < theNote->Id(); ++i )
        {
            aStartIndex = aString.indexOf(aSeparator, aStartIndex) + aSeparator.length() + 2;
        }
     }
    QString aText = theNote->Text() + "\r\n";
    aEndIndex = aString.indexOf(aSeparator, aStartIndex);
    aString.replace(aStartIndex,aEndIndex - aStartIndex, aText);
    UpdateFile(&aString);

}

void MainWindow::ClearMyShowWidget()
{
    delete myShowWidget;
    myShowWidget = nullptr;
}

void MainWindow::DeleteNote(Note* theNote)
{
    auto isFounded = std::find_if(myVectorNote.begin(),myVectorNote.end(),[theNote](Note* aNote){
        return theNote->Id() == aNote->Id();
    });

    if (isFounded == myVectorNote.end())
        return;

    std::vector<Note*> aTempVect;

    bool aDeleted = false;

    for (auto aNote : myVectorNote)
    {
        if (aNote->Id() == theNote->Id())
        {
            aDeleted = true;
            continue;
        }
        if (aDeleted)
            aNote->setId(aNote->Id() - 1);

        aTempVect.push_back(aNote);
    }
    myVectorNote[theNote->Id()]->~Note();
    myVectorNote = aTempVect;
    ShowNotes();
    DeleteFromFile(theNote->Id());
}

void MainWindow::SaveToFile(Note* theNote)
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );

    QString aMyText = theNote->Text().replace("-", "/-").replace("\\", "/\\");
    QFile anOutFile( "Notes.txt");


    if( anOutFile.open( QIODevice::Append | QIODevice::Text) ) {
        QTextStream stream( &anOutFile );

        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        const char* aText = aMyText.toStdString().c_str();
        stream <<  tr(aText);
        stream <<  "\n";
        stream << "----------";
        stream <<  "\n";
        anOutFile.close();
    }
}


void MainWindow::UpdateFile(QString* theString)
{
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
    QFile anOutFile( "Notes.txt");
    if( anOutFile.open( QIODevice::WriteOnly | QIODevice::Text) ) {
        QTextStream stream( &anOutFile );
        stream.setCodec(QTextCodec::codecForName("UTF-8"));

        theString->replace("\r\n", "\n");
        stream << *theString;
    }
}

void MainWindow::DeleteFromFile(const int theId)
{
    QString aString = GetQstringFromFile();
    int aStartIndex = 0;
    int aEndIndex   = 0;
    QString aSeparator = "----------";
    if (theId != 0)
    {
        for(int i = 0; i < theId; ++i )
        {
            aStartIndex = aString.indexOf(aSeparator, aStartIndex) + aSeparator.length() + 2;
        }
     }

    aEndIndex = aString.indexOf(aSeparator, aStartIndex) + aSeparator.length() + 2;
    aString.replace(aStartIndex,aEndIndex - aStartIndex, "");
    UpdateFile(&aString);

}

void MainWindow::ReadFromFile()
{

    for (int i = myVectorNote.size() - 1; i >= 0; --i)
    {
       delete myVectorNote[i];
       myVectorNote.pop_back();
    }

    QString aString = GetQstringFromFile();

    QStringList aListString = aString.split("\r\n----------\r\n");

    if (aListString.size() == 1)
    {
        Note* aNote = new Note(this);
        aNote->setId(Note::NoteSize);
        aNote->setText("4 Требование. "
                       "При первом запуске, приложение должно иметь одну заметку с текстом");
        myVectorNote.push_back(aNote);
        SaveToFile(aNote);
    }
    else
    {
        for (int i = 0; i< aListString.size() - 1; ++i)
        {
            //aListString[i].replace("\r\n", "");
            aListString[i].replace("/-", "-").replace("/\\", "\\");
            Note* aNote = new Note(this);
            aNote->SetAllFields(aListString[i]);
            myVectorNote.push_back(aNote);

        }

    }
}
