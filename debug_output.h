#ifndef DEBUG_OUTPUT_H
#define DEBUG_OUTPUT_H

#include <QString>
#include "mainwindow.h"

class DebugOutput
{    
public:
    static void init(void);
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & str);
    static void clogMessageHandler(std::string & str);
    static void handleMessage(QString str);
    static void setFileName(QString fileName);
    static void setMainWindow(MainWindow *mainWindow);

private:
    static QString fileName;
    static MainWindow *mainWindow;
};

class LogStringStream : public std::basic_streambuf<char, std::char_traits<char> > {
protected:
    int sync();
    int overflow(int c);

private:
    std::string buffer_;
};


#endif // DEBUG_OUTPUT_H
