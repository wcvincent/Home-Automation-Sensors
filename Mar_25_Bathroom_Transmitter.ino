// Bathroom Transmitter
// Send Humidity
// Send Temperature
// Type of Arduino Board Uno
// Led Power
// Led Internal fan
// Led Humidity Alert
// Device ID (1)

#include <VirtualWire.h>
#include "DHT.h"

// setup temp sensor
#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
const int fan_id_1_led = 11;                           // Remote Fan
const int power_led = 8;                          // Power Led
const int humid_id_1_led = 9;                       // Humidity
const int transmit_pin = 12;                      // Wireless Data Transmit
// Light Sensor Setup 
int ldr = A0;                                     // Analog pin to which LDR is connected
int ldr_value = 0;                                //that’s a variable to store LDR values
int ldr_sensitivity = 400;                        // Adjust for Sensitivity

int a;                                             // Value to hold ID of device
int b1;                                            // High Threshold Humidity
int c1;                                            // Low Threshold Humidity
int d;                                             // Delay in readsensors

// Setup Array
int array1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int array2[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// timing for reading temperature minimim 2 seconds = 2000
unsigned long intervaltemp = 5000; // the time we need to wait betwen reads of Temp Sensors
unsigned long previousMillis = 0; // millis() returns an unsigned long Temp Sensors

void setup() {
  vw_set_tx_pin(transmit_pin);
  vw_setup(2000);                                      // Bits per sec must be same receiver
  pinMode(fan_id_1_led, OUTPUT);                       // fan_id_1_led Humidity
  pinMode(power_led, OUTPUT);
  pinMode(humid_id_1_led, OUTPUT);
  digitalWrite(fan_id_1_led, LOW);
  digitalWrite(power_led, HIGH);
  digitalWrite(humid_id_1_led, LOW); 

 // Setup Serial Port 
  Serial.begin(9600);
  Serial.flush();
 // Set ID of Device 1 = Bathroom 2 = Cave 3 = Spare etc
  a = 1;                                          // Sets ID for Device
  b1 = 49;                                        // High Threshold Humidity
  c1 = b1-1;                                      // Low Threshold Humidity - 1 High
  d = 100;                                        // Delay between reading temperture
}
void loop() {
  readsensors();
  actions();
  senddata();
  testing();
  //plotgraph();
}
void readsensors() {
  if ((unsigned long)(millis() - previousMillis) >= intervaltemp);
  previousMillis = millis();

// read temperature and humidity
  array2[0] = dht.readHumidity();                // Bathroom Humumidity
  array2[1] = dht.readTemperature();             // Bathroom  Temp
  array2[2] = a;                                 // Send ID
  array2[3] = 0;                                 // Fan On Off
// read light level
  array1[3] = analogRead(ldr);
  ldr_value = analogRead(ldr);                   //Reads the ldr’s value through LDR   
  delay(d);
}
void actions(){
   if (array1[3] <= ldr_sensitivity &&  array2[0] <= b1) { 
   array2[3] = 1;
   digitalWrite(fan_id_1_led, HIGH); 
   digitalWrite(humid_id_1_led, LOW);     
   }
   if (array1[3] >= ldr_sensitivity&&  array2[0] <= b1) { 
   array2[3] = 0;
   digitalWrite(fan_id_1_led, LOW);
   digitalWrite(humid_id_1_led, LOW);   
   }  
   if (array1[3] <= ldr_sensitivity&&  array2[0] >= b1) { 
   array2[3] = 1;
   digitalWrite(fan_id_1_led, HIGH);
   digitalWrite(humid_id_1_led, HIGH);   
   } 
    if (array1[3] >= ldr_sensitivity&&  array2[0] >= b1) { 
   array2[3] = 1;
   digitalWrite(fan_id_1_led, HIGH);
   digitalWrite(humid_id_1_led, HIGH);   
   }  
}

void senddata() {
  vw_send((uint8_t *)array2, 8);
  vw_wait_tx();  
}
void testing() {
// send to serial temp humidity and LDR value
  Serial.print("Bathroom        Hum   ");
  Serial.println(array2[0]);
  Serial.print("Bathroom        Temp  ");
  Serial.println(array2[1]);
  Serial.print("Device          ID    ");
  Serial.println(array2[2]);
  Serial.print("Bathroom Light  Level ");
  Serial.println(ldr_value);
  Serial.println(" ");
  Serial.print("Bathroom        Fan    "); 
  Serial.println(array2[3]);
  Serial.println(" ");
  delay(1000);
} 
void plotgraph(){
    Serial.println(array2[0]);  
    delay(7500);
}

