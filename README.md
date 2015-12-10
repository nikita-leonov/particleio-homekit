# Siri + HomeKit + Particle.io + HomeBridge + Node.JS
I recently built a home automation PoC with a Particle.io, HomeBridge node.js framework and  HomeKit: [![video](https://www.dropbox.com/s/wfb5unfd5a8fkt8/vimeo.png?dl=1)](http://vimeo.com/nikitaleonov/siri-homekit-particle "video")
Video got some traction in specialized communities and I got a request explain how this is done. So, here it is, a guide how to build similar thing at home.

# Particle.io
My goal was to expose [Particle.io](http://particle.io) hardware features to be controlled with Siri through a HomeKit. Particle.io is an interesting device, since they provide not only a hardware, but also a cloud platform that allows you to expose hardware features as a REST API. Platform simplifies discoverability of a device, as it assigns permanent URL to it. It also simplifies access management by providing access tokens layer on a top of it. For my initial experiment I decided to create a Philips HUE hardware simulator, that will allow me to control on/off states, hue, brightness and saturation of Particle.io on-board LED. Since this is not a tutorial on programming Particle.io I will skip the part how to configure device, as well as how to program it. By a [link](hue-light-simulator.ino) you can review a code that I used for the demo.

With the code above I was able to control all planned characteristics of device by a simple curl commands like following:
```
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/powerState -d "value=1" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/hue -d "value=65" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/saturation -d "value=75" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/brightness -d "value=60" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
```

This is how simple it is to control real hardware over internet with Particle.io

# HomeKit + HomeBridge
HomeKit is an iOS based database of devices that provides an unified access to services and  characteristics, as well as manages metadata about grouping and locations such as homes, floors, rooms, zones etc. HomeKit devices should be discoverable via Bonjour, as well as implement specific HomeKit protocol. Implementing of all of these is a huge overhead for a small home experiment as my one. This is where [HomeBridge](https://github.com/nfarina/homebridge) enters the game. HomeBridge is JavaScript library for node.js that provides a set of device/platform specific implementations  on a  top of protocol stack required for HomeKit integration. So if you have devices like PhilipsHue or  WiMo you can integrate them with zero coding involved.
I will skip an instructions on installing node.js and HomeBridge as it is well described on other sites and will start directly from specific of this experiment. HomeBridge provides an [HTTP Device implementation](https://github.com/nfarina/homebridge/blob/master/accessories/Http.js) which is a lamp with an on/off states and brightness controllable with a pure HTTP requests. In theory if you have such device available you can again seamlessly integrate it by providing configuration with zero coding involved. However my Particle.io device has its own specific:
* Device require authorization in a form of authorization_token provided in a body of request.
* Every command that we expose require arguments provided in a body of request.
* Current device does not support some characteristics that we want to expose, such as hue and saturation.

All items above required some minor modifications in the code, the final solution you can find [here](ParticleAccessory.js). You can copy it into your accessories folder of HomeBridge to use. With this implementation Particle.io HUE device could be configured in JSON as following.

```
{
    "accessories": [
        {
            "accessory": "Particle",
            "name": "Particle Sensor",
            "platform_url": "https://api.particle.io/v1",
            "device_id": "53ff69063678505521262467",
            "access_token": "44864d2eb7e2837fec72b8272df66acc95f44386"
        }
    ]
}
```

After device is configured and HomeBridge server is up and running somewhere in the same network, as phone we can configure HomeKit device on it. There is no Apple's official HomeKit application yet, so as HomeBridge installation guide suggests I used one of apps available in the appstore to manage devices. I installed [Insteon+](https://itunes.apple.com/us/app/insteon+/id919270334?mt=8). It discovered my device as HomeKit device and allowed me to add and control it.

# Siri
As you saw in my original video I controlled my device with use of Siri. The good news that nothing need to be done. All HomeKit devices can be managed by Siri without any additional configuration. Enjoy.

# Future plans
While I successfully able to manage hardware as I needed, I believe the process of setup was cumbersome. I think that HomeBridge HTTP Device could be improved a lot and become a generic base for any device or platform integration. This is what I am working right now, writing my own implementation of HTTP Device to contribute back to HomeBridge codebase. I also plan to enable HomeKit integration for my Roomba as hardware experiment right after HomeBridge contribution will be completed.

# Feedback
I will appreciate any feedback and contributions. I am ready to discuss your questions in issues section, as well as accept any pull requests. Thanks!
