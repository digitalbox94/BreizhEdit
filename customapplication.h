/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef CUSTOMAPPLICATION_H
#define CUSTOMAPPLICATION_H

#include <QApplication>

#include "mainwindow.h"

class CustomApplication : public QApplication
{
public:
    CustomApplication(int &argc, char **argv);
    int main(int &argc, char **argv);

    void setMainWindow(MainWindow *window);

protected:
    bool event(QEvent *event) override;

private:
    // Main window
    MainWindow *window;

};

#endif // CUSTOMAPPLICATION_H
