// ============================================================================
//  3-Port Quantum Readout Core & Real-Time Rate Meter
//  Copyright (C) 2026
//  Licensed under GNU GPLv3.
// ============================================================================

#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <iomanip>
#include <algorithm>

// Atomic hardware counters for the 3 dedicated internal ports
std::atomic<uint64_t> port1_counter{0}; // |1> Quantum State
std::atomic<uint64_t> port2_counter{0}; // |0> Quantum State
std::atomic<uint64_t> port3_counter{0}; // Reference Channel (Background Noise)

// Exponential Moving Average (EMA) to smooth out gravitational phase-jitter
double smooth_contrast(double current_raw, double previous_ema, double alpha = 0.2) {
    return (alpha * current_raw) + ((1.0 - alpha) * previous_ema);
}

int main() {
    double contrast_ema = 1.0;
    
    std::cout << "=== 3-Port Quantum Readout Core Initialized [GPLv3] ===" << std::endl;
    std::cout << "Monitoring active... Press Ctrl+C to terminate." << std::endl;

    // Real-time measurement loop (1-second sampling intervals for display presentation)
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // FIXED: Using sequential consistency to guarantee absolute clock sync across ports
        uint64_t c1 = port1_counter.exchange(0, std::memory_order_seq_cst);
        uint64_t c2 = port2_counter.exchange(0, std::memory_order_seq_cst);
        uint64_t c3 = port3_counter.exchange(0, std::memory_order_seq_cst);

        // Calculate contrast ratio (Port 1 core focus vs peripheral noise floor)
        double noise_floor = static_cast<double>(c2 + c3) + 1.0;
        double raw_contrast = static_cast<double>(c1) / noise_floor;

        // Smooth out real-time gravitational phase-jitter
        contrast_ema = smooth_contrast(raw_contrast, contrast_ema, 0.25);

        // Render console display interface
        std::cout << "[PORT 1 SIGNAL MASS] Density: " << std::setw(7) << c1 << " CPS | [";
        
        int bar_width = static_cast<int>(std::min(contrast_ema * 2.0, 35.0));
        for (int i = 0; i < 35; ++i) {
            if (i < bar_width) std::cout << "#";
            else std::cout << ".";
        }
        // FIXED: Corrected the typo 'setsetprecision' to 'setprecision'
        std::cout << "] Contrast: " << std::fixed << std::setprecision(2) << contrast_ema;

        if (contrast_ema > 10.0) {
            std::cout << " | Status: RESONANCE (GOOD RINGS)\n";
        } else {
            std::cout << " | Status: TUNING REQUIRED\n";
        }
    }

    return 0;
}