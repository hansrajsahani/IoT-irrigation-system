// Blynk IOT Smart Plant Monitoring System


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include<Servo.h>

Servo myservo;

char auth[] = "u2KA7yDARSi8zJmZ068mBamhK7SXTyIH";  //Enter your Blynk Auth token
char ssid[] = "OnePlus Nord CE 3 Lite 5G";  //Enter your WIFI SSID
//char ssid[] = "realme 10 Pro+ 5G";
char pass[] = "d29ukzux";  //Enter your WIFI Password
//char pass[] = "16062002";

DHT dht(D2, DHT11);//(DHT sensor pin,sensor type)  D4 DHT11 Temperature Sensor
BlynkTimer timer;

//select line pin
#define S0 D3
#define S1 D4
#define S2 D5
#define S3 D6

#define servopin D8

#define soil A0     //A0 Soil Moisture Sensor

#define pump D1


//Create three variables for pressure
double T, P;
char status;

void setup()
{
  Serial.begin(9600);
  
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  delay(500);

  myservo.attach(servopin);
  pinMode(servopin, OUTPUT);

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(A0, INPUT);

  pinMode(pump, OUTPUT);
  
  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
  timer.setInterval(100L, DHT11sensor);
}


/*********************Get the DHT11 sensor values***********************/
void DHT11sensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);
}


/*********************Get the soil moisture values*********************/
void soilMoistureSensor()
{
  /*********start soil moisture 1*************/
  
  //for channel 15 of mux -> 1 1 1 1
  digitalWrite(S0,HIGH);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
   
  int value1 = analogRead(soil);
  value1 = map(value1, 0, 1024, 0, 100);
  value1 = (value1 - 100) * -1;
  Blynk.virtualWrite(V0, value1);

  if(value1 <= 20 )
  {
    myservo.write(180);
    while(value1 <= 60)
    {
      digitalWrite(pump, HIGH);
      delay(100);
      value1 = analogRead(soil);
      value1 = map(value1, 0, 1024, 0, 100);
      value1 = (value1 - 100) * -1;
      Blynk.virtualWrite(V0, value1);  
    }
    digitalWrite(pump, LOW);
    delay(1000);
  }
  

  /************ end soil moisture 1 *************/

  /*********** start soil moisture 2 ************/
  
  //for channel 14 of mux -> 0 1 1 1
  digitalWrite(S0,LOW);
  digitalWrite(S1,HIGH);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
    
  int value2 = analogRead(soil);
  value2 = map(value2, 0, 1024, 0, 100);
  value2 = (value2 - 100) * -1;
  Blynk.virtualWrite(V3, value2);       //virtual pin 3 for soil moisture 2

  if(value2 <= 20 )
  {
    myservo.write(90);
    while(value2 <= 60)
    {
      digitalWrite(pump, HIGH);
      delay(100);
      value2 = analogRead(soil);
      value2 = map(value2, 0, 1024, 0, 100);
      value2 = (value2 - 100) * -1;
      Blynk.virtualWrite(V3, value2);  
    }
    digitalWrite(pump, LOW);
    delay(1000);
  }

  /*********** end soil moisture 2 ***************/

  /*********** start soil moisture 3 **************/

  //for channel 13 of mux -> 1 0 1 1
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
    
  int value3 = analogRead(soil);
  value3 = map(value3, 0, 1024, 0, 100);
  value3 = (value3 - 100) * -1;
  Blynk.virtualWrite(V4, value3);       //virtual pin 4 for soil moisture 3

  if(value3 <= 20 )
  {
    myservo.write(0);
    while(value3 <= 60)
    {
      digitalWrite(pump, HIGH);
      delay(100);
      value3 = analogRead(soil);
      value3 = map(value3, 0, 1024, 0, 100);
      value3 = (value3 - 100) * -1;
      Blynk.virtualWrite(V4, value3);  
    }
    digitalWrite(pump, LOW);
    delay(1000);
  }

  /************* end soil moisture 3 *************/
}  



BLYNK_CONNECTED() 
{
  
}


void loop() 
{      
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}
