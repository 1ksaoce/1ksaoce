#include <Arduino.h>
#include <WiFi.h>
#include <stdlib.h>
#include <Servo.h>
//Firebase Library
#include <Firebase.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

//Firebase Declare
#define API_KEY "yOCt9u60Y2BPBRag3JISLFlc0x1iEnZblPwiJ8MZ"
#define DATABASE_URL "https://esp32-ac263-default-rtdb.firebaseio.com/"

#define WIFI_SSID "DUC DUY_VNPT"
#define WIFI_PASSWORD "duc06012002"

//input den phong khach
#define DenPK 4

//input den doi mau phong khach
#define BluePk 0
#define GreenPk 2
#define RedPK 15

//input quat phong ngu
#define In1 18
#define In2 19
#define Ena 5

//input den phong ngu
#define DenPN 26

//input den doi mau phong ngu
#define BluePN 27
#define GreenPN 14
#define RedPN 12

// input cam bien gas phong bep
#define DenPB 34
#define analog 32
#define digital 33

//input cam bien phong tam
#define DenPT

const int freq = 5000;  // tần số xung
const int ledChannel = 0; // kênh PWM
const int resolution = 8; // độ phân giải 8bit

//delay
unsigned long previousMillis = 0;     
const long interval = 1000; // giá trị delay (milliseconds)

int dataquat = 0;
int datadendoimaupn = 0;
int datadendoimaupk = 0;
int datadenpk = 0;
int datadenpn = 0 ;
int datahengiopk = 0;
int datahengiopn = 0;
int datahengioquat = 0;
int dataspeed = 0;
int datahengiopt,datahengiopb;
int datadenpt,datadenpb;
int ledState = LOW; 

int port = 8888;
WiFiServer Server(port);
FirebaseData firebaseData;


// dang nhap wifi
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
  Serial.println("on port");
  Serial.println(port);
}

// ket noi firebase 
void initFirebase(){
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.reconnectWiFi(true);
}
  String data5base  = "";

void initden(int dataden,int Den,int datahengio){
  pinMode(Den,OUTPUT);
  if (dataden == 1){
    digitalWrite(Den, HIGH);
  }else if (dataden == 0){
    digitalWrite(Den, LOW);
  }else {
    datahengio = data5base.toInt();
    if (datahengio == 1){
      digitalWrite(Den, HIGH);
    } else {
      digitalWrite(Den, LOW);
    }
  }
}
void initquat(){
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Quat"),&data5base)){
      dataquat = data5base.toInt();
  } 
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Speedfan"),&data5base)){
    dataspeed = data5base.toInt();
  }
  if(dataquat == 1){
    if (dataspeed == 1){
      //nhanh nhat    
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);
      ledcWrite(ledChannel, 255);
    }else if(dataspeed == 2){
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);      
      ledcWrite(ledChannel, 200);
    }else if(dataspeed == 3){
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , HIGH);      
      ledcWrite(ledChannel, 150);
    }
  }else if (dataquat == 0){
    //tat
      digitalWrite(In1 , LOW);
      digitalWrite(In2 , LOW); 
  }else {
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Hengioquat"),&data5base)){
    datahengioquat = data5base.toInt();
      if (datahengioquat == 1){
        if (dataspeed == 1){
      //nhanh nhat    
        digitalWrite(In1 , LOW);
        digitalWrite(In2 , HIGH);
        ledcWrite(ledChannel, 255);
      }else if(dataspeed == 2){
        digitalWrite(In1 , LOW);
        digitalWrite(In2 , HIGH);      
        ledcWrite(ledChannel, 200);
      }else if(dataspeed == 3){
        digitalWrite(In1 , LOW);
        digitalWrite(In2 , HIGH);      
        ledcWrite(ledChannel, 150);
      }
      }else {
        digitalWrite(In1 , LOW);
        digitalWrite(In2 , LOW); 
     
      }
    }
  }
}
void showRGB(int Red, int Blue,int Green)
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

void initdendoimau(int datadendoimau,int Red,int Blue,int Green){
  pinMode(Red,OUTPUT);
  pinMode(Green,OUTPUT);
  pinMode(Blue,OUTPUT);
  switch (datadendoimau){
    case 1: 
      //mau do
      Serial.println("Turn on Red light");   
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
    showRGB(Red,Blue,Green); 
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
void Phongkhach(){
  if (Firebase.RTDB.getString(&firebaseData, F("/phong khach/dendoimau"),&data5base)){
    datadendoimaupk = data5base.toInt();
    initdendoimau(datadendoimaupk,RedPK,BluePk,GreenPk);
  }
  //den
  if (Firebase.RTDB.getString(&firebaseData, F("/phong khach/Den"),&data5base)){
    datadenpk = data5base.toInt();
    if (Firebase.RTDB.getString(&firebaseData, F("/phong khach/Hengio"),&data5base)){
    datahengiopk = data5base.toInt();
    initden(datadenpk,DenPK,datahengiopk);
   }
  }   
}
void Phongngu(){
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/dendoimau"),&data5base)){
    datadendoimaupn = data5base.toInt();
    initdendoimau(datadendoimaupn,RedPN,BluePN,GreenPN);
  }
  //den
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Den"),&data5base)){
    datadenpn = data5base.toInt();
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Hengio"),&data5base)){
    datahengiopn = data5base.toInt();
    initden(datadenpn,DenPN,datahengiopn);
    }
  }
}
void Phongbep(){
  int value = analogRead(analog);
  Serial.println(value);
  delay(500);
  if (value >= 1000){
    Firebase.setInt (firebaseData, "/Phong bep/Sensorgas" ,1);
    delay(500);
  }else{
    Firebase.setInt (firebaseData, "/Phong bep/Sensorgas" ,0);
    delay(500);
  }
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Den"),&data5base)){
    datadenpb = data5base.toInt();
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Hengio"),&data5base)){
    datahengiopb = data5base.toInt();
    initden(datadenpb,DenPB,datahengiopb);
    }
  } 
}
void Phongtam(){
   if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Den"),&data5base)){
    datadenpt = data5base.toInt();
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Hengio"),&data5base)){
    datahengiopt = data5base.toInt();
    initden(datadenpt,DenPB,datahengiopt);
    }
  } 
}
void setup() {
  // put your setup code here, to run once:
  initWiFi();
  initFirebase();
  
  //init quat
  pinMode(Ena,OUTPUT);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  digitalWrite(Ena, LOW);
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  
  // setup kênh pwm
  ledcSetup(ledChannel, freq, resolution);
  // xuất pwm ra chân 14
  ledcAttachPin(Ena, ledChannel);
}
void loop() {
  initquat();
  Phongngu();
  Phongkhach();
  Phongbep();
  Phongtam();
  // put your main code here, to run repeatedly:
}
