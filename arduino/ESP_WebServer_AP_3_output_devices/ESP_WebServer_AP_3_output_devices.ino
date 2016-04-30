/*--------------------------------------------------
HTTP 1.1 Webserver as AccessPoint for ESP8266 
for ESP8266 adapted Arduino IDE

by Stefan Thesen 08/2015 - free for anyone

Does HTTP 1.1 with defined connection closing.
Handles empty requests in a defined manner.
Handle requests for non-exisiting pages correctly.

This demo allows to switch two functions:
Function 1 creates serial output and toggels GPIO2
Function 2 just creates serial output.

Serial output can e.g. be used to steer an attached
Arduino, Raspberry etc.
--------------------------------------------------*/

#include <ESP8266WiFi.h>

const char* ssid = "ESP-Accesspoint";
const char* password = "12345678";  // set to "" for open access point w/o passwortd

unsigned long ulReqcount;
const int DOOR = D0;     // the number of the pushbutton pin
const int PIR1 = D1;     // the number of the pushbutton pin
const int PIR2 = D2;     // the number of the pushbutton pin
const int ULTRA = D3;     // the number of the pushbutton pin
const int BUZZ = D4;     // the number of the pushbutton pin
const int FAN = D5;     // the number of the pushbutton pin
const int LIGHT = D6;     // the number of the pushbutton pin
const int LOCK = D7;     // the number of the pushbutton pin
unsigned int scount = 0;


// Create an instance of the server on Port 80
WiFiServer server(80);

void setup() 
{
  // setup globals
  ulReqcount=0; 

  
  // prepare GPIO2
  pinMode(LOCK, OUTPUT);
  digitalWrite(LOCK, 0);

  pinMode(BUZZ, OUTPUT);
  digitalWrite(BUZZ, 0);

  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, 0);

  pinMode(LIGHT, OUTPUT);
  digitalWrite(LIGHT, 0);

  
  pinMode(DOOR, INPUT);  
  pinMode(PIR1, INPUT);  
  pinMode(PIR2, INPUT);  
  pinMode(ULTRA, INPUT);  
  
  // start serial
  Serial.begin(9600);
  delay(1);
  
  // AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
}

