
#include <WiFi.h>
#include <HTTPClient.h>

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

const char* ssid     = "NetComm 9669";
const char* password = "gepedspsbu";

const char* serverName = "http://192.168.20.29:3000";

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

//  
//  delay(1000);

//Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

      // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;
    
    // Convert to inches
    distanceInch = distanceCm * CM_TO_INCH;
    
    // Prints the distance in the Serial Monitor
//    Serial.print("Distance (cm): ");
//    Serial.println(distanceCm);
//    Serial.print("Distance (inch): ");
//    Serial.println(distanceInch);
    
    
    // If you need an HTTP request with a content type: application/json, use the following:
    http.addHeader("Content-Type", "application/json");

    char distance_char[10];
    snprintf(distance_char, sizeof(distance_char), "%f", distanceCm);
//    String httpRequestData = "api_key="  array  "";

    std::string json("{\"distance\":");
    json += distance_char;
    json += "}";
    Serial.println(json.c_str());
    int httpResponseCode = http.POST(json.c_str());
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(1000); 
}
