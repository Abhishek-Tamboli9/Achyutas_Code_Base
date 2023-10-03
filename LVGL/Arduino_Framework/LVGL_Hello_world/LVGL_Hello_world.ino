/*
* Author		: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Thur 21 Sept 2023
* File 		: LVGL_Hello_world.ino
* Title		: LVGL Hello World Example
* Description 	: It displays a "Hello world" label in white text on a green background and initializes the LVGL library for creating graphical user interfaces.
*/

#include <lvgl.h>
#include <TFT_eSPI.h>

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

    	lv_init();

	#if LV_USE_LOG != 0
    		lv_log_register_print_cb( my_print ); /* register print function for debugging */
	#endif

    	tft.begin();          /* TFT init */
    	tft.setRotation( 4 ); /* Landscape orientation, flipped */

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

    	/* Create simple label 
    	lv_obj_t *label = lv_label_create( lv_scr_act() );
    	lv_label_set_text( label, "Hello Abhishek");
    	lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
 	*/
    
    	/*Change the active screen's background color*/
    	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x32a852), LV_PART_MAIN);

    	/*Create a white label, set its text and align it to the center*/
    	lv_obj_t * label = lv_label_create(lv_scr_act());
    	lv_label_set_text(label, "Hello world");
    	lv_obj_set_style_text_color(lv_scr_act(), lv_color_hex(0xffffff), LV_PART_MAIN);
    	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    	Serial.println( "Setup done" );
}

void loop()
{
    	lv_timer_handler();					 /* let the GUI do its work */
    	delay( 5 );
}
