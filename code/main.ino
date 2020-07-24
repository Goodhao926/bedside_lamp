#include <FastLED.h>            // 此示例程序需要使用FastLED库
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "controller.h"

// WIFI和MQTT的配置
const char* wifi_ssid = "HUAWEI-040T7E";
const char* wifi_pass = "cxqljh1971723929";
const char* mqtt_server = "192.168.3.19";
// WIFI和MQTT和控制器的配置
WiFiClient espClient;
PubSubClient client(espClient);
Controller controll;

//MQTT回调
void callback(char* topic ,byte* payload ,unsigned int length)
{
  char* msg = (char*)payload;

  controll.process(topic,msg,length);
  //debug

//  Serial.print("Message arrived [");
//  Serial.print(topic);
//  Serial.print("] ");
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//  }//串口打印出收到的信息
//  Serial.println();
}
void setup_wifi()
{
  WiFi.begin(wifi_ssid,wifi_pass);
  while(WiFi.status()!= WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  delay(500);
  Serial.print("IP :");
  Serial.println(WiFi.localIP());
// }
// void lightControl(char* msg,char *topic)
// {
//   Serial.println("MQTT信息：");
//   Serial.println(msg);
//   if(strcmp(topic,"/smart_home/light/color")==0)
//   {

//     char* r = strtok(msg,",");
//     char* g = strtok(NULL,",");
//     char* b = strtok(NULL,",");
// //    Serial.println( atol(r));
// //    Serial.println( atol(g));
// //    Serial.println( atol(b));
    
    
//     fill_solid(leds,NUM_LEDS,CRGB( atol(r),atol(g),atol(b)));
//     FastLED.show();
//     return;
//   }
//   if(strcmp(topic,"/smart_home/light/bright")==0)
//   {
// //   Serial.print("亮度");
// //   Serial.println(atol(msg)); 
    
//     FastLED.setBrightness(atol(msg));
//     FastLED.show();
//     return;
//   }

  
//   if(strcmp(msg,"power")==0)
//   {
//       Serial.println("开灯");
//       mode_whiteLight();
//       power = true;
//   }else if(strcmp(msg,"close")==0)
//   {
//     mode3_status = false;
//     mode4_status = false;
//     power = false;
//     mode_close();
    
    
//   }
//   else if(strcmp(msg,"mode3")==0)
//   {
//     power = true;
//     mode3_status = true;
   
   
    
//   }
//   else if(strcmp(msg,"mode4")==0)
//   {
//     power= true;
//     mode4_status = true;
    
//   }
  
}





////---氛围模式
//void mode1(CHSV c)//某颜色5个跑马灯循环
//{
//  for(int i=0;i<NUM_LEDS-4;i++)
//  {
//    for(int j=0;j<5;j++)
//    {
//      leds[i+j] = c;
//    }
//    FastLED.show();
//    delay(25);
//    fadeToBlackBy(leds,NUM_LEDS, 255);
//    
//  }
//}
//void mode2()
//{
//  for(int i=0;i<255;i+=8)
//  {
//  fill_rainbow(leds,NUM_LEDS,i,9);
//  FastLED.show();
//  delay(5);
//  }
//}
//void mode3(CHSV c)
//{
// 
//  for(int i=0; i<255;i++)
//  {
//   c.v = i;
//    fill_solid(leds,NUM_LEDS,c);
//    FastLED.show();
//    delay(5);
//  }
//  for(int i=255; i>0;i--)
//  {
//   c.v = i;
//    fill_solid(leds,NUM_LEDS,c);
//    FastLED.show();
//    delay(5);
//  }
//}
//void mode_whiteLight()
//{
//  FastLED.setBrightness(255);
//  fill_solid(leds,NUM_LEDS,CHSV(0,0,255));
//  FastLED.show();
//  Serial.println("白灯开启");
//}
//void mode_close()
//{
//  fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
//  FastLED.show();
//}
//void mode4(CHSV c)
//{
//  for(int i=0;i<5;i++)
//  {
//    
//    fill_solid(leds,NUM_LEDS,CHSV(random(0,255),255,255));    
//    FastLED.show();
//    delay(100);
//  }
//  mode1(c);
//}
void setup() { 
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
}
void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection....");
    if(client.connect("ESP8266 Client"))
    {
      Serial.print("MQTT connected");//MQTT连接成功
      // MQTT订阅
       client.subscribe("/smart_home/light/#");
    }else
    {
      Serial.print("Failed ,rc = ");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
    
  }
  
}


 
void loop () {
  if(!client.connected())
  {
    reconnect();
  }
  client.loop();
} 
