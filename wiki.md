------------------------------------------------------------------
DEVICES NEEDED:
1. DHT22 sensor (<$4)
2. Wemos D1 mini (<$10)
3. KanKun smart wifi plug socket ($20)

------------------------------------------------------------------
HARDWARE SETUP:
1. See DHT22 manuals (in the datasheets directory of this repo) to learn the
    pin numbering convention.
    DHT_1 (VDD)   --> Wemos 3.3V
    DHT_2 (DATA)  --> Wemos D0, also has a 10K res connected to DHT VDD
    DHT_3 (NULL)
    DHT_4 (GND)   --> Wemos GND
    Laptop port   --> Wemos micro usb

------------------------------------------------------------------
SOFTWARE SETUP FOR KANKUN SMART PLUG:
1. Install atom and then platformio.
2. If the platform is Manjaro (not windows):

    Allow the 'pio' command to run on the terminal:
        - Atom context menu > PlatformIO > Settings > PlatformIO IDE Terminal >
          Auto Run Command > export PATH=~/.platformio/penv/bin:$PATH
    Allow platformio to connect the device to the serial monitor:
        - Open linux terminal (ctrl + alt + t) and then type in
          sudo usermod -a -G uucp $USER
          Restart computer

3. Connect computer to home wifi. Then check the ip address and the subnet
   mask by doing:

    Manjaro taskbar > Network icon > right click > 'Connection Information'
    Note the IPv4 address, eg: 192.168.1.220
    and the subnet mask, eg: 255.255.255.0

4. Install nmap, fire up the terminal and scan your network for all active devices:

    nmap -sn [your IPv4 address goes here/CIDR notation for subnet mask]
    eg: nmap -sn 192.168.1.220/24

    Explanation of the command:
      - -sn option does a ping scan on the network for all active hosts.
      - Note that the CIDR notation (classeless inter domain routing notation) for
        255.255.0.0 is /16 and 255.255.255.0 is /24. In this case the subnet means
        that the network part of the IP is 192.168.1 and the rest ie. 220 is the
        host (laptop machine) address.
        If you are confused, watch this wonderful video which explains
        everything you need to know:
        https://www.youtube.com/watch?v=w0RM7PY34Bg

