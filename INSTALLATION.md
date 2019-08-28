# How to Install DESIRE

##Parts needed (with example sources):

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
 
 ##Software Setup
 
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
