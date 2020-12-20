#include "main.h"
#include <stdarg.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

GLuint  base;

void buildFont( ) {
    Display *dpy;
    XFontStruct *fontInfo; 
    base = glGenLists( 96 );
    dpy = XOpenDisplay( NULL );
    fontInfo = XLoadQueryFont( dpy, "-adobe-helvetica-medium-r-normal--18-*-*-*-p-*-iso8859-1" );
    if ( fontInfo == NULL )
    {
        fontInfo = XLoadQueryFont( dpy, "fixed" );
        if ( fontInfo == NULL )
        {
            fprintf( stderr, "no X font available?\n" );
            exit(2);
        }
    }

    glXUseXFont( fontInfo->fid, 32, 96, base );
    XFreeFont( dpy, fontInfo );
    XCloseDisplay( dpy );
    return;
}


void glPrint( const char *fmt, ... ) {
    char text[256]; 
    va_list ap;
    if ( fmt == NULL )
    return;
    va_start( ap, fmt );
    vsprintf( text, fmt, ap );
    va_end( ap );
    glPushAttrib( GL_LIST_BIT );
    glListBase( base - 32 );
    glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );
    glPopAttrib( );
}
