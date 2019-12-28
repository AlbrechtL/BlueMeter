# BlueMeter

Bosch PLR50C ranger finder Bluetooth decoder.

**This project is work in progress and not finished!**

## Basic Protocol
Source: https://www.eevblog.com/forum/projects/hacking-the-bosch-glm-20-laser-measuring-tape/msg1331649/#msg1331649

```
These commands work with Bosch PLR15 serialport

send frame:    [startbyte][command][length]([data])[checksum]
receive frame: [status][length][...][checksum]

Get serialnumber: C006004A
Get "firmware" version:C00400BA
Backlight ON:C0470020
Backlight OFF:C0480062
Laser ON:C0410096
Laser OFF:C042001E
Start measure:C04000EE

Example measure:
send C04000EE ->reply 00 04 13 0E 00 00 32
Change endianness
distance in mm=0x00000E13*0,05=180mm

Max measurement rate is about 4 times per second


Unknown commands:
C0030074 reply->000100AE


First byte in reply is status:
0x00 -> OK
0x01 -> communication timeout
0x03 -> checksumerror
0x04 -> unknown command
0x05 -> invalid accesslevel
0x08 ->harware error
0x10 ->device not ready
(0D=invalid accesslevel + harware error)
```

**Thanks to the following projects for the reverse engineering work!**

* https://github.com/piannucci/pymtprotocol
* https://github.com/philipptrenz/BOSCH-GLM-rangefinder



	
