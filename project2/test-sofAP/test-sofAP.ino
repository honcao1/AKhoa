#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266mDNS.h>
#include "logo.h"

ESP8266WebServer server(3004);

const byte RX = D1;
const byte TX = D2;
 
SoftwareSerial mySerial = SoftwareSerial(RX, TX); 

const char* ssid = "Hub2";
const char* passphrase = "12345678"; 

String btn;
String content;
int lengthCam[16];
String eCamera1, eCamera2, eCamera3, eCamera4, eCamera5, eCamera6, eCamera7, eCamera8;
String eCamera9, eCamera10, eCamera11, eCamera12, eCamera13, eCamera14, eCamera15, eCamera16;
bool set=0;

void handleCtrl() {
  content = "<!DOCTYPE HTML>\r\n";
  content += "<html>\r\n";
  content += "<head>\r\n";
  content += "<style>\r\n";
  content += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n";
  content += ".button { background-color: #5BC0DE; border: none; color: white; padding: 16px 40px;";
  content += "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\r\n";
  content += ".button2 {background-color: #e6e6e6;}\r\n";
  content += "</style>\r\n";
  content += "</head>\r\n";
  content += "<body>\r\n";
  content += "<img src='/img/logo_KCQ.png' alt=\'LOGO KCQ\' style= \'width:175px; height:119px\'></img>";
  content += "<br>\r\n";
  content += "<br>\r\n";
  content += "<br>\r\n";
  content += "<h1>He thong ON/OFF an ninh</h1>\r\n";
  if(set==1) {
    content += "<a href=\'/on\'><button class=\"button\">ON</button></a>\r\n";
    content += "<a href=\'/off\'><button class=\"button button2\">OFF</button></a>\r\n";
  } else {
    content += "<a href=\'/on\'><button class=\"button button2\">ON</button></a>\r\n";
    content += "<a href=\'/off\'><button class=\"button\">OFF</button></a>\r\n";
  }
  content += "</body>\r\n";
  content += "</html>\r\n";
  server.send(200, "text/html", content);
}

void handleRoot() {
    IPAddress ip = WiFi.softAPIP();
    IPAddress iplocal = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String ipStr1 = String(iplocal[0]) + '.' + String(iplocal[1]) + '.' + String(iplocal[2]) + '.' + String(iplocal[3]);

    String eStart;
    for (int i = 96; i < 101; ++i)
    {
      eStart += char(EEPROM.read(i));
    }
    Serial.print("Time Start: ");
    Serial.println(eStart);
    
    String eStop = "";
    for (int i = 101; i < 106; ++i)
    {
      eStop += char(EEPROM.read(i));
    }
    Serial.print("Time Stop: ");
    Serial.println(eStop);  
    
    content = "<!DOCTYPE HTML>\r\n";
    content += "<html>\r\n"; 
    content += "<p>WiFi sofAPIP:  ";
    content += ipStr;
    content += "</p>\r\n";
    content += "<p>WiFi LocalIP:  ";
    content += ipStr1;
    content += "</p>\r\n";
    content += "<form method='get' action='setting'>\r\n";
    content += "<label>SSID: </label><input name='ssid' length=32><br></br>\r\n";
    content += "<label>PASS: </label><input name='pass' length=64><br></br>\r\n";
    content += "<input type='submit' value='Gui'><br>\r\n</form></br>\r\n";
    content += "<form method='get' action='sim'>\r\n";
    content += "<label>SDT: </label><input name='sdt' length=32 type='number' size='10'><br></br>\r\n";
    content += "<input type='submit' value='Gui'><br>\r\n</form></br>\r\n";
    content += "<form method='get' action='time'>\r\n";
    content += "<label>START: </label><input name='TimeStart' type='time' value='";
    content += eStart;
    content += "'><br></br>\r\n";
    content += "<label>STOP: </label><input name='TimeStop' type='time' value='";
    content += eStop;
    content += "'><br></br>\r\n";
    content += "<input type='submit' value='Gui'><br>\r\n</form></br>\r\n";
    content += "<form method='get' action='setTime'>\r\n";
    content += "<label>Thoi Gian: </label><input name='dateTime' type='datetime-local'><br></br>\r\n";
    content += "<input type='submit' value='Gui'><br>\r\n</form></br>\r\n";
    content += "</html>";
    server.send(200, "text/html", content);  
}

