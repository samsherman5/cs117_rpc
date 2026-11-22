// --------------------------------------------------------------
//
//                        simplefunctionclient.cpp
//
//        Author: Noah Mendelsohn         
//   
//
//        This is a test program designed to call a few demonstration
//        functions, after first enabling the COMP 150-IDS rpcproxyhelper.
//        (The purpose of the helper is to open a TCP stream connection
//        to the proper server, and to leave the socket pointer where
//        the generated proxies can find it.
//
//        NOTE: Although this example does nothing except test the
//        functions, we may test your proxies and stubs with client
//        applications that do real work. 
//
//        NOTE: When actually testing your RPC submission, you will use
//        a different client application for each set of functions. This
//        one is just to show a simple example.
//
//        NOTE: The only thing that makes this different from 
//        an ordinary local application is the call to
//        rpcproxyinitialize. If you commented that out, you could
//        link this with the local version of simplefunction.o
//        (which has the remotable function implementations)							    
//
//        COMMAND LINE
//
//              structsclient <servername> 
//
//        OPERATION
//
//
//       Copyright: 2012 Noah Mendelsohn
//     
// --------------------------------------------------------------


// IMPORTANT! WE INCLUDE THE IDL FILE AS IT DEFINES THE INTERFACES
// TO THE FUNCTIONS WE'RE REMOTING. OF COURSE, THE PARTICULAR IDL FILE
// IS CHOSEN ACCORDING TO THE TEST OR APPLICATION
// 
// NOTE THAT THIS IS THE SAME IDL FILE INCLUDED WITH THE PROXIES
// AND STUBS, AND ALSO USED AS INPUT TO AUTOMATIC PROXY/STUB
// GENERATOR PROGRAM



#include "rpcproxyhelper.h"

#include "c150debug.h"
#include "c150grading.h"
#include <fstream>
#include <string>

using namespace std;          // for C++ std library
using namespace C150NETWORK;  // for all the comp150 utilities 

#include "structs.idl"

// forward declarations
void setUpDebugLogging(const char *logname, int argc, char *argv[]);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                    Command line arguments
//
// The following are used as subscripts to argv, the command line arguments
// If we want to change the command line syntax, doing this
// symbolically makes it a bit easier.
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

