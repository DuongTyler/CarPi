#!/bin/bash

if [ $(whoami) != "root" ]; then
	echo "You need to run this script as root in order to place all the config files correctly (use sudo)"
	exit 1
fi

echo "[apt] Installing pulseaudio"
apt -y install pulseaudio

echo "=== ACCESS POINT SECTION ==="
echo "[apt] Installing dnsmasq hostapd"
apt -y install dnsmasq hostapd

if [ -f /etc/dhcpcd.conf ]; then
	echo "You already have an existing dhcpcd.conf file, I renamed it to /etc/dhcpcd.conf.bak"
	mv /etc/dhcpcd.conf /etc/dhcpcd.conf.bak
fi
echo "Installing /etc/dhcpcd.conf"
cp etc/dhcpcd.conf /etc/dhcpcd.conf

if [ -f /etc/dnsmasq.conf ]; then
	echo "dnsmasq.conf already exists, appending .bak to original file"
	mv /etc/dnsmasq.conf /etc/dnsmasq.conf.bak
fi
echo "Installing /etc/dnsmasq.conf"
cp etc/dnsmasq.conf /etc/dnsmasq.conf

if [ ! -d /etc/hostapd ]; then
	echo "creating /etc/hostapd/"
	mkdir /etc/hostapd
fi
echo "Installing hostapd.conf to /etc/hostapd/."
cp etc/hostapd/hostapd.conf /etc/hostapd/hostapd.conf

echo "DAEMON_CONF=\"/etc/hostapd/hostapd.conf\"" >> /etc/default/hostapd

echo "Installing sysctl config, this may overwrite your original sysctl.conf file settings, please check the config after"
mv /etc/sysctl.conf /etc/sysctl.conf.bak
cp etc/sysctl.conf /etc/sysctl.conf

echo "Enabling and starting hostapd and dnsmasq"
systemctl unmask hostapd
systemctl unmask dnsmasq
systemctl enable hostapd
systemctl enable dnsmasq
systemctl start hostapd
systemctl start dnsmasq

echo "=== MPD SECTION ==="

echo "[apt] Installing mpd"
apt -y install mpd

echo "Installing MPD config"
cp etc/mpd.conf /etc/mpd.conf
mkdir /srv/Music
chown pi:pi /srv/Music
echo "MPD will read music files from /srv/Music"

echo "=== Pulseaudio Section ==="

echo "Installing default.pa file"
cp etc/pulse/default.pa /etc/pulse/default.pa
echo "Installing system.pa file"
cp etc/pulse/system.pa /etc/pulse/system.pa

echo "adding pacmd module loader to .profile"
echo "pulseaudio --start" >> /home/pi/.profile
echo "pacmd load-module module-native-protocol-tcp auth-ip-acl=127.0.0.1" >> /home/pi/.profile

echo "done!"
#[TODO: install the WIRINGPI option with libmpdclient and button controls]
