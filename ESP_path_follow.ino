#include <SoftwareSerial.h>
#define led0 6
#define led1 7
#define led2 8
#define led3 9
#define rtime 5000
#define DEBUG true
SoftwareSerial esp8266(2,3); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.(2,3)
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                           // and the RX line from the esp to   the Arduino's pin 3
String esptext=""; //String to take AT commands to esp8266
int d,i;
int l=0;
String cnt;
char pinNumber;
char temp;
int k, j,q;
int err=0;
void emergency(void);
void set(void);
void run1(char);
void right(void);
void stop1(void);
void setup()
{ pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH); //pin 13 id led on arduino
  //Serial.begin(9600);
  esp8266.begin(9600); 
    pinMode(7,OUTPUT);
  digitalWrite(7,LOW);
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
  pinMode(9,OUTPUT);
  digitalWrite(9,LOW);
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as station
sendData("AT+CIPMUX=1\r\n",10000,DEBUG); // configure for multiple connections
sendData("AT+CIPSERVER=1,80\r\n",3000,DEBUG); // turn on server on 
                                                port 80.
  digitalWrite(13,LOW);
  while(err==0) //loops until the path is set
  {
  set(); //This function takes a string of instructions from the HTML page
  }
}
 
void loop()
{ 
k=0; 
j=l; 
        for(i=0;i<l;i++) 
        { run1(cnt[i]); 
          emergency(); //To stop if the user presses Stop button
        }
          right();  //To turn the robot around
          delay(4900);
          stop1();
  
     for(q=0;q<l;q++) //After reaching the destination, replace 
                      all the Lefts are replaced by Rights
     {if(cnt[q]=='R')
     cnt[q]='L';
     else if(cnt[q]=='L')
     cnt[q]='R';
    }
       while(k<=(j-1)) //Loop to reverse the string of instructions
{   
  temp = cnt[k];
    cnt[k]=cnt[j-1];
    cnt[j-1]=temp;
    k++; 
    j--;
}
 emergency();
}

///*
//* Name: sendData
//* Description: Function used to send data to ESP8266.
//* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
//* Returns: The response from the esp8266 (if there is a response)
//*/
String sendData(String command, const int timeout, boolean debug) 
{
    String response = "";
   esp8266.print(command); // send the read character to the esp8266
   long int time = millis();
   while( (time+timeout) > millis())
    {while(esp8266.available())
   { // The esp has data so display its output to the serial window 
       char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    if(debug)
    {Serial.print(response);
    }
    return response;
}
//Name: forward
//Description: Moves the robot forward
void forward()
{  digitalWrite(led0,HIGH);
     digitalWrite(led1,LOW);
     digitalWrite(led2,HIGH);
     digitalWrite(led3,LOW);
}

//Name: backward
//Description: Moves the robot backward
void backward()
{
     digitalWrite(led0,LOW);
     digitalWrite(led1,HIGH);
     digitalWrite(led2,LOW);
     digitalWrite(led3,HIGH);
}
//Name: left
//Description: Moves the robot left
void left()
{
      digitalWrite(led0,LOW);
      digitalWrite(led1,HIGH);
      digitalWrite(led2,HIGH);
      digitalWrite(led3,LOW);  
}
//Name: right
//Description: Moves it right
void right()
{
      digitalWrite(led0,HIGH);
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,HIGH);
}
//
//Name:stop1
//Description: Stops the robot
void stop1()
{
      digitalWrite(led0,LOW);
      digitalWrite(led1,LOW);
      digitalWrite(led2,LOW);
      digitalWrite(led3,LOW);
}
//Name:readcnt
//Description: Concatenates a character to the string cnt
//Parameter: x-The character read by the wifi module coming from the html page
void readcnt(char x)
{cnt=cnt+x;
//Serial.print("read function cnt=");
//Serial.println(cnt);
  }
//Function to move the robot 
void run1(char x) 
{   
    switch(x)
  {case  'F':  forward();
               delay(rtime);
               stop1();
                 break;
   case 'B':  
              backward();
              delay(rtime);
              stop1();
              break;
   case 'R': right();
            delay(3000);
            stop1();
    break;
   case 'L':left();
           delay(3000);
           stop1();
            break; 
   case 'S':  stop1();
                break;
    }
  stop1();
   }
//Name:set
//Description: Reads the characters sent from the html page to the wifi module  
void set()
{int st=0;
while(st==0)
{
 
 if(esp8266.available()) // check if the esp is sending a message 
  {//Serial.print("Here");
    if(esp8266.find("+IPD,"))
    {
     {delay(1000); // wait for the serial buffer to fill up (read all the serial data)
      // get the connection id so that we can then disconnect
      int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48
      
     esp8266.find("pin=");
     
      

      pinNumber = esp8266.read();      // pinNumber += (esp8266.read()-48); // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number*/
          digitalWrite(10,HIGH);
     switch(pinNumber)
     {case  'F': readcnt('F');
                  run1('F');   
                 l++;                
                 break;
      case 'B': readcnt('B');
                 run1('B');
                l++;
                break;
      case 'R': readcnt('R');
                run1('R');
                l++;
                break;
      case 'L': readcnt('L');
                run1('L');
                l++;
                break; 
     case 'S':  readcnt('S');
                run1('S');
                l++;
                break;
      case 'C':st=1;
                err=1;
                break;
      }
      digitalWrite(10,LOW);
          
      // make close command
//      String closeCommand = "AT+CIPCLOSE=";
//      closeCommand+=connectionId; // append connection id
//      closeCommand+="\r\n";
//      
//      sendData(closeCommand,1000,DEBUG); // close connection
      }
    }    
  }
  }
}
//Function to stop the robot for emergencies
void emergency()
{
  if(esp8266.available()) // check if the esp is sending a message 
  {
   if(esp8266.find("+IPD,"))
    {
     {
      delay(1000); // wait for the serial buffer to fill up (read all the serial data)
      // get the connection id so that we can then disconnect
      int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns
      // the ASCII decimal value and 0 (the first decimal number) starts at 48
        esp8266.find("pin=");
         pinNumber = esp8266.read(); // get first number i.e. if the pin 13 then the 1st number is 1, then multiply to get 10
     digitalWrite(10,HIGH);
     switch(pinNumber)
     {
     case 'S':     
                run1('S');
                delay(5000);
                
                break;
            }
      digitalWrite(10,LOW);
     // digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin 
         
      // make close command
//      String closeCommand = "AT+CIPCLOSE=";
//      closeCommand+=connectionId; // append connection id
//      closeCommand+="\r\n";
//      
//      sendData(closeCommand,1000,DEBUG); // close connection
      }
    }   }}
