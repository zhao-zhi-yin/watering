
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
#include <Thread.h>
#include "H_project.h"

void initSystem();
void wifiTick();
bool postTempToOneNet();

int retry = 0;
int state;float mosit;
const int tcpPort = 80; 
char* DataStreams = "LIGHT";                // 数据流 LIGHT -- Stream "LIGHT"
int stream1;                           //用于存放返回"LIGHT"的数值 -- Saving the returned value for "LIGHT"
const char* DS_Humidity = "moisture";   // 数据流 湿度HUMI 
const size_t MAX_CONTENT_SIZE = 1024;                  // 最大内容长度 -- Maximum content size
const unsigned long HTTP_TIMEOUT = 2100;                // 超时时间 -- Timeout
//WiFiClient espClient;
WiFiClient client;

Thread myThread = Thread();
//声明方法
void initSystem();
void initOneNetMqtt();
void callback(char* topic, byte* payload, unsigned int length);
void sendhumi();
/**
 * 初始化
 */
void setup() {
  WiFi.mode(WIFI_AP_STA);
  pinMode(LED_BUILTIN, OUTPUT);  
  initSystem();
  initOneNetMqtt();
  myThread.onRun(sendhumi);
  myThread.setInterval(500);
}

void loop() {
  mosit= analogRead(A0);
  state = connectToOneNetMqtt();
  ESP.wdtFeed();  
  myThread.run();
  if(state == ONENET_RECONNECT){
     //重连成功 需要重新注册
     mqttClient.subscribe(TOPIC,1);
     mqttClient.loop();
  }else if(state == ONENET_CONNECTED){
     mqttClient.loop();
  }
  delay(2000);
}
void sendhumi(){
    if (!client.connect(OneNetServer, tcpPort)) 
  {
    Serial.println("connection failed！！！");
    return;
  }
  
    postData(int32_t(DEVICE_ID1),mosit);
    
      if (!client.connect(OneNetServer, tcpPort)) 
  {
    Serial.println("connection failed！！！");
    return;
  }
    Serial.println("====================================");
    Serial.print("Soil Moisture: "); 
    Serial.print(mosit);
    Serial.println(" %");
    Serial.println("====================================");
}
void initSystem(){
    int cnt = 0;
    Serial.begin (115200);
    Serial.println("\r\n\r\nStart ESP8266 MQTT");
    Serial.print("Firmware Version:");
    Serial.println(VER);
    Serial.print("SDK Version:");
    Serial.println(ESP.getSdkVersion());
    wifi_station_set_auto_connect(0);//关闭自动连接
    ESP.wdtEnable(5000);
    WiFi.disconnect();
    delay(100);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          cnt++;
          Serial.print(".");
          if(cnt>=40){
            cnt = 0;
            //重启系统
            delayRestart(1);
          }
     }
}

void initOneNetMqtt(){
    mqttClient.setServer(mqttServer,mqttPort);
    mqttClient.setClient(client);
    mqttClient.setCallback(callback);

    initOneNet(PRODUCT_ID,API_KEY,DEVICE_ID);
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
bool postTempToOneNet(){
   if(WiFi.status() != WL_CONNECTED) return false;
    Serial.println("TCP Client postTempToOneNet.");

    StaticJsonBuffer<230> jsonBuffer;
    //创建根，也就是顶节点
    JsonObject& root = jsonBuffer.createObject();
    //在root对象中加入data数组
    JsonArray& datastreams = root.createNestedArray("datastreams");
    JsonObject& stream = datastreams.createNestedObject();
    stream["id"] = "temperature";
    JsonArray& datapoints = stream.createNestedArray("datapoints");
    JsonObject& value1 = datapoints.createNestedObject();
    value1["value"] =  random(20, 50);
    JsonObject& value2 = datapoints.createNestedObject();
    value2["value"] =  random(20, 50);
       
    int len = root.measureLength();
    char buffer[100];
    root.printTo(buffer, 100);
    String data;
    for(int index = 0;index<len;index++){
           data += buffer[index];
    }
    return postToDeviceDataPoint(data);
}