void handleCamera() {
    getCamera();
    content = "<!DOCTYPE HTML>\r\n";
    content += "<html>\r\n"; 
    content += "<form method='get' action='kvcamera'>\r\n";
    content += "<label>Camera1: </label><input type= 'text' name='camera1' maxlength='15' value='";
    content += eCamera1;
    content += "'>\r\n";
    content += "<label>Camera2: </label><input name='camera2' maxlength='15' value='";
    content += eCamera2;
    content += "'>\r\n";
    content += "<label>Camera3: </label><input name='camera3' maxlength='15' value='";
    content += eCamera3;
    content += "'>\r\n";
    content += "<label>Camera4: </label><input name='camera4' maxlength='15' value='";
    content += eCamera4;
    content += "'><br></br>\r\n";
    content += "<label>Camera5: </label><input name='camera5' maxlength='15' value='";
    content += eCamera5;
    content += "'>\r\n";
    content += "<label>Camera6: </label><input name='camera6' maxlength='15' value='";
    content += eCamera6;
    content += "'>\r\n";
    content += "<label>Camera7: </label><input name='camera7' maxlength='15' value='";
    content += eCamera7;
    content += "'>\r\n";
    content += "<label>Camera8: </label><input name='camera8' maxlength='15' value='";
    content += eCamera8;
    content += "'><br></br>\r\n";
    content += "<label>Camera9: </label><input name='camera9' maxlength='15' value='";
    content += eCamera9;
    content += "'>\r\n";
    content += "<label>Camera10: </label><input name='camera10' maxlength='15' value='";
    content += eCamera10;
    content += "'>\r\n";
    content += "<label>Camera11: </label><input name='camera11' maxlength='15' value='";
    content += eCamera11;
    content += "'>\r\n";
    content += "<label>Camera12: </label><input name='camera12' maxlength='15' value='";
    content += eCamera12;
    content += "'><br></br>\r\n";
    content += "<label>Camera13: </label><input name='camera13' maxlength='15' value='";
    content += eCamera13;
    content += "'>\r\n";
    content += "<label>Camera14: </label><input name='camera14' maxlength='15' value='";
    content += eCamera14;
    content += "'>\r\n";
    content += "<label>Camera15: </label><input name='camera15' maxlength='15' value='";
    content += eCamera15;
    content += "'>\r\n";
    content += "<label>Camera16: </label><input name='camera16' maxlength='15' value='";
    content += eCamera16;
    content += "'><br></br>\r\n";
    content += "<input type='submit' value='Gui'><br>\r\n</form></br>\r\n";
    content += "</html>";
    server.send(200, "text/html", content);  
}

void handleWifi() {
  String qsid = server.arg("ssid");
  String qpass = server.arg("pass");

  if (qsid.length() > 0 && qpass.length() > 0) {
    Serial.println("clearing eeprom");
    for (int i = 0; i < 96; ++i) { EEPROM.write(i, 0); }
    Serial.println(qsid);
    Serial.println("");
    Serial.println(qpass);
    Serial.println("");

    for (int i = 0; i < qsid.length(); ++i)
    {
      EEPROM.write(i, qsid[i]);
      Serial.print("Wrote: ");
      Serial.println(qsid[i]); 
    }
    Serial.println("writing eeprom pass:"); 
    for (int i = 0; i < qpass.length(); ++i)
    {
      EEPROM.write(32+i, qpass[i]);
      Serial.print("Wrote: ");
      Serial.println(qpass[i]); 
    }    
    EEPROM.commit();
    
    content = "{\"Success\":\"saved to eeprom....... reset to boot into new wifi\"}";
  }
  server.send(200, "application/json", content);
  delay(200);
  ESP.restart();
}

void handleSim() {
  String qsdt = server.arg("sdt");

  if (qsdt.length() > 0) {
    Serial.print("SDT: ");
    Serial.println(qsdt);
  
    mySerial.print("sdt:"); mySerial.print(qsdt);

    delay(1000);
    content = "{\"Success\":\"ok\"}";
  }
  server.send(200, "application/json", content);
}

