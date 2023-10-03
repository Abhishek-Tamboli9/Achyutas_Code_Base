/*
* Author	: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Fri 29 Sept 2023
* File 		: LVGL-Speedometer.ino
* Title		: Displaying a Speedometer using LVGL
* Description 	: It displays a digital speedometer with graphics which can be controlled by input device such as rotary encoders etc.
*/

#include <lvgl.h>
#include <core/lv_obj.h>
#include <widgets/lv_btn.h>
#include "ui.h"
#include <TFT_eSPI.h>

#define outputA 34        /*  CLK Pin */
#define outputB 35        /*  DT Pin  */

#define MAX 130
#define MIN 0

int currentSpeed = 0;
char buffer1[5];
void reset(void);
/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp_drv );
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
    pinMode(outputA, INPUT);
    pinMode(outputB, INPUT);
    
    lv_init();

    #if LV_USE_LOG != 0
      lv_log_register_print_cb( my_print ); /* register print function for debugging */
    #endif

    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

     ui_init();
     reset();

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler();                                                  /* let the GUI do its work */
    delay( 5 );
    int currentState = digitalRead(outputA);
    static int prevState = currentState;

    if (currentSpeed >= MIN && currentSpeed <= MAX)
    {
        if (currentState != prevState)
        {
            if (digitalRead(outputB) != currentState)
            {
                currentSpeed--;
            }
            else
            {
                currentSpeed++;
            }
            Serial.print("Position:");
            Serial.println(currentSpeed);
        }
        prevState = currentState;
        delay(5);
    }
    else
    {
        if (currentSpeed > 129)
            currentSpeed = 130;
        else if (currentSpeed < 0)
            currentSpeed = 0;
    }
      int offset = 12 + currentSpeed; 
      lv_slider_set_value( ui_SpeedSlider,offset, LV_ANIM_OFF);
      snprintf(buffer1,sizeof(buffer1),"%d",currentSpeed);
      lv_label_set_text(ui_SpeedLabel,buffer1);
}

void reset(void)
{
    lv_slider_set_value( ui_SpeedSlider, 0, LV_ANIM_OFF); 
    lv_label_set_text(ui_SpeedLabel,"0"); 
}
