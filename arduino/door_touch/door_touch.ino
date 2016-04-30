#include <ESP8266WiFi.h>
 
const char* ssid = "Mi4";
const char* password = "1234567890";
 
int Pin = 14; //D5 IR sensor
int Pin1 = 12 ;// D6 IR sensor
int cap = 5; // D1 Touch sensor
int flag = 0;
int flag1 = 0;
WiFiServer server(80);
int i = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(Pin, INPUT);
  pinMode(cap,INPUT);
  digitalWrite(cap,LOW);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}
 
void loop() {
// Check if a client has connected
WiFiClient client = server.available();

// New logic IR sensor
  if( digitalRead(Pin) == HIGH ) // T1 state  
  {
    if(flag1 == 1)
    {
      i--;
      flag1 = 0;
     //delay(1000);
     Serial.println(i); 
     Serial.println("Exit"); 
    }
    else
    {
      flag = 1;
    }
  }

    if( digitalRead(Pin1) == HIGH )
  {
    if(flag == 1)
    {
      i++;
      flag = 0;
      //delay(1000);
      Serial.println(i); 
      Serial.println("Entry"); 
     }
    else
    {
      flag1 = 1;
    }
  }
  client.print(i); 
  
  // Touch sensor 
  if(digitalRead(cap)== LOW) 
    {
      Serial.println("No one is present at sensor1 ");
    }
    else
    Serial.println("Someone is present at sensor1");
delay(1000); 
}