void loop() 
{ 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  unsigned long ultimeout = millis()+250;
  while(!client.available() && (millis()<ultimeout) )
  {
    delay(1);
  }
  if(millis()>ultimeout) 
  { 
    Serial.println("client connection time-out!");
    return; 
  }
  
  // Read the first line of the request
  String sRequest = client.readStringUntil('\r');
  //Serial.println(sRequest);
  client.flush();
  
  // stop client, if request is empty
  if(sRequest=="")
  {
    Serial.println("empty request! - stopping client");
    client.stop();
    return;
  }
  
  // get path; end of path is either space or ?
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sPath="",sParam="", sCmd="";
  String sGetstart="GET ";
  int iStart,iEndSpace,iEndQuest;
  iStart = sRequest.indexOf(sGetstart);
  if (iStart>=0)
  {
    iStart+=+sGetstart.length();
    iEndSpace = sRequest.indexOf(" ",iStart);
    iEndQuest = sRequest.indexOf("?",iStart);
    
    // are there parameters?
    if(iEndSpace>0)
    {
      if(iEndQuest>0)
      {
        // there are parameters
        sPath  = sRequest.substring(iStart,iEndQuest);
        sParam = sRequest.substring(iEndQuest,iEndSpace);
      }
      else
      {
        // NO parameters
        sPath  = sRequest.substring(iStart,iEndSpace);
      }
    }
  }
  
  ///////////////////////////////////////////////////////////////////////////////
  // output parameters to serial, you may connect e.g. an Arduino and react on it
  ///////////////////////////////////////////////////////////////////////////////
  if(sParam.length()>0)
  {
    int iEqu=sParam.indexOf("=");
    if(iEqu>=0)
    {
      sCmd = sParam.substring(iEqu+1,sParam.length());
      Serial.println(sCmd);
    }
  }
  
  
  ///////////////////////////
  // format the html response
  ///////////////////////////
  String sResponse,sHeader;
  
  ////////////////////////////
  // 404 for non-matching path
  ////////////////////////////
  if(sPath!="/")
  {
    sResponse="<html><head><title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";
    
    sHeader  = "HTTP/1.1 404 Not found\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  ///////////////////////
  // format the html page
  ///////////////////////
  else
  {
    ulReqcount++;
    sResponse  = "<html><head><title>Demo ESP8266 </title><meta http-equiv=\"refresh\" content=\"3; URL=http://192.168.4.1\"></head><body>";
    sResponse += "<font color=\"#000000\"><body bgcolor=\"#d0d0f0\">";
    sResponse += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=yes\">";
    sResponse += "<h1>Home Automation Demo</h1>";
    sResponse += "Function 1 GPIO2 High.<BR>";
    sResponse += "Function 2.<BR>";

    if(digitalRead(DOOR) == HIGH)
      {
        sResponse += "DOOR is high.<BR>";  
        scount = 1;
       }
    else
      {
        sResponse += "DOOR is Low.<BR>";
        scount--;
      }
    if(digitalRead(PIR1) == HIGH)
      {
        sResponse += "PIR1 is high.<BR>"; 
        scount = 2;  
      }
    else
        {
          sResponse += "PIR1 Low.<BR>";
          scount--;
        }

    if(digitalRead(PIR2) == HIGH)
        {
          sResponse += "PIR2 is high.<BR>";  
          scount = 3;
        }
    else
        {
          sResponse += "PIR2 Low.<BR>";
          scount--;
        }

    if(digitalRead(ULTRA) == HIGH)
        {
          sResponse += "Some one entered<BR>";  
          scount = 4;
        }
    else
        {
          sResponse += "No One entered<BR>";
          scount--;
        }

        if((digitalRead(DOOR) == HIGH)||(digitalRead(PIR1) == HIGH)||(digitalRead(PIR2) == HIGH)||(digitalRead(ULTRA) == HIGH) )
          digitalWrite(BUZZ,0);
        else
          digitalWrite(BUZZ, 1);     
          Serial.println(scount);
    sResponse += "<FONT SIZE=+1>";
    sResponse += "<p>LIGHT <a href=\"?pin=FUNCTION1ON\"><button>High</button></a>&nbsp;<a href=\"?pin=FUNCTION1OFF\"><button>LOW</button></a></p>";
    sResponse += "<p>FAN <a href=\"?pin=FUNCTION2ON\"><button>HIGH</button></a>&nbsp;<a href=\"?pin=FUNCTION2OFF\"><button>LOW</button></a></p>";
    sResponse += "<p>DOOR <a href=\"?pin=FUNCTION3ON\"><button>HIGH</button></a>&nbsp;<a href=\"?pin=FUNCTION3OFF\"><button>LOW</button></a></p>";
    
    //////////////////////
    // react on parameters
    //////////////////////
    if (sCmd.length()>0)
    {
      // write received command to html page
      sResponse += "Command:" + sCmd + "<BR>";
      
      // switch GPIO
      if(sCmd.indexOf("FUNCTION1ON")>=0)
      {
        digitalWrite(LIGHT, 1);
      }
      else if(sCmd.indexOf("FUNCTION1OFF")>=0)
      {
        digitalWrite(LIGHT, 0);
      }

      else if(sCmd.indexOf("FUNCTION2ON")>=0)
      {
        digitalWrite(FAN, 1);
      }
      else if(sCmd.indexOf("FUNCTION2OFF")>=0)
      {
        digitalWrite(FAN, 0);
      }

      else if(sCmd.indexOf("FUNCTION3ON")>=0)
      {
        digitalWrite(LOCK, 1);
      }
      else if(sCmd.indexOf("FUNCTION3OFF")>=0)
      {
        digitalWrite(LOCK, 0);
      }


    }
    
    sResponse += "<FONT SIZE=-2>";
    sResponse += "<BR>Aufrufz&auml;hler="; 
    sResponse += ulReqcount;
    sResponse += "<BR>";
    sResponse += "Stefan Thesen 08/2015<BR>";
    sResponse += "</body></html>";
    
    sHeader  = "HTTP/1.1 200 OK\r\n";
    sHeader += "Content-Length: ";
    sHeader += sResponse.length();
    sHeader += "\r\n";
    sHeader += "Content-Type: text/html\r\n";
    sHeader += "Connection: close\r\n";
    sHeader += "\r\n";
  }
  
  // Send the response to the client
  client.print(sHeader);
  client.print(sResponse);
  
  // and stop the client
  client.stop();
  Serial.println("Client disonnected");
}

