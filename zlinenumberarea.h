#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(QPlainTextEdit *editor) : QWidget(editor) {
        textEditor = editor;
    }

    QSize sizeHint() const override {
        //return QSize(textEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        //textEditor->lineNumberAreaPaintEvent(event);
    }

private:
    QPlainTextEdit *textEditor;
};


#endif // LINENUMBERAREA_H
