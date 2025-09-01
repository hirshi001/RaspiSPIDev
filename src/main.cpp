#include <iostream>
#include <thread>
#include "SpidevMatrix/Max7219.h"


const int NUM_MATRICES = 4;

SPI spi("/dev/spidev0.0", nullptr);

int main() {
    spi_config_t cfg = {};
    cfg.mode = 0;
    cfg.speed = 1000000;
    cfg.bits_per_word = 8;
    spi.setConfig(&cfg);

    if (!spi.begin()) {
        std::cout << "SPI init failed\n";
        return 1;
    }
    std::cout << "SPI init successful\n";

    Max7219 matrix(NUM_MATRICES, spi);
    matrix.init();

    int width = NUM_MATRICES * 8;
    int height = 8;

    // Example: turn on a few pixels
    int x = 0, y = 0;
    matrix.setPixel(x, y, true);

    while (true) {
        for(int nx = 0; nx < width; ++nx) {
            matrix.setPixel(x, y, false); // Clear previous pixel
            x = nx;
            matrix.setPixel(x, y, true);  // Set new pixel
            int intensity = nx * 15 / width;
            matrix.setIntensity(intensity);
            matrix.updateMatrices();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}