/*
* Author		: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Thur 21 Sept 2023
* File 		: LVGL_Meter.ino
* Title		: Displaying a Simple Meter using LVGL
* Description 	: It displays a multi-colored meter with arcs and a needle indicator that animates to simulate changing values
*/

#include <lvgl.h>
#include <core/lv_obj.h>
#include <widgets/lv_btn.h>
#include <extra/widgets/meter/lv_meter.h>
#include <TFT_eSPI.h>

static lv_obj_t * meter;

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
static void set_value(void * indic, int32_t v)
{
     	lv_meter_set_indicator_value(meter, (lv_meter_indicator_t *)indic, v);
}
void setup()
{
 	Serial.begin( 115200 ); /* prepare for possible serial debug */

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

    	/********************  METER **************************/
    	meter = lv_meter_create(lv_scr_act());
	lv_obj_center(meter);
	lv_obj_set_size(meter, 200, 200);
	//lv_obj_set_size(meter, 120, 120);

	/*Add a scale first*/
	lv_meter_scale_t * scale = lv_meter_add_scale(meter);
	lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
	lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

	lv_meter_indicator_t * indic;
	
	/*Add a blue arc to the start*/
	indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
	lv_meter_set_indicator_start_value(meter, indic, 0);
	lv_meter_set_indicator_end_value(meter, indic, 20);

	/*Make the tick lines blue at the start of the scale*/
	indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_BLUE),lv_palette_main(LV_PALETTE_BLUE),false, 0);
	lv_meter_set_indicator_start_value(meter, indic, 0);
	lv_meter_set_indicator_end_value(meter, indic, 20);
	
	/*Add a red arc to the end*/
	indic = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
	lv_meter_set_indicator_start_value(meter, indic, 80);
	lv_meter_set_indicator_end_value(meter, indic, 100);

	/*Make the tick lines red at the end of the scale*/
	indic = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED),lv_palette_main(LV_PALETTE_RED), false,0);
	lv_meter_set_indicator_start_value(meter, indic, 80);
	lv_meter_set_indicator_end_value(meter, indic, 100);

	/*Add a needle line indicator*/
	indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

	/*Create an animation to set the value*/
	lv_anim_t a;
	lv_anim_init(&a);
	lv_anim_set_exec_cb(&a, set_value);
	lv_anim_set_var(&a, indic);
	lv_anim_set_values(&a, 0, 100);
	lv_anim_set_time(&a, 2000);
	lv_anim_set_repeat_delay(&a, 100);
	lv_anim_set_playback_time(&a, 500);
	lv_anim_set_playback_delay(&a, 100);
	lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
	lv_anim_start(&a);
	Serial.println( "Setup done" );
}

void loop()
{
    	lv_timer_handler();                                                  /* let the GUI do its work */
    	delay( 5 );
}
