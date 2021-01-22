/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QDialogButtonBox>

#include "ui_about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UIAbout; }
QT_END_NAMESPACE

/**
 * @brief The AboutDialog class
 */
class AboutDialog : public QDialog, public Ui::UIAbout
{
    Q_OBJECT
public:
    explicit AboutDialog(QDialog *parent = nullptr);
};


#endif // ABOUTDIALOG_H

