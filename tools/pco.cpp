#include <pcompiler/root_manager.hpp>
#include <pcompiler/input.hpp>
#include <pcompiler/options.hpp>
#include <pcompiler/engine.hpp>
#include <pcompiler/compilers.hpp>
#include <pcompiler/output.hpp>

#include <kar/kar.hpp>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QDateTime>

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

void messageOutputAll(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
        cout << "Debug: " << localMsg.constData() << endl;
        break;
    case QtWarningMsg:
        cout << "Warning: " << localMsg.constData() << endl;
        break;
    case QtCriticalMsg:
        cerr << "Critical: " << localMsg.constData() << endl;
        break;
    case QtFatalMsg:
        cerr << "Fatal: " << localMsg.constData() << endl;
        abort();
    }
}

void messageOutputNoDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type)
    {
    case QtDebugMsg:
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        cerr << "Critical: " << localMsg.constData() << endl;
        break;
    case QtFatalMsg:
        cerr << "Fatal: " << localMsg.constData() << endl;
        abort();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("KIPR Link Program Compiler");
    QCoreApplication::setApplicationVersion("1.0");
    
    qInstallMessageHandler(messageOutputAll);
    
    // get the command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Compiles a KIPR Link program");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("name", QCoreApplication::translate("main", "The program's name."));
    QCommandLineOption rootDirectoryOption("r",
            QCoreApplication::translate("main", "Root directory <directory>."),
            QCoreApplication::translate("main", "directory"));
    parser.addOption(rootDirectoryOption);
    QCommandLineOption debugOutputOption("d", QCoreApplication::translate("main", "Show debug output"));
    parser.addOption(debugOutputOption);
    
    parser.process(app);
    
    const QStringList args = parser.positionalArguments(); // name is at args.at(0), sourcce at args.at(1)
    
    QString rootDir = "/kovan";
    if(parser.isSet(rootDirectoryOption))
    {
        rootDir = parser.value(rootDirectoryOption);
    }
    if(!QDir(rootDir).exists())
    {
        cerr << QCoreApplication::translate("main", "Error: %1 does not name a directory").arg(rootDir).toStdString() << endl << endl;
        parser.showHelp(1); // this will exit the application with exit code 1
    }
    
    if(!parser.isSet(debugOutputOption))
    {
       qInstallMessageHandler(messageOutputNoDebug);
    }
    
    if(args.size() != 1)
    {
        cerr << QCoreApplication::translate("main", "Error: Too few arguments provided").toStdString() << endl << endl;
        parser.showHelp(1); // this will exit the application with exit code 1
    }
    const QString name = args.at(0);
    
    // unpack the archive
    Compiler::RootManager rootManager(rootDir);
    
    const QString archivePath = rootManager.archivesPath(name);
    
    QFile file(archivePath);
    kiss::KarPtr archive = kiss::Kar::load(archivePath);
    if(!archive)
    {
        cerr << QCoreApplication::translate("main", "Error: %1 is no KAR archive").arg(archivePath).toStdString() << endl << endl;
        parser.showHelp(1); // this will exit the application with exit code 1
    }
    
    const QString tmpDir = QDir::tempPath() + "/" + QDateTime::currentDateTime().toString("yyMMddhhmmss") + ".botui";
    if(!archive->extract(tmpDir))
    {
        cerr << QCoreApplication::translate("main", "Error: Could not extract the archive into %1").arg(tmpDir).toStdString() << endl << endl;
        return 1;
    }
    
    // collect the file names and the options for the compiler
    QStringList extracted;
    foreach(const QString& file, archive->files())
    {
        extracted << tmpDir + "/" + file;
    }
    Compiler::Input input =  Compiler::Input(extracted.begin(),extracted.end());
    //Compiler::Input::fromList(extracted);
    Compiler::Options opts = Compiler::Options::load("/home/stefan/tmp/platform.hints");
    
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
                cout << QString(file).remove(tmpDir + "/").toStdString() << " ";
            }
            cout << "} ";
        }
        
        if(output.generatedFiles().count() > 0)
        {
            cout << "-> { ";
            foreach(const QString& file, output.generatedFiles())
            {
                cout << QString(file).remove(tmpDir + "/").toStdString() << " ";
            }
            cout << "} ";
        }
        
        if(output.output().size() > 0)
        {
            cout << endl << '\t' << QString(output.output()).remove(tmpDir + "/").replace('\n', "\n\t").toStdString() << endl;
        }
        
        if(output.isSuccess())
        {
            cout << "was successful" << endl;
        }
        else
        {
            cout << endl << '\t' << QString(output.error()).remove(tmpDir + "/").replace('\n', "\n\t").toStdString() << endl;
            
            cout << "failed with exit code " << output.exitCode() << endl;
        }
    }
    
    return 0;
}


