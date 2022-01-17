/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

// Qt
#include <QtWidgets>

// BreizhEdit
#include "customtextedit.h"
#include "mainwindow.h"

/**
 * @brief CustomTextEdit::CustomTextEdit
 * @param parent
 */
CustomTextEdit::CustomTextEdit(QWidget *parent) : QPlainTextEdit(parent),
    m_lineNumberArea(new QLineNumberArea(this))
{
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    connect(this, &CustomTextEdit::updateRequest, this, &CustomTextEdit::updateLineNumberArea);
    connect(document(), &QTextDocument::blockCountChanged, this, &CustomTextEdit::updateLineNumberAreaWidth);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, [this](int){ m_lineNumberArea->update();});

    highlightCurrentLine();

    updateLineNumberAreaWidth();
}




/**
 * @brief CustomTextEdit::highlightCurrentLine
 *
 * Highlight the current line of text
 */
void CustomTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(QColor(50, 100, 150, 255)).lighter(80);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


/**
 * @brief CustomTextEdit::wheelEvent
 * @param inEvent wheel event
 *
 * Override the default wheelEvent event to enable CTRL-Scroll
 * functionality to increase/decrease the font size.
 */
void CustomTextEdit::wheelEvent(QWheelEvent *inEvent)
{
    static int MIN_FONT_SIZE = 6;
    static short SCROLL_INTERVAL = 1;

    // Check to see if the Ctrl key is pressed
    Qt::KeyboardModifiers result = QApplication::queryKeyboardModifiers();
    if ((result & Qt::ControlModifier) == Qt::ControlModifier) {

        // Get the current font
        QFont font = QFont(this->font());

        // Are we scrolling up or down?
        short direction = (inEvent->angleDelta().y() > 0 ? SCROLL_INTERVAL : -1 * SCROLL_INTERVAL);

        // Set the new font size
        int new_size = font.pointSize() + direction;
        new_size = new_size >= MIN_FONT_SIZE ? new_size : MIN_FONT_SIZE;
        font.setPointSize(new_size);
        this->setFont(font);

        // Don't actually scroll the window
        inEvent->ignore();
    }
    else
    {
        // Handle event normally
        QPlainTextEdit::wheelEvent(inEvent);
    }


}


/**
 * @brief CustomTextEdit::lineNumberAreaWidth
 * @return
 *
 * Get the LineNumber area widget pixel's width
 */
int CustomTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * (digits + 2);

    return space;
}


/**
 * @brief CustomTextEdit::updateLineNumberAreaWidth
 *
 * Sets the margins around the scrolling area to left, top, right and bottom.
 */
void CustomTextEdit::updateLineNumberAreaWidth()
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


/**
 * @brief CustomTextEdit::updateLineNumberArea
 * @param rect
 * @param dy
 *
 * Update portion of LineNumber area widget when needed
 */
void CustomTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        m_lineNumberArea->scroll(0, dy);
    else
        m_lineNumberArea->update(0, rect.y(), m_lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}






/**
 * @brief CustomTextEdit::lineNumberAreaPaintEvent
 * @param event
 *
 * Paint the LineNumber area widget
 */
void CustomTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::black);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    int offset = fontMetrics().horizontalAdvance(QLatin1Char('9'));

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0-offset, top, m_lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


/**
 * @brief CustomTextEdit::resizeEvent
 * @param e
 *
 * Resize the PlainTextEdit widget and adjust the corresponding LineNumber area widget
 */
void CustomTextEdit::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));

}




