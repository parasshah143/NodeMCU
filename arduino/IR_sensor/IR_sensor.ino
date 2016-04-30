  /**
 * Helloworld style, connect an ESP8266 to the IBM IoT Foundation
 * 
 * Author: Ant Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

//-------- Customise these values -----------
const char* ssid = "Mi4";
const char* password = "1234567890";
int Pin1 = 12 ;// D6 PIR
int i = 0;
long lastMsg = 0;
char msg[50];
int value = 0;
int Pin = 14; //D5 IR
#define ORG ""
#define DEVICE_TYPE "ESP8266"
#define DEVICE_ID "Test1"
#define TOKEN "<yourToken>"
const int trigPin = 4;  // D2
const int echoPin = 5;  // D1
int flag = 0;
int flag1 = 0;
int j = 0;
void setd();
long duration;
int distance;
int fdistance;
int dis[5];
//int i = 0;
//-------- Customise the above values --------

char server[] = ORG "192.168.43.196";
char topic[] = "topic";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setup() {
   pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
   pinMode(Pin, INPUT);
  pinMode(Pin1,INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
//  pinMode(led, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 Serial.begin(115200);
 Serial.println();
pinMode(Pin1,INPUT);
  Serial.print("Distance: ");
  Serial.print("setd ");
 Serial.print("Connecting to ");
 Serial.print(ssid);
 if (strcmp (WiFi.SSID(), ssid) != 0) {
 WiFi.begin(ssid, password);
 }
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");

 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
//  setd();
}

int counter = 0;

void loop() {
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }

 String payload = "{\"d\":{\"myName\":\"ESP8266.Test1\",\"counter\":";
 payload += counter;
 payload += "}}";
 
 Serial.print("Sending payload: ");
 Serial.println(payload);
 
// if (client.publish(topic, (char*) payload.c_str())) {
// Serial.println("Publish ok");
// } else {
// Serial.println("Publish failed");
// }

 if( digitalRead(Pin) == HIGH ) // IR detect
  {
      j++;
//      delay(250);
      Serial.println(j); 
      snprintf (msg, 75, "%ld",j);
      client.publish("topic", msg);
//      client.publish("topic", "IR");
      Serial.println("Entry1"); 
         delay(5);
  }

    
    
    if( digitalRead(Pin1) == HIGH ) // IR
  {
      client.publish("topic", "Motion");
      Serial.println("Motion"); 
         delay(5);
  }
//

//  //Ultrasonic part
//    // Sets the trigPin on HIGH state for 10 micro seconds
//  digitalWrite(trigPin, HIGH); 
//  delayMicroseconds(10);
//  digitalWrite(trigPin, LOW);
//  
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration = pulseIn(echoPin, HIGH); 
//
//  // Calculating the distance
//  distance= duration*0.034/2;  
//
//  // Prints the distance on the Serial Monitor
//  Serial.print("Distance: ");
//  Serial.println(distance);
//  if(distance < fdistance)
////  if(distance < 250)
//  {
////  digitalWrite(led, HIGH);
//  Serial.println("valid");
//  client.publish("topic", "Ultrasonic detected");
//  Serial.println("Ultrasonic detected"); 
//  delay(50);
//  }
//  else
//  {
////    digitalWrite(led, LOW);
//    Serial.println("invalid");
//  }
////  delay(1000);
////Ultrasonic finish

  
// ++counter;
 delay(10);
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.println("callback invoked");
}



void setd()
{
  for(i = 0; i<=5;i++)
  {
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH); 
  
    // Calculating the distance
    distance= duration*0.034/2;  
  
    // Prints the distance on the Serial Monitor
    dis[i] = distance;
    Serial.print("Distance: ");
    Serial.println(i);
    Serial.println(dis[i]);
    Serial.println(" ");
    delay(2000);
  
  }
  Serial.println("Loop finished ");
  fdistance = (dis[0]+dis[1]+dis[2]+dis[3]+dis[4])/5;
  Serial.println(fdistance);
}


