SET(SOURCES screens/ui_MainScreen.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_speedbg_png.c
    images/ui_img_speedfg_png.c
    images/ui_img_drawing1_png.c
    images/ui_img_final1_png.c
    images/ui_img_final11_png.c
    images/ui_img_189200015.c
    fonts/ui_font_Display16.c
    fonts/ui_font_Display24.c
    fonts/ui_font_Display20.c
    fonts/ui_font_Display70.c)

add_library(ui ${SOURCES})
