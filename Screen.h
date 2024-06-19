#include <Adafruit_SSD1306.h>
#include <splash.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

////// ALL OF THIS CODE FROM ADAFRUIT SSD1306 LIB //////

#if defined(I2C_BUFFER_LENGTH)
#define WIRE_MAX min(256, I2C_BUFFER_LENGTH) ///< Particle or similar Wire lib
#elif defined(BUFFER_LENGTH)
#define WIRE_MAX min(256, BUFFER_LENGTH) ///< AVR or similar Wire lib
#elif defined(SERIAL_BUFFER_SIZE)
#define WIRE_MAX                                                               \
  min(255, SERIAL_BUFFER_SIZE - 1) ///< Newer Wire uses RingBuffer
#else
#define WIRE_MAX 32 ///< Use common Arduino core default
#endif

#define ssd1306_swap(a, b)                                                     \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

#if ARDUINO >= 100
#define WIRE_WRITE wire->write ///< Wire write function in recent Arduino lib
#else
#define WIRE_WRITE wire->send ///< Wire write function in older Arduino lib
#endif

#ifdef HAVE_PORTREG
#define SSD1306_SELECT *csPort &= ~csPinMask;       ///< Device select
#define SSD1306_DESELECT *csPort |= csPinMask;      ///< Device deselect
#define SSD1306_MODE_COMMAND *dcPort &= ~dcPinMask; ///< Command mode
#define SSD1306_MODE_DATA *dcPort |= dcPinMask;     ///< Data mode
#else
#define SSD1306_SELECT digitalWrite(csPin, LOW);       ///< Device select
#define SSD1306_DESELECT digitalWrite(csPin, HIGH);    ///< Device deselect
#define SSD1306_MODE_COMMAND digitalWrite(dcPin, LOW); ///< Command mode
#define SSD1306_MODE_DATA digitalWrite(dcPin, HIGH);   ///< Data mode
#endif

#if (ARDUINO >= 157) && !defined(ARDUINO_STM32_FEATHER)
#define SETWIRECLOCK wire->setClock(wireClk)    ///< Set before I2C transfer
#define RESWIRECLOCK wire->setClock(restoreClk) ///< Restore after I2C xfer
#else // setClock() is not present in older Arduino Wire lib (or WICED)
#define SETWIRECLOCK ///< Dummy stand-in define
#define RESWIRECLOCK ///< keeps compiler happy
#endif

#if defined(SPI_HAS_TRANSACTION)
#define SPI_TRANSACTION_START spi->beginTransaction(spiSettings) ///< Pre-SPI
#define SPI_TRANSACTION_END spi->endTransaction()                ///< Post-SPI
#else // SPI transactions likewise not present in older Arduino SPI lib
#define SPI_TRANSACTION_START ///< Dummy stand-in define
#define SPI_TRANSACTION_END   ///< keeps compiler happy
#endif

// The definition of 'transaction' is broadened a bit in the context of
// this library -- referring not just to SPI transactions (if supported
// in the version of the SPI library being used), but also chip select
// (if SPI is being used, whether hardware or soft), and also to the
// beginning and end of I2C transfers (the Wire clock may be sped up before
// issuing data to the display, then restored to the default rate afterward
// so other I2C device types still work).  All of these are encapsulated
// in the TRANSACTION_* macros.

// Check first if Wire, then hardware SPI, then soft SPI:
#define TRANSACTION_START                                                      \
  if (wire) {                                                                  \
    SETWIRECLOCK;                                                              \
  } else {                                                                     \
    if (spi) {                                                                 \
      SPI_TRANSACTION_START;                                                   \
    }                                                                          \
    SSD1306_SELECT;                                                            \
  } ///< Wire, SPI or bitbang transfer setup
#define TRANSACTION_END                                                        \
  if (wire) {                                                                  \
    RESWIRECLOCK;                                                              \
  } else {                                                                     \
    SSD1306_DESELECT;                                                          \
    if (spi) {                                                                 \
      SPI_TRANSACTION_END;                                                     \
    }                                                                          \
  } ///< Wire, SPI or bitbang transfer end


////// END OF ALL OF THIS CODE FROM ADAFRUIT SSD1306 LIB //////

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

class Screen : public Adafruit_SSD1306 {
  public:
  Screen() : Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

  void display() {
    
    static const uint8_t PROGMEM dlist1[] = {
        SSD1306_PAGEADDR,
        0,                      // Page start address
        0xFF,                   // Page end (not really, but works here)
        SSD1306_COLUMNADDR, 0}; // Column start address
   

    long actionCount = 0;

    if (count == 0) {
      TRANSACTION_START

      ssd1306_commandList(dlist1, sizeof(dlist1));
      ssd1306_command1(WIDTH - 1); // Column end address

      count = WIDTH * ((HEIGHT + 7) / 8);

      wire->beginTransmission(i2caddr);
      WIRE_WRITE((uint8_t)0x40);

      ptr = buffer;
    }

    while (count) {
      if (bytesOut >= WIRE_MAX) {
        wire->endTransmission();
        wire->beginTransmission(i2caddr);
        WIRE_WRITE((uint8_t)0x40);
        bytesOut = 1;
      }
      WIRE_WRITE(*ptr++);
      bytesOut++;
      count--;
      actionCount++;

      if (actionCount >=16 && count != 0) {
        return;
      }
    }

    if (count == 0) {
      wire->endTransmission();
      TRANSACTION_END
    }
  }

  uint16_t count = 0;
  uint16_t bytesOut = 1;
  uint8_t *ptr;
};