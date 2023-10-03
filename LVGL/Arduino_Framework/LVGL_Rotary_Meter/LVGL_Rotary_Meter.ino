/*
* Author	: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Mon 03 Oct 2023
* File 		: LVGL_Rotary_Meter.ino
* Title		: Arduino-LVGL TFT Meter Interface with Rotary Encoder
* Description 	: It features a multi-colored meter with arcs and a needle indicator and the meter's value is controlled by a rotary encoder (outputA and outputB).
*/

#include <lvgl.h>
#include <core/lv_obj.h>
#include <widgets/lv_btn.h>
#include <extra/widgets/meter/lv_meter.h>
#include <TFT_eSPI.h>

#define outputA 34
#define outputB 35
//#define Switch 35
#define MAX 100
#define MIN 0

static lv_obj_t *meter;
lv_meter_indicator_t * indic;
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * screenHeight / 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  	Serial.printf(buf);
  	Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  	uint32_t w = (area->x2 - area->x1 + 1);
  	uint32_t h = (area->y2 - area->y1 + 1);

  	tft.startWrite();
  	tft.setAddrWindow(area->x1, area->y1, w, h);
  	tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  	tft.endWrite();

  	lv_disp_flush_ready(disp_drv);
}

static void set_value(void * indic, int32_t v)
{
  	lv_meter_set_indicator_value(meter, (lv_meter_indicator_t *)indic, v);
}

void setup()
{
  	Serial.begin(115200); /* prepare for possible serial debug */

  	lv_init();

	#if LV_USE_LOG != 0
  		lv_log_register_print_cb(my_print); /* register print function for debugging */
	#endif

  	tft.begin();          /* TFT init */
  	tft.setRotation(1);   /* Landscape orientation, flipped */

 	 lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 10);

  	/*Initialize the display*/
  	static lv_disp_drv_t disp_drv;
 	 lv_disp_drv_init(&disp_drv);
 
 	 /*Change the following line to your display resolution*/
  	disp_drv.hor_res = screenWidth;
 	 disp_drv.ver_res = screenHeight;
 	 disp_drv.flush_cb = my_disp_flush;
  	disp_drv.draw_buf = &draw_buf;
  	lv_disp_drv_register(&disp_drv);
  	Serial.println("LVGL setup done");

  	pinMode(outputA, INPUT);
  	pinMode(outputB, INPUT);
  
 	 /*Change the active screen's background color*/
 	 lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x7a596f), LV_PART_MAIN);
    
 	 meter = lv_meter_create(lv_scr_act());
  	lv_obj_center(meter);
 	// lv_obj_set_size(meter, 120, 120);
  	lv_obj_set_size(meter, 200, 200);     // for 320 * 240 display size

 	static lv_style_t style_meter;
 	lv_style_init(&style_meter);
  	lv_style_set_bg_color(&style_meter, lv_color_hex(0x000000));
  	lv_style_set_bg_opa(&style_meter, LV_OPA_50);
  	lv_style_set_border_width(&style_meter, 2);
  	lv_style_set_border_color(&style_meter, lv_color_black());
 	lv_obj_add_style(meter,&style_meter, LV_PART_MAIN); 

	 /*Add a scale first*/
  	lv_meter_scale_t * scale = lv_meter_add_scale(meter);
  	//lv_meter_set_scale_ticks(meter, scale, 21, 2, 5, lv_palette_main(LV_PALETTE_DEEP_PURPLE));		 // Reduced number of ticks
  	//lv_meter_set_scale_major_ticks(meter, scale, 4, 4, 8, lv_color_black(), 8); 						// Reduced number of major ticks
  	lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));       			// for 320 * 240 display size
 	lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);              					// for 320 * 240 display size

  	/*Add a blue arc to the start*/
  	indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_GREEN), 0);
  	lv_meter_set_indicator_start_value(meter, indic, 0);
  	lv_meter_set_indicator_end_value(meter, indic, 20);

  	/*Make the tick lines blue at the start of the scale*/
  	indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
  	lv_meter_set_indicator_start_value(meter, indic, 0);
  	lv_meter_set_indicator_end_value(meter, indic, 20);
  
  	/*Add a orange arc to the center*/
  	indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_ORANGE), 0);
  	lv_meter_set_indicator_start_value(meter, indic, 21);
  	lv_meter_set_indicator_end_value(meter, indic, 70);

 	/*Make the tick lines orange at the centre of the scale*/
  	indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_ORANGE), lv_palette_main(LV_PALETTE_ORANGE), false, 0);
  	lv_meter_set_indicator_start_value(meter, indic, 21);
  	lv_meter_set_indicator_end_value(meter, indic, 70);
  
  	/*Add a red arc to the end*/
  	indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
  	lv_meter_set_indicator_start_value(meter, indic, 71);
  	lv_meter_set_indicator_end_value(meter, indic, 100);

 	/*Make the tick lines red at the end of the scale*/
 	indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
  	lv_meter_set_indicator_start_value(meter, indic, 71);
  	lv_meter_set_indicator_end_value(meter, indic, 100);
  
  	/*Add a needle line indicator*/
  	indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);
  	Serial.println("Setup done");
}

void loop()
{
  	lv_timer_handler(); /* let the GUI do its work */

  	int currentState = digitalRead(outputA);
  	static int prevState = currentState;
  	static int counter = 0;

  	if (counter >= MIN && counter <= MAX)
  	{
    		if (currentState != prevState)
    		{
      			if (digitalRead(outputB) != currentState)
      			{
        				counter--;
      			}
      			else
      			{
        				counter++;
      			}
      			Serial.print("Position:");
      			Serial.print(counter);
      			set_value(indic, counter);
    		}
    		prevState = currentState;
    		delay(5);
  	}
  	else
  	{
    		if (counter > 100)
      			counter = 100;
    	else if (counter < 0)
     			counter = 0;
  	}
}
