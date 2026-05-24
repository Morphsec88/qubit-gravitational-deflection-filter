# 3-Port Quantum Readout Core & Real-Time Rate Meter

This repository contains an ultra-low latency, zero-storage allocation ($O(1)$) C++ processing core specifically designed for 3-channel quantum readout systems equipped with passive hardware filtering.

## Physical Architecture and the Core Problem

The ultimate bottleneck of classical quantum readout systems is the electromagnetic and thermal noise picked up by the analog cables coming out of the cryogenic zone, combined with phase-jitter caused by ambient gravitational waves. This software relies on a physical layout that solves noise filtering directly at the hardware level:

1. **In-Zone Termination:** The 3 physical ports (waveguides) terminate inside the cryogenic zone. No raw, noisy analog signals are brought out to the warm side.
2. **Metamaterial Filter Code (Rings):** Material rings (bakelite, aluminum, lead) wrapped around the outside of the tubes create a specific, custom permittivity profile.
3. **Spatial Masking:** The external sensors on the warm side can only see the absolute center of the ports. If the ring sequence code is perfect, the correct wavefront focuses directly onto the centerline. Unwanted wavefronts and parasitic modes are deflected by the microscopic spacetime ripples of gravitational waves toward the walls of the tube, keeping them physically masked so they never reach the detector.

### The 3 Dedicated Ports:
* **Port 1:** Optimized with specific rings for the |1⟩ quantum state (this is where the dense signal mass is expected).
* **Port 2:** Optimized with specific rings for the |0⟩ quantum state.
* **Port 3:** Reference channel (neutral / background noise monitoring).

---

## Software Architecture (Engineering Decisions)

* **Why Zero-Storage?** 
  Since spatial masking and hardware physics handle the data selection, the software does not need to analyze patterns or build gigabyte-sized raw timestamp arrays in RAM (which would immediately trigger OOM crashes at high frequencies). The code acts as a pure, bare-metal style Rate Meter.
* **Atomic Register Flushing (`std::atomic::exchange`):** 
  Synchronization across the 3 channels does not rely on software buffers. The hardware counters are read and cleared atomically within a single clock cycle, guaranteeing completely deterministic runtime.
* **Gravitational Jitter Suppression:** 
  Instantaneous focus shifts (twitching) caused by gravitational waves are smoothed out by a lightweight Exponential Moving Average (EMA) filter right before presentation, providing engineers with a clean, stable view of the resonance alignment.

---

## Laboratory Usage

The task of the lab engineers is to swap and arrange the material rings on the tubes until the display for **Port 1** shows a highly concentrated signal mass.

The software displays the Counts Per Second (CPS) and the contrast ratio in real-time. A high contrast ratio indicates that the other channels are quiet, meaning the noise has been successfully deflected into the masking zone.

```bash
# Compilation (Requires C++11 or newer)
g++ -O3 main.cpp -o quantum_readout

# Execution
./quantum_readout
```

### Display Output Example at Optimal Resonance:
```text
[PORT 1 SIGNAL MASS] Density:   18450 CPS | [#################################.......] Contrast: 12.45 | Status: RESONANCE (GOOD RINGS)
```

---

## License and Terms of Use

This project is open-source software licensed under the **GNU General Public License v3.0 (GPLv3)**.

### Under this license, you are free to:
* **Share and Modify:** Copy, distribute, and modify the source code.
* **Commercial Use:** Use this software for commercial purposes.

### Under the following conditions (Copyleft):
* **Source Disclosure:** If you modify this processing core or build upon it, you must release your modified version under the exact same GPLv3 license and make your source code public.
* **State Changes:** You must indicate if you have made changes to the original files.
* **No Warranty:** This software is provided "as is" without warranties of any kind. The author is not liable for any hardware damage or data loss.

For the full legal text of the license, please visit the official page: [https://gnu.org](https://gnu.org).