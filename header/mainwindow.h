#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

#include "note.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ShowNotes();

public slots:
    void AddNote(Note* theNote = nullptr);
    void Update(Note*);
    void ClearMyShowWidget();
    void DeleteNote(Note* );

private:
    void SaveToFile(Note*);
    void ReadFromFile();
    void DeleteFromFile(const int);
    void UpdateFile(QString*);
    void UpdateNoteInFile(Note* );
private:
    Ui::MainWindow *ui;
    std::vector<Note*> myVectorNote;
    QWidget* myShowWidget = nullptr;

    QScrollArea* myScrollArea;
    QWidget*     myScrollAreaWidget;
    QVBoxLayout* myScrollAreaVBoxLayout;
    QSpacerItem* myScrollAreaVBoxLayoutSpacer;
    QGridLayout* myScrollAreaGridLayout;
    QPushButton* myPushButton;

};
#endif // MAINWINDOW_H
