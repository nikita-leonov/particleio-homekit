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
    int result = -1;
    if (value.length() > 0) {
        float hueFloat = value.toFloat();
        gHue = hueFloat / 360.0;
        result = value.toInt();
    }
    updateLED();
    return result;
}

int saturation(String value) {
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

void hslToRgb(float h, float s, float l){
    float tempR, tempG, tempB;
    int r, g, b;

    if(l == 0){
        ledControl(0,0,0);
    }
    if(s == 0){
        r = g = b = l; // achromatic
    }else{
        float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        tempR = hue2rgb(p, q, h + 1.0/3.0);
        tempG = hue2rgb(p, q, h);
        tempB = hue2rgb(p, q, h - 1.0/3.0);
    }
    
    r = round(tempR * 255);
    g = round(tempG * 255);
    b = round(tempB * 255);

    ledControl(r,g,b);
}

float hue2rgb(float p, float q, float t){
    if(t < 0.0){ 
        t += 1.0;
    }
    if(t > 1.0){
        t -= 1.0;
    }
    if(t < 1.0/6.0){
        return p + (q - p) * 6.0 * t;
    }
    if(t < 1.0/2.0) {
        return q;
    }
    if(t < 2.0/3.0) {
        return p + (q - p) * (2.0/3.0 - t) * 6.0;
    }
    return p;
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
        hslToRgb(gHue, gSaturation, gBrightness);
    } else {
        ledControl(0,0,0);
    }
}
