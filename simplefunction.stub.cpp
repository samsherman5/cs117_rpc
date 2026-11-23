
#include <string>
#include "rpcstubhelper.h"

#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <cassert>

using namespace std;
using namespace C150NETWORK;
#include "simplefunction.idl"

void get_string_from_stream(string *string_p);
void get_float_from_stream(float *float_p);
void get_int_from_stream(int *int_p);
void send_string_to_stream(string s);
void send_int_to_stream(int i);
void send_float_to_stream(float f);

#define INITIAL_BUFF_SIZE 50
void __func1();
void __func2();
void __func3();
void dispatchFunction();
void __badFunction(char *functionName) {
    char doneBuffer[5] = "BAD";  // to write magic value DONE + null

    // Send the response to the client indicating bad function

    c150debug->printf(C150RPCDEBUG,"simplefunction.idl.stub.cpp: received call for nonexistent function %s()",functionName);
    RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}
void __func1(){
    func1();

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "simplefunction.stub.cpp: returned from func1 -- responding to client");

    send_string_to_stream("COMPLETE");
}
void __func2(){
    func2();

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "simplefunction.stub.cpp: returned from func2 -- responding to client");

    send_string_to_stream("COMPLETE");
}
void __func3(){
    func3();

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "simplefunction.stub.cpp: returned from func3 -- responding to client");

    send_string_to_stream("COMPLETE");
}
void dispatchFunction()
{
    string functionName;
    get_string_from_stream(&functionName);
    
    if (!RPCSTUBSOCKET->eof()) {

        if (functionName == "func1")
            __func1();
        else if (functionName == "func2")
            __func2();
        else if (functionName == "func3")
            __func3();
        else
            __badFunction((char*)functionName.c_str());
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
        readlen = RPCSTUBSOCKET->read(bufp + i, 1);
        
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
        c150debug->printf(C150RPCDEBUG,"simplefunction: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"simplefunction: EOF signaled on input");

        } else {
            throw C150Exception("simplefunction: unexpected zero length read without eof");
        }
    }
    // If we didn't get a null, input message was poorly formatted
    else if(!readnull) 
        throw C150Exception("simplefunction: string not null terminated or too long");

    
    string buffer(bufp);
    free(bufp);

    *string_p = buffer;
}

/* 
                SEND TO STREAM FUNCTIONS 
*/

void send_string_to_stream(string s)
{
    RPCSTUBSOCKET->write(s.c_str(), s.length()+1);
}

void send_int_to_stream(int i)
{
    send_string_to_stream(to_string(i));
}

void send_float_to_stream(float f)
{
    send_string_to_stream(to_string(f));
}
