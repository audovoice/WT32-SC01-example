#include "ui.h"
#define LGFX_USE_V1       // set to use new version of library
#include <LovyanGFX.hpp>  // main library


class LGFX : public lgfx::LGFX_Device {

  lgfx::Panel_ST7796 _panel_instance;
  lgfx::Bus_SPI _bus_instance;
  lgfx::Light_PWM _light_instance;
  lgfx::Touch_FT5x06 _touch_instance;

  

public:
  LGFX(void) {
    {
      auto cfg = _bus_instance.config();

      cfg.spi_host = VSPI_HOST; // Select the SPI to use ESP32-S2,C3 : SPI2_HOST or SPI3_HOST / ESP32 : VSPI_HOST or HSPI_HOST
      // * With the ESP-IDF version upgrade, VSPI_HOST and HSPI_HOST descriptions are deprecated, so if an error occurs, use SPI2_HOST and SPI3_HOST instead.
      cfg.spi_mode = 3;                  // Set SPI communication mode (0 ~ 3)
      cfg.freq_write = 27000000;         // SPI clock when sending (up to 80MHz, rounded to 80MHz divided by an integer)
      cfg.freq_read = 6000000;           // SPI clock when receiving
      cfg.spi_3wire = false;             // set to true if receiving on MOSI pin
      cfg.use_lock = true;               // set to true to use transaction lock
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel to use (0=not use DMA / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=auto setting)
      // * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) is recommended for the DMA channel. Specifying 1ch and 2ch is deprecated.
      cfg.pin_sclk = 14; // set SPI SCLK pin number
      cfg.pin_mosi = 13; // Set MOSI pin number for SPI
      cfg.pin_miso = -1; // set SPI MISO pin number (-1 = disable)
      cfg.pin_dc = 21;   // Set SPI D/C pin number (-1 = disable)

      _bus_instance.config(cfg);              // Apply the settings to the bus.
      _panel_instance.setBus(&_bus_instance); // Sets the bus to the panel.
    }

    {

      auto cfg = _panel_instance.config(); // Get the structure for display panel settings.

      cfg.pin_cs = 15;   // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = 22;  // pin number where RST is connected (-1 = disable)
      cfg.pin_busy = -1; // pin number to which BUSY is connected (-1 = disable)

      // * The following setting values ​​are set to general default values ​​for each panel, and the pin number (-1 = disable) to which BUSY is connected, so please try commenting out any unknown items.

      cfg.memory_width = 320;  // Maximum width supported by driver IC
      cfg.memory_height = 480; // Maximum height supported by driver IC
      cfg.panel_width = 320;   // actual displayable width
      cfg.panel_height = 480;  // actual displayable height
      cfg.offset_x = 0;        // Panel offset in X direction
      cfg.offset_y = 0;        // Panel offset in Y direction
      cfg.offset_rotation = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits = 1;
      cfg.readable = false;
      cfg.invert = false;
      cfg.rgb_order = false;
      cfg.dlen_16bit = false;
      cfg.bus_shared = false;

      _panel_instance.config(cfg);
    }

    {
      auto cfg = _light_instance.config(); // Get the structure for backlight configuration.

      cfg.pin_bl = 23;     // pin number to which the backlight is connected
      cfg.invert = false;  // true to invert backlight brightness
      cfg.freq = 44100;    // backlight PWM frequency
      cfg.pwm_channel = 1; // PWM channel number to use

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance); // Sets the backlight to the panel.
    }

    {
      // Configure settings for touch screen control. (delete if not necessary)
      auto cfg = _touch_instance.config();

      cfg.x_min = 0;    // Minimum X value (raw value) obtained from the touchscreen
      cfg.x_max = 319;  // Maximum X value (raw value) obtained from the touchscreen
      cfg.y_min = 0;    // Minimum Y value obtained from touchscreen (raw value)
      cfg.y_max = 479;  // Maximum Y value (raw value) obtained from the touchscreen
      cfg.pin_int = 39; // pin number to which INT is connected
      cfg.bus_shared = false;
      cfg.offset_rotation = 0;

      // For I2C connection
      cfg.i2c_port = 1;    // Select I2C to use (0 or 1)
      cfg.i2c_addr = 0x38; // I2C device address number
      cfg.pin_sda = 18;    // pin number where SDA is connected
      cfg.pin_scl = 19;    // pin number to which SCL is connected
      cfg.freq = 400000;   // set I2C clock

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance); // Set the touchscreen to the panel.
    }
    setPanel(&_panel_instance);
  }
};


#include <lvgl.h>


LGFX tft;

#define screenWidth 480
#define screenHeight 320

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

void my_touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
  uint16_t touchX, touchY;
  bool touched = tft.getTouch(&touchX, &touchY);
  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;
    data->point.x = touchX;
    data->point.y = touchY;
  }
}



