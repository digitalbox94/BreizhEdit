/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>

// BreizhEdit
#include "highlighter.h"
#include "searchdialog.h"
#include "jumpdialog.h"
#include "aboutdialog.h"
#include "documentationdialog.h"

#define EDITOR_VERSION "v1.1"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fileLoad(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void filePrint();
    void fileQuit();
    void editSearch();
    void replaceString();
    void findReplaceString();
    void findString();
    void findStringBackward();
    void editJumpTo();
    void findLine();
    void viewSetFont();
    void viewSetView100Percent();
    void viewIncrease();
    void viewDecrease();
    void showAbout();
    void showDocumentation();

    void documentWasModified();
    bool confirmAction();
    void showCursorPosition();

private:
    void additionalSetupUi();
    void writeSettings();
    void readSettings();
    void performConnections();
    void setCurrentFile(const QString &fileName);
    void fileWrite(const QString &fileName);

    // Access to user interface
    Ui::MainWindow *ui;

    // Current filename
    QString currentFilename;

    // Original font size
    int originalFontSize;

    // Text highlighter
    Highlighter *highlighter;

    // Search dialog
    SearchDialog *searchDialog = NULL;

    // Jump dialog
    JumpDialog *jumpDialog = NULL;

    // About dialog
    AboutDialog *aboutDialog = NULL;

    // Documentation dialog
    DocumentationDialog *docDialog = NULL;

    // Last search indicator
    bool lastSearch = false;
};
#endif // MAINWINDOW_H
