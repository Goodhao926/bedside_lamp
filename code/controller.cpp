#include "controller.h"
#include "Arduino.h"
#include <PubSubClient.h>
#define NUM_LEDS 15       // LED灯珠数量
#define LED_DT 5                // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序
uint8_t max_bright = 128;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
CRGB leds[NUM_LEDS];            // 建立光带leds

extern PubSubClient client;
Controller::Controller()
{
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带
  FastLED.setBrightness(max_bright);                            // 设置光带亮度
}
bool Controller::getStatus()
{
    return current_status;
}
void Controller::process(char* topic, char* msg,unsigned int len)
{
  //对信息进行预处理
  char msg_[128];
  strcpy(msg_,"");
  strncat(msg_,msg,len);
  Serial.print("主题:");
  Serial.print(topic);
  Serial.print("信息:");
  Serial.println(msg_);
  
  // 进入开关命令主题
  int res;
  if(strcmp(topic,topic_cmd)==0)
  {
     //开灯
  if(res == 0)
  {
    if(current_status == false)
    {
      current_status = true;
      mode_whiteLight();
      publishState();
    }
  }
  //关灯
  res = strcmp(msg_,cmd_closeLight);
  if(res == 0)
  {
    if(current_status == true)
    {
      current_status = false;
      mode_closeLight();
      publishState();
    }
  }
  }
  
 
  // 接受并显示RGB
  res = strcmp(topic,topic_setRgb);
  if(res == 0)
  {
     current_status = true;
     char* r_ = strtok(msg_,",");
     char* g_ = strtok(NULL,",");
     char* b_ = strtok(NULL,",");
     r = atol(r_);
     g = atol(g_);
     b = atol(b_);
     Serial.println( atol(r_));
     Serial.println( atol(g_));
     Serial.println( atol(b_));
     mode_rgb(r,g,b);
     return;
  }
  // 接收亮度
  res = strcmp(topic,topic_setBrightness);
  if(res == 0)
  {
    //将字符串亮度转换成整型亮度
    int bright = atol(msg_);
    mode_setBrightness(bright);
    return;
  }
  
}
//  灯处理函数
// 关灯
void Controller::mode_closeLight()
{
  fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
  FastLED.show();
  Serial.println("关灯");
}

//打开白灯
void  Controller::mode_whiteLight()
{
  r=255; g=255; b=255;
  fill_solid(leds,NUM_LEDS,CHSV(0,0,255));
  FastLED.show();
  client.publish(topic_rgbState,"255,255,255");
}
void Controller::mode_rgb(short int red,short int green,short int blue)
{
  fill_solid(leds,NUM_LEDS,CRGB(red,green,blue));
  FastLED.show();
  char rgb[11],r_[4],g_[4],b_[4];
  // 将参数转成全局设置
  r=red;g=green;b=blue;
  // 构建RGB字符串，模板为255,255,255
  itoa(r,r_,10);
  itoa(g,g_,10);
  itoa(b,b_,10);
  // 用字符串复制将原来内存随机产生覆盖，防止出现结束符导致溢出
  
  strcpy(rgb,r_);
  strcat(rgb,",");
  strcat(rgb,g_);
  strcat(rgb,",");
  strcat(rgb,b_);
  client.publish(topic_rgbState,rgb);
}
void Controller::mode_setBrightness(short int bright)
{
  FastLED.setBrightness(bright);
  FastLED.show(); 
  Serial.println("亮度");
  Serial.print(bright);
  char bright_str[4];
  itoa(bright,bright_str,10);
  client.publish(topic_brightnessState,bright_str);
}
void Controller::publishState()
{
  if(current_status == true)
  {
    client.publish(topic_state,cmd_openLight);
  }else
  {
    client.publish(topic_state,cmd_closeLight);
  }
}
