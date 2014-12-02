#ifndef Stuff_h
#define Stuff_h

#include <Arduino.h>

#define DATA_1 (PORTC |=  0X01)    // DATA 1    // for UNO
#define DATA_0 (PORTC &=  0XFE)    // DATA 0    // for UNO
#define STRIP_PINOUT (DDRC=0xFF)    // for UNO



class Strip {
  public:
 
    Strip(){
      
      
    };

void send_strip(uint32_t data)
{
  int i;
  unsigned long j=0x800000;
  
 
  for (i=0;i<24;i++)
  {
    if (data & j)
    {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");    
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      
/*----------------------------*/
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");  
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");  
      __asm__("nop\n\t");  
      __asm__("nop\n\t");        
/*----------------------------*/      
      DATA_0;
    }
    else
    {
      DATA_1;
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");    
      DATA_0;
/*----------------------------*/      
       __asm__("nop\n\t");
      __asm__("nop\n\t");
      __asm__("nop\n\t");      
/*----------------------------*/         
    }

    j>>=1;
  }  
};

void reset_strip()
{
  DATA_0;
  delayMicroseconds(20);
};

void mySend(unsigned long data[10]){
  int j=0;
  uint32_t temp_data;
    noInterrupts();
    for (j=0;j<10;j++)
    {
      temp_data=data[j];
      send_strip(temp_data);
    }
    interrupts();

}; 
  
};

class LCD {
  public:
    SoftwareSerial* lcd;
    
    boolean lightIsOn;
  
    LCD(int RxPin, int TxPin, int baud){
      
      lcd = new SoftwareSerial(RxPin,TxPin); //rx, tx
      
      lcd->begin(baud);
      
      lcd->write(12);
      
            
    };
    
    void toggleLight(){
      if(lightIsOn)
        lightOff();
      else
        lightOn();
    
    };
    
    void lightOn(){
      lcd->write(17);
      lightIsOn = true;
    
    };
    
    void lightOff(){
      lcd->write(18);
      lightIsOn = false;
    };
    
    void setPos(unsigned int r,unsigned int c){
      if(r == 0)
        lcd->write(128 + c);
      else
        if(r == 1)
          lcd->write(148 + c);
        else{} 
  
     };
     
     void clear(){
       lcd->write(12); 
     };
     
     void print(String theString){
        lcd->print(theString); 
     };
     
     void print(int theInt){
        lcd->print(theInt); 
     };
     void print(unsigned long theLong){
        lcd->print(theLong); 
     };
     void print(unsigned long theLong, int format){
        lcd->print(theLong, HEX); 
     };
     
     
    
    
  
};



class Pattern {
     
	public:
	       
		String name;
		       
		int frameCount;
		       
		int durations[10];
		       
		unsigned long pattern[][10];
		
		Pattern(int newFrames){
		     
		  name = "Pattern";
			 
		  frameCount = newFrames;
			  
		  for(int i = 0; i < frameCount; i++)
		    for(int j = 0; j < 10; j++)            
		      setLED(i,j,0);
		};
		       
		void setDurations(int newDurations[], int length){
		  for(int i = 0; i < length; i++)
		    durations[i] = newDurations[i];
		};

		String getName(){
		  return name;
		};
		       
		void setName(String newName){         
		  name = newName;
		};
		
		boolean setLED(int thisFrame, int thisLED, unsigned long newValue){
		  if(thisFrame > frameCount)
		    return false;
		  else{
		    pattern[thisFrame][thisLED] = newValue;
		    return true;
		  }
		};
		
		unsigned long getLED(int thisFrame, int thisLED){
		 
		  return pattern[thisFrame][thisLED];
		};        
		
		void printPattern(){
			  
		  for(int i = 0; i < frameCount; i++){
		    for(int j = 0; j < 10; j++){
		      Serial.print(pattern[i][j], HEX);
		      Serial.print(", ");
			       
		    }         
		    Serial.println();
		  }
		};
};


class Project {
  public:
    LCD* lcd;
    
    Pattern* patterns[10];
    
    Strip* strip;

