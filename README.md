# Linux Serial Sniffer

### What is
This is a prototype of a Linux Serial Sniffer,  Linux Kernel components still need to be completed.
The sniffer can intercept data passing through a specified serial port, and display it on the terminal.

### Compiled SubProjects
- **LGSniffer/bin/SerialSniffer**: the client application
- **LGSniffer/bin/snifferTests**: unit and integration tests
- **LGSniffer/lib/libsniffer.so**: dynamic library used by the client application in order to subscribe to serial port traffic and receive kernel messages
- **LGSniffer/modules/functions_override.ko and serial_bridge.ko** : Kernel modules that intercept serial port traffic
- **LGSniffer/scripts/*** : some useful scripts.

### How it works
Serial port traffic is intercepted by a kernel module via KProbe and data are sent to user space using Generic Netlink as communication method. User space can subscribe to the serial traffic corresponding to a specified port and can receive only traffic related to that port.
A dynamic library has been created in order to communicate with the Kernel, so the client app only have to use a small set of interface functions in order to perform the necessary tasks.

### Tech Stack
Build System:
- Cmake

Test:
- Catch2 framework

User space external libraries:
- libnl

Kernel modules features:
- KProbe
- Generic Netlink