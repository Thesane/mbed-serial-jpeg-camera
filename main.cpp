
#include "mbed.h"
#include "JPEGCamera.h"

Serial control(p28, p27); // tx, rx read from bluetooth module VERIFIED
JPEGCamera camera(p13, p14); // TX, RX to camera

//Camera time
Timer timer;

int main()
{
    int message = 0;
    
    control.baud(115200);
    
    //Camera Initialization and picture sizing
    timer.start();
    camera.setPictureSize(JPEGCamera::SIZE160x120);
    wait(3);
    //Continuously wait for commands
    while(1) 
    {
        //Check for bytes over serial from bluetooth/xbee
        if(control.readable()) 
        {
            message=control.getc();    
            //Take a picture (note that this takes a while to complete and will only only work for 999 pictures
            if (message==210)
            {
                if (camera.isReady()) 
                {
                    if (camera.takePicture()) 
                    {
                        int image_size = camera.getImageSize();
                        while (camera.isProcessing()) 
                        {
                            camera.processPicture(control);
                        }
                    }    
                    else 
                    {
                        control.printf("D 1 \n");
                    }
                } 
                else 
                {
                    control.printf("D 0 \n");
                }
            }
        }
    }
}
