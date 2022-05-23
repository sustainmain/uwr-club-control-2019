# uwr-club-control-2019

This is an old project I made to control my underwater robotics club's ROV. The ROV had an Arduino underwater and a Raspberry Pi in the control box above water, so this repo has code for both.

The pilot controls the ROV using a USB gamepad connected to the Pi. The Pi reads the gamepad's joysticks, buttons, and triggers, constructs a data packet with all the values, and sends it over a serial connection to the Arduino. The Arduino extracts each value from the data packet and moves the thrusters, claws, and tool packages accordingly. The Arduino sends back a temperature reading to the Pi, which the Pi displays on screen for the pilot.

## Arduino

The final Arduino control program is `arduino/arduino_control/arduino_control.ino`.

All the tests I made as I went are in `arduino/tests/`. The directory has tests for serial communication as well as tests for using the ESCs.

## Pi

The final Raspberry Pi control program is `pi/pi-control.py`. Since the Pi is intended to be run without a keyboard or mouse, `pi/button-listener.py` allows the pilot to restart the control program or shutdown the Pi using GPIO buttons. 

All the tests are in `pi/tests`, and sorted into 3 main categories.
* `joystick-turtle-test/` has all the tests for reading input from a USB gamepad
* `packet-test/` has a test that sends malformed data packets
* `serial-test/` has all the tests for serial communication

`pi/pi-control.py` is automatically started by the `pi/services/pi-control` init.d service. Likewise, `pi/button-listener.py` has a `pi/services/button-listener` init.d service. Both services will put program logs in the `logs/` directory.

### Installation

* Copy all the files in this repo's `pi` folder to `/home/pi/uwr-club-control-2019/`
* Copy both service files into `/etc/init.d/`
* Enter the following commands:
```
sudo chmod 755 /etc/init.d/button-listener
sudo chmod 755 /etc/init.d/pi-control
sudo update-rc.d /etc/init.d/button-listener defaults
sudo update-rc.d /etc/init.d/pi-control defaults
```
