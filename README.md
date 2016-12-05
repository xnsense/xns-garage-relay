# xns-garage-relay

[This library is for Adafruit Feather 32uF Bluefruit LE] (http://www.xnsense.no/product-page/31bd7d04-8b25-3bc4-8551-c8b5a67e70f6)

Requires Arduino version 1.6.4 or higher

[Requires Adafruit nRF51 BLE Library](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51)

[Requires Membrane 3x4 Matrix KeyPad] (http://www.xnsense.no/product-page/0f418644-bfb9-f2e8-8f2c-6acad78df267)

[Optional: Magnetic contact switch] (http://www.xnsense.no/product-page/365bc775-8778-c8aa-6a28-b97bd0168d1c)

[Optional: Alu. Box] (http://www.xnsense.no/product-page/a16cff8d-4411-0be9-3b90-e9a75275c1dc)

My goal was to create an simple solution so the kids could use an KeyPad to open the Garage door. 

#Setup:
Adafruit Feather 32uF Bluefruit LE

Adafruit Feather Mini Relay FeatherWing

Membrane 3X4 Matrix KeyPad

Network Cable (TP) or other cable with 7 wires

#Relay Setup
Find the input source on the garage port and wire it to the Feather Mini Relay (NOTE: Check the Garage Door Datasheet first to make sure you connect the correct input and that the Feather Mini Relay can handle the load, see Adafruit product link for details)

Solder an wire from SET pin to your prefered GPIO (Mine: 0)

#Magnetic contact switch Setup
I choose to add an Magnetc contact to the setup so I can query the door status through BLE. Connect one wire to GND and the other to GPIO 5


#KeyPad Setup

4 rows (horizontal)

* Vertical row 1: GPIO 13
* Vertical row 2: GPIO 12
* Vertical row 3: GPIO 11
* Vertical row 4: GPIO 10

2. 3 rows (vertical)

* Horizontal row 1: GPIO 9
* Horizontal row 1: GPIO 6
* Horizontal row 1: GPIO 5
