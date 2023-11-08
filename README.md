# drm-framebuffer
A simple drm framebuffer tool to test drm devices. It takes data from stdin and will put it to the display. 

## Usage:

The behavior of this tool should be similar to what was possible with:
```bash
dd if=/dev/urandom of=/dev/fb0
```

The following would be the equivalent:
```bash
dd if=/dev/urandom | drm-framebuffer -d /dev/dri/card0 -c HDMI-A-1
```

Additionally you can request information about the display configuration with the following two commands:
```bash
# Available connectors, crtcs and encoders
drm-framebuffer -d /dev/dri/card0 -l
connectors:
Number: 95 Name: eDP-1 Encoder: 94 Crtc: 51
Number: 103 Name: DP-1 Encoder: 102 Crtc: 72
Number: 112 Name: HDMI-A-1 Encoder: 0
Number: 118 Name: DP-2 Encoder: 0
Number: 124 Name: HDMI-A-2 Encoder: 0
Framebuffers:
CRTCs: 51 72 93
encoders: 94 102 104 105 106 117 119 120 121

#Resolution
drm-framebuffer -d /dev/dri/card0 -c DP-1 -r
#0 - 3840x2160@60 (ht: 4000 hs: 3888 he: 3920 hskew: 0, vt: 2222  vs: 2163 ve: 2168 vscan: 0, flags: 0x9) - preferred
#1 - 3840x2160@60 (ht: 4400 hs: 4016 he: 4104 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x6)
#2 - 3840x2160@60 (ht: 4400 hs: 4016 he: 4104 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#3 - 3840x2160@60 (ht: 4400 hs: 4016 he: 4104 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#4 - 3840x2160@50 (ht: 5280 hs: 4896 he: 4984 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#5 - 3840x2160@30 (ht: 4400 hs: 4016 he: 4104 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#6 - 3840x2160@30 (ht: 4400 hs: 4016 he: 4104 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#7 - 3840x2160@25 (ht: 5280 hs: 4896 he: 4984 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#8 - 3840x2160@24 (ht: 5500 hs: 5116 he: 5204 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#9 - 3840x2160@24 (ht: 5500 hs: 5116 he: 5204 hskew: 0, vt: 2250  vs: 2168 ve: 2178 vscan: 0, flags: 0x5)
#10 - 2560x1440@60 (ht: 2720 hs: 2608 he: 2640 hskew: 0, vt: 1481  vs: 1443 ve: 1448 vscan: 0, flags: 0x6)
#11 - 2048x1280@60 (ht: 2208 hs: 2096 he: 2128 hskew: 0, vt: 1317  vs: 1283 ve: 1289 vscan: 0, flags: 0x6)
#12 - 1920x1080@60 (ht: 2200 hs: 2008 he: 2052 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#13 - 1920x1080@60 (ht: 2200 hs: 2008 he: 2052 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#14 - 1920x1080@50 (ht: 2640 hs: 2448 he: 2492 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#15 - 1920x1080@30 (ht: 2200 hs: 2008 he: 2052 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#16 - 1920x1080@30 (ht: 2200 hs: 2008 he: 2052 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#17 - 1920x1080@25 (ht: 2640 hs: 2448 he: 2492 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#18 - 1920x1080@24 (ht: 2750 hs: 2558 he: 2602 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#19 - 1920x1080@24 (ht: 2750 hs: 2558 he: 2602 hskew: 0, vt: 1125  vs: 1084 ve: 1089 vscan: 0, flags: 0x5)
#20 - 1600x900@60 (ht: 1800 hs: 1624 he: 1704 hskew: 0, vt: 1000  vs: 901 ve: 904 vscan: 0, flags: 0x5)
#21 - 1280x1024@75 (ht: 1688 hs: 1296 he: 1440 hskew: 0, vt: 1066  vs: 1025 ve: 1028 vscan: 0, flags: 0x5)
#22 - 1280x1024@60 (ht: 1688 hs: 1328 he: 1440 hskew: 0, vt: 1066  vs: 1025 ve: 1028 vscan: 0, flags: 0x5)
#23 - 1152x864@75 (ht: 1600 hs: 1216 he: 1344 hskew: 0, vt: 900  vs: 865 ve: 868 vscan: 0, flags: 0x5)
#24 - 1280x720@60 (ht: 1650 hs: 1390 he: 1430 hskew: 0, vt: 750  vs: 725 ve: 730 vscan: 0, flags: 0x5)
#25 - 1280x720@60 (ht: 1650 hs: 1390 he: 1430 hskew: 0, vt: 750  vs: 725 ve: 730 vscan: 0, flags: 0x5)
#26 - 1280x720@50 (ht: 1980 hs: 1720 he: 1760 hskew: 0, vt: 750  vs: 725 ve: 730 vscan: 0, flags: 0x5)
#27 - 1024x768@75 (ht: 1312 hs: 1040 he: 1136 hskew: 0, vt: 800  vs: 769 ve: 772 vscan: 0, flags: 0x5)
#28 - 1024x768@60 (ht: 1344 hs: 1048 he: 1184 hskew: 0, vt: 806  vs: 771 ve: 777 vscan: 0, flags: 0xA)
#29 - 800x600@75 (ht: 1056 hs: 816 he: 896 hskew: 0, vt: 625  vs: 601 ve: 604 vscan: 0, flags: 0x5)
#30 - 800x600@60 (ht: 1056 hs: 840 he: 968 hskew: 0, vt: 628  vs: 601 ve: 605 vscan: 0, flags: 0x5)
#31 - 720x576@50 (ht: 864 hs: 732 he: 796 hskew: 0, vt: 625  vs: 581 ve: 586 vscan: 0, flags: 0xA)
#32 - 720x480@60 (ht: 858 hs: 736 he: 798 hskew: 0, vt: 525  vs: 489 ve: 495 vscan: 0, flags: 0xA)
#33 - 720x480@60 (ht: 858 hs: 736 he: 798 hskew: 0, vt: 525  vs: 489 ve: 495 vscan: 0, flags: 0xA)
#34 - 640x480@75 (ht: 840 hs: 656 he: 720 hskew: 0, vt: 500  vs: 481 ve: 484 vscan: 0, flags: 0xA)
#35 - 640x480@60 (ht: 800 hs: 656 he: 752 hskew: 0, vt: 525  vs: 490 ve: 492 vscan: 0, flags: 0xA)
#36 - 640x480@60 (ht: 800 hs: 656 he: 752 hskew: 0, vt: 525  vs: 490 ve: 492 vscan: 0, flags: 0xA)
#37 - 720x400@70 (ht: 900 hs: 738 he: 846 hskew: 0, vt: 449  vs: 412 ve: 414 vscan: 0, flags: 0x6)
```

The -s option can be used to force a specific resolution from the list received with -r:
```
dd if=/dev/urandom | drm-framebuffer -d /dev/dri/card0 -c HDMI-A-1 -s 36
```
This example sets the resolution to 640x480@60.

## Dependencies
This tool requires libdrm to compile and work.
  
## Compile
To compile the tool simply type "make" with a valid gcc set trough the environment variable CC. Also make sure the drm headers and libraries are available (LDFLAGS, CFLAGS).

## Install
Copy the executable drm-framebuffer to your target and execute it.
