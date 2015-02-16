#include <pcompiler/root_manager.hpp>
#include <pcompiler/input.hpp>
#include <pcompiler/options.hpp>
#include <pcompiler/engine.hpp>
#include <pcompiler/compilers.hpp>
#include <pcompiler/output.hpp>

#include <kar/kar.hpp>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDateTime>

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

void messageOutputAll(QtMsgType type, const char* msg)
{
    switch (type)
    {
    case QtDebugMsg:
        cout << "Debug: " << msg << endl;
        break;
    case QtWarningMsg:
        cout << "Warning: " << msg << endl;
        break;
    case QtCriticalMsg:
        cerr << "Critical: " << msg << endl;
        break;
    case QtFatalMsg:
        cerr << "Fatal: " << msg << endl;
        abort();
    }
}

void messageOutputNoDebug(QtMsgType type, const char* msg)
{
    switch (type)
    {
    case QtDebugMsg:
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        cerr << "Critical: " << msg << endl;
        break;
    case QtFatalMsg:
        cerr << "Fatal: " << msg << endl;
        abort();
    }
}

inline string toStdString(const QString& str)
{
    const QByteArray str_ba = str.toUtf8();
    return string(str_ba.constData());
}

void usage(const QString& appName)
{
    cout << toStdString(QCoreApplication::translate("main",
        "Usage: %1 [options] name\n"
        "Compiles a KIPR Link program\n"
        "Options:\n"
        "  -h, --help      Displays this help.\n"
        "  -v, --version   Displays version information.\n"
        "  -r <directory>  Root directory <directory>.\n"
        "  -o <file>       Compilation options file <file>.\n"
        "  -d              Show debug output\n"
        "\n"
        "Arguments:\n"
        "  name            The program's name.\n"
       ).arg(appName)) << endl;
}

int main(int argc, char *argv[])
{
    const QString appVersion = "1.0";
    
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("KIPR Link Program Compiler");
    QCoreApplication::setApplicationVersion(appVersion);
    
    qInstallMsgHandler(messageOutputNoDebug);
    
    // get the command line arguments
    const QStringList args = QCoreApplication::arguments();
    const QString appName = (args.size() > 0) ? args.at(0) : "programcompiler";
    
    QString rootDir = "/kovan";
    QString optionsFile = "/etc/kovan/platform.hints";
    QString name = "";
    
    for(int i = 1; i < args.size(); i++)
    {
        if(args.at(i) == "-r")
        {
            if(++i >= args.size())
            {
                usage(appName);
                return 1;
            }
            else
            {
                rootDir = args.at(i);
            }
        }
        if(args.at(i) == "-o")
        {
            if(++i >= args.size())
            {
                usage(appName);
                return 1;
            }
            else
            {
                optionsFile = args.at(i);
            }
        }
        else if(args.at(i) == "-d")
        {
            qInstallMsgHandler(messageOutputAll);
        }
        else if(args.at(i) == "-h" || args.at(i) == "--help")
        {
            usage(appName);
            return 0;
        }
        else if(args.at(i) == "-v" || args.at(i) == "--version")
        {
            cout << toStdString(appName) << ", version " << toStdString(appVersion) << endl;
            return 0;
        }
        else
        {
            name = args.at(i);
        }
    }
    
    if(!QDir(rootDir).exists())
    {
        cerr << toStdString(QCoreApplication::translate("main", "Error: %1 does not name a directory").arg(rootDir)) << endl << endl;
        usage(appName);
        return 1;
    }
    
    if(!QFile(optionsFile).exists())
    {
        cerr << toStdString(QCoreApplication::translate("main", "Error: %1 does not name a file").arg(optionsFile)) << endl << endl;
        usage(appName);
        return 1;
    }
    
    if(name == "")
    {
        cerr << toStdString(QCoreApplication::translate("main", "Error: Too few arguments provided")) << endl << endl;
        usage(appName);
        return 1;
    }
    
    // unpack the archive
    Compiler::RootManager rootManager(rootDir);
    
    const QString archivePath = rootManager.archivesPath(name);
    
    QFile file(archivePath);
    kiss::KarPtr archive = kiss::Kar::load(archivePath);
    if(!archive)
    {
        cerr << toStdString(QCoreApplication::translate("main", "Error: %1 is no KAR archive").arg(archivePath)) << endl << endl;
        usage(appName);
        return 1;
    }
    
    const QString tmpDir = QDir::tempPath() + "/" + QDateTime::currentDateTime().toString("yyMMddhhmmss") + ".botui";
    if(!archive->extract(tmpDir))
    {
        cerr << toStdString(QCoreApplication::translate("main", "Error: Could not extract the archive into %1").arg(tmpDir))
             << endl << endl;
        return 1;
    }
    
    // collect the file names and the options for the compiler
    QStringList extracted;
    foreach(const QString& file, archive->files())
    {
        extracted << tmpDir + "/" + file;
    }
    
    Compiler::Input input = Compiler::Input::fromList(extracted);
    Compiler::Options opts = Compiler::Options::load(optionsFile);
    
    // compile the files
    Compiler::Engine engine(Compiler::Compilers::instance()->compilers());
    Compiler::OutputList ol = engine.compile(input, opts);
    
    // install the program
    ol << rootManager.install(ol, name);
    
    // output the result
    int jobCnt = 1;
    foreach(const Compiler::Output& output, ol)
    {
        cout << "Program compilation job #" << jobCnt++ << ": ";
        
        if(output.files().count() > 0)
        {
            cout << "{ ";        
            foreach(const QString& file, output.files())
            {
                cout << toStdString(QString(file).remove(tmpDir + "/")) << " ";
            }
            cout << "} ";
        }
        
        if(output.generatedFiles().count() > 0)
        {
            cout << "-> { ";
            foreach(const QString& file, output.generatedFiles())
            {
                cout << toStdString(QString(file).remove(tmpDir + "/")) << " ";
            }
            cout << "} ";
        }
        
        if(output.output().size() > 0)
        {
            cout << endl << '\t' << toStdString(QString::fromUtf8(output.output()).remove(tmpDir + "/").replace('\n', "\n\t")) << endl;
        }
        
        if(output.isSuccess())
        {
            cout << "was successful" << endl;
        }
        else
        {
            cout << endl << '\t' << toStdString(QString::fromUtf8(output.error()).remove(tmpDir + "/").replace('\n', "\n\t")) << endl;
            
            cout << "failed with exit code " << output.exitCode() << endl;
        }
    }
    
    return 0;
}


