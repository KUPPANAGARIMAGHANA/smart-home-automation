#include <ESP8266WiFi.h>
#include <PubSubClient.h>

  void callback(char* subtopic, byte* payload, unsigned int payloadLength);

const char* ssid = "MLRITM@DGL";
const char* password = "digi@123";

#define ORG "zkmd4c"
#define DEVICE_TYPE "NODEMCU_12"
#define DEVICE_ID "1234"
#define TOKEN "Ashwitha-123"
String data;
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char subtopic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
/////////////////////////////////////////////////////////

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

#define led D1
void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(led,OUTPUT);
  wifiConnect();/* user defined
  */mqttConnect();
}

void loop() {
  if (!client.loop()) {
    mqttConnect();
  }
delay(100);
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) 
    {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() 
{
  if (client.subscribe(subtopic)) 
  {
    Serial.println("subscribe to cmd OK");
  } 
  else 
  {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* subtopic, byte* payload, unsigned int payloadLength) 
{
  Serial.print("callback invoked for topic: "); Serial.println(subtopic);

  for (int i = 0; i < payloadLength; i++) 
  {
    
    data += (char)payload[i];
  }
Serial.println(data);
if(data == "light_on")
{
  digitalWrite(led,HIGH);
  Serial.println("Light is Switched ON");
}
else if(data == "light_off")
{
  digitalWrite(led,LOW);
  Serial.println("Light is Switched OFF");
}
data ="";
}
