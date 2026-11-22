
#include <string>
#include "rpcstubhelper.h"

#include <cstdio>
#include <cstring>
#include "c150debug.h"
#include <cassert>

using namespace std;
using namespace C150NETWORK;
#include "structs.idl"

void get_string_from_stream(string *string_p);
void get_float_from_stream(float *float_p);
void get_int_from_stream(int *int_p);
void send_string_to_stream(string s);
void send_int_to_stream(int i);
void send_float_to_stream(float f);

#define INITIAL_BUFF_SIZE 50
void get_Person_from_stream(Person *struct_p);
void send_Person_to_stream(Person to_send);
void get_StructWithArrays_from_stream(StructWithArrays *struct_p);
void send_StructWithArrays_to_stream(StructWithArrays to_send);
void get_ThreePeople_from_stream(ThreePeople *struct_p);
void send_ThreePeople_to_stream(ThreePeople to_send);
void get_Person_10_from_stream(Person *arr_p);
void send_Person_10_to_stream(Person *to_send);
void get_rectangle_from_stream(rectangle *struct_p);
void send_rectangle_to_stream(rectangle to_send);
void __ageSum();
void __area();
void __findPerson();
void dispatchFunction();
void __badFunction(char *functionName) {
    char doneBuffer[5] = "BAD";  // to write magic value DONE + null

    // Send the response to the client indicating bad function

    c150debug->printf(C150RPCDEBUG,"structs.idl.stub.cpp: received call for nonexistent function %s()",functionName);
    RPCSTUBSOCKET->write(doneBuffer, strlen(doneBuffer)+1);
}
void __ageSum(){
    StructWithArrays swa;
    get_StructWithArrays_from_stream(&swa);
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: invoking ageSum");
    int result = ageSum(swa);

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: returned from ageSum -- responding to client");

    send_int_to_stream(result);
}
void __area(){
    rectangle r;
    get_rectangle_from_stream(&r);
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: invoking area");
    int result = area(r);

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: returned from area -- responding to client");

    send_int_to_stream(result);
}
void __findPerson(){
    ThreePeople tp;
    get_ThreePeople_from_stream(&tp);
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: invoking findPerson");
    Person result = findPerson(tp);

    /* send back result */
    c150debug->printf(C150RPCDEBUG, "structs.stub.cpp: returned from findPerson -- responding to client");

    send_Person_to_stream(result);
}
void dispatchFunction()
{
    string functionName;
    get_string_from_stream(&functionName);
    
    if (!RPCSTUBSOCKET->eof()) {

        if (functionName == "ageSum")
            __ageSum();
        else if (functionName == "area")
            __area();
        else if (functionName == "findPerson")
            __findPerson();
        else
            __badFunction((char*)functionName.c_str());
    }
}

void get_Person_from_stream(Person *struct_p)
{
    get_string_from_stream(&(struct_p->firstname));
    get_string_from_stream(&(struct_p->lastname));
    get_int_from_stream(&(struct_p->age));
}
void send_Person_to_stream(Person to_send)
{
    send_string_to_stream(to_send.firstname);
    send_string_to_stream(to_send.lastname);
    send_int_to_stream(to_send.age);
}
void get_StructWithArrays_from_stream(StructWithArrays *struct_p)
{
    get_int_from_stream(&(struct_p->aNumber));
    get_Person_10_from_stream(struct_p->people);
}
void send_StructWithArrays_to_stream(StructWithArrays to_send)
{
    send_int_to_stream(to_send.aNumber);
    send_Person_10_to_stream(to_send.people);
}
void get_ThreePeople_from_stream(ThreePeople *struct_p)
{
    get_Person_from_stream(&(struct_p->p1));
    get_Person_from_stream(&(struct_p->p2));
    get_Person_from_stream(&(struct_p->p3));
}
void send_ThreePeople_to_stream(ThreePeople to_send)
{
    send_Person_to_stream(to_send.p1);
    send_Person_to_stream(to_send.p2);
    send_Person_to_stream(to_send.p3);
}
void get_Person_10_from_stream(Person *arr_p)
{
    for (int i = 0; i < 10; i++) {
        get_Person_from_stream(arr_p + i);
    }
}
void send_Person_10_to_stream(Person *to_send)
{
    for (int i = 0; i < 10; i++) {
        send_Person_to_stream(to_send[i]);
    }
}
void get_rectangle_from_stream(rectangle *struct_p)
{
    get_int_from_stream(&(struct_p->x));
    get_int_from_stream(&(struct_p->y));
}
void send_rectangle_to_stream(rectangle to_send)
{
    send_int_to_stream(to_send.x);
    send_int_to_stream(to_send.y);
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
        c150debug->printf(C150RPCDEBUG,"structs: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"structs: EOF signaled on input");

        } else {
            throw C150Exception("structs: unexpected zero length read without eof");
        }
    }
    // If we didn't get a null, input message was poorly formatted
    else if(!readnull) 
        throw C150Exception("structs: string not null terminated or too long");

    
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

void send_float_to_steram(float f)
{
    send_string_to_stream(to_string(f));
}
