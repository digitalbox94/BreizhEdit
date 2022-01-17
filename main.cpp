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
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    CustomApplication app(argc, argv);
    //QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("DigitalBox");
    QCoreApplication::setApplicationName("BreizhEdit");
    QCoreApplication::setApplicationVersion(EDITOR_VERSION);

    // Define command line options and arguments
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow window;
    app.setMainWindow(&window);

    // Load file if specified
    if (!parser.positionalArguments().isEmpty())
        window.fileLoad(parser.positionalArguments().first());
    window.show();

    return app.exec();
}


