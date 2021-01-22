/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

// Qt
#include "QString"
#include "QPushButton"

// BreizhEdit
#include "searchdialog.h"

/**
 * @brief SearchDialog::SearchDialog
 * @param parent
 */
SearchDialog::SearchDialog(QDialog *parent) :
    QDialog(parent)
{
    setupUi(this);

    // Define default button
    pushNext->setAutoDefault(true);
    pushNext->setDefault(true);

}

