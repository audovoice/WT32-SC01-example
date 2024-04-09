# WT32-SC01-example
Simple display example for WT32-SC01, using a Squareline Studio generated GUI.

This project if set up to run with PlatformIO in VS Code. 

The board used for this is, ESP32 Development board - WT32-SC01 with 320x480 capacitive multi-touch screen. 

Not to be confused with the WT32-SC01 Plus which has a difforent type of display with a faster updating screen.


WT32-SC01 uses an SPI screen. It has a ESP32-WROVER-B module.

Pin info.

    TFT (ST7796)
        TFT_RST=22
        TFT_SCLK=14
        TFT_DC=21
        TFT_CS=15
        TFT_MOSI=13
        TFT_MISO=-1
        TFT_BCKL=23
    Touch (FT6336U)
        TOUCH_SDA=18
        TOUCH_SCL=19
        I2C_TOUCH_ADDRESS=0x38

        
This project can have squareline studio generated project files dopped in to replace the gui. 

This example combines code from several projects.

SC01_Plus_HMI_example
https://github.com/jaimesbga/SC01_Plus_HMI_example

LVGL8-WT32-SC01-Arduino
https://github.com/sukesh-ak/LVGL8-WT32-SC01-Arduino

Lumia-ESP32 
https://github.com/fbiego/Lumia-ESP32


