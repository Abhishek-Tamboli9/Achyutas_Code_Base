/*
* Author	: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Mon 25 Sept 2023
* File 		: LVGL_Button_click.ino
* Title		: LVGL widget Button Interface with Switch
* Description 	: It allows interaction through Switch, responding to Switch(button) presses and updating the button's label text when clicked
*/

#include <lvgl.h>
#include <core/lv_obj.h>
#include <widgets/lv_btn.h>
#include <TFT_eSPI.h>

#define SW 5

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 128;
static const uint16_t screenHeight = 160;

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
lv_indev_t * indev_button;

void lv_port_indev_init(void)
{
    	/*Initialize your button if you have*/
   	button_init();
    
    	/*Register a button input device*/
    	static lv_indev_drv_t indev_drv;
    	lv_indev_drv_init(&indev_drv);
    	indev_drv.type = LV_INDEV_TYPE_BUTTON;
    	indev_drv.read_cb = button_read;
    	indev_button = lv_indev_drv_register(&indev_drv);

    	/*Assign buttons to points on the screen*/
    	static const lv_point_t btn_points[1] = {
       		 {10, 50},   /*Button 0 -> x:10; y:20*/
    	};
    	lv_indev_set_button_points(indev_button, btn_points);
}

/*Initialize your buttons*/
static void button_init(void)
{
    	pinMode(SW,INPUT);    /*  Input */
    	Serial.println("Switch Initialised");
}

/*Will be called by the library to read the button*/
static void button_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{

   	static uint8_t last_btn = 0;

    	/*Get the pressed button's ID*/
    	int8_t btn_act = button_get_pressed_id();

    	if(btn_act >= 0) {
        		data->state = LV_INDEV_STATE_PR;
        		last_btn = btn_act;
    	}
    	else {
        		data->state = LV_INDEV_STATE_REL;
    	}

    	/*Save the last pressed button's ID*/
    	data->btn_id = last_btn;
}

/*Get ID  (0, 1, 2 ..) of the pressed button*/
static int8_t button_get_pressed_id(void)
{
    	uint8_t i;

    	/*Check to buttons see which is being pressed (assume there are 2 buttons)*/
    	for(i = 0; i < 1; i++) 
    	{
        		/*Return the pressed button's ID*/
        		if(button_is_pressed(i)) 
        		{
          		Serial.println("Button id 0");
            		return i;
        		}
    	}
    	/*No button pressed*/
    	Serial.println("No Button id");
    	return -1;
}

/*Test if `id` button is pressed or not*/
static bool button_is_pressed(uint8_t id)
{
    	uint8_t buttonState = digitalRead(SW);
    	if(buttonState == HIGH)
    	{
      		Serial.println("Button is pressed");    
      		return true;
    }
   	else
    	{  
     		Serial.println("Button not pressed"); 
     		return false;
    	}
}

static void btn_event_cb(lv_event_t * e)
{
    	static uint8_t cnt = 0;
    	lv_event_code_t code = lv_event_get_code(e);
    	lv_obj_t * btn = lv_event_get_target(e);
    	if(code == LV_EVENT_CLICKED) 
    	{
        		cnt++;
        		/*Get the first child of the button which is the label and change its text*/
        		lv_obj_t * label = lv_obj_get_child(btn, 0);
        		lv_label_set_text_fmt(label, "Button: %d", cnt);
    	}
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

    	lv_obj_t * btn = lv_btn_create(lv_scr_act());                                         							 	/*Add a button the current screen*/
    	lv_obj_set_pos(btn, 10, 50);                                                          									/*Set its position*/
    	lv_obj_set_size(btn, 110, 50);                                                        									/*Set its size*/
    	lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    	lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);                           					/*Assign a callback to the button*/

    	static lv_style_t style_shadow;
    	lv_style_init(&style_shadow);
    	lv_style_set_shadow_width(&style_shadow, 10);
    	lv_style_set_shadow_spread(&style_shadow, 5);
    	lv_style_set_shadow_color(&style_shadow, lv_palette_main(LV_PALETTE_BLUE));
    	lv_obj_add_style(btn, &style_shadow, 0);
    
    	lv_obj_t * label = lv_label_create(btn);                                              								/*Add a label to the button*/
    	lv_label_set_text(label, "Button");                                                  									/*Set the labels text*/
    	lv_obj_center(label);

    	lv_port_indev_init();

    	Serial.println( "Setup done" );
}

void loop()
{
    	lv_timer_handler();                                                  											/* let the GUI do its work */
    	delay( 5 );
}
