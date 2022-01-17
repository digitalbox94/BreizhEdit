/* Copyright (c) 2020-2021 DigitalBox

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License, version 2.0,
   as published by the Free Software Foundation.

*/

// Qt
#include <QtWidgets>
#include <QTextStream>
#include <QSettings>
#include <QPrinter>
#include <QPrintDialog>

// BreizhEdit
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Build UI from ui file
    ui->setupUi(this);

    // Build additional UI through code
    additionalSetupUi();
    setCentralWidget(ui->textEdit);

    // Get settings
    readSettings();

    // Define hightlighter
    highlighter = new Highlighter(ui->textEdit->document());

    // Important to handle full screen refresh on Mac !
    setUnifiedTitleAndToolBarOnMac(true);

    // Perform Connections
    performConnections();

    // Default filename
    setCurrentFile(QString());

}


MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Only proceed if user agrees
    if (confirmAction())
    {
        // Save application's settings
        writeSettings();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}


/**
 * @brief MainWindow::performConnections
 *
 * Perform the connections of all events (signals) to Qt Objects/Methods
 */
void MainWindow::performConnections()
{
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::fileNew);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::fileOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::fileSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::fileSaveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::filePrint);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::fileQuit);

    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::editSearch);
    connect(ui->actionJump_to_Line, &QAction::triggered, this, &MainWindow::editJumpTo);
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::editSearch);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &CustomTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &CustomTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &CustomTextEdit::paste);
    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &CustomTextEdit::undo);

    connect(ui->actionSelectFont, &QAction::triggered, this, &MainWindow::viewSetFont);
    connect(ui->actionView100, &QAction::triggered, this, &MainWindow::viewSetView100Percent);
    connect(ui->actionIncreaseView, &QAction::triggered, this, &MainWindow::viewIncrease);
    connect(ui->actionDecreaseView, &QAction::triggered, this, &MainWindow::viewDecrease);

    connect(ui->actionProjectInfo, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionQuickHelp, &QAction::triggered, this, &MainWindow::showDocumentation);

    connect(ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(showCursorPosition()));

    connect(searchDialog->pushCancel, SIGNAL(clicked()), searchDialog, SLOT(hide()));
    connect(searchDialog->pushReplace, SIGNAL(clicked()), this, SLOT(replaceString()));
    connect(searchDialog->pushFindReplace, SIGNAL(clicked()), this, SLOT(findReplaceString()));
    connect(searchDialog->pushNext, SIGNAL(clicked()), this, SLOT(findString()));
    connect(searchDialog->pushPrevious, SIGNAL(clicked()), this, SLOT(findStringBackward()));

    connect(jumpDialog->pushCancel, SIGNAL(clicked()), jumpDialog, SLOT(hide()));
    connect(jumpDialog->pushOk, SIGNAL(clicked()), this, SLOT(findLine()));

    connect(aboutDialog->pushOk, SIGNAL(clicked()), aboutDialog, SLOT(hide()));

    connect(docDialog->pushOk, SIGNAL(clicked()), docDialog, SLOT(hide()));

    connect(ui->textEdit->document(), &QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
}


/**
 * @brief MainWindow::additionalSetupUi
 */
void MainWindow::additionalSetupUi()
{
    // Search Dialog
    searchDialog = new SearchDialog();
    searchDialog->setWindowTitle("Search");

    // Jump Dialog
    jumpDialog = new JumpDialog();
    jumpDialog->setWindowTitle("Jump To");

    // About Dialog
    aboutDialog = new AboutDialog();
    aboutDialog->setWindowTitle("About BreizhEdit");

    // Documentation Dialog
    docDialog = new DocumentationDialog();
    docDialog->setWindowTitle("Commands");

    QFont font("Menlo", 12);
    ui->textEdit->setFont(font);
    originalFontSize = ui->textEdit->font().pointSize();

    QPalette p = ui->textEdit->palette();
    p.setColor(QPalette::Base, QColor(40, 40, 40, 255));
    p.setColor(QPalette::Text, QColor(255, 255, 255, 255));
    p.setColor(QPalette::Highlight, QColor(120, 120, 0, 255));

    ui->textEdit->setPalette(p);

    ui->textEdit->setWordWrapMode(QTextOption::NoWrap);

    statusBar()->showMessage(tr("Ready"));
}


/**
 * @brief MainWindow::fileOpen
 */
void MainWindow::fileOpen()
{
    QString fileName = NULL;

    // Only proceed if user agrees
    if (confirmAction())
    {
        fileName = QFileDialog::getOpenFileName(this);
        // Check file was specified
        if (!fileName.isEmpty())
            fileLoad(fileName);
    }
}


/**
 * @brief MainWindow::fileLoad
 * @param fileName
 */
void MainWindow::fileLoad(const QString &filename)
{
    QFile file(filename);
    QFileInfo fileInfo(filename);

    // Unable to open file
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot open file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(filename), file.errorString()));
        return;
    }
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    currentFilename=filename;
    setWindowTitle(fileInfo.fileName());

    // Update status bar
    statusBar()->showMessage(tr("File loaded"), 2000);
}

