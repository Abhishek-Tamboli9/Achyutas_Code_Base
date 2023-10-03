# LVGL using Arduino Framework
## Setting up the LVGL library:
  1. Download the LVGL library using library manager of Arduino IDE.
  2. Go to Tools > manage libraries
  3. Search for lvgl and install.
     ![image](https://github.com/Abhishek-Tamboli9/Achyutas_Code_Base/assets/28736491/5b995095-a35b-4353-8547-34d697fe8418)
  4. Now go to installed library path of lvgl. In my case:-> C:\Users\svcs\Documents\Arduino\libraries\lvgl
  5. Copy lv_conf_template.h header file into the Arduino Libraries directory next to the lvgl library folder.
  6. Change it’s name from lv_conf_template to lv_conf.h
![image](https://github.com/Abhishek-Tamboli9/Achyutas_Code_Base/assets/28736491/4a801334-554f-4ea3-8bed-d462eeaf79ba)
  7. Now open the lv_conf.h file and do the following settings.
  8. Change the first #if 0 to #if 1.
  9. Set the color depth of you display in LV_COLOR_DEPTH.
  10. et LV_TICK_CUSTOM 1.

  ## Setting up the drivers for display (TFT display only):
  ### Method 1: Copy User_Setup.h file from TFT_eSPI directory to your installed library path of TFT_eSPI. In my case:-> C:\Users\svcs\Documents\Arduino\libraries\TFT_eSPI
  ### Method 2:
  1. Download the TFT_eSPI library using library manager of Arduino IDE.
  2. Go to Tools > manage libraries.
  3. Search for TFT_eSPI by Bodmer and install. 
![image](https://github.com/Abhishek-Tamboli9/Achyutas_Code_Base/assets/28736491/96326d71-93a9-4b34-a6da-c260df069d56)
  4. Now go to installed library path of TFT_eSPI. In my case:-> C:\Users\svcs\Documents\Arduino\libraries\TFT_eSPI
  5. Now open User_Setup.h file and do the following changes.
     ![image](https://github.com/Abhishek-Tamboli9/Achyutas_Code_Base/assets/28736491/a14e3a7a-6780-47ed-9dda-e87518d00e6b)

  6. As here I am using IL9341 driver with esp32 so this configuration is based on the same. Please check your driver and make the necessary changes.
  7. Select driver #define ILI9341_DRIVER.
  8. Select the color order according to your driver
  9. Define the pixel width and height of your display.
  10. Define the pins that are used to interface with the display.
  11. Define the SPI frequency -> #define SPI_FREQUENCY  40000000
