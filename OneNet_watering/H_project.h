#ifndef _MAIN_H__
#define _MAIN_H__


extern "C" {
#include "user_interface.h"
#include "smartconfig.h"
}

/************** ESP8266相关操作 **************************/
void delayRestart(float t);
void delayNs(uint8_t m);
/*********************************************************/

/*************** OneNet MQTT相关操作 ****************************/
void initOneNet(const char *productId,const char *apiKey,const char *deviceId);
int connectToOneNetMqtt();
unsigned long lastOneNetPostTick=0;
/*********************************************************/

#define ONENET_DISCONNECTED 1 //已经断开
#define ONENET_CONNECTED 2    //已经连接上
#define ONENET_RECONNECT 3    //重连成功

//常量
#define VER             "MQTT_LED_V1.0"
const char* ssid = "Honor 9i";//wifi账号
const char* password = "1122334455";//wifi秘密

/*************** OneNet HTTP相关操作 *********************/
const char* OneNetServer = "api.heclouds.com";
/*********************************************************/

/*************** OneNet HTTP相关操作 *********************/
WiFiClient client;
HTTPClient http;
String response;
const char* host = "api.heclouds.com";
bool postToDeviceDataPoint(String data);
/*********************************************************/

/*********************** 项目相关操作 ********************/
#define VER             "HTTP_TEMP_V1.1"

const unsigned long BAUD_RATE = 115200;// serial connection speed
unsigned long lastWiFiCheckTick = 0;
bool ledState = 0;
unsigned long lastOneNetPostTick=0;
/*********************************************************/
//OneNet相关
PubSubClient mqttClient;

const char* mqttServer = "183.230.40.39";//mqtt服务器
const uint16_t mqttPort = 6002;
#define PRODUCT_ID    "302360"
#define API_KEY    "BBBWybbMz7rJt17T37MMMeEanwY="
#define DEVICE_ID "576026435"
#define DEVICE_ID1 "575958425"
#define TOPIC     "LED_CONTROL"

#endif
