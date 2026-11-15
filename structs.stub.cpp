
// ======================================================================
//
//                        COMMON SUPPORT FUNCTIONS
//
// ======================================================================

/*  getFloatFromStream 

    gets a float from the stream
    
    args: 
        none
        
    returns: 
        float

    notes: 
        uses getStringFromStream() 
        
*/
float getFloatFromStream()
{
    string stringFromStream = getStringFromStream();
    float floatFromStream = stof(stringFromStream);

    return floatFromStream;
}

/*  getIntFromStream 

    gets an int from the stream
    
    args: 
        none
        
    returns: 
        int

    notes: 
        uses getStringFromStream() 
        
*/
int getIntFromStream()
{
    string stringFromStream = getStringFromStream();
    int intFromStream = stoi(stringFromStream);

    return intFromStream;
}

//                   getStringfromStream
//
//   Helper routine to read a string from the stream. 
//   Note that this code is the same for all stubs, so can be generated
//   as boilerplate.
//
//   Important: this routine must leave the sock open but at EOF
//   when eof is read from client.
string getStringFromStream() 
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
        c150debug->printf(C150RPCDEBUG,"structs.stub.cpp: read zero length message, checking EOF");
        if (RPCSTUBSOCKET-> eof()) {
            c150debug->printf(C150RPCDEBUG,"structs.stub.cpp: EOF signaled on input");

        } else {
            throw C150Exception("structs.stub.cpp: unexpected zero length read without eof");
        }
    }
    // If we didn't get a null, input message was poorly formatted
    else if(!readnull) 
        throw C150Exception("structs.stub.cpp: string not null terminated or too long");

    
    string buffer(bufp);
    free(bufp);

    return buffer;
}

/* 
                SEND TO STREAM FUNCTIONS 
*/

void sendStringToStream(string s)
{
    RPCSTUBSOCKET->write(s.c_str(), s.length()+1);
}

void sendIntToStream(int i)
{
    sendStringToStream(to_string(i));
}

void sendFloatToStream(float f)
{
    sendStringToStream(to_string(f));
}
