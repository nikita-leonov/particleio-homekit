var types = require("../api").homebridge.hapLegacyTypes;
var Service = require("../api").homebridge.hap.Service;
var Characteristic = require("../api").homebridge.hap.Characteristic;
var request = require("request");

function ParticleAccessory(log, config) {
  this.log = log;

  // url info
  this.platform_url = config['platform_url'];
  this.device_id = config['device_id'];
  this.access_token = config['access_token'];

  // device info
  this.name = config['name'];
}

ParticleAccessory.prototype = {

  httpRequest: function(url, method, data, callback) {
    request({
      url: url,
      form: data,
      method: method
    },
    function (error, response, body) {
      callback(error, response, body)
    })
  },

  deviceUrl: function(platform_url, device_id) {
    return platform_url+'/devices/'+device_id;
  },

  functionUrl: function(name, device_url) {
    return device_url+'/'+name;
  },

  setCharacteristic: function(name, value) {
    var device_url = this.deviceUrl(this.platform_url, this.device_id);
    var function_url = this.functionUrl(name, device_url);

    console.log('Calling function ' + name + ' with value ' + value + '.');

    var request_data = {'value' : value, 'access_token': this.access_token};

    this.httpRequest(function_url, "POST", request_data, function(error, response, body) {
      if (error) {
        console.error('Http request for function ' + name + ' failed:', error);
      } else {
        console.log('Http request for function ' + name + ' succeeded.');
      }
    });
  },

  getServices: function() {
    var that = this;
    return [{
      sType: types.ACCESSORY_INFORMATION_STYPE,
      characteristics: [{
        cType: types.NAME_CTYPE,
        onUpdate: null,
        perms: ["pr"],
        format: "string",
        initialValue: this.name,
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Name of the accessory",
        designedMaxLength: 255
      },
      {
        cType: types.MANUFACTURER_CTYPE,
        onUpdate: null,
        perms: ["pr"],
        format: "string",
        initialValue: "Http",
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Manufacturer",
        designedMaxLength: 255
      },
      {
        cType: types.MODEL_CTYPE,
        onUpdate: null,
        perms: ["pr"],
        format: "string",
        initialValue: "Rev-1",
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Model",
        designedMaxLength: 255
      },
      {
        cType: types.SERIAL_NUMBER_CTYPE,
        onUpdate: null,
        perms: ["pr"],
        format: "string",
        initialValue: "A1S2NASF88EW",
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "SN",
        designedMaxLength: 255
      },
      {
        cType: types.IDENTIFY_CTYPE,
        onUpdate: null,
        perms: ["pw"],
        format: "bool",
        initialValue: false,
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Identify Accessory",
        designedMaxLength: 1
      }]
    },
    {
      sType: types.LIGHTBULB_STYPE,
      characteristics: [{
        cType: types.NAME_CTYPE,
        onUpdate: null,
        perms: ["pr"],
        format: "string",
        initialValue: this.name,
        supportEvents: true,
        supportBonjour: false,
        manfDescription: "Name of service",
        designedMaxLength: 255
      },
      {
        cType: types.POWER_STATE_CTYPE,
        onUpdate: function(value) {
          that.setCharacteristic("powerState", value ? 1 : 0);
        },
        perms: ["pw","pr","ev"],
        format: "bool",
        initialValue: 1,
        supportEvents: true,
        supportBonjour: false,
        manfDescription: "Change the power state",
        designedMaxLength: 1
      },
      {
        cType: types.HUE_CTYPE,
        onUpdate: function(value) {
          that.setCharacteristic("hue", value);
        },
        perms: ["pw","pr","ev"],
        format: "int",
        initialValue: 100,
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Adjust Hue of Light",
        designedMinValue: 0,
        designedMaxValue: 100,
        designedMinStep: 1,
        unit: "%"
      },
      {
        cType: types.SATURATION_CTYPE,
        onUpdate: function(value) {
          that.setCharacteristic("saturation", value);
        },
        perms: ["pw","pr","ev"],
        format: "int",
        initialValue: 100,
        supportEvents: false,
        supportBonjour: false,
        manfDescription: "Adjust Saturation of Light",
        designedMinValue: 0,
        designedMaxValue: 100,
        designedMinStep: 1,
        unit: "%"
      },
      {
        cType: types.BRIGHTNESS_CTYPE,
        onUpdate: function(value) {
          that.setCharacteristic("brightness", value);
        },
        perms: ["pw","pr","ev"],
        format: "int",
        initialValue:  0,
        supportEvents: true,
        supportBonjour: false,
        manfDescription: "Adjust Brightness",
        designedMinValue: 0,
        designedMaxValue: 100,
        designedMinStep: 1,
        unit: "%"
      }]
    }];
  }
};

module.exports.accessory = ParticleAccessory;

