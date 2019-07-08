#include <TimerOne.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "main.h"
#include <RCSwitch.h>
#include <SoftwareSerial.h>

SoftwareSerial wifi = SoftwareSerial(3, 2);
RCSwitch mySwitch = RCSwitch();

String txtSMS="", dataSim="", dataWifi="";

bool checkSMS=0, checkTK=0, checkId=0, checkbtn=0;
bool btnStatut=0, rtc=0;

void setup() {
  Serial.begin(9600);
  wifi.begin(9600);
  mySwitch.enableTransmit(10);
  
  pinMode(13, OUTPUT); led_off;
  pinMode(9, OUTPUT); rst_off;
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  init_sim(15);
  sendSMS("Tat an ninh");
  while(1) {
    if(checkSMS==1) {
      _delay(40);
      if(dataSim.indexOf("rst")>=0 || dataSim.indexOf("Rst")>=0) {
        //Serial.println("Rest esp");
        rst_on; delay(500); rst_off;
        sendSMS("Reset thanh cong.");
      }

      if(dataSim.indexOf("on")>=0 || dataSim.indexOf("On")>=0){
        if(btnStatut==0) {
          btnStatut=1; 
          led_on; 
          sendSMS("Bat an ninh");
          wifi.print("set1");
        }
      }

      if(dataSim.indexOf("off")>=0 || dataSim.indexOf("Off")>=0){
        if(btnStatut==1) {
          btnStatut=0; 
          led_off; 
          sendSMS("Tat an ninh");
          wifi.print("set0");
        }
      }

      if(dataSim.indexOf("check")>=0) {
        //Serial.println("check TK");
        KTTK();
      }

      checkSMS=0;
    }

    if(checkTK==1) {
      _delay(40);
      checkTK=0;

      txtSMS = dataSim.substring(dataSim.indexOf("STB"), dataSim.indexOf("d.")+2);
      sendSMS(txtSMS);
    }

    if(btnStatut==1) {
      delay(100);
      if(checkId==1) {
        if(dataWifi.indexOf("=1")>0) {
          txtSMS = "Co dot nhap san 1 -- " + dataWifi.substring(4);
          sendSMS(txtSMS);
        } else if(dataWifi.indexOf("=2")>0) {
          txtSMS = "Co dot nhap san 2 -- " + dataWifi.substring(4);
          sendSMS(txtSMS);
        } else if(dataWifi.indexOf("=3")>0) {
          txtSMS = "Co dot nhap san 3 -- " + dataWifi.substring(4);
          sendSMS(txtSMS);
        } else if(dataWifi.indexOf("=4")>0) {
          txtSMS = "Co dot nhap san 4 -- " + dataWifi.substring(4);
          sendSMS(txtSMS);
        }
        checkId=0;
        mySwitch.send(nbuzz, 24);
        delay(1000);
      }
    } else {
      checkId=0;
    }

    if(checkbtn==1) {
      if(dataWifi.indexOf(":1")>0) {
        btnStatut=1; led_on;
        sendSMS("Bat an ninh");
      }
      if(dataWifi.indexOf(":0")>0) {
        btnStatut=0; led_off;
        sendSMS("Tat an ninh");
      }
      checkbtn=0;
    }

    getDataSim();
    getDataWifi();
    getStatutBtn();

    tmElements_t tm;
    if (RTC.read(tm)) {
      if(tm.Hour==22 && tm.Minute==0 && tm.Second==0) {
        if(btnStatut==0) {
          btnStatut=1;
          led_on;
          sendSMS("Bat an ninh");
          wifi.print("set1");
        }
      }
      
      if(tm.Hour==6 && tm.Minute==0 && tm.Second==0) {
        if(btnStatut==1) {
          btnStatut=0; 
          led_off; 
          sendSMS("Tat an ninh");
          wifi.print("set0");
        }
      }

      if(tm.Hour==5 && tm.Minute==0 && tm.Second==0) {
        rst_on; delay(500); rst_off;
      }
    }
    delay(1000);
  }
}

void getDataSim() {
  if(Serial.available()) {
      dataSim="";
      while(Serial.available()) {
        char c = (char)Serial.read(); delay(10);
    
        dataSim += c; 
      }
      //Serial.println(dataSim);
      if(dataSim.indexOf("+CMT")>=0) checkSMS=1;
      if(dataSim.indexOf("+CUSD")>=0) checkTK=1;
    }
}

void getDataWifi() {
  if(wifi.available()>0) {
    dataWifi = "";
    while(wifi.available()>0) {
      char c = (char)wifi.read(); delay(10);
      dataWifi += c;
    }
    //Serial.println(dataWifi);
    if(dataWifi.indexOf("id")>=0) checkId = 1;
    if(dataWifi.indexOf("btn")>=0) checkbtn = 1;
  }
}

void getStatutBtn() {
  if(button==0) {
    _delay(20);
    while(button==0);
    btnStatut = !btnStatut;

    if(btnStatut==1) {
      led_on;
      wifi.print("set1");
      sendSMS("Bat an ninh");
    } else {
      led_off;
      wifi.print("set0");
      sendSMS("Tat an ninh");
    }
  }
}

void init_sim(int n) {
  for(int i=0; i<n; i++) {
    led_on; delay(500);
    led_off; delay(500);
  }

  Serial.print("ATE0\r\n"); delay(500);
  Serial.print("AT+CMGF=1\r\n"); delay(500);
  Serial.print("AT+CNMI=2,2,0,0,0\r\n"); delay(500);
  Serial.print("AT+CMGDA=\"DEL ALL\"\r\n"); delay(500);
  Serial.flush(); delay(1000);
}

void sendSMS(String message) {
  Serial.print("AT+CMGS=\"");
  Serial.print(phone_number);
  Serial.print("\"\r\n"); delay(500);
  Serial.print(message); delay(500);
  Serial.print((char)26); delay(500);
  Serial.flush(); delay(1000);
}

void KTTK() {
  Serial.print("AT+CUSD=1\r\n"); delay(500);
  Serial.print("ATD*101#;\r\n"); delay(500);
  Serial.flush(); delay(1000);
}

void _delay(int n) {
  for(int i=0; i<n; i++) {
    delay(100);
  }
}

