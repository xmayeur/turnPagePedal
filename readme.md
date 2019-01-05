## turnPagePedal ##



turnPagePedal is a project to create a two-buttons pedal device that enables page turns for a score displaying application on a tablet.

It emulates a keyboard to basically send the cursor left/right commands. Additional commands may be sent by programming long press on one of the pedal button to actionate da-capo or cross page jumps command.

Special combinations of putton presses allow entering in admin mode and configuring some behaviour or parameters of the device.

This repo also contains the Arduino code for a DFRobot Beetle BLE Bluno microcontroller as a HID keyboard sending left and right cursor code when buttons are pushed

# Electronic schema #

This is very simple:

![Schema](img/pedalTurnPageSchema.JPG)

# Photos of the Prototype

![Photo 1](img/overall.jpg)

![Photo 2](img/details.jpg)


# Functional behaviour #

**Normal mode**

The following functionality is implemented when using the left & right buttons:

- Left button (red button) short press sends a left cursor keystroke when released
- right button (blue button) short press sends a right cursor keystroke when released
- left button long press send a "c" character when released 

- if you maintain the right button pressed while pressing the left one n times, it changes the longpress period from 500ms to ntimes * 500 ms (max 3000 ms)

**Admin mode**

Maintain the left button pressed while pressing three times the right one to enter the admin mode:

- press the left button set the Beetle BLE device in Central role and transparent mode - this mode is needed to program the device with the Arduino IDE
- or press the right button set the device in Peripheral and HID mode - this mode is the normal working mode to connect the device as a keyboard

**Button state transition table**

The following table resumes the actions taken according to buttons' state changes.

Rows indicates the 'FROM' state; columns the 'TO' state.
States are represented by a two letters code indicating the respective **H**igh or **L**ow state of the left and right buttons. So '**LH**' means that the left button is being pressed, while the right button not.



<table style="border-collapse:collapse;border-spacing:0" class="tg"><tr><th style="font-family:Arial, sans-serif;font-size:14px;font-weight:bold;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">            TO<br>&nbsp;&nbsp;&nbsp;&nbsp;FROM</th><th style="font-family:Arial, sans-serif;font-size:14px;font-weight:bold;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">HH</th><th style="font-family:Arial, sans-serif;font-size:14px;font-weight:bold;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">HL</th><th style="font-family:Arial, sans-serif;font-size:14px;font-weight:bold;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">LH</th><th style="font-family:Arial, sans-serif;font-size:14px;font-weight:bold;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">LL</th></tr><tr><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;font-weight:bold;text-align:left">HH</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">start<br>&nbsp;&nbsp;rtimer<br>&nbsp;&nbsp;&nbsp;&nbsp;if admin mode: <br>&nbsp;&nbsp;&nbsp;&nbsp;    set HID mode , restart device</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">start<br>&nbsp;&nbsp;ltimer<br>&nbsp;&nbsp;&nbsp;&nbsp;if admin mode: <br>&nbsp;&nbsp;&nbsp;&nbsp;    set TRANSPARENT mode; restart<br>&nbsp;&nbsp;device</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td></tr><tr><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;font-weight:bold;text-align:left">HL</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">stop<br>&nbsp;&nbsp;rtimer<br>&nbsp;&nbsp;&nbsp;&nbsp;if admin mode:<br>&nbsp;&nbsp;&nbsp;&nbsp;    admin mode = false<br>&nbsp;&nbsp;&nbsp;&nbsp;else:<br>&nbsp;&nbsp;&nbsp;&nbsp;    if rtimer &lt; period: send RIGHT<br>&nbsp;&nbsp;ARROW<br>&nbsp;&nbsp;&nbsp;&nbsp;   </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">increase<br>&nbsp;&nbsp;lcounter<br>&nbsp;&nbsp;&nbsp;&nbsp;</td></tr><tr><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;font-weight:bold;text-align:left">LH</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">stop<br>&nbsp;&nbsp;ltimer<br>&nbsp;&nbsp;&nbsp;&nbsp;if admin mode:<br>&nbsp;&nbsp;&nbsp;&nbsp;      admin mode = false<br>&nbsp;&nbsp;&nbsp;&nbsp;else:<br>&nbsp;&nbsp;&nbsp;&nbsp;    if rtimer &lt; period: send LEFT<br>&nbsp;&nbsp;ARROW<br>&nbsp;&nbsp;&nbsp;&nbsp;    if rtimer &gt; period:  send 'C' char</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left">increase<br>&nbsp;&nbsp;rcounter<br>&nbsp;&nbsp;&nbsp;&nbsp;if rcounter &gt; 3: <br>&nbsp;&nbsp;&nbsp;&nbsp;     admin mode = true</td></tr><tr><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;font-weight:bold;text-align:left;vertical-align:top">LL</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left;vertical-align:top">if lcounter &gt;<br>&nbsp;&nbsp;0:      <br>&nbsp;&nbsp;&nbsp;&nbsp;     period = lcounter * 500ms<br>&nbsp;&nbsp;&nbsp;&nbsp;reset rcounter, lcounter</td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left;vertical-align:top"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left;vertical-align:top"> </td><td style="font-family:Arial, sans-serif;font-size:14px;padding:10px 5px;border-style:solid;border-width:1px;overflow:hidden;word-break:normal;border-color:inherit;text-align:left;vertical-align:top"> </td></tr></table>