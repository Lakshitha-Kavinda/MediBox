# MediBox
# MediBox - Smart Medicine Reminder System

## Overview

MediBox is an ESP32-based smart medicine reminder system that helps users manage their medication schedules effectively. The device features an OLED display, multiple alarms, environmental monitoring, and WiFi connectivity with NTP time synchronization.

## Features

### Core Functionality
- **Dual Medicine Alarms**: Set up to 2 independent medication reminders
- **Real-time Clock**: WiFi-enabled time synchronization via NTP server
- **Environmental Monitoring**: Temperature and humidity tracking with DHT22 sensor
- **Interactive Menu System**: Easy navigation using physical buttons
- **Visual & Audio Alerts**: LED indicator and buzzer for alarms and warnings

### Advanced Features
- **Snooze Function**: 5-minute snooze option for medication reminders
- **Time Zone Support**: Configurable UTC offset (±12 to +14 hours, 0.5-hour increments)
- **Alarm Management**: View, set, and delete individual alarms
- **Environmental Warnings**: Automatic alerts for temperature and humidity outside safe ranges
- **Persistent Alarms**: Alarms continue until acknowledged by user

## Hardware Requirements

### Components
- **ESP32 Development Board**
- **OLED Display**: 128x64 SSD1306 (I2C)
- **DHT22 Sensor**: Temperature and humidity monitoring
- **Buzzer**: Audio alerts
- **LED**: Visual indicator
- **Push Buttons (4)**: UP, DOWN, OK, CANCEL

### Pin Configuration
```
OLED Display (I2C): SDA, SCL (default ESP32 I2C pins)
DHT22 Sensor: GPIO 12
Buzzer: GPIO 5
LED Indicator: GPIO 15
Buttons:
  - UP: GPIO 33
  - DOWN: GPIO 35
  - OK: GPIO 32
  - CANCEL: GPIO 34
```

### Circuit Diagram
The circuit connections are defined in the `diagram.json` file for Wokwi simulation.

## Software Dependencies

### Libraries Required
- **Adafruit GFX Library** (v1.12.0+)
- **Adafruit SSD1306** (v2.5.13+)
- **DHT sensor library for ESPx** (v1.19+)
- **WiFi** (ESP32 built-in)
- **Wire** (I2C communication)

### Development Environment
- **PlatformIO**: Recommended IDE extension for VS Code
- **Arduino Framework**: For ESP32 development
- **Wokwi**: Online simulation platform (optional)

## Installation & Setup

### 1. Hardware Assembly
Connect all components according to the pin configuration above.

### 2. Software Setup
```bash
# Clone the repository
git clone <repository-url>
cd "Basic MediBox"

# Install PlatformIO (if not already installed)
# Open in VS Code with PlatformIO extension

# Build and upload
pio run --target upload
```

### 3. WiFi Configuration
Modify the WiFi credentials in `main.cpp`:
```cpp
WiFi.begin("Your_WiFi_SSID", "Your_Password", 6);
```

## Usage Guide

### Initial Setup
1. Power on the device
2. Wait for WiFi connection and time synchronization
3. The main screen displays current time in DD:HH:MM:SS format

### Menu Navigation
- **Press OK** to enter the main menu
- **UP/DOWN buttons** to navigate menu options
- **OK button** to select an option
- **CANCEL button** to go back or exit

### Menu Options
1. **Set Time** - Manually adjust the current time
2. **Set Alarm 1** - Configure first medication reminder
3. **Set Alarm 2** - Configure second medication reminder
4. **Disable Alarms** - Turn off all alarm functionality
5. **View Alarms** - Display all active alarms
6. **Delete Alarms** - Remove specific alarms
7. **Set Time Zone** - Configure UTC offset

### Alarm Management
- **Setting Alarms**: Use UP/DOWN to adjust hours/minutes, OK to confirm
- **Alarm Triggered**: Device displays "MEDICINE TIME", LED lights up, buzzer sounds
- **Stop Alarm**: Press CANCEL to stop and mark as taken
- **Snooze**: Press OK to snooze for 5 minutes

### Environmental Monitoring
The system continuously monitors:
- **Temperature Range**: Optimal 24-32°C
- **Humidity Range**: Optimal 65-85%
- **Warnings**: Automatic alerts for values outside safe ranges

## Environmental Thresholds

### Temperature Monitoring
- **High Alert**: >35°C
- **Low Alert**: <35°C (Note: This appears to be a typo in code)
- **Warning Range**: <24°C or >32°C

### Humidity Monitoring
- **High Alert**: >40%
- **Low Alert**: <20%
- **Warning Range**: <65% or >85%

## Troubleshooting

### Common Issues
1. **WiFi Connection Failed**
   - Check WiFi credentials
   - Ensure network is available
   - Verify ESP32 is within range

2. **Time Not Updating**
   - Check internet connection
   - Verify NTP server accessibility
   - Confirm time zone settings

3. **Sensor Readings Incorrect**
   - Check DHT22 connections
   - Verify sensor is not damaged
   - Ensure proper power supply

4. **Display Issues**
   - Verify I2C connections (SDA/SCL)
   - Check display address (0x3C)
   - Ensure adequate power supply

## Technical Specifications

### System Requirements
- **Microcontroller**: ESP32 (240MHz dual-core)
- **Memory**: 4MB Flash minimum
- **Operating Voltage**: 3.3V
- **WiFi**: 802.11 b/g/n
- **Display**: 128x64 OLED (I2C)

### Performance Characteristics
- **Time Accuracy**: NTP synchronized (±1 second)
- **Sensor Update Rate**: Continuous monitoring
- **Alarm Precision**: 1-minute resolution
- **Battery Life**: Depends on power source and usage

## File Structure
```
├── src/
│   └── main.cpp          # Main application code
├── include/              # Header files
├── lib/                  # Local libraries
├── test/                 # Test files
├── platformio.ini        # PlatformIO configuration
├── wokwi.toml           # Wokwi simulation config
├── diagram.json         # Circuit diagram
└── README.md            # This file
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is open source. Please check the repository for license details.

## Support

For issues, questions, or contributions, please refer to the project repository or contact the development team.

## Version History

- **v1.0**: Initial release with basic alarm and environmental monitoring functionality

---

**Note**: This project is designed for educational and personal use. For medical applications, please consult with healthcare professionals and ensure compliance with relevant regulations.
