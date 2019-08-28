# How to Install DESIRE

## Parts needed (with example sources):

1. Raspberry Pi 3B+ ~$35.00 USD ([US:Adafruit](https://www.adafruit.com/product/3775))
 - Other models may work, Pi 3B+ is what has been tested
2. Required Raspberry Pi accessories
 - Case for Raspberry Pi 3 (Prices vary; will update if I find a great one that works well with the hat & cables.  Some case is will be required though so you can safely set the Raspberry pi down.)
 - High quality 5v MicroUSB power supply for Raspberry Pi ~$10.00 USD ([US:Amazon](https://www.amazon.com/CanaKit-Raspberry-Supply-Adapter-Listed/dp/B00MARDJZ4))
 - 16GB+ MicroSD card (might be able to get away with 8GB but 16GB / 32GB is a very safe bet) ~$8.00 ([US:Amazon](https://www.amazon.com/Samsung-MicroSDHC-Adapter-MB-ME32GA-AM/dp/B06XWN9Q99))
 - Note: [Kits](https://www.amazon.com/CanaKit-Raspberry-Starter-Premium-Black/dp/B07BCC8PK7) are available that include all of the above parts plus some superfluous ones
3. Waveshare Serial Expansion HAT (SC16IS752) ~$15.00 USD ([US:Amazon](https://www.amazon.com/gp/product/B07KGGHBPW) [Aliexpress](https://www.aliexpress.com/i/32967417322.html))
 - Provides two high-quality UART interfaces perfect for connecting to the DE1 machine and to the Decent BLE adaptor
 - Other 3.3v serial UART devices may work.  Note that the DE BLE adaptor requires a 3.3v (not 5v) supply. 
4. 12+ Female-Female jumper wires ~$2.00 USD ([US:Adafruit](https://www.adafruit.com/product/1950))
 - Currently one ribbon of 5 wires and one ribbon of 6 wires are needed to connect to the DE1 machine and Decent BLE adaptor, respectively
 - The connection to the DE1 Machine needs ~6in / 15cm length in order to comfortably place the Raspberry Pi on top of the machine
 - Note that when the a custom circuit board (PCB) for this project becomes available, the cabling needs may change
 
 ## Software Setup
 
1. Install [Raspbian Buster](https://www.raspberrypi.org/downloads/raspbian/) on the SD card
 - Note, do not use Raspbian Jessie or earlier as we make use of kernel modules included as of Buster
 - You may choose to use Lite or one of the Desktop configurations, depending on your SD card size and whether you think you will ever hook the Raspberry Pi up to a monitor.
 - Follow detailed installation instructions [here](https://desertbot.io/blog/headless-raspberry-pi-3-bplus-ssh-wifi-setup) including a means of immediately accessing the Raspberry Pi in a "headless" manner (no monitor / keyboard / mouse).  
2. Install both the SD card and the Waveshare SC16IS752 card onto the Raspberry Pi and power it up
3. Enable the SC16IS752 on the Raspberry Pi
 - Login and edit /boot/config.txt
 `sudo nano /boot/config.txt`
 - Add the following line to the bottom of that file:
 `dtoverlay=sc16is752-i2c`
 - Save and quit by hitting `ctrl-X` and answering yes
4. Enable i2c on the Raspberry Pi
 - Execute the command ` sudo raspi-config`
 - Use the down-arrow to choose `Interfacing Options` and hit Enter
 - Use the down-arrow to choose `I2C` and hit Enter
 - Say "Yes" to all questions
 - Use the right-arrow to select `Back` and `Finish`
 - Say "Yes" when asked to reboot, or reboot manually by executing `sudo reboot`
5. Confirm that the Waveshare SC16IS752 card is now working properly
 - Execute `ls /dev/ttySC*`
 - Verify you can see the devices `/dev/ttySC0` and `/dev/ttySC1`
6. Install some needed libraries and make sure the system is up-to-date
 - Execute the following commands: 
 `sudo apt-get update
 blah`
 - Execute the command `sudo apt-get upgrade`
 - Execute the command `sudo apt-get install libgoogle-glog-dev libevent-dev libgflags-dev`
 - Reboot by executing `sudo reboot`
7. Download and build DESIRE
 - Execute the command `git clone https://github.com/reedtaylor/desire.git`
 - Execute the commands `cd desire; make`

## Hardware Setup
__CAUTION -- YOU MIGHT WRECK YOUR MACHINE, OR WORSE!__

 - This process will get easier after we have a circuit board (PCB) made to simplify all the wiring.  
 - If you have no experience with electronics work of this kind, this might not be a good starter project as you are messing with a multi-thousand-dollar machine that happens to shoot out boiling hot water!   
  - John is a stand-up guy but he's made it clear that tinkerers who wreck their machines will need to pay for the replacement parts etc..  
  
  __You are doing this at your own risk!  If you feel uncertain, don't do it yourself -- find someone with experience!__

1. Power off the Decent machine and unplug it from power (remove the power cable from the rear of the machine).
2. If installed, remove the tubing and Cat5 cable for the plubming/catering kit
3. Remove the translucent back panel according to the instructions at the top of [this post](https://3.basecamp.com/3671212/buckets/7351439/documents/1798545355)  (Remove 4 T10 torx screws & the knob from the water supply lifter)
4. Carefully remove the Bluetooth adaptor from its socket by pinching it on the sides and pulling.  Avoid touching the pins or the surface of the circuit board.  Do not use much force.  Carefully set aside in a safe place.
5. Attach a 5-pin ribbon cable to the BOTTOM 5 positons of the LEFT-HAND header on the machine, as you look at it from the rear.  This is the header closer to the steam-wand side.
![Wire placement](https://user-images.githubusercontent.com/8826853/63828449-7a5e6600-c934-11e9-8ddf-dcdbb7588da0.png)
6. __STAY TUNED__ writing will continue...
