# cslight


## Requirements

- Arduino (e. g. Uno, Nano)
- SMD RBG LED-Strip (tested with WS2812, WS2801 probaly). You can choose the amount of LEDs (Note: more than 20 LEDs may need an additional power supply)
- a few jumper wires
- USB cable
- uses http://www.teuniz.net/RS-232/
- CS:GO
- for the server: node.js

## Installation

### Arduino & LEDs
- connect 5V and GND of LED strip to the arduino
- connect DATA of LED strip to pin 11
- mount LED strip behind your display as you wish
- connect arduino to your PC via USB

### Server
note: you need only one server in your cs-team for using cslight. All teammates have to configure the server ip and port.
- edit socket_port, Post-Server port and host
- start server with node.js

### Client
- compile the client using the Makefile
- edit cslight.conf (placed in the same folder as the compiled cslight-client:
	- ip and port of your node.js server
	- COMPORT the arduino is connected to. In windows you can find the number in Control Panel > Devices and Printers (Systemsteuerung > Geräte und Drucker). Note: Value is COMX - 1 e.g. if it is COM3 you have to configure the value 2
	- BAUDRATE should not be changed
	- NUM_LEDS is not yet in use
	- STATIC_RGB defines a value for static light while no data is received. Without any characters behind the "=" you can deactivate the static light.
	- CRIT-values can be used to configure personal colors for different game situations
	Note: STATIC_RGB and CRIT-values should be declared in this scheme: 255-255-255. Three Dashes and the final dot are mandatory.

### CS-Config
- place "gamestate_integration_cslight.cfg" in your Steam config path (..../Steam/steamapps/common/Counter-Strike\ Global\ Offensive/csgo/cfg/)
For further information about Valves gamestate_integration please check https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration


## Running
- start the compiled cslight-client
- a short light-test will show you, if everything is connected properly
