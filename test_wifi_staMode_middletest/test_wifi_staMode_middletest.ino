#include <ESP8266WiFi.h>  //wifi기능하는 라이브러리
#include <ESP8266WebServer.h>   //web server용 라이브러리

#define rledPin D1   //D1포트는 rled포트
#define gledPin D2
#define rbuttonPin D3   //D3포트는 button포트
#define gbuttonPin D4

//char *ssidSTA = "TP-Link_5069";   // 외부 공유기 이름 지정
//char *passSTA = "64170305";   //공유기 비밀번호 지정

char *ssidSTA = "KT_GiGA_2G_Wave2_A95E";   // 외부 공유기 이름 지정
char *passSTA = "fxa50kk735";   //공유기 비밀번호 지정

bool b_red_current;   //rbutton의 현재상태 변수
bool b_red_old = true;   //rbutton의 과거상태 변수
bool rledState;   //rled의 상태 변수

bool b_green_current;   //gbutton의 현재상태 변수
bool b_green_old = true;   //gbutton의 과거상태 변수
bool gledState;   //gled의 상태 변수

float temp, humi;
int tempMax = 10;
int tempMin = -10;
int humiMax = 30;
int humiMin = -30;
String endDeviceIP;   //esp8266의 ip주소
int rbrightness = 0;  //red:0~255쓰기위해 밝기 전역변수선언
int gbrightness = 0;  //green:0~255쓰기위해 밝기 전역변수선언

//===Web server 객체생성
ESP8266WebServer webserver(80); //  http://서버IP주소:80/


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  //직렬통신 속도[bps]
  Serial.println("");
  Serial.println("==== project: midterm exam : two button ====");
  Serial.println("====Made by kmc, 2020041018 1반====");
 //====STATION mode====
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssidSTA);
 Serial.println();
 WiFi.begin(ssidSTA, passSTA);
 //waiting for connect to 외부 공유기
 while(WiFi.status() != WL_CONNECTED){   //WL= 와이어리스
  delay(500);
  Serial.print(".");
 }
 Serial.println();
 Serial.println("WiFi connencted!");
 Serial.print("STATION mode IP address!: ");
 Serial.println(WiFi.localIP());
 endDeviceIP = WiFi.localIP().toString();   //할당받는 ip주소를 받아 문자열로 바꿔줌
 

  pinMode(rledPin, OUTPUT);   //D1 출력
  pinMode(rbuttonPin, INPUT);  //D3 입력

  pinMode(gledPin, OUTPUT);
  pinMode(gbuttonPin, INPUT);
  
  //===web server구현
  //---Event 처리함수 설정
  webserver.on("/", handleRoot);  //공인IP주소 들어오면 handleRoot에 붙인다
  webserver.onNotFound(handleNotFound);
  webserver.on("/mc", handleMainControl);  //main control링크 클릭시 handleMainContrl호출
  webserver.on("/on",handleON);
  webserver.on("/off",handleOFF);
  webserver.on("/led.cgi", handleONOFF);
  webserver.on("/ledbrightness.cgi", handleBrightControl);   //밝기 조절 버튼 제어
  webserver.on("/ledbright", handleBrightness);   //밝기조절 페이지 가기
  //===Web server start!
  webserver.begin();
  Serial.println("Web server started!");
}