5. The return of the nmap command will tell you what IP addresses have already
   been taken (and by what devices). Choose an IP that is not already taken
   (ie. an IP that isn't in that list).

6. Plug in (to a wall socket) the KanKun SP3 Wifi Plug device.
7. Wait 10 to 20 seconds and connect to the wifi called 'OK_SP3'.
8. SSH into it by opening a terminal and typing in:
    ssh root@192.168.10.253
9. The terminal will ask for a password which is:
    p9z34c
   If that doesn't work try:
    admin
   Or try:
    1234
10. Tell the switch how to connect to your local network by typing:
      vi /etc/config/wireless

   Change the file to look exactly like the below with
   the ssid (eg. '5turtles') and the key (ie. the wifi password) filed in:

     config wifi-device  radio0                                        
             option type     mac80211                                  
             option channel  11                                        
             option hwmode   11ng                                      
             option path     'platform/ar933x_wmac'                    
             option htmode   HT20                                      
             list ht_capab   SHORT-GI-20                               
             list ht_capab   SHORT-GI-40                               
             list ht_capab   RX-STBC1                                  
             list ht_capab   DSSS_CCK-40                               
             # REMOVE THIS LINE TO ENABLE WIFI:                        
             option disabled 0                                         
             option country CN                                         

     config wifi-iface                                                 
             option device 'radio0'                                    
             option network 'wwan'                                     
             option ssid [enter your home ssid name here without the brackets]                                    
             option mode 'sta'                                         
             option encryption 'psk-mixed'                             
             option key [enter your home network password here]                                     

11. Tell the switch to connect over the wifi by typing:

      vi /etc/config/network

    Change the file to look similar to the below.
    Note that the only thing that is different from the default is the bottom most section that I added:

      config interface 'loopback'
              option ifname 'lo'
              option proto 'static'
              option ipaddr '127.0.0.1'
              option netmask '255.0.0.0'

      config globals 'globals'
              option ula_prefix 'fd04:11bf:4580::/48'

      config interface 'lan'
              option ifname 'eth0'
              option type 'bridge'
              option proto 'static'
              option ipaddr '192.168.10.253'
              option netmask '255.255.255.0'
              option ip6assign '60'

      config interface 'wwan'
              option proto 'static'
              option ipaddr [put in the IP that you know is available (by using nmap earlier) into here eg. '192.168.1.224']
              option gateway '192.168.1.1'
              option netmask '255.255.255.0'

12. Now in the same ssh terminal type:
      reboot

    Wait 10 - 20 seconds. If your computer doesn't automatically reconnect to
    your home wifi you can do that now. When the plug is back up you should be
    able to reconnect over SSH using the IP you gave it.

    If it doesn't work, you can reset to factory settings and start all over
    by keeping the button pressed for 4-5 seconds.

    You can exit the currently dead ssh session by pressing these keys:
      "enter" then "~" then "."

13. SSH to the IP you gave the socket. (You need to be connected to your home wifi).
      ssh root@[whatever your new IP is]
14. Change the password of the device.
      passwd
15a. Use a script called json.cgi (already in this repo in cgi-bin dir,
    originally known as relay.cgi and then later as json.cgi) that lets you
    control the switch over http. This script behaves like a hardware relay.
    We'll install it in the /www/cgi-bin folder and then copy over the file:
        mkdir /www/cgi-bin
    Note that if you ever need to see it, you can't do ls directly from root,
    instead you should do "cd /www" and then do "ls".
15b. Close the ssh session.
15c. Open a new terminal and navigate to the directory which has the script
    on the laptop/local machine, then do:
        scp json.cgi root@192.168.1.224:/www/cgi-bin
    You will be asked for the password and the terminal will confirm that the
    file is copied over by outputting something like this:
        json.cgi                       100% 3730    49.0KB/s   00:00
16. SSH into the device again and make the file executable:
        ssh root@[whatever IP address you gave it]
        chmod +x /www/cgi-bin/json.cgi
17. Open the web browser and type in the following (note you will have to change
    the IP part of the URL to whatever IP you have assigned to your device):

    Get general switch device info:
    http://192.168.1.224/cgi-bin/json.cgi

    Get the current state of the switch:
    http://192.168.1.224/cgi-bin/json.cgi?get=state

    Turn on the switch:
    http://192.168.1.224/cgi-bin/json.cgi?set=on  
    Turn on the switch after a delay:
    http://192.168.1.224/cgi-bin/json.cgi?set=on&mins=60

    Turn off the switch:
    http://192.168.1.224/cgi-bin/json.cgi?set=off
    Turn off the switch after a delay:
    http://192.168.1.224/cgi-bin/json.cgi?set=off&mins=60


CREDITS:
http://www.homeautomationforgeeks.com/openhab_http.shtml#kankun
https://www.home-assistant.io/components/switch.kankun/
https://webworxshop.com/2017/05/08/my-home-automation-setup

OTHER SOURCES:
https://github.com/homedash/kankun-json
http://benlo.com/esp8266/KankunSmartPlug.html

------------------------------------------------------------------
SOFTWARE SETUP FOR HOME ASSISTANT (HASS):
1. Install dependencies if they are not already installed.
      sudo pacman install python3 python3-venv python3-pip

2. Add an account for Home Assistant called homeassistant.
   Since this account is only for running Home Assistant the extra argument
   of -rm is added to create a system account and create a home directory.
   The arguments -G uucp adds the user to the dialout group
   (so that useraccount has control of RS-232 serial ports and devices connected to them).
   uucp/dialout group is required for using Z-Wave and Zigbee controllers.
      sudo useradd -rm homeassistant -G uucp

3. Create a directory for the installation of Home Assistant and
   change the owner to the homeassistant account.
      cd /srv
      sudo mkdir homeassistant
      sudo chown homeassistant:homeassistant homeassistant

4. Create and change to a virtual environment for Home Assistant.
   This will be done as the homeassistant account.
      sudo -u homeassistant -H -s
      cd /srv/homeassistant
      python3 -m venv .
      source bin/activate

5. Once you have activated the virtual environment (notice the prompt change),
   install a required python package.
      python3 -m pip install wheel

6. Now install homeassistant via pip
      pip3 install homeassistant

7. Start Home Assistant for the first time.
   Doing the below will complete the installation, create the .homeassistant configuration
   directory in the /home/homeassistant directory and install any basic dependencies.
      cd /srv/homeassistant
      hass

7. You can now reach your installation on your Raspberry Pi/PC over
   the web interface on http://ipaddress:8123.
   (Change the ipaddress part to be whatever your ip address is).

SOURCE:
https://www.home-assistant.io/docs/installation/raspberry-pi/

8. Next you can setup HASS so that it auto-starts on laptop boot up.
   Most linux distro's use systemd to manage daemons.
   To check this is the case, with your distro, do:
      ps -p 1 -o comm=
   If the return value is systemd. Then continue.

9. Navigate command line to the config folder of this repo. Then execute:
      scp home-assistant\@homeassistant.service /etc/systemd/system/
   Note that you may need to sudo to do this command.
   Note also that you need to do \@ and not just @.

10. Reload systemd to make the daemon aware of the new configuration
      sudo systemctl --system daemon-reload

11. Have Home Assistant start automatically at boot, enable the service.
      sudo systemctl enable home-assistant@homeassistant

12. To start Home Assistant now, use this command:
      sudo systemctl start home-assistant@homeassistant

    Note: You can also substitute the 'start' above with 'stop' to stop Home Assistant,
    'restart' to restart Home Assistant, and ‘status’ to see a brief status report.

ADDITIONAL INFO:
- To disable the automatic start, use this command:
      sudo systemctl disable home-assistant@homeassistant
- To get Home Assistant’s logging output, simple use journalctl:
      sudo journalctl -f -u home-assistant@homeassistant
- Because the log can scroll quite quickly, you can select to view only the error lines:
      sudo journalctl -f -u home-assistant@homeassistant | grep -i 'error'
- When working on Home Assistant, you can easily restart the system and
  then watch the log output by combining the above commands using &&
      sudo systemctl restart home-assistant@homeassistant && sudo journalctl -f -u home-assistant@homeassistant

SOURCE:
https://www.home-assistant.io/docs/autostart/systemd/

------------------------------------------------------------------
PRE-SOFTWARE SETUP FOR GIT-CRYPT:
1. Check whether you have GPG installed already (ie. type gpg --version).

   Otherwise download the GPG command line tool from https://www.gnupg.org/download/
   Unpack and install it, ie.
      tar -xf [downloaded file name]

   Navigate to the unpacked directory and then do:
      ./configure
      make
      make check
      make install

2. Generate a GPG key pair:
      gpg --full-generate-key

  When prompted choose:
   the default key (RSA and RSA),
   max. key size (4096)
   and no expiry (0)

  To list GPG keys for which you have both a public and private key type:
  (A private key is required for signing commits or tags.)
      gpg --list-secret-keys --keyid-format LONG

3. Copy the GPG key ID that starts with sec. In the following example, that's 30F2B65B9246B6CA.

      sec   rsa4096/30F2B65B9246B6CA 2017-08-18 [SC]
            D5E4F29F3275DC0CDA8FFC8730F2B65B9246B6CA
      uid                   [ultimate] Mr. Robot <mr@robot.sh>
      ssb   rsa4096/B7ABC0813E4028C0 2017-08-18 [E]

4. Export the public key of that ID (replace your key ID from the previous step)
   This will print the GPG key ID, in ASCII armor format.

      gpg --armor --export [your own GPG key ID goes here, eg. 30F2B65B9246B6CA]

5. Add the generated GPG key to your Gitlab account:
   https://gitlab.com/profile/gpg_keys

6. Now tell git which key to use.
      git config --global user.signingkey [your own GPG key ID goes here, eg. 30F2B65B9246B6CA]

SOURCE:
https://docs.gitlab.com/ee/user/project/repository/gpg_signed_commits/index.html

------------------------------------------------------------------
SOFTWARE SETUP FOR GIT-CRYPT:
1. Download git crypt from:
   https://www.agwa.name/projects/git-crypt/downloads/git-crypt-0.6.0.tar.gz

2. Extract the file and install it with the following:
      cd git-crypt-0.6.0 (ie. cd the terminal to whatever name you downloaded git-crypt as).
      make
      make install

3. List your gpg key (that you added earlier) by doing:
      gpg -k

   Copy the pub USER_ID key. Eg. D5E4F29F3275DC0CDA8FFC8730F2B65B9246B6CA

4. Add gpg user key so that you can share the repo with yourself or others:
      git-crypt add-gpg-user [USER_ID]
   Replace the '[USER_ID]' part with the key you copied in step 3.

5. Navigate to the repo you are interested in and then initiate git-crypt:
      cd repo
      git-crypt init

6. Specify files to encrypt by creating a .gitattributes file in the repository, like this:
      secretfile filter=git-crypt diff=git-crypt
      *.key filter=git-crypt diff=git-crypt

/*
NOTE A:
After cloning a repository with encrypted files, unlock with:
      git-crypt unlock

NOTE B:
The .gitattributes file is documented in the gitattributes(5) man page.
The file pattern format is the same as the one used by .gitignore,
as documented in the gitignore(5) man page, with the exception that
specifying merely a directory (e.g. `/dir/`) is NOT sufficient to
encrypt all files beneath it.

Also note that the pattern `dir/*` does not match files under
sub-directories of dir/.  To encrypt an entire sub-tree dir/, place the
following in dir/.gitattributes:

      * filter=git-crypt diff=git-crypt
      .gitattributes !filter !diff

The second pattern is essential for ensuring that .gitattributes itself
is not encrypted.

SOURCE:
https://www.agwa.name/projects/git-crypt/
