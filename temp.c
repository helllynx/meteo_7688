#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <SFE_BMP180.h>
#include <SPI.h>
#include <Wire.h>


#define OLED_RESET 4
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16
#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

#define DHTPIN 8 // what digital pin we're connected to
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SH1106 display(OLED_RESET);

SFE_BMP180 pressure;
double baseline; // baseline pressure


void setup() {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally!
  // (neat!)
  display.begin(SH1106_SWITCHCAPVCC,
                0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // BMP180
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1)
      ; // Pause forever.
  }

  // DHT
  dht.begin();

  delay(2000);
}

void loop() {

  // BMP180
  double P = getPressure() * 0.750062;


  // DHT22
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  display.clearDisplay();
  // OLED SH1106
  display.setCursor(0, 0);
  display.print("Hum: ");
  display.print(h);
  display.println("%");

  display.print("DHT_T: ");
  display.print(hic);
  display.println("C");

  display.print("Press: ");
  display.print(P);
  display.println(" mmHG");

  display.display();
  delay(2000);

}

double getPressure() {
  char status;
  double T, P, p0, a;

  // You must first get a temperature measurement to perform a pressure reading.

  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0) {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0) {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res,
      // longest wait). If request is successful, the number of ms to wait is
      // returned. If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0) {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature
        // measurement (T). (If temperature is stable, you can do one
        // temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P, T);
        if (status != 0) {
          return (P);
        } else
          Serial.println("error retrieving pressure measurement\n");
      } else
        Serial.println("error starting pressure measurement\n");
    } else
      Serial.println("error retrieving temperature measurement\n");
  } else
    Serial.println("error starting temperature measurement\n");
}
