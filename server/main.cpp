/**********************************************************************
** Copyright (C) 2010 froglogic GmbH.
** All rights reserved.
**********************************************************************/

#include "server.h"

#include "database.h"
#include "../hooklib/tracelib_config.h"
#include "../convertdb/getopt.h"

#include <QCoreApplication>
#include <QFile>

#include <iostream>
#include <string>

namespace Error
{
    const int None = 0;
    const int CommandLineArgs = 1;
    const int Database = 2;
}

using namespace std;

static void printUsage(const string &app)
{
    cout << "Usage: " << app << " --help" << endl
         << "       " << app << " [--port <port>] <.trace-file>" << endl;
}

int main( int argc, char **argv )
{
    QCoreApplication app( argc, argv );

    GetOpt opt;
    bool help;
    QString traceFile, portStr;
    opt.addSwitch("help", &help);
    opt.addOption('p', "port", &portStr);
    opt.addOptionalArgument("trace_file", &traceFile);
    if (!opt.parse()) {
	cout << "Invalid command line argument. Try --help." << endl;
	return Error::CommandLineArgs;
    }

    if (help) {
	printUsage(argv[0]);
	return Error::None;
    }
    if (traceFile.isEmpty()) {
	cout << "Missing trace file argument. Try --help." << endl;
	return Error::CommandLineArgs;
    }
    QString errMsg;
    if (!Database::isValidFileName(traceFile, &errMsg)) {
        cout << errMsg.toLocal8Bit().constData() << endl;
        return Error::CommandLineArgs;
    }
    int port = TRACELIB_DEFAULT_PORT;
    if (!portStr.isEmpty()) {
	bool ok;
	port = portStr.toInt(&ok);
	if (!ok) {
	    cout << "Invalid port number '"
		 << portStr.toLocal8Bit().constData()
		 << "' given." << endl;
	    return Error::CommandLineArgs;
	}
    }

    QSqlDatabase database;
    if (QFile::exists(traceFile)) {
        database = Database::open(traceFile, &errMsg);
    } else {
        database = Database::create(traceFile, &errMsg);
    }
    if (!database.isValid()) {
        cout << "Failed to open log database: "
             << errMsg.toLocal8Bit().constData()
             << endl;
        return Error::Database;
    }

    Server server(traceFile, database, port);

    return app.exec();
}

