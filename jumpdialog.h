/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef JUMPDIALOG_H
#define JUMPDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>

#include "ui_jumpto.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UIJumpDialog; }
QT_END_NAMESPACE

/**
 * @brief The JumpDialog class
 *
 * Inherits from QDialog and UIJumpDialog
 */
class JumpDialog : public QDialog, public Ui::UIJumpDialog
{
    Q_OBJECT
public:
    explicit JumpDialog(QDialog *parent = nullptr);

};

#endif // JUMPDIALOG_H
