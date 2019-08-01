# CarPi

This project is useless for anybody who has an aux cord and a built-in modern media center already in their car.

I will eventually have all the config files available in this repository, with private data expunged of course.

# DEPENDENCIES
```
dnsmasq
hostapd
mpd
wiringpi
libmpdclient [https://github.com/MusicPlayerDaemon/libmpdclient]

```

# INSTALLATION
Either run the script [TODO]
or install manually as shown below
All commands should be run from the root folder of the git repository
(or you could be really risky and straight up `cp -r etc/ /etc` and so on)

## WIFI ACCESS POINT
Install the dnsmasq and hostapd packages
`apt install dnsmasq hostapd`
Copy the config files for /etc/dhcpcd.conf
`cp etc/dhcpcd.conf /etc/dhcpcd.conf`
Backup the dhcp server config (dnsmasq)
`mv /etc/dnsmasq.conf /etc/dnsmasq.conf.bak`
`cp etc/dnsmasq.conf /etc/dnsmasq.conf`
Copy the hostapd daemon config
`cp etc/hostapd/hostapd.conf /etc/hostapd/hostapd.conf`
Make sure to change the SSID and WPA_PASSPHRASE to something else
Copy this config file to tell hostapd where to load the config from, or just add the line DAEMON_CONF="/etc/hostapd/hostapd.conf"
`cp etc/default/hostapd /etc/default/hostapd`
In /etc/sysctl.conf, uncomment the net.ipv4.ip_forward=1
Enable and start the daemons
`systemctl enable hostapd`
`systemctl enable dnsmasq`
`systemctl start hostapd`
`systemctl start dnsmasq`

## MPD SERVER
Install the Music Player Daemon Package
`apt install mpd`
Copy the mpd config
`cp etc/mpd.conf /etc/mpd.conf`

### Allow MPD to stream to Pulseaudio
Copy the pulseaudio defaults
`cp etc/pulse/default.pa /etc/pulse/default.pa`

## Pulseaudio
[TODO]

## PiTFT button support (button.c is configured for the Pi 3 B version)
If you have the older TFT screen for the Pi A, you need to remap the GPIO buttons in button.c
This section allows the buttons to control the MPD server.
Install the libmpdclient and wiringPi packages
`apt install wiringpi`
follow this github link for the libmpdclient:
https://github.com/MusicPlayerDaemon/libmpdclient




