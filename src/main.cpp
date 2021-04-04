// clocktime v2021.0
// by Andrew Huster (ahuster)
// Compatible with ESP8266 and ESP82

// includes
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// check which ESP board we're on
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <ezTime.h>

// SET YOUR TIMEZONE HERE
static const char ourtz[] = "America/Detroit";
// SET YOUR WIFI SSID AND PASSWORD HERE
static const char ssid[] = "Kilobyte";
static const char wifipw[] = "ytterby1";
// Set this to false to not blink the colon every second
static const bool blinkcolon = true;

Adafruit_7segment matrix = Adafruit_7segment();
Timezone myTZ;

bool colon = false;
String timeDigs;
char buf[5] = "0000";

void setup() {
	Wire.begin();
	matrix.begin(0x70);
	Serial.begin(115200);
	WiFi.begin(ssid, wifipw);
	setDebug(INFO);

	matrix.setBrightness(7);
	matrix.print(0xdead, HEX);
	matrix.drawColon(false);
	matrix.writeDisplay();
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print("...");
	}
	Serial.println("Connected.");

	waitForSync();

	if (myTZ.setLocation(FPSTR(ourtz)))
	{
		Serial.println("tzlookup good");
	}
	else
	{
		Serial.println("tzlookup fail");
		myTZ = UTC;
	}
}

void loop() {
	events();
	if(secondChanged())
	{
		timeDigs = myTZ.dateTime("Hi");
		timeDigs.toCharArray(buf, 5);
		matrix.print(atoi(buf), DEC);
		if (blinkcolon)
		{
			matrix.drawColon(colon); // blinks the colon on or off every second
			colon = !colon;
		}
		matrix.writeDisplay();
	}
}