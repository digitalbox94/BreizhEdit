/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>

#include "ui_searchdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UISearchDialog; }
QT_END_NAMESPACE

/**
 * @brief The SearchDialog class
 *
 * Inherits from QDialog and UISearchDialog
 */
class SearchDialog : public QDialog, public Ui::UISearchDialog
{
    Q_OBJECT
public:
    explicit SearchDialog(QDialog *parent = nullptr);

};

#endif // SEARCHDIALOG_H
