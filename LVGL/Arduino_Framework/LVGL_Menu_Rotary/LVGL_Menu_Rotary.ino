/*
* Author		: Abhishek Tamboli (https://github.com/Abhishek-Tamboli9) 
* Date		: Fri 29 Sept 2023
* File 		: LVGL_Menu_Rotary.ino
* Title		: Browsing menu with encoder.
* Description 	:  The code sets up LVGL for creating graphical user interfaces and integrates the encoder and button for user input.
			   It uses a rotary encoder and a push button to browse from given menu options and select the options from menu. 
*/

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <core/lv_obj.h>
#include <widgets/lv_btn.h>

#define outputA 34    /*  CLK Pin */
#define outputB 35    /*  DT Pin  */
#define SWITCH 5          /*  Switch  */
 static lv_obj_t * meter;

/*
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#define ROTARY_ENCODER_A_PIN 34
#define ROTARY_ENCODER_B_PIN 35
#define ROTARY_ENCODER_BUTTON_PIN 18
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4
//AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
*/

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;
lv_indev_t * indev_encoder;
//lv_obj_t * _menu;

int currentState;
int prevState;

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


/*------------------
 * Encoder
 * -----------------*/
 
/*Initialize your encoder*/
static void encoder_init(void)
{
    pinMode(outputA, INPUT);
    pinMode(outputB, INPUT);
    pinMode(SWITCH,INPUT);    
    Serial.println("Encoder Initialised");
    currentState = digitalRead(outputA);
    prevState = currentState;
}

uint8_t key_pressed()
{
    uint8_t buttonState = digitalRead(SWITCH);

    if (buttonState == LOW)
    { 
      Serial.println("Enter button pressed");
      return LV_KEY_ENTER;
    }
    else 
    { 
      Serial.println("Enter button not pressed");
      return 0;
    }
}
/*static uint8_t enc_get_new_moves()
{
  //dont print anything unless value changed
  if (rotaryEncoder.encoderChanged())
  {
    int32_t value = rotaryEncoder.readEncoder();
    Serial.print("Value: ");
    Serial.println(value);
    return value;
  }
  else
    return 0;
}

void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}*/

static uint8_t enc_get_new_moves()
{
     //int currentState = digitalRead(outputA);
     //int prevState = currentState;
    currentState = digitalRead(outputA);
    if (currentState != prevState)
    {
        if (digitalRead(outputB) != currentState)
        {
            prevState = currentState;
            return 1;//LV_KEY_LEFT;
        }
        else
        {
            prevState = currentState;
            return -1; //LV_KEY_RIGHT;
        }
     
    }
    else if(currentState == prevState)
    {
        prevState = currentState;
        return 0;  
    }
}


/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static int32_t diff;
    
    Serial.println("Inside Encoder Read_function");
    if(key_pressed())
    {  
      data->state = LV_INDEV_STATE_PRESSED;
    }
    else 
    {
      data->state = LV_INDEV_STATE_RELEASED;
    }
    /* Optionally you can also use enc_diff, if you have encoder*/
    diff = enc_get_new_moves();
    if (diff > 0)
    {
        data->enc_diff = LV_KEY_LEFT; // Rotated clockwise
    }
    else if (diff < 0)
    {
        data->enc_diff = LV_KEY_RIGHT; // Rotated counterclockwise
    }
    else
    {
        data->enc_diff = 0; // No rotation
    }
}

