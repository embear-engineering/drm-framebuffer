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
3840x2160
```

## Dependencies
This tool requires libdrm to compile and work.
  
## Compile
To compile the tool simply type "make" with a valid gcc set trough the environment variable CC. Also make sure the drm headers and libraries are available (LDFLAGS, CFLAGS).

## Install
Copy the executable drm-framebuffer to your target and execute it.
