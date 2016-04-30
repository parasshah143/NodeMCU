#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "parasnodeMCU";
const char *password = "1234567890";
// defines variables
long duration;
int distance;
char msg[50];
ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() 
{
  server.send(200, "text/html", msg);
//  server.send(200, "text/html", "<h1>Distance is  </h1>");
  Serial.println("Start");
  Serial.println("Stop");
  Serial.println(msg);

}
 
 // defines pins numbers
const int trigPin = 4;  // D2
const int echoPin = 5;  // D1
const int led = 13;


void setup() 
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(led, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(led, LOW);
  Serial.print("Distance: ");
}

void loop() 
{ 
  // Clears the trigPin
 /* digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  */
  // Sets the trigPin on HIGH state for 10 micro seconds
 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH); 

  // Calculating the distance distance= duration*0.034/2;  

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  snprintf (msg, 75, "%ld",distance);
  if(distance < 50)
  {
  digitalWrite(led, HIGH);
  Serial.print("You are below 50 meter \n");
  delay(1000);
  }
  else
  {
  digitalWrite(led, LOW);
  Serial.print("You are above 50 meter \n");
  delay(1000);
  }
  server.handleClient();
   
}