    int buttonPin[3];
    
    boolean buttonState[3];
    
    int pots[3];

    int patternCount;

    int currPattern;
    
    unsigned long lastPrint;

    Project(int RxPin, int TxPin, int baud, int b0, int b1, int b2){
      
      strip = new Strip();
     
      buttonPin[0] = b0;
      buttonPin[1] = b1;
      buttonPin[2] = b2;
      
      patternCount = 0;
      
      currPattern = 0;
      
      lastPrint = 0;
      
      patterns[0] = new Pattern(1);
      
      for(int i = 0; i < 3; i++){
        
        pinMode(buttonPin[i],INPUT_PULLUP);  
        
      }
      
      lcd = new LCD(RxPin,TxPin,baud);
      
      lcd->lightOn();
    };
    
    void readInputs(){
      for(int i = 0; i < 3; i++)
        buttonState[i] = !digitalRead(buttonPin[i]);

      pots[0] = analogRead(A1);
      pots[1] = analogRead(A2);
      pots[2] = analogRead(A3);    
        
    };
    
    void printInputs(){
  
      for(int i = 0; i < 3; i++){
        Serial.print(buttonState[i]);
        Serial.print(":");
      }
      Serial.println();
    
      for(int i = 0; i < 3; i++){
        Serial.print(pots[i]);
        Serial.print(":");
      }    
      Serial.println("\n");
  
    }; 
    
    void createNewPattern(){
        int thisFrame = 0;

        do{        
          readInputs();
          
          thisFrame = map(pots[0],0,680,1,10);
          
          if(thisFrame > 10)
            thisFrame = 10;

          if(thisFrame < 1)
            thisFrame = 1;
          
          if(millis() - lastPrint > 100){
            
            lcd->clear();
            lcd->print("# of Frames:");
            lcd->setPos(1,0);
            lcd->print(thisFrame);

            lastPrint = millis();            
          }
        
        }while(!buttonState[2]);
        
          patterns[patternCount] = new Pattern(thisFrame);
          
        for(int i = 0; i < patterns[patternCount]->frameCount; i++){
          for(int j = 0; j < 10; j++){
           
            unsigned long value = 0xffffff;
            do{
              
              readInputs();
              
              int r,g,b;
              
              r = map(pots[0],0,680,0,256);
              if(r < 0)
                r=0;
              if(r > 255)
                r=255;
                
              g = map(pots[1],0,680,0,256);
              if(g < 0)
                g=0;
              if(g > 255)
                g=255;
                
              b = map(pots[2],0,680,0,256);              
                if(b < 0)
                b=0;
              if(b > 255)
                b=255;            
                
              value = (r * 0x010000) + (g * 0x000100) + (b * 0x000001);
              if(millis() - lastPrint > 100){

            
                lcd->clear();
                lcd->print(value, 1);

                lcd->setPos(0,13);                
                lcd->print(i);
                lcd->print(",");
                lcd->print(j);
                lcd->setPos(1,0);

                
                lcd->print(r);
                lcd->print(":");
                lcd->print(g);
                lcd->print(":");
                lcd->print(b);              

                lastPrint = millis();  
              }
              
            }while(!buttonState[1]);
            
            patterns[patternCount]->setLED(i,j,value);
            
            while(buttonState[1]){readInputs();}
            
            
          }
        }
        
 
        
        patterns[patternCount]->printPattern();
        patternCount++;
      
    };
    
    void setCurrentPattern(){
      currPattern = map(pots[0],0,680,0,patternCount);
      
      if(currPattern > patternCount)
        currPattern = patternCount;
      
      if(currPattern < 0)
        currPattern = 0;
    };
    
    void printDisplay(){
      if(millis() - lastPrint > 100){
        patterns[currPattern]->printPattern();
        
        lcd->clear();
  
        lcd->print(currPattern);
        lcd->print("/");
        lcd->print(patternCount);
  
        lcd->print(" ");
  
        lcd->setPos(1,13);
        lcd->print("New");
        
        lastPrint = millis(); 
      }
  
    };  
};

#endif
