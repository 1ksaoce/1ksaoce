#include <Arduino.h>
#include <WiFi.h>
//Firebase Library
#include <Firebase.h>
#include <addons/RTDBHelper.h>
#include <Robojax_L298N_DC_motor.h>
//Firebase Declare
#define API_KEY "yOCt9u60Y2BPBRag3JISLFlc0x1iEnZblPwiJ8MZ"
#define DATABASE_URL "https://esp32-ac263-default-rtdb.firebaseio.com/"

#define WIFI_SSID "Ducnguyen"
#define WIFI_PASSWORD "00000000"

//Phong khach:
//input den phong khach
#define DenPK 4
//input den doi mau phong khach
#define BluePk 0
#define GreenPk 2
#define RedPK 15
//input cua
#define EnB 21
#define In3 22
#define In4 23
#define Door 1
#define sensor 33 //cam bien hong ngoai

//Phong ngu:
//input quat phong ngu
#define In1 18
#define In2 19
#define Ena 5
#define Fan 2
//input den phong ngu
#define DenPN 26
//input den doi mau phong ngu
#define BluePN 27
#define GreenPN 14
#define RedPN 12

//Phong bep:
// input cam bien gas phong bep
#define analog 34
//input den phong bep
#define DenPB 16
#define coi 25

//Phong tam:
//input cam bien phong tam
#define DenPT 13

hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

const int dongcua = 0;
const int mocua = 1;

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
int datadenpb = 0;
int datacua = 0;
int temp = 1;

int port = 8888;
WiFiServer Server(port);
FirebaseData firebaseData;
Robojax_L298N_DC_motor robot(In1, In2, Ena, In3, In4, EnB, true);


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
 int Sen; 
void initcua(int data){
  if(Sen == 1){
    if (data == temp){
     robot.brake(Door);
     delay(20);
    }else{
      if(data == 0){
        robot.rotate(Door, 100, mocua);
        delay(20);
      }else if(data == 1){
        robot.rotate(Door, 100, dongcua);
        delay(20);
     } 
   }
  }else{
    if(data = 0){
      robot.brake(Door);
    }else{
      robot.rotate(Door, 100, mocua);
      Firebase.setInt (firebaseData, "/phong khach/cua" ,0); 
      delay(20);
    }
  }
  temp = data;
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
      robot.rotate(Fan, 100, 1);
    }else if(dataspeed == 2){
      robot.rotate(Fan, 85, 1);
    }else if(dataspeed == 3){
      robot.rotate(Fan, 70, 1);
    }
  }else if (dataquat == 0){
    //tat
      robot.brake(Fan);
  }else {
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong ngu/Hengioquat"),&data5base)){
    datahengioquat = data5base.toInt();
      if (datahengioquat == 1){
        if (dataspeed == 1){
      //nhanh nhat    
        robot.rotate(Fan, 100, 1);
      }else if(dataspeed == 2){
        robot.rotate(Fan, 80, 1);
      }else if(dataspeed == 3){
        robot.rotate(Fan, 70, 1);
      }
      }else {
        robot.brake(Fan);
     
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
   if (Firebase.RTDB.getString(&firebaseData, F("phong khach/cua"),&data5base)){
    datacua = data5base.toInt();
    initcua(datacua);
  }
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
int value;
void IRAM_ATTR Sensorgas(){
  value = analogRead(analog);
  Serial.print("Sensorgas = ");
  Serial.println(value);
  Sen = digitalRead(sensor);
  Serial.print("Sensordoor = ");
  Serial.println(Sen);
  if (value > 1200){
    Serial.println("bat coi");
    digitalWrite(coi,HIGH);
  }else{
    digitalWrite(coi,LOW);
  }
}
void Phongbep(){
  if(value > 1200){
    Firebase.setString(firebaseData,"/Phong bep/Sensorgas",1);
    delay(50);
  }else{
    Firebase.setString(firebaseData,"/Phong bep/Sensorgas",0);
    delay(50);
  }
  if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Den"),&data5base)){
    datadenpb = data5base.toInt();
    if (Firebase.RTDB.getString(&firebaseData, F("/Phong bep/Hengio"),&data5base)){
    datahengiopb = data5base.toInt();
    initden(datadenpb,DenPB,datahengiopb);
    }
  } 
}
void setup() {
  // put your setup code here, to run once:
   timer = timerBegin(0, 80, true); // Timer 0, prescaler of 80 (1MHz clock)
  
  // Attach the onTimer function as the interrupt handler
  timerAttachInterrupt(timer, &Sensorgas, true);
  
  // Set the timer to trigger every 1 second (1000000 microseconds)
  timerAlarmWrite(timer, 1000000, true);
  
  // Start the timer
  timerAlarmEnable(timer);
  pinMode(sensor, INPUT);
  pinMode(coi,OUTPUT);
  initWiFi();
  initFirebase();
  robot.begin();
}
void loop() {
  initquat();
  Phongngu();
  Phongkhach();
  Phongbep();
  // put your main code here, to run repeatedly:
}
