/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/


// BreizhEdit
#include "qlinenumberarea.h"
#include "customtextedit.h"


/**
 * @brief QLineNumberArea::QLineNumberArea
 * @param parent
 */

QLineNumberArea::QLineNumberArea(CustomTextEdit *parent) :
    QWidget(parent)
{
    m_editParent=parent;
}


QSize QLineNumberArea::sizeHint() const
{
    return QSize(m_editParent->lineNumberAreaWidth(), 0);
}


/**
 * @brief QLineNumberArea::paintEvent
 * @param event
 */
void QLineNumberArea::paintEvent(QPaintEvent* event)
{
    m_editParent->lineNumberAreaPaintEvent(event);
}


