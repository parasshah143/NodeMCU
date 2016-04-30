#include <ESP8266WiFi.h>
 
const char* ssid = "Mi4";
const char* password = "1234567890";

float val=0;
float ldr=0;
float voltage;

WiFiServer server(80);

void setup() 
{
  Serial.begin(115200);
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
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
 
void loop()
{
// Check if a client has connected
WiFiClient client = server.available();
val=analogRead(A0);
voltage = ((val*5)/1024);
Serial.print("voltage is ");
Serial.println(voltage);
delay(1500);
if(val>= 0 && voltage<= 1)
Serial.print("between 0-25 percent and ");
else if(voltage>1 && voltage<=2)
Serial.print("between 25-50 percent and ");
else if(voltage>2&& voltage<=3)
Serial.print("between 50-75 percent and ");
else if(voltage>3&& voltage<=4)
Serial.print("between 75-100 percent and ");
else if(voltage>4&& voltage<=5)
Serial.print("between 75-100 percent and ");
}
