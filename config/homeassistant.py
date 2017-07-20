'''Python script containing configuration commands for homeassistant.
Uncomment and run as needed.
'''
import os

#To check status of homeassistant:
os.system('sudo systemctl status home-assistant@homeassistant')

#To start homeassistant: (Replace start with stop to stop the program)
#os.system('sudo systemctl start home-assistant@homeassistant')
#os.system('sudo systemctl restart home-assistant@homeassistant')

#To automatically start homeassistant at boot
#Note that this runs a .service script that needs to be written by you. See here for more details:
#https://home-assistant.io/docs/autostart/systemd/
#os.system('sudo systemctl enable home-assistant@homeassistant')
