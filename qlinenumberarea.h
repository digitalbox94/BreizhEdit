/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef QLINENUMBERAREA_H
#define QLINENUMBERAREA_H

#include <QWidget>

class CustomTextEdit;

/**
 * @brief The QLineNumberArea class
 */
class QLineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit QLineNumberArea(CustomTextEdit *parent = nullptr);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:

    CustomTextEdit* m_editParent;

};

#endif // QLINENUMBERAREA_H

