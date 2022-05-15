#ifndef NOTE_H
#define NOTE_H

#include <QWidget>
#include <QMargins>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class Note : public QWidget
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = nullptr);
    static int NoteSize;
    ~Note();
private:
    QString myText;
    QColor  myBackGroundColor;
    int myId =-1;
    QPushButton* myDeleteButton;
    QVBoxLayout* myVBoxLayout;
    QLabel* myQLabel;
public:
    int Id() const {return myId;}
    void setId(const int& theid) {myId = theid;}
    void setText(const QString& theQString);
    QString Text() const {return myText;}
    void SetAllFields(const QString&);
signals:
    void TextChanged();
    void Clicked(Note* );
    void DeleteThisNote(Note*);
private slots:
    void DeleteNote();


    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

};



#endif // NOTE_H
