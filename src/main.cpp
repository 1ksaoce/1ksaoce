#include <Arduino.h>
#include <WiFi.h>

//Firebase Library
#include <Firebase.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//Firebase Declare
#define API_KEY "yOCt9u60Y2BPBRag3JISLFlc0x1iEnZblPwiJ8MZ"
#define DATABASE_URL "https://esp32-ac263-default-rtdb.firebaseio.com/"


#define WIFI_SSID "2304_PN2"
#define WIFI_PASSWORD "560280140"

//input den 
#define Den 12

//input den doi mau
#define Blue 0
#define Green 2
#define Red 15

//input quat
#define In1 14
#define In2 19
#define Ena 18


const int freq = 5000;  // tần số xung
const int ledChannel = 0; // kênh PWM
const int resolution = 8; // độ phân giải 8bit

int dataden = 0;
int dataquat = 0;
int datadendoimau = 0;

FirebaseData firebaseData ;

void initWiFi(){
  Serial.begin(115200);  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");                                                                                                                                                                                          
  while (WiFi.status() != WL_CONNECTED ){
  Serial.print(".");
  delay(1000);
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}
void initFirebase(){


  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.reconnectWiFi(true);
}
  String data5base = ""; 

void initden(){
  if (Firebase.RTDB.getString(&firebaseData, F("/Controldevice/Den"),&data5base)){
      dataden = data5base.toInt();
  }
  if (dataden == 1){
    digitalWrite(Den, HIGH);
  }else{
    digitalWrite(Den, LOW);
  }  
}

void initquat(){
   if (Firebase.RTDB.getString(&firebaseData, F("/Controldevice/Quat"),&data5base)){
      dataquat = data5base.toInt();
  }
  if (dataquat == 1){
      //nhanh nhat    
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);
      ledcWrite(ledChannel, 255);
    }else if(dataquat == 2){
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);      
      ledcWrite(ledChannel, 171);
    }else if(dataquat == 3){
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);      
      ledcWrite(ledChannel, 100);
    }else{
      //tat
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , LOW); 
    }
}
void showRGB()
{
	  digitalWrite(Red, HIGH);
    digitalWrite(Blue, LOW);
    digitalWrite(Green, LOW);
    delay(200);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, LOW);
    digitalWrite(Green, HIGH);
    delay(200);
    digitalWrite(Red, LOW);
    digitalWrite(Blue, HIGH);
    digitalWrite(Green, LOW);
    delay(200);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, LOW);
    digitalWrite(Green, HIGH);
    delay(200);
    digitalWrite(Red, LOW);
    digitalWrite(Blue,  HIGH);
    digitalWrite(Green, HIGH);
    delay(200);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, HIGH);
    digitalWrite(Green, LOW);
    delay(200);
    digitalWrite(Red, HIGH);
    digitalWrite(Blue, HIGH);
    digitalWrite(Green, HIGH);
    delay(200);  
}
  
void initdendoimau(){
  if (Firebase.RTDB.getString(&firebaseData, F("/Controldevice/den doi mau"),&data5base)){
    datadendoimau = data5base.toInt();
  }
  switch (datadendoimau){
    case 1: 
      //mau do
      Serial.println("Turn on red light");   
      digitalWrite(Red, HIGH);
      digitalWrite(Blue, LOW);
      digitalWrite(Green, LOW);
      break; 
    case 2: 
      //mau xanh la
      digitalWrite(Red, LOW);
      digitalWrite(Blue, LOW);
      digitalWrite(Green, HIGH);
      break; 
    case 3: 
      //mau XANH DUONG
      digitalWrite(Red, LOW);
      digitalWrite(Blue, HIGH);
      digitalWrite(Green, LOW);
      break; 
    case 4: 
      //mau VANG
      digitalWrite(Red, HIGH);
      digitalWrite(Blue, LOW);
      digitalWrite(Green, HIGH);
      break;
    case 5: 
      //mau LAM
      nhay:      
      digitalWrite(Red, LOW);
      digitalWrite(Blue,  HIGH);
      digitalWrite(Green, HIGH);
      break; 
    case 6: 
      //mau TIM
      digitalWrite(Red, HIGH);
      digitalWrite(Blue, HIGH);
      digitalWrite(Green, LOW);
      break;
    case 7:
       //mau trang
      digitalWrite(Red, HIGH);
      digitalWrite(Blue, HIGH);
      digitalWrite(Green, HIGH);
      break;
    case 8: 
    while(datadendoimau == 8){ 
    showRGB(); 
    goto nhay;
    break;      
    }                   
    case 0:  
    //TAT
      digitalWrite(Red, LOW);
      digitalWrite(Blue, LOW);
      digitalWrite(Green, LOW);
      break;
  }  
}

void setup() {
  // put your setup code here, to run once:
  initWiFi();
  initFirebase();
  pinMode(Den,OUTPUT);
  digitalWrite(Den, LOW);
  
  pinMode(Ena,OUTPUT);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  digitalWrite(Ena, LOW);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
    
  pinMode(Red,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(Blue,OUTPUT);
  
  digitalWrite(Red, LOW);
  digitalWrite(Blue, LOW);
  digitalWrite(Green, LOW);
  
  // setup kênh pwm
  ledcSetup(ledChannel, freq, resolution);
  // xuất pwm ra chân 14
  ledcAttachPin(Ena, ledChannel);
}
void loop() {
  initquat();
  initdendoimau();
  initden();
  // put your main code here, to run repeatedly:
}
