# 💊 MediBox - Smart Medicine Storage System

<div align="center">

![ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)
![Arduino](https://img.shields.io/badge/Framework-Arduino-00979D.svg)
![PlatformIO](https://img.shields.io/badge/IDE-PlatformIO-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

*An intelligent IoT-based medicine storage system with environmental monitoring and automated controls*

</div>

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Software Architecture](#software-architecture)
- [Installation](#installation)
- [Usage](#usage)
- [MQTT Integration](#mqtt-integration)
- [Configuration](#configuration)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## 🔍 Overview

MediBox is a smart medicine storage system built with ESP32 that monitors environmental conditions, provides medication reminders, and automatically adjusts storage conditions to maintain optimal medicine preservation. The system features WiFi connectivity, MQTT communication, and real-time sensor monitoring.

## ✨ Features

### 🕒 Time & Alarm Management
- **Real-time Clock**: NTP server synchronization for accurate timekeeping
- **Multiple Alarms**: Set up to 2 medication reminder alarms
- **Time Zone Support**: Configurable UTC offset
- **Visual Display**: OLED screen showing time and status information

### 🌡️ Environmental Monitoring
- **Temperature Monitoring**: Real-time temperature tracking with DHT sensor
- **Light Intensity Measurement**: LDR sensor for ambient light detection
- **Data Averaging**: Configurable sampling and reporting intervals
- **MQTT Publishing**: Sensor data transmitted to cloud services

### 🔧 Automated Controls
- **Servo Motor Control**: Automated opening/closing based on environmental conditions
- **Intelligent Angle Calculation**: Dynamic servo positioning based on:
  - Light intensity levels
  - Temperature readings
  - Configurable controlling factors
- **Warning System**: Audio alerts for critical conditions

### 🌐 IoT Connectivity
- **WiFi Integration**: Wireless network connectivity
- **MQTT Protocol**: Real-time data transmission and remote configuration
- **Remote Control**: Adjustable parameters via MQTT topics
- **Cloud Integration**: Compatible with IoT platforms

### 🎵 Audio Feedback
- **Buzzer Alerts**: Musical tone alarms for medication reminders
- **Warning Sounds**: Critical condition notifications
- **Customizable Tones**: Multiple note sequences for different alerts

## 🔧 Hardware Components

| Component | Purpose | Pin Connection |
|-----------|---------|----------------|
| **ESP32 DevKit** | Main microcontroller | - |
| **OLED Display (SSD1306)** | Information display | I2C (SDA/SCL) |
| **DHT22 Sensor** | Temperature & humidity | Pin 12 |
| **LDR Sensor** | Light intensity | Pin 36 (ADC) |
| **Servo Motor** | Automated lid control | Pin 14 |
| **Buzzer** | Audio alerts | Pin 5 |
| **LED Indicator** | Status indication | Pin 18 |
| **Push Buttons** | User interface | Pins 32, 33, 34, 35 |

### 📐 Circuit Diagram
*Circuit diagrams available in the `/Diagrams` folder*

## 🏗️ Software Architecture

```
MediBox System
├── Hardware Abstraction Layer
│   ├── Sensor Drivers (DHT, LDR)
│   ├── Display Controller (OLED)
│   └── Actuator Control (Servo, Buzzer)
├── Core Functions
│   ├── Time Management
│   ├── Alarm System
│   └── Environmental Monitoring
├── Communication Layer
│   ├── WiFi Manager
│   └── MQTT Client
└── User Interface
    ├── Menu System
    └── Button Handlers
```

## 🚀 Installation

### Prerequisites
- **PlatformIO IDE** or **Arduino IDE**
- **ESP32 Board Package**
- **Required Libraries** (automatically managed by PlatformIO)

### Setup Steps

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Lakshitha-Kavinda/MediBox.git
   cd MediBox
   ```

2. **Open Project**
   ```bash
   # Using PlatformIO
   cd Improved_medibox
   pio run
   
   # Or open MediBox.code-workspace in VS Code
   ```

3. **Configure WiFi Credentials**
   ```cpp
   // Add your WiFi credentials in main.cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

4. **Configure MQTT Broker**
   ```cpp
   // Update MQTT broker settings
   const char* mqtt_server = "YOUR_MQTT_BROKER";
   const int mqtt_port = 1883;
   ```

5. **Upload to ESP32**
   ```bash
   pio run --target upload
   ```

## 📱 Usage

### Initial Setup
1. **Power on** the device
2. **Connect to WiFi** (credentials in code)
3. **Set current time** using the menu system
4. **Configure alarms** for medication schedules

### Menu Navigation
- **UP/DOWN buttons**: Navigate menu options
- **OK button**: Select/confirm
- **CANCEL button**: Go back/exit

### Menu Options
1. **Set Time**: Adjust current time
2. **Set Alarm 1/2**: Configure medication reminders
3. **Disable Alarms**: Turn off alarm functionality
4. **View Alarms**: Display current alarm settings
5. **Delete Alarms**: Remove alarm configurations
6. **Set Time Zone**: Adjust UTC offset

### Automatic Operations
- **Environmental monitoring** runs continuously
- **Servo motor** adjusts position based on conditions
- **Data publishing** to MQTT topics at configured intervals

## 🌐 MQTT Integration

### Published Topics
| Topic | Data Type | Description |
|-------|-----------|-------------|
| `LDR_Value_220316V` | Integer | Average light intensity readings |
| `Temperature_Value_220316V` | Integer | Average temperature readings |

### Subscribed Topics
| Topic | Purpose | Data Format |
|-------|---------|-------------|
| `LDR_Sample_Config_220316V` | Sampling interval | Milliseconds (int) |
| `LDR_Send_Config_220316V` | Publishing interval | Milliseconds (int) |
| `Theta_Offset_Config_220316V` | Servo offset angle | Degrees (int) |
| `Light_Intensity_Config_220316V` | Light threshold | Float (0.0-1.0) |
| `Controlling_Factor_Config_220316V` | Control sensitivity | Float (0.0-1.0) |
| `Ideal_Storage_Temperature_Config_220316V` | Target temperature | Celsius (int) |

### Example MQTT Commands
```bash
# Configure sampling interval (5 seconds)
mosquitto_pub -h broker.hivemq.com -t "LDR_Sample_Config_220316V" -m "5000"

# Set ideal storage temperature (25°C)
mosquitto_pub -h broker.hivemq.com -t "Ideal_Storage_Temperature_Config_220316V" -m "25"
```

## ⚙️ Configuration

### Default Settings
```cpp
// Sampling Configuration
unsigned long samplingInterval = 5000;      // 5 seconds
unsigned long sendingInterval = 120000;     // 2 minutes

// Servo Control Parameters
int theta_offset = 30;                       // Base angle offset
float light_intensity = 0.5;                // Light threshold
float controlling_factor = 0.75;            // Control sensitivity
int ideal_storage_temp = 30;                // Target temperature (°C)
```

### WiFi & MQTT Setup
Update these variables in `main.cpp`:
```cpp
const char* ssid = "YOUR_NETWORK_NAME";
const char* password = "YOUR_NETWORK_PASSWORD";
const char* mqtt_server = "YOUR_MQTT_BROKER_IP";
```

## 📁 Project Structure

```
MediBox/
├── README.md                          # This file
├── Basic MediBox/                     # Initial version
│   └── src/
├── Improved_medibox/                  # Main project
│   ├── src/
│   │   └── main.cpp                   # Main application code
│   ├── platformio.ini                 # PlatformIO configuration
│   ├── include/                       # Header files
│   ├── lib/                          # Custom libraries
│   └── test/                         # Test files
└── Diagrams/                         # Circuit diagrams and schematics
    ├── Time_from_Wifi.json
    └── diagram.json
```

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add some AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open** a Pull Request

### Development Guidelines
- Follow existing code style and conventions
- Add comments for complex logic
- Test thoroughly on hardware before submitting
- Update documentation for new features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👥 Authors

- **Lakshitha Kavinda** - *Initial work* - [@Lakshitha-Kavinda](https://github.com/Lakshitha-Kavinda)

## 🙏 Acknowledgments

- **Adafruit** for excellent sensor libraries
- **ESP32 Community** for comprehensive documentation
- **PlatformIO** for the amazing development environment
- **Arduino Community** for foundational framework

## 📞 Support

If you encounter any issues or have questions:

1. Check the [Issues](https://github.com/Lakshitha-Kavinda/MediBox/issues) page
2. Create a new issue with detailed description
3. Include hardware setup and error messages

---

<div align="center">

**⭐ Star this repository if you find it helpful!**

</div>
