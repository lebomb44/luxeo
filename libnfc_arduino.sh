git clone https://github.com/nfc-tools/libnfc.git
cd libnfc
git checkout libnfc-1.7.1
wget --content-disposition "http://nfc-tools.org/index.php?title=Libnfc:Arduino&action=raw&anchor=pn532_uart.patch"
patch -p0 < pn532_uart.patch
autoreconf -vis
./configure --with-drivers=pn532_uart --sysconfdir=/etc --prefix=/usr
make clean all
sudo make install
sudo mkdir -p /etc/nfc/devices.d
cd /etc/nfc/
wget --content-disposition "http://nfc-tools.org/index.php?title=Libnfc:Arduino&action=raw&anchor=libnfc.conf"
