#include "math.h"

int gPowerState = 1;
float gHue = 1;
float gSaturation = 1;
float gBrightness = 1;

int redPin = A0;    // RED pin of the LED to PWM pin **A0**
int greenPin = D0;  // GREEN pin of the LED to PWM pin **D0**
int bluePin = D1;   // BLUE pin of the LED to PWM pin **D1**

void setup() {
   pinMode(redPin, OUTPUT);
   pinMode(greenPin, OUTPUT);
   pinMode(bluePin, OUTPUT);

   //Exposed internal functions as cloud API
   Particle.function("powerState", powerState);
   Particle.function("hue", hue);
   Particle.function("saturation", saturation);
   Particle.function("brightness", brightness);
}

void loop() {
    /* Do nothing */
}

/* Cloud methods */

int powerState(String value) {
    int result = 1;
    if (value.length() > 0) {
        gPowerState = (value.toInt() == 1);
        result = gPowerState;
    }
    updateLED();
    return result;
}

int hue(String value) {
    delay(200);
    int result = -1;
    if (value.length() > 0) {
        float hueFloat = value.toFloat();
        gHue = hueFloat;// / 360.0;
        result = value.toInt();
    }
    updateLED();
    return result;
}

int saturation(String value) {
    delay(200);
    int result = -1;
    if (value.length() > 0) {
        float saturationFloat = value.toFloat();
        gSaturation = saturationFloat / 100.0;
        result = value.toInt();
    }
    updateLED();
    return result;
}

int brightness(String value) {
    delay(200);
    int result = -1;

    if (value.length() > 0) {
        float brightnessFloat = value.toFloat();
        gBrightness = value.toFloat() / 100.0;
        result = value.toInt();
    }

    updateLED();

    return result;
}

/* LED color manipulation methods */

void hsvToRgb(float h, float s, float v) {
	float r, g, b;
	int i;
	float f, p, q, t;

	if(s == 0) {
		// Achromatic (grey)
		r = g = b = v;
		ledControl(round(r * 255), round(g * 255), round(b * 255));
	}

	h /= 60; // sector 0 to 5
	
	i = floor(h);
	f = h - i; // factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
 
	switch(i) {
		case 0:
			r = v;
			g = t;
			b = p;
			break;
 
		case 1:
			r = q;
			g = v;
			b = p;
			break;
 
		case 2:
			r = p;
			g = v;
			b = t;
			break;
 
		case 3:
			r = p;
			g = q;
			b = v;
			break;
 
		case 4:
			r = t;
			g = p;
			b = v;
			break;
 
		default: // case 5:
			r = v;
			g = p;
			b = q;
	}
 
	ledControl(round(r * 255), round(g * 255), round(b * 255));
}

void ledControl(int redValue, int greenValue, int blueValue)
{
   // write the mixed color
   analogWrite(redPin, redValue);
   analogWrite(greenPin, greenValue);
   analogWrite(bluePin, blueValue);
}

// Only update if turned on
void updateLED() {
    if (gPowerState) {
        hsvToRgb(gHue, gSaturation, gBrightness);
    } else {
        ledControl(0,0,0);
    }
}
