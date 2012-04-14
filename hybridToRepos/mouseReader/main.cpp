#include <stdlib.h>

#include <stdio.h>
#include <iostream>
#include <qstring.h>
#include <yarp/os/Network.h>
#include <yarp/os/Port.h>
#include <yarp/os/Bottle.h>

#include <linux/input.h>
#include <fcntl.h>




using namespace yarp::os;
using namespace std;


// to find the device:   cat /proc/bus/input/devices
#define MOUSEFILE "/dev/input/event10"

int z = 0;
int yaw = 0;
bool done = false;
QString retString = "d,0,0,";

int counterLeft = 0; 
int counterRight = 0;
int counterFront = 0;
int counterBack = 0; 


//--------------------------------------------------------------------------
void sendString(int code, int val, Port* output1, Port* output2){

Bottle bot;
bot.clear();


switch ( code )
  {
   case 0: //yaw
	{
           {
        if (val < 0){
            //retString = "d,-800,800,";
            counterLeft++;
            done = true;
            }
        

        if (val > 0){
            //retString = "d,800,-800,";
            counterRight++;
            done = true;    
        }


       if (counterLeft > 10){
       retString = "d,-800,800,";
       counterLeft = 0;
       }
 
       if (counterRight > 10){
       retString = "d,800,-800,";
       counterRight = 0;
       }
      }
        
        break;
      }

   case 1: //z
     {

       if (val < 5){
          retString =  "d,800,800,";
          //counterFront++;
          done = true; 
       }  
       
       else if (val > 5){
          retString = "d,-800,-800,";
          //counterBack++;
          done = true;    
        }


       if (counterFront > 15){
          retString = "d,800,800,";
          counterFront = 0; 
         } 

      if (counterBack > 15){
          retString = "d,-800,-800,";
          counterBack = 0;
        }

       }






// un-comment the folllow to print out 
   std::cout << retString << std::endl;   



        break;
      default:
        break;
  }


//if (!done) {
//   bot.addString(retString);
//   output2->write(bot);
//   std::cout << "stopping..:" << retString << std::endl;
//   return;
//}

if (code == 0){   // yaw
    bot.addString(retString);
    output2->write(bot);
    std::cout << "sending yaw..:" << retString << std::endl;
}

else if (code == 1){  // z
    bot. addString(retString);
    output1->write(bot);
    std::cout << "sending z..:" << retString << std::endl;
    }

 

}




//-----------------------------------------------------------------------------






int main()
{
   Network yarp;
   Port output1, output2;
   output1.open("/opticMouse/out/z");
   output2.open("/opticMouse/out/yaw");
   
   QString curString;

  int fd;
  struct input_event ie;

  if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
           perror("opening device");
           exit(EXIT_FAILURE);
}



int code, val;
double times, timemics; //time sec and micsec
while(read(fd, &ie, sizeof(struct input_event))) {

    // printf .....
    bool done = false; 

    if (ie.type == 2){
    code = ie.code;
    val = ie.value;
    times = ie.time.tv_sec;
    timemics = ie.time.tv_usec;
    sendString(code, val, &output1, &output2);    
    }
  }
  return 0;

}



