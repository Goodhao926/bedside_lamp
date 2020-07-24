#include <FastLED.h>


class Controller
{
private:
    bool current_status = false;// 灯的状态true开false关
    const char* cmd_openLight= "on";// 开指令
    const char* cmd_closeLight= "off";// 关指令
    const char* topic_cmd = "/smart_home/light/"; // 开关命令主题
    const char* topic_setRgb = "/smart_home/light/rgb/set";// RGB设置主题
    const char* topic_rgbState = "/smart_home/light/rgb/state";// RGB状态主题
    const char* topic_setBrightness = "/smart_home/light/brightness/set";// 设置亮度主题
    const char* topic_brightnessState = "/smart_home/light/brightness/state"; // 亮度状态主题
    const char* topic_state = "/smart_home/light/state" ;// 灯的开关状态主题
    int ledNum;
    short int r,g,b;
    void publishState();
   
    
    
public:
    Controller();//将leds和led的数量传进来
    bool getStatus();
    void process(char* topic, char* msg,unsigned int len);
    void mode_whiteLight();
    void mode_closeLight();
    void mode_rgb(short int red,short int green,short int blue);
    void mode_setBrightness(short int brightness);

    
};