void lv_port_indev_init(void)
{
    encoder_init();
    
    /*Register a encoder input device*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv);
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

static void set_value(void * indic, int32_t v)
{
     lv_meter_set_indicator_value(meter, (lv_meter_indicator_t *)indic, v);
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */
 
  /*  Initialize rotary encoder */
  //rotaryEncoder.begin();
  //rotaryEncoder.setup(readEncoderISR);
  
  //bool circleValues = false;
  //rotaryEncoder.setBoundaries(-1, 1, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  //rotaryEncoder.setAcceleration(250); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  
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

    /*  Input device Interface  */
    lv_port_indev_init();
    
    /*  Create the group  */
    lv_group_t * group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(indev_encoder, group);

    /*  MENU page creation  */
    /*Create a menu object*/
    lv_obj_t * _menu = lv_menu_create(lv_scr_act());
    //lv_menu_set_mode_root_back_btn(_menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    //lv_obj_add_event_cb(_menu, btnEventHandler, LV_EVENT_CLICKED, _menu);
    lv_obj_set_size(_menu, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
    lv_obj_center(_menu);
    lv_obj_clear_flag( _menu, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
    lv_obj_set_style_bg_color(_menu, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_opa(_menu, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(_menu, lv_color_hex(0x81808E), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_grad_dir(_menu, LV_GRAD_DIR_VER, LV_PART_MAIN| LV_STATE_DEFAULT);

    static lv_style_t style_label;
    lv_style_init(&style_label);
    lv_style_set_text_color(&style_label, lv_color_white());
 
    lv_group_add_obj(group, _menu);
    lv_obj_add_flag(_menu, LV_OBJ_FLAG_CLICKABLE |LV_OBJ_FLAG_SCROLL_ON_FOCUS | LV_OBJ_FLAG_CLICK_FOCUSABLE);
    lv_group_focus_next(group);
  
    lv_obj_t * label;
    lv_obj_t * cont;
    
    /*  Sub 1 page */
    lv_obj_t * sub_1_page = lv_menu_page_create(_menu, "Meter");
    cont = lv_menu_cont_create(sub_1_page);
    //label = lv_label_create(cont);
    //lv_label_set_text(label, "Hello, It's Page 1");

    /********************  METER **************************/
    meter = lv_meter_create(cont);
    lv_obj_center(meter);
    //lv_obj_set_size(meter, 200, 200);
    //lv_obj_set_size(meter, 120, 120);
    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    //lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    //lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);
    lv_meter_set_scale_ticks(meter, scale, 21, 2, 5, lv_palette_main(LV_PALETTE_GREY)); // Reduced number of ticks
    lv_meter_set_scale_major_ticks(meter, scale, 4, 4, 8, lv_color_black(), 8); // Reduced number of major ticks

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

    /*  Sub 2 page */
    lv_obj_t * sub_2_page = lv_menu_page_create(_menu, "Push Button");
    cont = lv_menu_cont_create(sub_2_page);
//    label = lv_label_create(cont);
//    lv_label_set_text(label, "Hello, It's Page 2");
    
    lv_obj_t * btn = lv_btn_create(cont);                                                  					/*Add a button the current screen*/
     lv_obj_set_pos(btn, 160, 120);
    //lv_obj_center(btn);/*Set its position*/
    lv_obj_set_size(btn, 110, 50);                                                        					/*Set its size*/
    lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
    lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);                           		/*Assign a callback to the button*/ 

    lv_obj_t * label2 = lv_label_create(btn);                                              					/*Add a label to the button*/
    lv_label_set_text(label2, "Button");                                                   					/*Set the labels text*/
    lv_obj_center(label2);

    /*  Create a main page  */
    lv_obj_t * _mainPage = lv_menu_page_create(_menu, NULL);

    /*  Sub_1 Page  */
    cont = lv_menu_cont_create(_mainPage);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Meter");
    lv_menu_set_load_page_event(_menu, cont, sub_1_page);

    lv_group_add_obj(group, cont);
    lv_group_focus_obj(cont);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE |LV_OBJ_FLAG_SCROLL_ON_FOCUS | LV_OBJ_FLAG_CLICK_FOCUSABLE );
    lv_obj_add_style(label, &style_label, LV_PART_MAIN);
    
    /*  Sub_2 Page  */        
    cont = lv_menu_cont_create(_mainPage);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Button");
    lv_menu_set_load_page_event(_menu, cont, sub_2_page);
    
    lv_group_add_obj(group, cont);
    lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE |LV_OBJ_FLAG_SCROLL_ON_FOCUS | LV_OBJ_FLAG_CLICK_FOCUSABLE );
    lv_group_focus_obj(cont);
    lv_obj_add_style(label, &style_label, LV_PART_MAIN);

    lv_menu_set_page(_menu, _mainPage);
    lv_group_focus_next(lv_group_get_default());   
    //lv_group_focus_next(group);
    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler();                                                  /* let the GUI do its work */
    delay( 5 );
}