void handleTime() {
  String qstart = server.arg("TimeStart");
  String qstop = server.arg("TimeStop");

  if (qstart.length() > 0 && qstop.length() > 0 ) {
    Serial.print("START: ");
    Serial.println(qstart);
    Serial.print("STOP: ");
    Serial.println(qstop);

    Serial.println("clearing eeprom");
    for (int i = 0; i < 10; ++i) { EEPROM.write(96+i, 0); }

    for (int i = 0; i < qstart.length(); ++i)
    {
      EEPROM.write(96+i, qstart[i]);
      Serial.print("Wrote: ");
      Serial.println(qstart[i]); 
    }
    Serial.println("writing eeprom pass:"); 
    for (int i = 0; i < qstop.length(); ++i)
    {
      EEPROM.write(101+i, qstop[i]);
      Serial.print("Wrote: ");
      Serial.println(qstop[i]); 
    }    
    EEPROM.commit();
  
    mySerial.print("time:"); mySerial.print(qstart); mySerial.print("-"); mySerial.print(qstop);

    delay(1000);
    content = "{\"Success\":\"ok\"}";
  }
//  server.send(200, "application/json", content);
    delay(1000);
    handleRoot();
}

void handleSetTime() {
  String qDateTime = server.arg("dateTime");
  Serial.println(qDateTime);
  mySerial.print("setTime" + qDateTime);
  content = "{\"Success\":\"ok\"}";
  server.send(200, "application/json", content);
}

