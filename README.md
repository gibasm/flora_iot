# Flora IOT

This is a personal project used to water my plants when I'm not home :)

# Hardware:

```text
----------
| pico_w |- VBUS/GND -- 12V power adapter with a step down converter to 5V
|        |- GPIO26 -- moisture sensor's analog output
|        |- GPIO2  -- DC-DC relay that powers on a small water valve
|--------|
```
for more look in: *source/gpio\_conf.h*

# Building and running:

1. ``cmake -B build -DWIFI_PWD='your WiFi password' -DWIFI_SSID='your WiFi's SSID' -G Ninja -DPICO_SDK_PATH=path/to/pico-sdk``
2. ``ninja -C build``
3. Plug in the pico with a usb cable with bootsel button pressed, 
   then use whatever you like (dd/file manager etc) to move the binary (build/flora.uf2) to pico's flash. 
4. Use a serial terminal app of choice (picocom/minicom etc) and open up the device (e.g. /dev/ttyACM0) to
   get the device's assigned IP and see the log output.