void loop() {
  //red
  b_red_current = digitalRead(rbuttonPin);   //rbutton의 현재상태 읽음
  rledState = digitalRead(rledPin);   //rled의 현재상태 읽음
  //green
  b_green_current = digitalRead(gbuttonPin);   //gbutton의 현재상태 읽음
  gledState = digitalRead(gledPin);   //gled의 현재상태 읽음
  
  if(b_red_current == false && b_red_old == true){
    Serial.println("-------------------------------");
    Serial.println("red Button down!");
    if(rledState){
      digitalWrite(rledPin, LOW);   //LED OFF
      Serial.println("====RED led state:   OFF====");   
    }
    else{
     digitalWrite(rledPin, HIGH);   //LED ON
     Serial.println("====RED led state:  ON====");    
    }
  }
     
  //green
  if(b_green_current == false && b_green_old == true){
    Serial.println("-------------------------------");
    Serial.println("green Button down!");
    if(gledState){
      digitalWrite(gledPin, LOW);   //LED OFF
      Serial.println("====GREEN led state:   OFF====");   
    }
    else{
     digitalWrite(gledPin, HIGH);   //LED ON
     Serial.println("====GREEN led state:  ON====");    
    }
  }
     b_red_old = b_red_current;
     b_green_old = b_green_current;

     delay(200);   //버튼 안정화

  
     webserver.handleClient();   //클라이언트로부터 서버로 접속을 기다리는 대기상태
}


  void handleRoot(){   // "/"에 대한 "Event 처리함수"
    temp = 25.0 + rand()%(tempMax-tempMin+1)+tempMin;   //가상온도
    humi = 50.0 + rand()%(humiMax-humiMin+1)+humiMin;   //가상습도

    String message = "";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    message += "<h1 style='background-color:grey;color:white'>";
    message += "===학번:2020041018-반:1-이름:Koh_myeong_chol===";
    message += "</h1>";
    //첫번째 Paragraph
    message += "<p>";
    message += "<h1 style='background-color:blue;color:white'>";
    message += "===LED CONTROL Guide===";
    message += "</h1>";
    //red
    message += "<h3>";
    message += "Enter \"http://";
    message += endDeviceIP;
    message += "/on?led=r\" in URL주소창 ---> Rled를 ON시킴!";
    message += "</h3>";
    
    message += "<h3>";
    message += "Enter \"http://";
    message += endDeviceIP;
    message += "/off?led=r\" in URL주소창 ---> Rled를 OFF시킴!";
    message += "</h3>";
    //green
    message += "<h3>";
    message += "Enter \"http://";
    message += endDeviceIP;
    message += "/on?led=g\" in URL주소창 ---> Gled를 ON시킴!";
    message += "</h3>";
    
    message += "<h3>";
    message += "Enter \"http://";
    message += endDeviceIP;
    message += "/off?led=g\" in URL주소창 ---> Gled를 OFF시킴!";
    message += "</h3>";
    //red
    message += "<br>";
    message += "<a href=\"/on?led=r\"> Control Rled ON </a>";
    message += "</br>";
    message += "<a href=\"/off?led=r\"> Control Rled OFF </a>";
    //green
    message += "<br>";
    message += "<a href=\"/on?led=g\"> Control Gled ON </a>";
    message += "</br>";
    message += "<a href=\"/off?led=g\"> Control Gled OFF </a>";  
    message += "</p>";
    
    //두번째 Paragrph
    message += "<p>";
    message += "<h1 style='background-color:red;color:white'>";
    message += "===Monitoring-Button/LED/Temp/Humi===";
    message += "</h1>";
    //red
    message += "<h3>";
    message += "REDbutton상태:";
    message += String(b_red_current);
    message += String(b_red_current)?  "[안 눌림]":"[눌림]";
    message += "</h3>";
    message += "<h3>";
    message += "GREENbutton상태:";
    message += String(b_green_current);
    message += String(b_green_current)?  "[안 눌림]":"[눌림]";
    message += "</h3>";
    message += "<h3>";
    message += "REDled상태:";
    message += String(rledState);
    message += (rledState)? "[on]":"[off]";
    message += "</h3>";
    //green
    message += "<h3>";
    message += "GREENled상태:";
    message += String(gledState);
    message += (gledState)? "[on]":"[off]";
    message += "</h3>";
    //온도
    message += "<h3>";
    message += "Temparture";
    message += temp;
    message += "</h3>";
    //습도
    message += "<h3>";
    message += "Humidity:";
    message += humi;
    message += "</h3>";
    message += "</p>";
    
    message += "<br>";
    message += "<a href=\"/mc\"> Go to Main Control Page </a>";   //href = /mc
    
    message += "<br>";
    message += "<a href=\"/ledbright\"> Go to Bright Control Page </a>";     
    
    message += "</body>";
    message += "</html>";

    webserver.sendHeader("Refresh", "1");
    webserver.send(200, "text/html; charset=utf-8", message);   //plain으로가 아니라 html로 문서를 보낸다
  }
 void handleON() {
    for(uint8_t i=0; i<webserver.args(); i++) {
      if(webserver.argName(i) == "led") {
        if(webserver.arg(i) == "r") {
          //red
          digitalWrite(rledPin, HIGH);
          rledState = digitalRead(rledPin);
        } else if(webserver.arg(i) == "g") {
           //green
          digitalWrite(gledPin, HIGH);
          gledState = digitalRead(gledPin);
        }
      }
      //if(webser "gledbright"
    }
    String message ="";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    message += "<p>";
    message += "<h1 style='background-color:green;color:white'>";
    message += "===LED 제어 후 상태===";
    message += "</h1>";
    //red
    message += "<h3>";
    message += "REDled 제어 후 상태";
    message += String(rledState);
    message += (rledState)? "[on]":"[off]";
    message += "</h3>";
    //green
    message += "<h3>";
    message += "GREENled 제어 후 상태";
    message += String(gledState);
    message += (gledState)? "[on]":"[off]";
    message += "</h3>";
    message += "</p>";
    
    
    message += "<a href=\"/\">Go to Home Page</a>";       
    message += "</body>";
    message += "</html>";

    webserver.send(200, "text/html; charset=utf-8", message);   //plain으로가 아니라 html로 문서를 보낸다
  }