const int serverArg = 1;     // server name is 1st arg


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                           main program
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
int main(int argc, char *argv[]) {

    //  Set up debug message logging
    setUpDebugLogging("structsclientdebug.txt", argc, argv);

    // Make sure command line looks right
    if (argc != 2) {
        fprintf(stderr,"Correct syntxt is: %s <servername> \n", argv[0]);
        exit(1);
    }

    //  DO THIS FIRST OR YOUR ASSIGNMENT WON'T BE GRADED!
    GRADEME(argc, argv);

    //     Call the functions and see if they return
    try {
        // Set up the socket so the proxies can find it
        rpcproxyinitialize(argv[serverArg]);

        // Call (possibly remote) func1
        rectangle r;
        r.x = 4;
        r.y = 5;
        printf("Calling area()\n");
        int result = area(r);                          // remote call (we hope!)
        printf("Returned from area(): %d\n", result);

        // CALLING findPerson
        ThreePeople tp;
        Person person1;
        person1.firstname = "Megan";
        person1.firstname = "Monroe";
        person1.age = 9000;

        Person person2;
        person2.firstname = "Mark";
        person2.lastname = "Sheldon";
        person2.age = 64;

        Person person3;
        person3.firstname = "Noah";
        person3.firstname = "Mendelsystems";
        person3.age = 30;
        tp.p1 = person1;
        tp.p2 = person2;
        tp.p3 = person3;

        printf("Calling findPerson()\n");
        Person person = findPerson(tp);
        printf("Returned from findPerson()\n");

        Person person4;
        person4.firstname = "Shark";
        person4.firstname = "Meldon";
        person4.age = 128;
        Person person5;
        person5.firstname = "Sam";
        person5.firstname = "Sherman";
        person5.age = 12;
        Person person6;
        person6.firstname = "Amelia";
        person6.firstname = "Bermack";
        person6.age = 75;
        Person person7;
        person7.firstname = "Shepard";
        person7.firstname = "Rodgers";
        person7.age = 73847291;
        Person person8;
        person8.firstname = "Andrew";
        person8.firstname = "Rozenblit";
        person8.age = 45;
        Person person9;
        person9.firstname = "Lumen";
        person9.firstname = "Mayorga";
        person9.age = 890;
        Person person10;
        person10.firstname = "Repard";
        person10.firstname = "Shodgers";
        person10.age = 123456789;
        StructWithArrays swa;
        swa.aNumber = 0;
        swa.people[0] = person1;
        swa.people[1] = person2;
        swa.people[2] = person3;
        swa.people[3] = person4;
        swa.people[4] = person5;
        swa.people[5] = person6;
        swa.people[6] = person7;
        swa.people[7] = person8;
        swa.people[8] = person9;
        swa.people[9] = person10;

        printf("Calling ageSum()\n");
        int agesum = ageSum(swa);                         // remote call (we hope!)
        printf("Returned from ageSum(): %d\n", agesum);
    }
    //  Handle networking errors -- for now, just print message and give up!
    catch (C150Exception& e) {
        // Write to debug log
        c150debug->printf(C150ALWAYSLOG,"Caught C150Exception: %s\n",
                          e.formattedExplanation().c_str());
        // In case we're logging to a file, write to the console too
        cerr << argv[0] << ": caught C150NetworkException: " << e.formattedExplanation() << endl;
    }

    // Note that Linux will cleanly close the socket upon exit
    // as long as there is no unread data on the connection.

     return 0;
    
}



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
//                     setUpDebugLogging
//
//        For COMP 150-IDS, a set of standards utilities
//        are provided for logging timestamped debug messages.
//        You can use them to write your own messages, but 
//        more importantly, the communication libraries provided
//        to you will write into the same logs.
//
//        As shown below, you can use the enableLogging
//        method to choose which classes of messages will show up:
//        You may want to turn on a lot for some debugging, then
//        turn off some when it gets too noisy and your core code is
//        working. You can also make up and use your own flags
//        to create different classes of debug output within your
//        application code
//
//        NEEDSWORK: should be factored into shared code w/pingstreamserver
//        NEEDSWORK: document arguments
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
 
void setUpDebugLogging(const char *logname, int argc, char *argv[]) {

     //   
     //           Choose where debug output should go
     //
     // The default is that debug output goes to cerr.
     //
     // Uncomment the following three lines to direct
     // debug output to a file. Comment them
     // to default to the console.
     //
     // Note: the new DebugStream and ofstream MUST live after we return
     // from setUpDebugLogging, so we have to allocate
     // them dynamically.
     //
     //
     // Explanation: 
     // 
     //     The first line is ordinary C++ to open a file
     //     as an output stream.
     //
     //     The second line wraps that will all the services
     //     of a comp 150-IDS debug stream, and names that filestreamp.
     //
     //     The third line replaces the global variable c150debug
     //     and sets it to point to the new debugstream. Since c150debug
     //     is what all the c150 debug routines use to find the debug stream,
     //     you've now effectively overridden the default.
     //
     ofstream *outstreamp = new ofstream(logname);
     DebugStream *filestreamp = new DebugStream(outstreamp);
     DebugStream::setDefaultLogger(filestreamp);

     //
     //  Put the program name and a timestamp on each line of the debug log.
     //
     c150debug->setPrefix(argv[0]);
     c150debug->enableTimestamp(); 

     //
     // Ask to receive all classes of debug message
     //
     // See c150debug.h for other classes you can enable. To get more than
     // one class, you can or (|) the flags together and pass the combined
     // mask to c150debug -> enableLogging 
     //
     // By the way, the default is to disable all output except for
     // messages written with the C150ALWAYSLOG flag. Those are typically
     // used only for things like fatal errors. So, the default is
     // for the system to run quietly without producing debug output.
     //
     c150debug->enableLogging(C150ALLDEBUG | C150RPCDEBUG | C150APPLICATION | C150NETWORKTRAFFIC | 
                  C150NETWORKDELIVERY); 
}
