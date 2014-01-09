/**********************************************************************
** Copyright (C) 2010 froglogic GmbH.
** All rights reserved.
**********************************************************************/

#ifndef TRACELIB_GETCURRENTTHREADID_H
#define TRACELIB_GETCURRENTTHREADID_H

#include "tracelib_config.h"
#include "config.h" // for uint64_t

TRACELIB_NAMESPACE_BEGIN

typedef unsigned long ProcessId;
typedef unsigned long ThreadId;

uint64_t getCurrentProcessStartTime();
ProcessId getCurrentProcessId();
ThreadId getCurrentThreadId();

TRACELIB_NAMESPACE_END

#endif // !defined(TRACELIB_GETCURRENTTHREADID_H)

