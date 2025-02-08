# RealTimeCutVADCXXLibrary

**RealTimeCutVADCXXLibrary** is the core C++ implementation of a real-time Voice Activity Detection (VAD) system utilizing **ONNX Runtime** for running Silero VAD models and **WebRTC's Audio Processing Module (APM)** for advanced audio preprocessing. This library is responsible for the detailed audio segmentation and detection algorithms. It is used in **RealTimeCutVADLibrary**.

---

## Features

- **Real-time Voice Activity Detection (VAD)** using Silero models (v4 and v5)
- **Real-time WAV generation with callback support** for seamless integration
- **High-performance inference** with ONNX Runtime (C++)
- **Advanced audio preprocessing** using WebRTC's APM (noise suppression, gain control, high-pass filtering)
- **Accurate voice segmentation algorithms** implemented entirely in C++
- **Cross-platform compatibility** through XCFramework

---

## Building the XCFramework

**RealTimeCutVADCXXLibrary** is the core C++ implementation, designed to be compiled into an XCFramework. This project is not intended for direct integration via Swift Package Manager (SPM) or CocoaPods.

### How to Build

You can generate the XCFramework by running the provided build script. This script compiles the library for iOS devices, iOS simulators, and macOS, and then packages them into a single XCFramework.

### Steps to Build:

1. Clone the repository:

```bash
git clone https://github.com/helloooideeeeea/RealTimeCutVADCXXLibrary.git
cd RealTimeCutVADCXXLibrary
```

2. Run the build script:

```bash
./build_xcframework.sh
```

### Script Details

The `build_xcframework.sh` script will:
- Clean any existing build artifacts.
- Build the library for:
  - iOS devices (`iphoneos`)
  - iOS simulators (`iphonesimulator`)
  - macOS (`macosx`)
- Create the XCFramework combining all platforms.

### Output

Upon successful execution, the XCFramework will be generated at:

```bash
./build/RealTimeCutVADCXXLibrary.xcframework
```

You can then integrate this XCFramework into your iOS or macOS projects.

---

## Algorithm Explanation

### ONNX Runtime for Silero VAD
This library leverages **ONNX Runtime (C++)** to run Silero VAD models efficiently, ensuring fast and accurate voice activity detection.

### WebRTC's Audio Processing Module (APM)
WebRTC's APM is used for:

- **High-pass Filtering**: Removes low-frequency noise.
- **Noise Suppression**: Reduces background noise for clearer detection.
- **Gain Control**: Adaptive digital gain control enhances audio levels.
- **Sample Rate Conversion**: Converts audio to 16 kHz for Silero VAD compatibility.

### Audio Processing Workflow

1. **Input Audio Configuration**: Supports sample rates of 8 kHz, 16 kHz, 24 kHz, and 48 kHz.
2. **Audio Preprocessing**: 
   - Audio is split into chunks and processed with APM.
   - Audio is converted to 16 kHz for compatibility with Silero VAD.
3. **Voice Activity Detection**:
   - Processed audio is passed to Silero VAD via ONNX Runtime.
   - VAD outputs a probability score indicating voice activity.
4. **Voice Segmentation Algorithm**:
   - **Voice Start**: Triggered when the probability exceeds the start threshold for a set number of frames.
   - **Voice End**: Triggered when silence is detected over a set number of frames.
5. **Output**: The final WAV data is generated, including proper header information for immediate playback or saving.

### WebRTC APM Configuration

```cpp
config.gain_controller1.enabled = true;
config.gain_controller1.mode = webrtc::AudioProcessing::Config::GainController1::kAdaptiveDigital;
config.gain_controller2.enabled = true;
config.high_pass_filter.enabled = true;
config.noise_suppression.enabled = true;
config.transient_suppression.enabled = true;
config.voice_detection.enabled = false;
```

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.



