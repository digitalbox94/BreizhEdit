/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

// Qt
#include "QString"
#include "QPushButton"

// BreizhEdit
#include "documentationdialog.h"

/**
 * @brief DocumentationDialog::DocumentationDialog
 * @param parent
 */
DocumentationDialog::DocumentationDialog(QDialog *parent) :
    QDialog(parent)
{
    setupUi(this);

    // Define default button
    pushOk->setAutoDefault(true);
    pushOk->setDefault(true);

}