//off
 void handleOFF() {
    for(uint8_t i=0; i<webserver.args(); i++) {
      if(webserver.argName(i) == "led") {
        if(webserver.arg(i) == "r") {
          //red
          digitalWrite(rledPin, LOW);
          rledState = digitalRead(rledPin);
        } else if(webserver.arg(i) == "g") {
           //green
          digitalWrite(gledPin, LOW);
          gledState = digitalRead(gledPin);
        }
      }
    }
    String message ="";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    message += "<p>";
    message += "<h1 style='background-color:green;color:white'>";
    message += "===LED 제어 후 상태===";
    message += "</h1>";
    //red
    message += "<h3>";
    message += "REDled 제어 후 상태";
    message += String(rledState);
    message += (rledState)? "[on]":"[off]";
    message += "</h3>";
    //green
    message += "<h3>";
    message += "GREENled 제어 후 상태";
    message += String(gledState);
    message += (gledState)? "[on]":"[off]";
    message += "</h3>";
    message += "</p>";
    
    
    message += "<a href=\"/\">Go to Home Page</a>";       
    message += "</body>";
    message += "</html>";

    webserver.send(200, "text/html; charset=utf-8", message);   //plain으로가 아니라 html로 문서를 보낸다
  }

  
  void handleNotFound() {
    String message = "";
    message += "404\n";
    message += "File Not Found\n\n";
    message += "URI: ";
    message += webserver.uri();
    message += "\nArguments:";
    message += webserver.args();
    message += "\n";
    for(uint8_t i = 0; i < webserver.args(); i++) {   //i=0, 1, 2
      message += "   " +
      webserver.argName(i) +   //i, j, k
      ":" + webserver.arg(i) + "\n";   //2, 3, 4
    }
    webserver.send(404, "text/plain; xharset=utf-8", message);
  }

  void handleMainControl() {
    String message ="";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    message += "<h1 style='background-color:green;color:white'>";
    message += "===LED ON/OFF 상태===";
    message += "</h1>";
    //red
    message += "<h2>";
    message += "REDled 현재 상태";
    message += String(rledState);
    message += (rledState)? "[on]":"[off]";
    message += "</h2>";
    //green
    message += "<h2>";
    message += "GREENled 현재 상태";
    message += String(gledState);
    message += (gledState)? "[on]":"[off]";
    message += "</h2>";   
    message += "<h1 style='background-color:red;color:white'>";
    message += "===LED ON/OFF 제어===";
    message += "</h1>";
    //Paragraph
    //get
    message += "<p>";
    //red
    message += "<form method=\"get\" action=\"/led.cgi\">";  //get방식으로 보낸후 url뒤에 /led.cgi붙여라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"rLEDcontrol\" value=\"1\">REDled Turn ON!";  //radio button클릭시 /led.cgi?/Rledstatus=1 붙여 보내라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"rLEDcontrol\" value=\"0\">REDled Turn OFF!";  //radio button클릭시 /led.cgi?/Rledstatus=0 붙여 보내라.
    //green
    message += "<br>";
    message += "<input type=\"radio\" name=\"gLEDcontrol\" value=\"1\">GREENled Turn ON!";  //radio button클릭시 /led.cgi?/Gledstatus=1 붙여 보내라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"gLEDcontrol\" value=\"0\">GREENled Turn OFF!";  //radio button클릭시 /led.cgi?/Gledstatus=0 붙여 보내라.
    
    message += "<br>";
   message += "<input type=\"submit\" value=\"GET방식으로 보내기\" style=\"width:300px; font-size:27px; color:white; background-color: grey;\">";
    message += "</form>";
    message += "</p>";
    //post
    message += "<p>";
    //red
    message += "<form method=\"post\" action=\"/led.cgi\">";  //get방식으로 보낸후 url뒤에 /led.cgi붙여라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"rLEDcontrol\" value=\"1\">REDled Turn ON!";  //radio button클릭시 /led.cgi 붙여 보내라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"rLEDcontrol\" value=\"0\">REDled Turn OFF!";  //radio button클릭시 /led.cgi 붙여 보내라.
    //green
    message += "<br>";
    message += "<input type=\"radio\" name=\"gLEDcontrol\" value=\"1\">GREENled Turn ON!";  //radio button클릭시 /led.cgi 붙여 보내라.
    message += "<br>";
    message += "<input type=\"radio\" name=\"gLEDcontrol\" value=\"0\">GREENled Turn OFF!";  //radio button클릭시 /led.cgi 붙여 보내라.
    
    message += "<br>";
    message += "<input type=\"submit\" value=\"POST방식으로 보내기\" style=\"width:300px; font-size:27px; color:white; background-color: grey;\">";

    message += "<form method=\"post\" action=\"/led.cgi\">";  //get방식으로 보낸후 url뒤에 /led.cgi붙여라.
    message += "<br>";
    message += "<input type=\"checkbox\" name=\"rLEDcontrol\" value=\"1\">REDled Turn ON!";  //check button클릭시 /led.cgi 붙여 보내라.
    message += "<br>";
    message += "<input type=\"checkbox\" name=\"rLEDcontrol\" value=\"0\">REDled Turn OFF!";  //check button클릭시 /led.cgi 붙여 보내라.
    //green
    message += "<br>";
    message += "<input type=\"checkbox\" name=\"gLEDcontrol\" value=\"1\">GREENled Turn ON!";  //check button클릭시 /led.cgi 붙여 보내라.
    message += "<br>";
    message += "<input type=\"checkbox\" name=\"gLEDcontrol\" value=\"0\">GREENled Turn OFF!";  //check button클릭시 /led.cgi 붙여 보내라.
    
    message += "<br>";
    message += "<input type=\"submit\" value=\"POST방식으로 보내기\" style=\"width:300px; font-size:27px; color:white; background-color: grey;\">";
    message += "</form>";
    message += "</p>";
    
    message += "<a href=\"/\">Go to Home Page</a>";    
    message += "<br>";
    message += "<a href=\"/ledbright\">Go to Bright Control Page</a>";
    message += "</body>";
    message += "</html>";

    webserver.send(200, "text/html; charset=utf-8", message);   //plain으로가 아니라 html로 문서를 보낸다
  }
  void handleONOFF() {
     //red
     for(uint8_t i=0; i<webserver.args(); i++) {
      if(webserver.argName(i) == "rLEDcontrol") {
        int state = webserver.arg(i).toInt();
        if(state) {
          digitalWrite(rledPin, HIGH);
        }else{ 
          digitalWrite(rledPin, LOW);
        }
        rledState = digitalRead(rledPin);
    //green
      }else if(webserver.argName(i) == "gLEDcontrol") {
        int state = webserver.arg(i).toInt();
        if(state) {
          digitalWrite(gledPin, HIGH);
        }else{ 
          digitalWrite(gledPin, LOW);
        }
        gledState = digitalRead(gledPin);
     }
     }
     
    String message ="";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    //red
    message += "<h2>";
    message += "REDled 현재 상태:";
    message += String(rledState);
    message += (rledState)? "[ON]":"[OFF]";  //1이면 ON 0이면 0FF
    message += "</h2>";
    //green
    message += "<h2>";
    message += "GREENled 현재 상태:";
    message += String(gledState);
    message += (gledState)? "[ON]":"[OFF]";  //1이면 ON 0이면 0FF
    message += "</h2>";
    
    message += "<br>";
    message += "<a href=\"/\">Go to Home Page</a>";  //홈페이지 가는 링크추가
    message += "<br>";
    message += "<a href=\"/mc\">Go to MainControl Page</a>";  //mc로 가는 링크
    message += "<br>";
    message += "<a href=\"/ledbright\">Go to Bright Control Page</a>";
    
    message += "</body>";
    message += "</html>";

    
    webserver.send(200, "text/html; charset=utf-8", message);
  } 
     
  void handleBrightness() {
    showBrightnessPage();
  }

  void showBrightnessPage(){
    String message = "";
    message += "<html>";
    message += "<head>";
    message += "</head>";
    message += "<body>";
    message += "<h2 style='background-color:green;color:white'>";
    message += "===LED 밝기 상태===";
    message += "</h2>";
    //===new
    //red
    message += "<b>REDled brightness(0:OFF-Dark~Bright-255:ON) is </b>";
    message += rbrightness;
    //green
    message += "<br>";
    message += "<b>GREENLled brightness(0:OFF-Dark~Bright-255:ON) is </b>";
    message += gbrightness;
    message += "<h2 style='background-color:red;color:white'>";
    message += "===LED 밝기 제어===";
    message += "</h2>";
    //get
    message += "<form method=\"get\" action=\"/ledbrightness.cgi\">";   //get방식으로 보내 action=작동시켜 
    //message += "<br>";
    message += "rBrightness";
    message += "<input type=\"text\" name=\"rbrightness\">";
    message += "<br>";
    message += "gBrightness";
    message += "<input type=\"text\" name=\"gbrightness\">";
    message += "<br>";
    message += "<input type=\"submit\" value=\"GET방식으로 보내기\" style=\"width:300px; font-size:27px; color:white; background-color: grey;\">";
    message += "</form>";
    //post
    message += "<form method=\"post\" action=\"/ledbrightness.cgi\">";   //get방식으로 보내 action=작동시켜 
    message += "<br>";
    message += "rBrightness";
    message += "<input type=\"text\" name=\"rbrightness\">";
    message += "<br>";
    message += "gBrightness";
    message += "<input type=\"text\" name=\"gbrightness\">";
    message += "<br>";
    message += "<input type=\"submit\" value=\"POST방식으로 보내기\" style=\"width:300px; font-size:27px; color:white; background-color: grey;\">";
    message += "</form>";
    
    message += "<br>";
    message += "<a href=\"/\">Go to Home Page</a>";  //홈페이지 가는 링크추가
    message += "<br>";
    message += "<a href=\"/mc\">Go to Main Control</a>";  //mc로 가는 링크
    
    message += "</body>";
    message += "</html>";
    
    webserver.send(200, "text/html; charset=utf-8", message);
   
  }
  //red
  void handleBrightControl() {
    //red
     for(uint8_t i=0; i<webserver.args(); i++) {
      if(webserver.argName(i) == "rbrightness") {
        int rled = webserver.arg(i).toInt();
        rbrightness = rled;
        analogWrite(rledPin, rbrightness);
        showBrightnessPage();
      }
    //green
    else if(webserver.argName(i) == "gbrightness") {
        int gled = webserver.arg(i).toInt();
        gbrightness = gled;
        analogWrite(gledPin, gbrightness);
        showBrightnessPage();
      }
  }
}
