/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef DOCUMENTATIONDIALOG_H
#define DOCUMENTATIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>

#include "ui_docdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UIDocDialog; }
QT_END_NAMESPACE

/**
 * @brief The DocumentationDialog class
 */
class DocumentationDialog : public QDialog, public Ui::UIDocDialog
{
    Q_OBJECT
public:
    explicit DocumentationDialog(QDialog *parent = nullptr);
};


#endif // DOCUMENTATIONDIALOG_H



