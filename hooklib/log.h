/* tracetool - a framework for tracing the execution of C++ programs
 * Copyright 2010-2016 froglogic GmbH
 *
 * This file is part of tracetool.
 *
 * tracetool is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * tracetool is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with tracetool.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRACELIB_ERRORLOG_H
#define TRACELIB_ERRORLOG_H

#include "tracelib_config.h"

#include "timehelper.h"

#include <stdarg.h>
#include <stdio.h>
#include <iosfwd>
#include <string>
#include <vector>
#include <set>

#ifdef _WIN32
#  include <windows.h>
#  define vsnprintf _vsnprintf
#endif

TRACELIB_NAMESPACE_BEGIN

class LogOutput
{
public:
    virtual ~LogOutput();

    virtual void write( const std::string &msg ) = 0;
protected:
    LogOutput();

private:
    LogOutput( const LogOutput &other );
    void operator=( const LogOutput &other );
};

class Log
{
public:
    Log( LogOutput* statusOutput, LogOutput* errorOutput );
    ~Log();

    void writeError( const std::string &msg );
    inline void writeError( const char *format, ... ) {
        va_list argList;
        va_start( argList, format );

        std::vector<char> buf( 1024, '\0' );
        vsnprintf( &buf[0], buf.size() - 1, format, argList );
        va_end( argList );
        writeError( std::string( &buf[0] ) );
    }

    void writeStatus( const std::string &msg );
    inline void writeStatus( const char *format, ... ) {
        va_list argList;
        va_start( argList, format );

        std::vector<char> buf( 1024, '\0' );
        vsnprintf( &buf[0], buf.size() - 1, format, argList );
        va_end( argList );
        writeStatus( std::string( &buf[0] ) );
    }

private:
    Log( const Log &other );
    void operator=( const Log &other );
    LogOutput* m_statusOutput;
    LogOutput* m_errorOutput;
};

#ifdef _WIN32
class DebugViewLogOutput : public LogOutput
{
public:
    virtual void write( const std::string &msg );
};
#endif

class NullLogOutput : public LogOutput
{
public:
    virtual void write( const std::string &msg );
};

class StreamLogOutput : public LogOutput
{
public:
    explicit StreamLogOutput( std::ostream &stream );
    virtual ~StreamLogOutput();

    virtual void write( const std::string &msg );

private:
    std::ostream &m_stream;
};

class FileLogOutput : public LogOutput
{
public:
    FileLogOutput( const std::string &filename );
    ~FileLogOutput();
    bool isOpen() const;
    virtual void write( const std::string &msg );
    std::ostream &stream() const;
private:
    std::ofstream *m_fileStream;
    StreamLogOutput *m_internalOutput;
};

class MultiplexLogOutput : public LogOutput
{
public:
    virtual ~MultiplexLogOutput();
    void addOutput( LogOutput * );
    virtual void write( const std::string &msg );

private:
    std::set<LogOutput*> m_multiplexedOutputs;
};

TRACELIB_NAMESPACE_END

#endif // !defined(TRACELIB_ERRORLOG_H)


