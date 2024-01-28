#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char *ssid = "RH-2.4G-87BEF0";
const char *password = "AppaNnas245";

int motor1pin1 = 18;
int motor1pin2 = 19;
int motor_state = 0;
int total_state = 0;
int ir_state = 0;
int current_motor = 34;
int current_total = 35;

int IRSensor = 21;

WebServer server(80);

int inputNumber = 0;
float temperature = 16;
int pwm = 0;

void handleConnection() {
  // String html = "<html><body>";
  // html += "<h1>Enter a number:</h1>";
  // html += "<form action='/submit' method='post'>";
  // html += "Number: <input type='text' name='number'>";
  // //html += "<input type='submit' value='Submit'>";
  // html += "</form>";
  // html += "</body></html>";
  String html = "Welcome to Tejas Train!";
  // for 
  server.send(200, "text/html", html);
  Serial.println("New client here");
}

float current_motor_sense()
  {float AcsValue = 0;
  float AcsValueF = 0;
  AcsValue = analogRead(current_motor);     //Read current sensor values   
  AcsValueF = (AcsValue*3.3/4095-2.5) /0.185;
  Serial.println("Motor current is");
  Serial.println(AcsValue);
  Serial.println("Motor current is also");
  Serial.println(AcsValueF);
  return AcsValueF;
  }

int ir_sense()
  {int sensorStatus = 0;
   sensorStatus = digitalRead(IRSensor);
  return sensorStatus;
  }


float current_total_sense()
  {float AcsValue1 = 0;
  float AcsValueF1= 0;
  AcsValue1 = analogRead(current_total);     //Read current sensor values   
  AcsValueF1 = (AcsValue1*3.3/4095-2.5) /0.185;
  Serial.println("Total current is");
  Serial.println(AcsValue1);
  Serial.println("Total current is also");
  Serial.println(AcsValueF1);
  return AcsValueF1;
  }




void handleReadings() {
  Serial.println("Handling Readings now");
  String resp = "";
  String jsonStr1 = server.arg("plain");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonStr1);
    if (doc.containsKey("motor")) {
      Serial.println("hehe");
    }
  motor_state = doc["motor"];
  Serial.println(motor_state); 
  total_state = doc["total"];
  ir_state = doc["ir"];
  Serial.println(total_state);
  float  motor_curr = 0;
  float  total_curr = 0;
  int ir_read =0;
  if(motor_state==1){
    motor_curr = current_motor_sense();
    resp += " motor_current=";
    resp += String(motor_curr);
  }
  if(total_state==1){
    motor_curr = current_total_sense();
    resp += " total_current=";
    resp += String(total_curr);
  }
    if(ir_state==1){
    ir_read = ir_sense();
    resp += " ir=";
    resp += String(ir_read);
  }

  Serial.println("Response is ");
  Serial.println(resp);
  server.send(200, "text/html", resp);


  }


void handlePWM() {
  Serial.println("Handling PWM now");
  String jsonStr = server.arg("plain");
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonStr);
  if (doc.containsKey("pwm")) {
    int pwmr = doc["pwm"];
    digitalWrite(motor1pin1, LOW);
    analogWrite(motor1pin2, pwmr);
    Serial.print("New PWM: ");
    Serial.println(pwmr);
  }

  else 
    Serial.print("Not able to set PWM");
  }
  //server.send(200, "text/plain", "Number submitted");
  // if (server.hasArg("number")) {
  //   inputNumber = server.arg("number").toInt();
  //   Serial.print("Input number: ");
  //   Serial.println(inputNumber);
  // }
  //server.send(200, "text/plain", "Number submitted");






void setup() {
  Serial.begin(115200);
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(current_motor, INPUT);
  pinMode(current_total, INPUT); 
  pinMode(IRSensor, INPUT); 
 
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("Server on");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", HTTP_GET, handleConnection);
  //server.on("/submit", HTTP_GET, handleSubmit);    // multiple functions  after one
  server.on("/pwm", HTTP_POST, handlePWM);
  server.on("/readings", HTTP_POST, handleReadings);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
