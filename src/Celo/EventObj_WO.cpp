#include "RemOutput.h"
#include "StringHelp.h"

EventObj_WO::EventObj_WO( int fd ) : EventObj_WP( fd ), prim_rem_out( 0 ), last_rem_out( 0 ) {
}

EventObj_WO::~EventObj_WO() {
    cl_rem();
}

bool EventObj_WO::out() {
    while ( prim_rem_out ) {
        if ( not prim_rem_out->write( this ) )
            return true;

        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
    return false;
}

void EventObj_WO::send_cst( const char *data, ST size, bool end ) {
    ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
    if ( real < 0 )
        return cl_rem();

    size -= real;
    if ( size > 0 ) {
        data += real;
        append( new RemOutputData( data, size, end ) );
    }
}

void EventObj_WO::send_cst( const char *data ) {
    send_cst( data, strlen( data ) );
}

void EventObj_WO::send_str( const char *data, ST size, bool end ) {
    ST real = ::send( fd, data, size, end ? MSG_NOSIGNAL : MSG_NOSIGNAL | MSG_MORE );
    if ( real < 0 )
        return cl_rem();

    size -= real;
    if ( size > 0 ) {
        data += real;
        append( new RemOutputDataCopy( data, size, end ) );
    }
}

void EventObj_WO::send_str( const char *data ) {
    send_str( data, strlen( data ) );
}

void EventObj_WO::send_fid( int src, ST off, ST len ) {
    off_t offset = off;
    ssize_t size = sendfile( fd, src, &offset, len );
    if ( size < 0 ) {
        if ( errno == EAGAIN )
            return append( new RemOutputFile( src, offset, len ) );
        return cl_rem();
    }

    len -= size;
    if ( len )
        return append( new RemOutputFile( src, offset, len ) );

    close( src );
}

void EventObj_WO::append( RemOutput *rem_out ) {
    rem_out->next = 0;

    if ( last_rem_out )
        last_rem_out->next = rem_out;
    last_rem_out = rem_out;

    if ( not prim_rem_out ) {
        prim_rem_out = rem_out;
        //
        poll_out();
    }
}

void EventObj_WO::cl_rem() {
    while( prim_rem_out ) {
        RemOutput *o = prim_rem_out;
        prim_rem_out = prim_rem_out->next;
        delete o;
    }
}
