# desire
desire: decent espresso serial interface repeater

__DESIRE is a standalone, Raspberry Pi-based server application that serves as a replacement for the Decent Bluetooth (BLE) adaptor.__

__DESIRE aims to simplify and enable other Decent projects__  You may want to surface APIs, RESTful interfaces, build new apps, new UIs etc.  DESIRE wants help you do this without the need to build out a means of connecting directly to the DE.  Those low level capabilities are implemented and provided by DESIRE.  It also gives you some friendly logging capabilities for debugging communications with the machine.

__DESIRE aims to provide a more flexible platform for connectivity.__  For example, people wishing to communcate with the DE without using wireless protocols (which have been found unreliable in some noisy-RF environments such as trade shows) might use the DESIRE system as a means to enable Ethernet or USB serial communication with a controller device.

__DESIRE is happy to have roommates on the Raspberry Pi__   A Raspberry Pi is a pretty heavyweight machine for the kind of task DESIRE is performing, so if you need an execution environment for your system that happens to be local to the machine itelf -- go for it!  Multiple applications can run on the same Raspberry Pi as Desire and each can individually to talk to the machine without conflict and without requiring a remote connection of any form.

![DESIRE Architecture](https://user-images.githubusercontent.com/8826853/63563763-3d5b3380-c530-11e9-8516-5a0bcb1ca89b.png)
[update -- the above is no longer precisely accurate in terms of which BT adaptor is doing what; will update]

DESIRE is not aiming to provide a high-level API for the Decent machine.  The hope is that higher-level APIs such as RESTful interfaces etc. will be implemented in application layers constructed atop DESIRE.

DESIRE won't define many (if any) new APIs or interfaces at all, relative to what is native to the stock DE machine.  The main extensions likely offered by the project are things like porting Ray Heasman's existing "DE serial protocol over UART" and "DE serial protocol over BLE" to some new interfaces, like (for example) "DE serial protocol over TCP" (done) and "DE serial protocol over USB" (coming).  These protocol-ports will aspire to be as-close-to-identical-as-possible to the native protocols of the DE.

__Project Status__

Done:

- Local fileio communication over stdio
- Multiplexing multiple connections at the same time is implemented
- TCP connectivity 

Coming:

- PCB to make connecting to the DE easier
- Documentation on how to set it all up
- More interfaces (USB, BLE, named pipes)
