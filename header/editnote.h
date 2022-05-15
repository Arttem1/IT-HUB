#ifndef EDITNOTE_H
#define EDITNOTE_H

#include <QMainWindow>
#include <QTextEdit>
#include "note.h"
#include <memory>

namespace Ui {
class editNote;
}

class editNote : public QMainWindow
{
    Q_OBJECT

public:
    explicit editNote(Note* theNote, QWidget *parent = nullptr);
    ~editNote();

    QTextEdit* mytxtEdit;

public slots:
    void save();

 signals:
    void NoteCreated(Note*);
    void TheWindowIsClosing();
private:
    Note* myNote;
    Ui::editNote *ui;


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // EDITNOTE_H
