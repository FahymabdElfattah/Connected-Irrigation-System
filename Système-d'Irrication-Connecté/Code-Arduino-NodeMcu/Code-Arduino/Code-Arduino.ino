#include <Wire.h>
#include <stdio.h>
#include <DHT.h>

String key[4]={"a","b","c","d","e","f"};
String valueSend[4]={"","----","",""};



String sendVar ="";

float airHumidity = 0.0 ,solHumidity  = 0.0 ,airTemperature = 0.0 ,waterLevel=0.0,light=0.0;

unsigned char stat = 0 ,pompeState = 0, usCmd = 0;
//________________________________CONFIGURATION ___________________________________________
#define DHTPIN 5            
#define DHTTYPE DHT11       
DHT dht(DHTPIN, DHTTYPE);
void setup() {
 //_____________________________CONFIGURATION OF GPIOs PINs _______________________________________
  pinMode(A0,INPUT);
  pinMode(5,INPUT);
  pinMode(7,OUTPUT);
  pinMode(9,INPUT);
 //_________join i2c bus with address 8___________________
 Wire.begin(8);
 //_________register receive event # register request event               
 Wire.onReceive(receiveEvent); 
 Wire.onRequest(requestEvent);
 //________start serial for debug________________________ 
 Serial.begin(115200);          
}
void loop() {
 
  if (solHumidity  < 50 || pompeState == 1){
    digitalWrite(7,HIGH);
    stat = 1;
  }else{
    digitalWrite(7,LOW);
    stat = 0;
  }
  //-----------------------

 solHumidity  = map(solHumidity ,4,679,0,100) ; 
 valueSend[0]= String(airTemperature) ;
 valueSend[1]= String(stat) ;
 valueSend[2]= String(airHumidity) ;
 valueSend[3]= String(solHumidity ) ;
 valueSend[4]= String(light) ;
 valueSend[5]= String(waterLevel) ;
  readSensor();
  
}
//____function that executes whenever data is received from master
void receiveEvent(int howMany) {
 pompeState="" ;
 while (0 <Wire.available()) {
    char c = Wire.read();      
    pompeState = c.toInt();
  }  
  //Serial.println(pompeState);           
}

//___function that executes whenever data is requested from master
void requestEvent() {
  for (int i = 0; i <4; i++) {
    sendVar = key[i];
    Wire.print(sendVar);
    sendVar = valueSend[i];
    Wire.print(sendVar);
  }
  sendVar ="z";
  Wire.print(sendVar);
}


void readSensor(){
  float h1 = dht.readHumidity();
  float h2 = analogRead(A1);
  float t1 = dht.readTemperature();
  float w = analogRead(A0);
  float ldr = analogRead(A2);
  if (isnan(h1)) {    
    
  }else {
    airHumidity = h1;
  }

  if (isnan(h2)) {    
    
  }else {
    solHumidity  = h2 ;
  }

  if (isnan(t1)) {    
    
  }else {
    airTemperature = t1 ;
  }
  if (isnan(w)) {    
    
  }else {
    waterLevel = w ;
  }
  if (isnan(ldr)) {    
    
  }else {
    light = ldr ;
  }
}