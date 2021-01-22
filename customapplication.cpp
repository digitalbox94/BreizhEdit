/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

// Qt
#include <QtWidgets>

// BreizhEdit
#include "customapplication.h"
#include "mainwindow.h"

/**
 * @brief CustomApplication::CustomApplication
 * @param argc
 * @param argv
 */
CustomApplication::CustomApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{

}



/**
 * @brief CustomApplication::event
 * @param event
 * @return
 */
bool CustomApplication::event(QEvent *event)
{
    // Open with
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
        window->fileLoad(openEvent->file());
    }

    return QApplication::event(event);
}

/**
 * @brief setMainWindow
 * @param currentWindow
 */
void CustomApplication::setMainWindow(MainWindow *currentWindow)
{
    window=currentWindow;
}