/**
 * @brief MainWindow::confirmAction
 * @return
 */
bool MainWindow::confirmAction()
{
    QMessageBox::StandardButton reply = QMessageBox::No;

    // Document not modified : proceed
    if (!ui->textEdit->document()->isModified())
        return true;

    // Document modified : confirm the action by user
    reply = QMessageBox::warning(this, "Warning", "The document has not been saved.\n\nAre you sure you want to proceed without saving ?",
                                  QMessageBox::Yes|QMessageBox::No);
    // Ok confirmed
    if (reply == QMessageBox::Yes)
            return true;

    // Cancelled by user
    return false;
}



/**
 * @brief MainWindow::writeSettings
 */
void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}


/**
 * @brief MainWindow::readSettings
 */
void MainWindow:: readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty())
    {
        const QRect availableGeometry = QApplication::primaryScreen()->availableGeometry();
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    }
    // Restore window's geometry
    else
    {
        restoreGeometry(geometry);
    }
}


/**
 * @brief MainWindow::editSearch
 */
void MainWindow::editSearch()
{
    if (searchDialog == NULL)
    {
        searchDialog = new SearchDialog();
        searchDialog->setWindowTitle("Search");
    }

    searchDialog->lineEdit->setFocus();
    searchDialog->labelStatus->setText("");
    searchDialog->show();
}


/**
 * @brief MainWindow::replaceString
 */
void MainWindow::replaceString()
{
    if (lastSearch == true)
    {
      ui->textEdit->textCursor().insertText(searchDialog->lineEditReplace->text());
      if (ui->textEdit->find(searchDialog->lineEdit->text()))
      {
          lastSearch = true;
      }
      else
      {
          lastSearch = false;
      }
    }
    else
    {
        searchDialog->labelStatus->setText("No text to replace");
    }
}

/**
 * @brief MainWindow::findReplaceString
 */
void MainWindow::findReplaceString()
{
    if (ui->textEdit->find(searchDialog->lineEdit->text()))
    {
        searchDialog->labelStatus->setText("");
        ui->textEdit->textCursor().insertText(searchDialog->lineEditReplace->text());
    }
    else
    {
        searchDialog->labelStatus->setText("Text not found");
    }
}


/**
 * @brief MainWindow::findString
 */
void MainWindow::findString()
{
    if (ui->textEdit->find(searchDialog->lineEdit->text()))
    {
        searchDialog->labelStatus->setText("");
        lastSearch = true;
    }
    else
    {
        searchDialog->labelStatus->setText("Text not found");
        lastSearch = false;
    }
}


/**
 * @brief MainWindow::findStringBackward
 */
void MainWindow::findStringBackward()
{
    QTextDocument::FindFlags flag = QTextDocument::FindBackward;
    if (ui->textEdit->find(searchDialog->lineEdit->text(), flag))
    {
        searchDialog->labelStatus->setText("");
        lastSearch = true;
    }
    else
    {
        searchDialog->labelStatus->setText("Text not found");
        lastSearch = false;
    }
}


/**
 * @brief MainWindow::viewSetFont
 */
void MainWindow::viewSetFont()
{
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok, ui->textEdit->font());

    if (ok==true)
    {
        ui->textEdit->setFont(font);
        originalFontSize=font.pointSize();
    }
}

/**
 * @brief MainWindow::viewSetView100Percent
 */
void MainWindow::viewSetView100Percent()
{
    QFont font=ui->textEdit->font();
    font.setPointSize(originalFontSize);
    ui->textEdit->setFont(font);

    statusBar()->showMessage(QString("View: 100%"), 2000);
}

/**
 * @brief MainWindow::viewIncrease
 */
void MainWindow::viewIncrease()
{
    QFont font=ui->textEdit->font();
    font.setPointSize(ui->textEdit->font().pointSize()+1);
    ui->textEdit->setFont(font);

    // Update status bar accorginly
    int viewPercent=ui->textEdit->font().pointSize() * 100 / originalFontSize;
    statusBar()->showMessage(QString("View: "+QString::number(viewPercent)+"%"));
}



/**
 * @brief MainWindow::viewDecrease
 */
void MainWindow::viewDecrease()
{
    static int MIN_FONT_SIZE = 6;

    QFont font=ui->textEdit->font();
    int newPointSize;

    newPointSize=ui->textEdit->font().pointSize()-1;
    if (newPointSize>=MIN_FONT_SIZE)
    {
        font.setPointSize(newPointSize);
        ui->textEdit->setFont(font);

        // Update status bar accorginly
        int viewPercent=ui->textEdit->font().pointSize() * 100 / originalFontSize;
        statusBar()->showMessage(QString("View: "+QString::number(viewPercent)+"%"));
    }

}


/**
 * @brief MainWindow::showCursorPosition
 */
