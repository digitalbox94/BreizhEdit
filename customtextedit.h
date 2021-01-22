/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

// Qt
#include <QPlainTextEdit>
#include <QWidget>

// BreizhEdit
#include "qlinenumberarea.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE


/**
 * @brief The CustomTextEdit class
 */

class CustomTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    CustomTextEdit(QWidget *parent = 0);


public Q_SLOTS:
    /**
     * @brief lineNumberAreaWidth
     * Get the LineNumber area widget pixel's width
     * @return
     */
    int lineNumberAreaWidth();

    /**
     * @brief updateLineNumberAreaWidth
     * Sets the margins around the scrolling area to left, top, right and bottom
     */
    void updateLineNumberAreaWidth();

    /**
     * @brief updateLineNumberArea
     * Update portion of LineNumber area widget when needed
     * @param rect Area that has to be updated.
     */
    void updateLineNumberArea(const QRect& rect, int dy);


    /**
     * @brief lineNumberAreaPaintEvent
     * Paint the LineNumber area widget
     * @param event
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);


protected:
    void wheelEvent(QWheelEvent *inEvent) override;

    /**
     * @brief resizeEvent
     * Resize the PlainTextEdit widget and adjust the corresponding LineNumber area widget
     */
    void resizeEvent(QResizeEvent* e) override;


private slots:
    void highlightCurrentLine();

private:
    // Widget representing the linenumber
    QLineNumberArea* m_lineNumberArea;
};


#endif // CUSTOMTEXTEDIT_H
