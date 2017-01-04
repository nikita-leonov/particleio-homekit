boolean gPowerState = true;
float gHue = 1;
float gSaturation = 1;
float gBrightness = 1;

void setup()
{
   RGB.control(true);

   updateLED();

   //Exposed internal functions as cloud API
   Particle.function("powerState", powerState);
   Particle.function("hue", hue);
   Particle.function("saturation", saturation);
   Particle.function("brightness", brightness);
}

/* Cloud methods */

int powerState(String value) {
    int result = -1;

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
        gHue = (float)value.toInt() / 100.0;
        result = value.toInt();
    }

    updateLED();

    return result;
}

int saturation(String value) {
    int result = -1;

    if (value.length() > 0) {
        gSaturation = (float)value.toInt() / 100.0;
        result = value.toInt();
    }

    updateLED();

    return result;
}

int brightness(String value) {
    int result = -1;

    if (value.length() > 0) {
        gBrightness = (float)value.toInt() / 100.0;
        result = value.toInt();
    }

    updateLED();

    return result;
}

/* LED color manipulation methods */

float hueToRGB(float p, float q, float t) {
    if(t < 0.0) t += 1.0;
    if(t < 1.0/6.0) return p + (q - p) * 6.0 * t;
    if(t < 1.0/2.0) return q;
    if(t < 2.0/3.0) return p + (q - p) * (2.0/3.0 - t) * 6.0;

    return p;
}

void applyHSL(float h, float s, float l) {
    float r = 0;
    float g = 0;
    float b = 0;

    if(s == 0) {
        r = g = b = l;
    } else {
        float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
        float p = 2.0 * l - q;
        r = hueToRGB(p, q, h + 1.0/3.0);
        g = hueToRGB(p, q, h);
        b = hueToRGB(p, q, h - 1.0/3.0);
    }

    RGB.color((int)(r*255.0), (int)(g*255.0), (int)(b*255.0));
    RGB.brightness((int)(l*255.0));
}

void updateLED() {
    if (gPowerState) {
        applyHSL(gHue, gSaturation, gBrightness);
    } else {
        applyHSL(0, 0, 0);
    }
}
