HOW TO IMPLEMENT THE PHYSICAL DEVICE SETUP:
1. See DHT22 manuals (in the datasheets directory of this repo) to learn the
    pin numbering convention.
    DHT_1 (VDD)   --> Wemos 3.3V
    DHT_2 (DATA)  --> Wemos D0, also has a 10K res connected to DHT VDD
    DHT_3 (NULL)
    DHT_4 (GND)   --> Wemos GND
    Laptop port   --> Wemos micro usb

HOW TO IMPLEMENT THE SOFTWARE SETUP:
1. Install atom and then platformio.
2. If the platform is Manjaro (not windows):
    Allow the 'pio' command to run on the terminal:
        - Atom context menu > PlatformIO > Settings > PlatformIO IDE Terminal >
          Auto Run Command > export PATH=~/.platformio/penv/bin:$PATH
    Allow platformio to connect the device to the serial monitor:
        - Open terminal (ctrl + alt + t) and then type in
          sudo usermod -a -G uucp $USER
3.
