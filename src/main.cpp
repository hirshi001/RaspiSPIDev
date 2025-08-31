#include <iostream>
#include <vector>
#include <thread>
#include "spidev_lib++.h"


const int NUM_MATRICES = 4;

std::vector<uint8_t> matrixBuffer(NUM_MATRICES * 8, 0x00); // All pixels off


SPI spi("/dev/spidev0.0", nullptr);

// Send buffer to all matrices
void updateMatrices() {
    for (int row = 0; row < 8; ++row) {
        std::vector<uint8_t> tx;
        // MAX7219 expects: for N chained modules, send data for last module first
        for (int m = NUM_MATRICES - 1; m >= 0; --m) {
            tx.push_back(row + 1);               // Row address (1-8)
            tx.push_back(matrixBuffer[m * 8 + row]); // Row data
        }
        std::vector<uint8_t> rx(tx.size());
        spi.xfer(tx.data(), tx.size(), rx.data(), rx.size());
    }
}

// Turn a pixel on/off in matrix coordinates (0,0 = top-left)
void setPixel(int x, int y, bool on) {
    if (x < 0 || x >= NUM_MATRICES*8 || y < 0 || y >= 8) return;
    int matrix = x / 8;
    int col = x % 8;
    if (on)
        matrixBuffer[matrix * 8 + y] |= (1 << col);
    else
        matrixBuffer[matrix * 8 + y] &= ~(1 << col);
}

int main() {
    spi_config_t cfg = {};
    cfg.mode = 0;
    cfg.speed = 1000000;
    cfg.bits_per_word = 8;
    spi.setConfig(&cfg);

    if (!spi.begin()) {
        printf("SPI init failed\n");
        return 1;
    }

    // Initialize MAX7219
    for (int m = 0; m < NUM_MATRICES; ++m) {
        std::vector<uint8_t> initTx;
        initTx.insert(initTx.end(), {0x09, 0x00}); // Decode mode: none
        initTx.insert(initTx.end(), {0x0A, 0x03}); // Intensity
        initTx.insert(initTx.end(), {0x0B, 0x07}); // Scan limit: all rows
        initTx.insert(initTx.end(), {0x0C, 0x01}); // Shutdown: normal operation
        initTx.insert(initTx.end(), {0x0F, 0x00}); // Display test: off
        std::vector<uint8_t> rx(initTx.size());
        spi.xfer(initTx.data(), initTx.size(), rx.data(), rx.size());
    }

    // Example: turn on a few pixels
    setPixel(0, 0, true);   // Top-left of first matrix
    setPixel(7, 7, true);   // Bottom-right of first matrix
    setPixel(8, 0, true);   // Top-left of second matrix
    setPixel(31, 7, true);  // Bottom-right of last matrix
    updateMatrices();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}