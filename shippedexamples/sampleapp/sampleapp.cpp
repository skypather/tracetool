/**********************************************************************
** Copyright (C) 2010 froglogic GmbH.
** All rights reserved.
**********************************************************************/

#include "acme.h"
#include "person.h"

#include <tracelib.h>

#include <cstdio>
#include <string>
#include <sstream>

TRACELIB_NAMESPACE_BEGIN
    template <>
    VariableValue convertVariable<const Person *>( const Person *p ) {
        char buf[ 32 ];
        sprintf( buf, "0x%08x", p );
        return VariableValue::stringValue( &buf[0] );
    }
TRACELIB_NAMESPACE_END

int main()
{
    TRACELIB_TRACE
    ACME::GUI::Widget w;
    w.repaint( false );
}
