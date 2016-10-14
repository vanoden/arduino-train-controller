# arduino-train-controller
Run N or HO Scale trains with your Arduino

The Arduino Train Controller interfaces with an AdaFruit Motor Controller v2.3
to power 12 volt DC model trains.  It takes commands via the serial port to
accelerate, decelerate or set the direction of the engine on a specific track.
It also employs momentum, gradually increasing or decreasing speed until it
reaches the specified target.

The application is designed to be wrapped in another, more user friendly
interface that communicates with it via serial port.  A simple web application
will be made to work with it.  Of course, the serial port could easily by
substituted for by Bluetooth classic serial port or nRF24L01 wireless.

The application takes the simple, two character commands.  The first character
is the track to be controlled and the second is the action to be taken.  The
track will be a number, 1-4, and the action can be:

<ol>
<li>a - Accelerate</li>
<li>d - Decelerate</li>
<li>s - Stop</li>
<li>&lt; - Go Forward</li>
<li>&gt; - Go Backward</li>
</ol>

For example, to make the train on track 2 slow down, simple send the string:
2d

