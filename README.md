# GMUHexBot
#### Created for GMU Robotics Club

Public repository for the hexbot's arduino code.

Contains:
* hexBotWalk.ino - the sketch making robot walk, includes calculated offsets.
* offsetCalcTool.ino - the sketch that aids in testing servos and finding offset values.
* hexBotLegsComms.ino - Communications protocol for the slave Arduino, in this case the leg controler, to read.
* hexBotBrainComms.ino - Communications protocol for the master Arduino, acting as a brain, to send commands out via I<sup>2</sup>C.

#### Leg Assignment
<pre>
     Front
   __________
 4-|-5   11-|-10
   |        |
 8-|-9    3-|-2
   |        |
 0-|-1    7-|-6
   ----------
</pre>

#### Videos

[Walking, strafing, and turning](https://www.youtube.com/playlist?list=PLwq6DT8sjevVq0LsvVSGXAnbzRftWv6IH)

#### Links

[Dr. Lofaro's Wiki Page](http://wiki.lofarolabs.com/index.php/Main_Page)<br>
[Dynamixel XL-320 Robot Actuator](http://support.robotis.com/en/product/dynamixel/xl-series/xl-320.htm)
