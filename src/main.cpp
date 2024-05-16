#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <sys/ioctl.h>
#include <asm/termbits.h>
#include <unistd.h> // write(), read(), close()
#include <stdlib.h>
#include <iostream>
#include <iomanip>

int serial_open(const char *file)
{
    int fd = open(file, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        std::cout << "Error in serial port open(): " << strerror(errno) << std::endl;
        return -1;
    }

    struct termios2 tio;
    if (ioctl(fd, TCGETS2, &tio)) {
        std::cout << "Error getting serial port settings: " << strerror(errno) << std::endl;
        return -1;
    }

    // raw mode, custom baud rate (153600)
    tio.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tio.c_oflag &= ~OPOST;
    tio.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tio.c_cflag &= ~(CSIZE | PARENB | CBAUD);
    tio.c_cflag |= CS8 | BOTHER;
    tio.c_ispeed = 153600;
    tio.c_ospeed = 153600;

    if (ioctl(fd, TCSETSF2, &tio)) {
        std::cout << "Error setting serial port settings: " << strerror(errno) << std::endl;
        return -1;
    }

    struct termios2 tio2;
    if (ioctl(fd, TCGETS2, &tio2)) {
        std::cout << "Error getting serial port settings: " << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << "speeds: i " << tio.c_ispeed << " o " << tio.c_ispeed << std::endl;

    return fd;
}

int main()
{
    int serial_port_fd = serial_open("/dev/serial0");
    if (serial_port_fd < 0) {
        return EXIT_FAILURE;
    }
    unsigned char byte;

    std::cout << std::hex << std::setfill('0');
    while (true) {
        int i = 16;
        while (i--) {
            ssize_t rc = read(serial_port_fd, &byte, 1);
            if (rc == -1) {
                std::cout << std::endl << "Error in serial port read(): " << strerror(errno) << std::endl;
                return EXIT_FAILURE;
            } else if (rc == 0) {
                std::cout << std::endl << "Unexpected EOF, exiting" << std::endl;
                return EXIT_FAILURE;
            }
            
            std::cout << std::setw(2) << (int)byte;
            if (i != 0) {
                std::cout << " ";
            } else {
                std::cout << std::endl;
            }
            std::cout << std::flush;
        }
    }

    close(serial_port_fd);
    return 0;
}
