  /***************************************************/
 /*get yarp messages and write them to a serial port*/
/***************************************************/


#include <stdlib.h>
#include <iostream>
#include <qstring.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>
#include<yarp/os/all.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include <sstream>
 
//#include <SerialStream.h>
//#include <cstdlib>
//#include <string>


 
using namespace yarp::os;
using namespace std;

int fd;
int n;
QString curString;
 
int main(int argc, char *argv[]) 
{
   Network yarp;
   BufferedPort<Bottle> port;
   port.open("/robot/in/");


while (true){
    cout << "waiting" <<  endl;
    Bottle *curString = port.read();
 
const char *stringstring =  (const char *) curString;

int printf(const char* curString);
 
//const char text[10] = (const char *) curString;

   if (curString != NULL){
 
     cout << curString->toString().c_str() << endl;
 
      fd = open("/dev/ttyACM0", O_RDWR);	
//      fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);

      if (fd == -1)
      {
	perror("could not open: /dev/ttyUSB0 ");
      }
      else 
{
        n = write(fd, curString->toString().c_str(), 8);
       //n = write(fd, "ATZ\r", 5);
   if (n < 0)
    fputs("write() of 8 bytes failed!\n", stderr);
  else
   // printf ("Write succeed n = %i\n" );
      cout << n << endl;

      //return (fd);
            
       }
     close(fd);
      }
    } 
}
 




 



