# cslight

uses http://www.teuniz.net/RS-232/

## Requirements
 
- Arduino (e. g. Uno, Nano)
- SMD RBG LED-Strip (testet with WS2812, WS2801 probaly). You can chose the amount
- a few jumper wires
- USB cable

## Installation

### Arduino & Leds
- connect 5V and GND of led strip to the arduino
- connect DATA of led strip
- mount led strip behind your display as you wish 
- connect arduino to your PC via USB
Note: more than 20 leds could probaly needs an additional power supply


### Server
note: you need only one server in your cs-team for using cslight. The teammates have to configure the server ip and port.
- edit socket_port, Post-Server port and host
- start server with node.js

### Client
- compile the client by using the Makefile
- edit cslight.conf (placed in the same folder as the compiled cslight-client:
	- ip and port of running node server
	- COMPORT the arduino is connected to. In windows you can find the number in Control Panel > Devices and Printers (Systemsteuerung > Geräte und Drucker). Note: Value is COMX - 1 e.g. :if it is COM3 you have to configure the value 2	
	- BAUDRATE should not be changed normally
	- NUM_LEDS is not in use yet
	- STATIC_RGB defines a value for static light while no datas are received. Without any charactes behind the "=" you can deactivate the static light.
	- CRIT-values can be used to configure personal colors for differend game situations
	Note: STATIC_RGB and CRIT-values should have been declared under this scheme: 255-255-255. Three Dashes and the ending dot are mandatory.
	
### CS-Config
- place "gamestate_integration_cslight.cfg" in your Steam config path (..../Steam/steamapps/common/Counter-Strike\ Global\ Offensive/csgo/cfg/)
For further informations about Valves gamestate_integration please check https://developer.valvesoftware.com/wiki/Counter-Strike:_Global_Offensive_Game_State_Integration


## Running
- start the compiled cslight-client
- a short light-test will show you, if everything is properly connected
