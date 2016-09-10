# xns-garage-relay

This library is for Adafruit Feather 32uF Bluefruit LE (https://www.adafruit.com/products/2829)

Requires Arduino version 1.6.4 or higher

[Requires Adafruit nRF51 BLE Library](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51)

[Requires Membrane 3x4 Matrix KeyPad] (https://www.adafruit.com/products/419)

Optional: Magnetic contact switch

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
