# How to Install DESIRE

## Parts needed (with example sources):

1\. Raspberry Pi 3B+ ~$35.00 USD ([US:Adafruit](https://www.adafruit.com/product/3775))
   * Other models may work, Pi 3B+ is what has been tested
   
2\. Required Raspberry Pi accessories
   * Case for Raspberry Pi 3 (Prices vary; will update if I find a great one that works well with the hat & cables.  Some case is required though so you can safely set the Raspberry pi down.)
   * High quality 5v MicroUSB power supply for Raspberry Pi ~$10.00 USD ([US:Amazon](https://www.amazon.com/CanaKit-Raspberry-Supply-Adapter-Listed/dp/B00MARDJZ4))
   * 16GB+ MicroSD card (might be able to get away with 8GB but 16GB / 32GB is a very safe bet) ~$8.00 ([US:Amazon](https://www.amazon.com/Samsung-MicroSDHC-Adapter-MB-ME32GA-AM/dp/B06XWN9Q99))
   * Note: [Kits](https://www.amazon.com/CanaKit-Raspberry-Starter-Premium-Black/dp/B07BCC8PK7) are available that include all of the above parts plus some superfluous ones
   
3\. Waveshare Serial Expansion HAT (SC16IS752) ~$15.00 USD ([US:Amazon](https://www.amazon.com/gp/product/B07KGGHBPW) [Aliexpress](https://www.aliexpress.com/i/32967417322.html))
   * Provides two high-quality UART interfaces perfect for connecting to the DE1 machine and to the Decent BLE adaptor
   * Other 3.3v serial UART devices may work.  Note that the DE BLE adaptor requires a 3.3v (not 5v) supply. 
   
4\. 6+ Female-Female jumper wires, and 5+ Male-Female jumper wires ~$2.00 USD ([US:Adafruit](https://www.adafruit.com/product/1950))
   * Currently one ribbon of 5 wires and one ribbon of 6 wires are needed to connect to the DE1 machine and Decent BLE adaptor, respectively
   *  The connection to the DE1 Machine (Male-female) needs ~6in / 15cm length in order to comfortably place the Raspberry Pi on top of the machine.  The connection to the BLE adaptor can be shorter if desired.
   * Note that when the a custom circuit board (PCB) for this project becomes available, the cabling needs may change
 
 ## Software Setup
 
1\. Install [Raspbian Buster](https://www.raspberrypi.org/downloads/raspbian/) on the SD card
   * Note, do not use Raspbian Jessie or earlier as we make use of kernel modules included as of Buster
   * You may choose to use Lite or one of the Desktop configurations, depending on your SD card size and whether you think you will ever hook the Raspberry Pi up to a monitor.
   * Follow detailed installation instructions [here](https://desertbot.io/blog/headless-raspberry-pi-3-bplus-ssh-wifi-setup) including a means of immediately accessing the Raspberry Pi in a "headless" manner (no monitor / keyboard / mouse).  

2\. Put the Raspberry Pi into a case, install both the SD card and the Waveshare SC16IS752 card onto the Raspberry Pi and power it up

3\. Enable the SC16IS752 on the Raspberry Pi
   * Login and edit /boot/config.txt
 `sudo nano /boot/config.txt`
   * Add the following line to the bottom of that file:
 `dtoverlay=sc16is752-i2c`
   * Save and quit by hitting `ctrl-X` and answering yes

4\. Enable i2c on the Raspberry Pi
   * Execute the command ` sudo raspi-config`
   * Use the down-arrow to choose `Interfacing Options` and hit Enter
   * Use the down-arrow to choose `I2C` and hit Enter
   * Say "Yes" to all questions
   * Use the right-arrow to select `Back` and `Finish`
   * Say "Yes" when asked to reboot, or reboot manually by executing `sudo reboot`

5\. Confirm that the Waveshare SC16IS752 card is now working properly
   * Execute `ls /dev/ttySC*`
   * Verify you can see the devices `/dev/ttySC0` and `/dev/ttySC1`

## Hardware Setup
__CAUTION -- YOU MIGHT WRECK YOUR MACHINE, OR WORSE!__

* This process will get easier after we have a circuit board (PCB) made to simplify all the wiring.  
* If you have no experience with electronics work of this kind, this might not be a good starter project as you are messing with a multi-thousand-dollar machine that happens to shoot out boiling hot water!   
* John is a stand-up guy but he's [made it clear](https://3.basecamp.com/3671212/buckets/7351439/messages/1705452967#__recording_1714433599) that tinkerers who wreck their machines can expect to pay for expensive replacement parts, etc..  
  
  __You are doing this at your own risk!  If you feel uncertain, don't do it yourself -- find someone with experience!__

1\. Power off the Decent machine and unplug it from power (remove the power cable from the rear of the machine).

2\. Power down the Raspberry Pi and disconnect it from power.

3\. If installed, remove the tubing and Cat5 cable for the plubming/catering kit

4\. Remove the translucent back panel according to the instructions at the top of [this post](https://3.basecamp.com/3671212/buckets/7351439/documents/1798545355)  (Remove 4 T10 torx screws & the knob from the water supply lifter)

5\. Carefully remove the Bluetooth adaptor from its socket by pinching it on the sides and pulling.  Avoid touching the pins or the surface of the circuit board.  Do not use much force.  Carefully set aside in a safe place.

6\. Attach a 5-wire ribbon cable to the BOTTOM 5 positons of the LEFT-HAND header on the machine, as you look at it from the rear.  This is the header closer to the steam-wand side.
![Wire placement](https://user-images.githubusercontent.com/8826853/63828449-7a5e6600-c934-11e9-8ddf-dcdbb7588da0.png)

7\. Connect the other end of the 5-wire ribbon cable to the Waveshare SC16IS752 as shown below.  
   * Note that here we are connecting to UART "A" so pins should be labeled "RTSA, CTSA, RDXDA"
   * Note that the wire numbering 1-6 in the image is counting from top to bottom on the UART.  Double check the 
   * Note that wire 1 is not connected on either side

![Wiring diagram - DE to UART](https://user-images.githubusercontent.com/8826853/63908733-316ae800-c9ee-11e9-8edf-31715db8afa0.png)

8\. Retrieve the Bluetooth adaptor removed in step 4, and connect it using a 6-wire ribbon cable to the Waveshare SC16IS752 as shown below.  
   * Note that here we are connecting to UART "B" so pins should be labeled "RTSB, CTSB, RDXDB"
   * Note that the BT adaptor is shown here face-on, with the notch at the bottom.  In this orientation you are attaching wires to back of the BT adaptor, at the top-left.
   * Note that wire 1 _is_ connected for this device, unlike for the DE 
   * Note that several pairs of wires (3 and 4, 5 and 6) are "flipped" for this device, when comparing to how the DE was wired

![Wiring diagram - BLE to UART](https://user-images.githubusercontent.com/8826853/63909283-1ac59080-c9f0-11e9-874c-202ea894fa10.png)

## Software setup

1\. Reconnect power to the raspberry pi and sign in

2\. Install some needed libraries and make sure the system is up-to-date
   * Execute the following commands: 
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libgoogle-glog-dev libevent-dev libgflags-dev
sudo reboot
```
   
3\. Download and build DESIRE by running the commands:
```
cd ~
git clone https://github.com/reedtaylor/desire.git
cd desire
make
```

4\. Run desire in a verbose debugging mode by runnnig the command
```
~/desire/build/apps/desire -v=4 -logtostderr
```

5\. Switch on the DE and note that Desire reflects a few messages coming from the machine

6\. If it hasn't happened already, re-connect your stock tablet to the DE BLE adaptor.  This may involve going into the Settings --> Machine tab on the decent app, tapping "Search" and then tapping the shown Bluetooth ID of the machine.  For more details, see [this page] on how to pair from scratch.  (Remember, as far as the tablet / app are concerned, they are still talking to the same machine since it's the self-same Bluetooth adaptor.)

7\. You should shortly start seeing a lot of messages in your terminal window indicating that DESIRE is passing messages.   Similar to the following:
```
I0829 00:15:41.185611  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1DEB001200002FFC304A0B3EE95A0000300066
I0829 00:15:41.190371  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [Q]177C0F00
I0829 00:15:41.391851  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E04001400002FBF304C133ED05A0000300067
I0829 00:15:41.399902  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [Q]177B0F00
I0829 00:15:41.601378  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E1D001300002FC9304C343EE95A0000300067
I0829 00:15:41.808393  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E36001200002FDD304C983ED05A0000300067
I0829 00:15:42.017215  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E4F001300002FBF304B763ED05A0000300067
I0829 00:15:42.225594  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E68001300002FD3304CCD3EE15A0000300067
I0829 00:15:42.435197  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E81001300002FB5304E1F3ED95A0000300067
I0829 00:15:42.643450  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1E9A001100002FD3304E763EC85A0000300067
I0829 00:15:42.851097  3303 dispatcher.cpp:70] Dispatcher:  >>> DE1-->BLE sent [M]1EB3001300002FBF304F3F3EE15A0000300067
```

8\. Check the tablet app to confirm that:
   * Temperature appears to be updating as expected
   * Actions taken (e.g. start and stop flush) are reflected on the machine

9\. Congratulations, the basics of the DESIRE system are working properly!   Let's kill the instance of DESIRE that is running and put it into a less chatty mode by typing
```
killall desire
nohup ~/desire/build/apps/desire -v=3 &
```

If you ever need to restart desire, e.g. if the Raspberry Pi reboots, you can do so by running the above commands.

Desire will now be running in the background.  You can check on the status of the running program by running `cat /tmp/desire.INFO`.  Note that for performance reasons, writes to this logfile is buffered so may not be 100% up to date.  If you want realtime logging from desire, use the `-logtostderr` command line option.


__Coming later:__
* Installing DESIRE as a proper system service