void handleKvCamera(){
  String qCamera1 = server.arg("camera1"); String qCamera2 = server.arg("camera2");
  String qCamera3 = server.arg("camera3"); String qCamera4 = server.arg("camera4");
  String qCamera5 = server.arg("camera5"); String qCamera6 = server.arg("camera6");
  String qCamera7 = server.arg("camera7"); String qCamera8 = server.arg("camera8");
  String qCamera9 = server.arg("camera9"); String qCamera10 = server.arg("camera10");
  String qCamera11 = server.arg("camera11"); String qCamera12 = server.arg("camera12");
  String qCamera13 = server.arg("camera13"); String qCamera14 = server.arg("camera14");
  String qCamera15 = server.arg("camera15"); String qCamera16 = server.arg("camera16");
//  Serial.println(qCamera1);
  for (int i = 106; i < 363; ++i) { EEPROM.write(i, 0); }

  lengthCam[0] = qCamera1.length();
  
  for (int i = 0; i < lengthCam[0]; ++i)
  {
    EEPROM.write(i+106, qCamera1[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera1[i]); 
  }

  lengthCam[1] = qCamera2.length();
  for (int i = 0; i < qCamera2.length(); ++i)
  {
    EEPROM.write(i+121, qCamera2[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera2[i]); 
  }

  lengthCam[2] = qCamera3.length();
  for (int i = 0; i < qCamera3.length(); ++i)
  {
    EEPROM.write(i+136, qCamera3[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera3[i]); 
  }

  lengthCam[3] = qCamera4.length();
  for (int i = 0; i < qCamera4.length(); ++i)
  {
    EEPROM.write(i+151, qCamera4[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera4[i]); 
  }

  lengthCam[4] = qCamera5.length();
  for (int i = 0; i < qCamera5.length(); ++i)
  {
    EEPROM.write(i+166, qCamera5[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera5[i]); 
  }

  lengthCam[5] = qCamera6.length();
  for (int i = 0; i < qCamera6.length(); ++i)
  {
    EEPROM.write(i+181, qCamera6[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera6[i]); 
  }

  lengthCam[6] = qCamera7.length();
  for (int i = 0; i < qCamera7.length(); ++i)
  {
    EEPROM.write(i+196, qCamera7[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera7[i]); 
  }

  lengthCam[7] = qCamera8.length();
  for (int i = 0; i < qCamera8.length(); ++i)
  {
    EEPROM.write(i+211, qCamera8[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera8[i]); 
  }

  lengthCam[8] = qCamera9.length();
  for (int i = 0; i < qCamera9.length(); ++i)
  {
    EEPROM.write(i+226, qCamera9[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera9[i]); 
  }

  lengthCam[9] = qCamera10.length();
  for (int i = 0; i < qCamera10.length(); ++i)
  {
    EEPROM.write(i+241, qCamera10[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera10[i]); 
  }

  lengthCam[10] = qCamera11.length();
  for (int i = 0; i < qCamera11.length(); ++i)
  {
    EEPROM.write(i+256, qCamera11[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera11[i]); 
  }

  lengthCam[11] = qCamera12.length();
  for (int i = 0; i < qCamera12.length(); ++i)
  {
    EEPROM.write(i+271, qCamera12[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera12[i]); 
  }

  lengthCam[12] = qCamera13.length();
  for (int i = 0; i < qCamera13.length(); ++i)
  {
    EEPROM.write(i+286, qCamera13[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera13[i]); 
  }

  lengthCam[13] = qCamera14.length();
  for (int i = 0; i < qCamera14.length(); ++i)
  {
    EEPROM.write(i+301, qCamera14[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera14[i]); 
  }

  lengthCam[14] = qCamera15.length();
  for (int i = 0; i < qCamera15.length(); ++i)
  {
    EEPROM.write(i+316, qCamera15[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera15[i]); 
  }

  lengthCam[15] = qCamera16.length();
  for (int i = 0; i < qCamera16.length(); ++i)
  {
    EEPROM.write(i+331, qCamera16[i]);
    Serial.print("Wrote: ");
    Serial.println(qCamera16[i]); 
  }

  for (int i = 0; i < 16; ++i)
  {
    EEPROM.write(i+346, lengthCam[i]);
    Serial.print("Wrote: ");
    Serial.println(lengthCam[i]); 
  }
  
  EEPROM.commit();
//  content = "{\"Success\":\"ok\"}";
//  server.send(200, "application/json", content);
  delay(1000);
  handleCamera();
}
/*
void handleMess() {
  String qmess = server.arg("id");
  String qtime = server.arg("eTime");

  if(qmess.length() > 0 && qtime.length() > 0) {
    Serial.println(qmess);
    Serial.println("");
    Serial.println(qtime);
    Serial.println("");

    qtime = qtime.substring(0, 19);
    qtime.replace('T', ' ');

    mySerial.print("id=" + qmess + qtime);
    
    content = "{\"Success\": \"ok\"}";
  }
  server.send(200, "application/json", content);
}
*/

void handleMess() {
  String qmess = server.arg("id");
  String qtime = server.arg("eTime");

  getCamera(); delay(1000);
  
  if(qmess.length() > 0) {
    Serial.println(qmess);
    Serial.println("");
    int idCam = qmess.toInt();
    switch(idCam){
      case 1: mySerial.print("id"+eCamera1); Serial.println(eCamera1); break;
      case 2: mySerial.print("id"+eCamera2); Serial.println(eCamera2); break;
      case 3: mySerial.print("id"+eCamera3); Serial.println(eCamera3); break;
      case 4: mySerial.print("id"+eCamera4); Serial.println(eCamera4); break;
      case 5: mySerial.print("id"+eCamera5); Serial.println(eCamera5); break;
      case 6: mySerial.print("id"+eCamera6); Serial.println(eCamera6); break;
      case 7: mySerial.print("id"+eCamera7); Serial.println(eCamera7); break;
      case 8: mySerial.print("id"+eCamera8); Serial.println(eCamera8); break;
      case 9: mySerial.print("id"+eCamera9); Serial.println(eCamera9); break;
      case 10: mySerial.print("id"+eCamera10); Serial.println(eCamera10); break;
      case 11: mySerial.print("id"+eCamera11); Serial.println(eCamera11); break;
      case 12: mySerial.print("id"+eCamera12); Serial.println(eCamera12); break;
      case 13: mySerial.print("id"+eCamera13); Serial.println(eCamera13); break;
      case 14: mySerial.print("id"+eCamera14); Serial.println(eCamera14); break;
      case 15: mySerial.print("id"+eCamera15); Serial.println(eCamera15); break;
      case 16: mySerial.print("id"+eCamera16); Serial.println(eCamera16); break;
    }
    
    content = "{\"Success\": \"ok\"}";
  }
  server.send(200, "application/json", content);
}

void handleWebRequests(){
  //if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void handleImg() {
  server.send_P(200, "image/png", logo_kcq_png, logo_kcq_png_size);  
}

void testWifi(void) {
  int c = 0;
  Serial.println("Waiting for Wifi to connect to ESP8266");  
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED) { return; } 
    delay(500);
    Serial.print(".");    
    c++;
  }
} 

void getCamera(){
  for(int i=0; i<16; ++i){
      lengthCam[i] = (int)EEPROM.read(i+346);
    }
    
    eCamera1 ="";
    for (int i = 0; i < lengthCam[0]; ++i) {
      eCamera1 += (char)EEPROM.read(i+106);
    }

    eCamera2="";
    for (int i = 0; i < lengthCam[1]; ++i) {
      eCamera2 += char(EEPROM.read(i+121));
    }

    eCamera3="";
    for (int i = 0; i < lengthCam[2]; ++i) {
      eCamera3 += char(EEPROM.read(i+136));
    }

    eCamera4="";
    for (int i = 0; i < lengthCam[3]; ++i) {
      eCamera4 += char(EEPROM.read(i+151));
    }
    eCamera5="";
    for (int i = 0; i < lengthCam[4]; ++i) {
      eCamera5 += char(EEPROM.read(i+166));
    }
    eCamera6="";
    for (int i = 0; i < lengthCam[5]; ++i) {
      eCamera6 += char(EEPROM.read(i+181));
    }
    eCamera7="";
    for (int i = 0; i < lengthCam[6]; ++i) {
      eCamera7 += char(EEPROM.read(i+196));
    }
    eCamera8="";
    for (int i = 0; i < lengthCam[7]; ++i) {
      eCamera8 += char(EEPROM.read(i+211));
    }
    eCamera9="";
    for (int i = 0; i < lengthCam[8]; ++i) {
      eCamera9 += char(EEPROM.read(i+226));
    }
    eCamera10="";
    for (int i = 0; i < lengthCam[9]; ++i) {
      eCamera10 += char(EEPROM.read(i+241));
    }
    eCamera11="";
    for (int i = 0; i < lengthCam[10]; ++i) {
      eCamera11 += char(EEPROM.read(i+256));
    }
    eCamera12="";
    for (int i = 0; i < lengthCam[11]; ++i) {
      eCamera12 += char(EEPROM.read(i+271));
    }
    eCamera13="";
    for (int i = 0; i < lengthCam[12]; ++i) {
      eCamera13 += char(EEPROM.read(i+286));
    }

    eCamera14="";
    for (int i = 0; i < lengthCam[13]; ++i) {
      eCamera14 += char(EEPROM.read(i+301));
    }

    eCamera15="";
    for (int i = 0; i < lengthCam[14]; ++i) {
      eCamera15 += char(EEPROM.read(i+316));
    }

    eCamera16="";
    for (int i = 0; i < lengthCam[15]; ++i) {
      eCamera16 += char(EEPROM.read(i+331));
    }
}

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
  
  EEPROM.begin(512);
   
  WiFi.mode(WIFI_STA);

  Serial.println("Reading ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading  password");
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);  
  if ( esid.length() > 1 ) {
      WiFi.begin(esid.c_str(), epass.c_str());
      testWifi();
  }
  
  WiFi.softAP(ssid, passphrase, 6);
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  //khi reset thi trang thai lun off
  mySerial.print("rst");

  //mDNS
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", handleCtrl);
  server.on("/on", [](){
    set=1;
    mySerial.print("btn:1");
    handleCtrl();  
  });
  
  server.on("/off", [](){
    set=0;
    mySerial.print("btn:0");
    handleCtrl();
  });

  server.on("/getdata", [](){
    if(set=1) content = "{'on': 1, 'off': 0}";
    else content = "{'on': 0, 'off': 1}";
    server.send(200, "application/json", content);
  });
  
  server.on("/wifi", handleRoot);
  server.on("/camera", handleCamera);
  server.on("/setting", handleWifi);
  server.on("/sim", handleSim);
  server.on("/time", handleTime);
  server.on("/setTime", handleSetTime);
  server.on("/kvcamera", handleKvCamera);
  server.on("/mess1", handleMess);
  server.on("/img/logo_KCQ.png", handleImg);
  server.onNotFound(handleWebRequests);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
  btn="";
  if(mySerial.available() > 0) {
    while(mySerial.available() > 0) {
      char c = (char)mySerial.read();
      delay(10);

      btn += c;
    }

    if(btn.indexOf("set1")>=0) set=1;
    else if(btn.indexOf("set0")>=0) set=0;
  }
}
