/**************************************************************
 *
 *	This is a simple program to control a stand-alone
 *	ESP8266 module using the Blynk API over the
 *	internet.
 *
 **************************************************************/
//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <Arduino.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>

#define HEART_BEAT_PIN 16
#define RELAY_PIN 14

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "Blynk_API_AUTH_TOKEN";
// Your WiFi credentials.
char ssid[] = "SSID";
char pass[] = "YourWiFiPassword";

volatile int state;
volatile int count;

SimpleTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
	// You can send any value at any time.
	// Please don't send more that 10 values per second.
	Blynk.virtualWrite(V5, millis() / 1000);
}

BLYNK_WRITE(V0)
{
	int val = param.asInt();

	if(val == 1 || val == 0)
	{
		state = param.asInt();
	}
	else
	{
		state = 0;
	}
}

void setup()
{
	Serial.begin(115200); // See the connection status in Serial Monitor
	Blynk.begin(auth, ssid, pass);
	// Setup a function to be called every second
	timer.setInterval(1000L, myTimerEvent);

	count = 0;
}

void loop()
{
	Blynk.run(); // Initiates Blynk
	timer.run(); // Initiates SimpleTimer

	if(state == 1)
	{
		digitalWrite(RELAY_PIN, HIGH);		//If the state is HIGH, then output HIGH to the SSR
	}
	else
	{
		digitalWrite(RELAY_PIN, LOW);		//If the state is LOW, then output LOW to the SSR;
	}

	count++;	// Add on to the counter, counter used for heartbeat
	if(count > 19)	//every 20 loops of the function, a heartbeat output will be triggered to indicate the uC is still alive.
	{
		digitalWrite(HEART_BEAT_PIN, HIGH);
		myTimerEvent();				//This is used instead of delay() to stop the tcp conection dropping.
		count = 0;				//reset the heartbeat counter.
		digitalWrite(HEART_BEAT_PIN, LOW);	//turn off the output.
	}
}
