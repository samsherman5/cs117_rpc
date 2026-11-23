
#include <string>
#include "rpcproxyhelper.h"

#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <cassert>

using namespace std;
using namespace C150NETWORK;
#include "testarray2.idl"

void get_string_from_stream(string *string_p);
void get_float_from_stream(float *float_p);
void get_int_from_stream(int *int_p);
void send_string_to_stream(string s);
void send_int_to_stream(int i);
void send_float_to_stream(float f);

#define INITIAL_BUFF_SIZE 50
void get_int_15_from_stream(int arr_p[15]);
void send_int_15_to_stream(int to_send[15]);
void get_int_24_from_stream(int arr_p[24]);
void send_int_24_to_stream(int to_send[24]);
void get_int_24_15_from_stream(int arr_p[24][15]);
void send_int_24_15_to_stream(int to_send[24][15]);
int sqrt(int x[24], int y[24][15], int z[24][15]);
int sqrt(int x[24], int y[24][15], int z[24][15])
{
    /* send remote call */
    c150debug->printf(C150RPCDEBUG,"testarray2.proxy.cpp: sqrt() invoked");
    
    /* write argument */
    send_string_to_stream("sqrt");

    send_int_24_to_stream(x);

    send_int_24_15_to_stream(y);

    send_int_24_15_to_stream(z);

    c150debug->printf(C150RPCDEBUG,"testarray2.proxy.cpp: sqrt() invocation sent, waiting for response");
    int return_value;
    get_int_from_stream(&return_value);
    c150debug->printf(C150RPCDEBUG,"testarray2.proxy.cpp: sqrt() successful return from remote call");
    return return_value;
}
void get_int_15_from_stream(int arr_p[15])
{
    for (int i = 0; i < 15; i++) {
        get_int_from_stream(arr_p + i);
    }
}
void send_int_15_to_stream(int to_send[15])
{
    for (int i = 0; i < 15; i++) {
        send_int_to_stream(to_send[i]);
    }
}
void get_int_24_from_stream(int arr_p[24])
{
    for (int i = 0; i < 24; i++) {
        get_int_from_stream(arr_p + i);
    }
}
void send_int_24_to_stream(int to_send[24])
{
    for (int i = 0; i < 24; i++) {
        send_int_to_stream(to_send[i]);
    }
}
void get_int_24_15_from_stream(int arr_p[24][15])
{
    for (int i = 0; i < 24; i++) {
        get_int_15_from_stream(arr_p[i]);
    }
}
void send_int_24_15_to_stream(int to_send[24][15])
{
    for (int i = 0; i < 24; i++) {
        send_int_15_to_stream(to_send[i]);
    }
}

// ======================================================================
//
//                        COMMON SUPPORT FUNCTIONS
//
// ======================================================================

/*  get_float_from_stream 

    gets a float from the stream
    
    args: 
        float pointer
        
    returns: 
        void

    notes: 
        uses getStringFromStream() 
        
*/
void get_float_from_stream(float *float_p)
{
    string to_get;
    get_string_from_stream(&to_get);
    float floatFromStream = stof(to_get);

    *float_p = floatFromStream;
}

/*  get_int_from_stream 

    gets an int from the stream
    
    args: 
        int pointer
        
    returns: 
        n/a

    notes: 
        uses getStringFromStream() 
        
*/
void get_int_from_stream(int *int_p)
{
    string to_get;
    get_string_from_stream(&to_get);
    int intFromStream = stoi(to_get);

    *int_p = intFromStream;
}

//                   get_string_from_stream
//
//   Helper routine to read a string from the stream. 
//   Note that this code is the same for all stubs, so can be generated
//   as boilerplate.
//
//   Important: this routine must leave the sock open but at EOF
//   when eof is read from client.
void get_string_from_stream(string *string_p) 
{
    unsigned i = 0;
    char *bufp = (char *)malloc(INITIAL_BUFF_SIZE); // allocate memory for buffer
    assert(bufp != NULL);
    bool readnull;
    ssize_t readlen;                        // amount of data read from socket
    unsigned bufSize = INITIAL_BUFF_SIZE;

    // Read a message from the stream
    // -1 in size below is to leave room for null
    readnull = false;
    while(!readnull) {
        /* expand buffer if necessary */
        if (i == (bufSize - 1)) {
            bufSize *= 2;
            char *new_buf = (char *)realloc(bufp, bufSize);
            assert(new_buf != NULL);
            bufp = new_buf;
        }

        /* read a byte into the buffer from the stream */
        readlen = RPCPROXYSOCKET->read(bufp + i, 1);
        
        /* check for EOF or error */
        if (readlen == 0) {
            break;
        }

        /* check for NULL terminator */
        if (*(bufp + i) == '\0') {
            readnull = true;
            break;
        }

        /* increment i for next iteration*/
        i++;
    }

    
    // With TCP streams, we should never get a 0 length read
    // except with timeouts (which we're not setting in pingstreamserver)
    // or EOF
    if (readlen == 0) {
        c150debug->printf(C150RPCDEBUG,"testarray2: read zero length message, checking EOF");
        if (RPCPROXYSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"testarray2: EOF signaled on input");

        } else {
            throw C150Exception("testarray2: unexpected zero length read without eof");
        }
    }
    // If we didn't get a null, input message was poorly formatted
    else if(!readnull) 
        throw C150Exception("testarray2: string not null terminated or too long");

    
    string buffer(bufp);
    free(bufp);

    *string_p = buffer;
}

/* 
                SEND TO STREAM FUNCTIONS 
*/

void send_string_to_stream(string s)
{
    RPCPROXYSOCKET->write(s.c_str(), s.length()+1);
}

void send_int_to_stream(int i)
{
    send_string_to_stream(to_string(i));
}

void send_float_to_stream(float f)
{
    send_string_to_stream(to_string(f));
}
