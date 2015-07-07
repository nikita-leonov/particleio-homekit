I recently built a home automation PoC with a Particle.io, HomeBridge node.js framework and  HomeKit  — [link](http://vimeo.com/nikitaleonov/siri-homekit-particle). Video got some traction in specialized commmunities and I got a request explain how this is done. So, here it is, a guide how to build similar thing at home. 

# Particle.io
My goal was to expose Particle.io hardware features to be controlled with Siri through a HomeKit. Particle.io is an interesting device, since they provide not only a hardware, but also a cloud platform that allows you to expose hardware features as a REST API. For my initial experiment I decided to create a Philips HUE hardware, that will allows me to control on/off states, hue, brightness and saturation of Particle.io on-board LED. Since this is not a tutorial on programming particle.io I will skip the part how to configure device, as well as how to write code. By a [link](hue-light-simulator.ino) you can review a code that I used for the demo. 

With the code above I was able to control all planned characteristics of device by a simple curl commands like following:
```
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/powerState -d "value=1" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/hue -d "value=65" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/saturation -d "value=75" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
curl -X POST https://api.particle.io/v1/devices/53ff69063678505521262467/brightness -d "value=60" -d access_token=44864d2eb7e2837fec72b8272df66acc95f44386
```

For sure for your own device you will need to insert your personal deviceId as well as auth-token, but anyway it explains how simple it is to control hardware over internet.

# HomeKit + HomeBridge
HomeKit is a iOS based database of devices that provides an unified access to services and  characteristics, as well as manages metadata about grouping and locations such as homes, floors, rooms, zones etc. HomeKit devices should be discoverable via Bonjour, as well as implement specific HomeKit protocol. Implementing of all of these is a huge overhead for a small home experiment as ours. This is where HomeBridge is enters the game. HomeBridge is JavaScript library for node.js that provides a set of device/platform specific implementations  on a  top of protocol stack required for HomeKit integration. So if you have devices like PhilipsHue or  WiMo you can integrate them with zer coding involved.
I will skip an instructions on installing node.js and HomeBridge and will start directly from specific of this experiment. HomeBridge provides an HTTP Device implementation <link> which is a lamp with an on/off states and brightness controllable with a pure HTTP requests. In theory if you have such device available you can again seamleslyy integrate it by providing configuration with zero coding involved. However our particle.io device has its own specific:
Device require authorization in a form of aurhorization_token provided in a body of request.
Every comand that we expose require arguments provided in a body of request.
Current device does not support some characteristics that we want to expose, such as hue and saturation.
All items above required some minor modifications in the code, the final solution you can find here. Now my custom particle.io HUE device could be configured in JSON as following. 

After device is configured and you have your HomeBridge server up and running somehwere in the same network, as your phone you can configure your HomeKit device on it. There is no Apple's official HomeKit applicaiton yet, so as HomeBridge installation guide suggests I used one of apps available in the appstore to manage devices. I installed <app name and link>. It discovered my device as HomeKit device and allowed me to add control it.

# Siri
As you saw in my original video I controlled my device with use of Siri. The good news that nothing need ot be done. All HomeKit devices can be managed by Siri without any additional configuration. Enjoy.

# Future plans
While I was successefully able to manage hardware as I needed I believe the process of setup was cumbersome. I think that HomeBridge HTTP Device could be improved a lot and become a generic base for any device or platform integration. This is what I am working right now, writing my own implementation of HTTP Device to contribute back to HomeBridge codebase. I also plan to enable HomeKit integration for my Roomba right after HomeBridge contribution will be completed.