void MainWindow::showCursorPosition()
{
    // as cursor position changed, the last search is no more true
    lastSearch = false;

    int line = ui->textEdit->textCursor().blockNumber()+1;
    int pos = ui->textEdit->textCursor().columnNumber()+1;
    statusBar()->showMessage(QString("Line %1:%2").arg(line).arg(pos));
}


/**
 * @brief MainWindow::setCurrentFile
 * @param fileName
 */
void MainWindow::setCurrentFile(const QString &fileName)
{
    QFileInfo fileInfo(fileName);

    currentFilename = fileName;
    ui->textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = fileInfo.fileName();
    if (currentFilename.isEmpty())
        shownName = "Untitled";
    setWindowTitle(shownName);
}


/**
 * @brief MainWindow::fileSave
 * @return
 */
void MainWindow::fileSave()
{
    // First time saving
    if (currentFilename.isEmpty())
    {
        return fileSaveAs();
    }
    // Filename already known
    else
    {
        return fileWrite(currentFilename);
    }
}


/**
 * @brief MainWindow::fileWrite
 * @param fileName
 * @return
 */
void MainWindow::fileWrite(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Warning"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
}


/**
 * @brief MainWindow::saveAs
 * @return
 */
void MainWindow::fileSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Select filename");
    if (! fileName.isNull())
    {
      fileWrite(fileName);
    }
}


/**
 * @brief MainWindow::fileQuit
 */
void MainWindow::fileQuit()
{
    // Only proceed if user agrees
    if (confirmAction())
    {
        // Save application's settings
        writeSettings();
        QCoreApplication::quit();
    }
}


/**
 * @brief MainWindow::fileNew
 */
void MainWindow::fileNew()
{
    if (confirmAction()) {
        ui->textEdit->clear();
        setCurrentFile(QString());
    }
}


/**
 * @brief MainWindow::filePrint
 */
void MainWindow::filePrint()
{
    QPrinter printer;

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() == QDialog::Rejected)
        return;
    ui->textEdit->print(&printer);
}


/**
 * @brief MainWindow::documentWasModified
 */
void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
}


/**
 * @brief MainWindow::editJumpTo
 */
void MainWindow::editJumpTo()
{
    if (jumpDialog == NULL)
    {
        jumpDialog = new JumpDialog();
        jumpDialog->setWindowTitle("Jump To");
    }

    jumpDialog->labelStatus->setText("");
    jumpDialog->lineNo->setText("");
    jumpDialog->lineNo->setFocus();
    jumpDialog->show();
}

/**
 * @brief MainWindow::findLine
 */
void MainWindow::findLine()
{
    int lineNumber=jumpDialog->lineNo->text().toInt()-1;
    QTextBlock textBlock=ui->textEdit->document()->findBlockByLineNumber(lineNumber);
    if (textBlock.isValid())
    {
        QTextCursor cursor(textBlock);
        ui->textEdit->setTextCursor(cursor);
        jumpDialog->hide();
    }
    else
    {
        jumpDialog->labelStatus->setText("Line not found");
    }
}


/**
 * @brief MainWindow::showAbout
 */
void MainWindow::showAbout()
{
    if (aboutDialog == NULL)
    {
        aboutDialog = new AboutDialog();
        aboutDialog->setWindowTitle("Info");
    }

    QString aboutText = QString("<b>BreizhEdit ")
            + QString(EDITOR_VERSION)
            + QString("</b><br><i>'Text Your Life!'</i><br><br>")
            + QString("Modern GUI application using Qt<br><br>")
            + QString("Home : <a href=\"https://github.com/digitalbox94/BreizhEdit/\" style=\"color: rgb(232,112,45);\">https://github.com/digitalbox94</a><br><br>")
            + QString("© 2020 DigitalBox");

    aboutDialog->setStyleSheet("QWidget{min-width:100px; color:white; background-color:rgb(40,40,40);}");
    aboutDialog->setFont(QFont("Menlo", 12));
    aboutDialog->labelText->setTextFormat(Qt::RichText);
    aboutDialog->labelText->setText(aboutText);

    aboutDialog->pushOk->setFocus();
    aboutDialog->show();
}


/**
 * @brief MainWindow::showDocumentation
 */
void MainWindow::showDocumentation()
{

    if (docDialog == NULL)
    {
        docDialog = new DocumentationDialog();
        docDialog->setWindowTitle("Documentation");
    }

    docDialog->setStyleSheet("QWidget{min-width:100px; color:white; background-color:rgb(38,39,43);}");
    docDialog->setFont(QFont("Menlo", 12));
    docDialog->labelDocumentation->setTextFormat(Qt::RichText);
    docDialog->labelDocumentation->setText((tr("<b><u>Commands</u></b><br><br>"
                                      "Cmd + F : find & replace text<br>"
                                      "Cmd + G : go to line<br>"
                                      "Cmd + Scroll Up : zoom view<br>"
                                      "Cmd + Scroll Down : unzoom view<br>"
                                    )));

    docDialog->pushOk->setFocus();
    docDialog->show();

}




