#include <avr/pgmspace.h>
#include <SoftwareSerial.h>	
#include "stuff.h"
    

unsigned long lastPrint = 0;

Project project(6,7,9600,2,3,4);

void setup(){ 
  
  project.strip->strip[0] = 0xff0000;
  project.strip->updateStrip();
  Serial.begin(9600); 
      
  project.lcd->lightOn();         

}
     
void loop(){
      
      project.readInputs();
      project.printDisplay();  
      
      if(project.buttonState[0] == 1)
        project.createNewPattern();        
   
      if(project.buttonState[1] == 1)       
        project.setCurrentPattern();
}


