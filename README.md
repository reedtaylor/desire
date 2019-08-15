# desire
desire: decent espresso serial interface repeater

__DESIRE is a standalone, Raspberry Pi-based server application that serves as a replacement for the Decent Bluetooth (BLE) adaptor.__

The aims of the DESIRE project are:

- To enable the DE to more easily talk over different kinds of interfaces than just BLE (while still leaving BLE as a great option) 
- To enable multiple controllers to talk to a single DE at the same time, over those interfaces

__Why DESIRE?__

DESIRE aims to free other Decent projects to focus on higher level abstractions, such as surfacing RESTful interfaces, building new apps, new UIs etc., without requiring each such project to individually build out ways to e.g. connect to the DE over TCP, require unpairing of the stock tablet, etc.  Those low level capabilities would be provided by DESIRE.

DESIRE aims to provide a more flexible platform for connectivity.  For example, people wishing to communcate with the DE without using wireless protocols (which have been found unreliable in some noisy-RF environments such as trade shows) could use the DESIRE system as a means to enable USB serial communication with a controller device.

And, because the system will be running on a Raspberry Pi device connected directly to the machine, DESIRE happens to come packaged with an execution environment for other applications such as web servers, app backends, etc..  These applications running on the same Raspberry Pi as Desire can each individually to talk to the machine without requiring a wireless connection of any form.


__What DESIRE is not:__

DESIRE is not aiming to provide a high-level API for the Decent machine.  Higher-level APIs such as RESTful APIs etc. may be implemented in application layers constructed atop DESIRE (or not).

In fact, DESIRE won't define many (if any) new APIs or interfaces at all, relative to what is native to the stock DE machine.  (The main extensions likely to be offered by the project are perhaps porting Ray Heasman's existing "DE serial protocol over UART" and "DE serial protocol over BLE" to some new interfaces, like (for example) "DE serial protocol over TCP" and "DE serial protocol over USB".  These protocol-ports would aspire to be as-close-to-identical-as-possible to Ray's existing specs.)

And, just to put it out there: DESIRE does not want to enable off-label behaviors on the DE that could pose danger or risk to users.  The job of DESIRE is to forward communication packets around in a way that extends and enhances the safe use of the machine.  (So, if someone from Decent Inc. requests that DESIRE prevent some form of control - for example, by requiring that only one paired device be able to perform a "start" operation on a pre-1.3 machine - the DESIRE project will listen and follow suit.)

At the outset, the project is envisioned to move through 3 major milestones:

__Milestone 1, aka "bridge of desire":__ This executable will transparently bridge the existing serial spec from the DE UART to any TCP connection to a specific port, as well as (optionally) to local stdio on the RPi. This is pretty easy to do because Ray's serial specs just send ascii messages of roughly this form:
(image coming)

What this binary does is bridge messages of that form back and forth from the decent UART to the other interfaces.  I may build  be some internal management of e.g. subscribers to various forms of messages, but it's not clear how important / robust that needs to be since the DE machine itself is chatty and doesn't seem to pay much attention to the "unsubscribe" messages I have played with so far.  I may also do mild protocol checking / packet dropping in cases of wildly malformed comms, to prevent hard-to-fix weirdnesses.

This setup does not define any new API or protocol.  It's literally what's on the existing DE UART.

Note: stdio / command line comms are probably not useful to people other than me; it does allow me to easily test buffering / error checking / etc. while also blowing some smoke through the pipes of the event-driven system.   (I might add some pretty-printing or other conveniences later on.)

But with TCP sockets, this milestone opens the door to "BLE free" comms between the DE and controllers running either on the RPi or on a local network.  Also multiple controllers will be able to talk to a single DE at the same time without developers needing to worry about it much.

__Milestone 2, aka "desire fulfilled":__ This milestone will add two additional forms of bridging:
Bridging to the BLE device native to the RPi. This will allow the original, stock tablet to be re-paired to a DESIRE-equipped DE machine, which will restore full functionality, while still leaving room for other projects to interface with the machine via TCP.  This is probably a big win for folks who don't want to mess with wires more than once.
Hopefully, we will also achieve bridging via wired USB which I think might save some folks' bacon at tradeshows.  

