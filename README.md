# MediBox
###  Smart Medicine Reminder and Storage System

<img width="935" height="490" alt="Screenshot 2025-07-29 200753" src="https://github.com/user-attachments/assets/9362aece-531f-457e-b794-b53f3190a9e2" />


## 📘 Overview

This project implements a **Medibox** system using the **ESP32**, combining two progressive assignments aimed at medication schedule management and safe medicine storage. It features an interactive OLED-based alarm and environment monitoring system, enhanced with real-time data handling via **Node-RED**. 

---

## Version 1: Smart Alarm-Based Medibox System

<img width="1310" height="654" alt="Screenshot 2025-07-29 192600" src="https://github.com/user-attachments/assets/415fc59c-cc45-421a-aa16-92dc447f8a37" />

### ✅ Features Implemented

- **Interactive Menu (OLED + Buttons):**
  - Set time zone (UTC offset).
  - Set up to 2 medication alarms.
  - View and delete active alarms.

- **Time Synchronization:**
  - Fetch current time from **NTP server** using Wi-Fi.
  - Display real-time clock on OLED in the selected time zone.

- **Alarm System:**
  - Buzzer and LED alarm when medication time is reached.
  - **Snooze or Stop** alarm using a push button.
  - Snooze reactivates alarm after 5 minutes.

- **Environmental Monitoring (DHT22 Sensor):**
  - Temperature and Humidity readings displayed.
  - Warnings on OLED and LED indication when readings exceed healthy limits:
    - **Healthy Temperature:** 24°C to 32°C  
    - **Healthy Humidity:** 65% to 80%

<img width="962" height="534" alt="Screenshot 2025-07-29 192853" src="https://github.com/user-attachments/assets/556e1830-aa49-4508-b92f-06eee180b21e" />


## Hardware Implementaion

![WhatsApp Image 2025-07-29 at 20 48 55_fc3bf246](https://github.com/user-attachments/assets/e2ab3426-ab4d-4a11-b411-2aa1f7b3198e)

---

## 🛠️ Vesion 2: Enhanced Medibox with Smart Storage Conditions

<img width="1293" height="537" alt="Screenshot 2025-07-29 200721" src="https://github.com/user-attachments/assets/90aa30b2-f95c-4f26-8851-8f611eb4e202" />


###  Light Intensity Monitoring

- **LDR Sensor** collects light readings every **ts seconds** (default: 5s).
- Readings averaged over **tu seconds** (default: 2 min).
- Values normalized (range 0–1).
- Data sent to **Node-RED dashboard** for visualization.

####  Node-RED Dashboard (Group: Light Monitor)

- Live numeric display of averaged light intensity.
- Historical chart of light trends.
- Two sliders for adjusting:
  - Sampling interval (`ts`)
  - Upload interval (`tu`)

---

###  Dynamic Shaded Sliding Window (Servo Motor Control)

- Controls light exposure based on:
  - Light intensity (`I`)
  - Temperature (`T`)
  - User-defined parameters

####  Motor Angle Calculation:
θ = θoffset + (180 − θoffset) × I × γ × ln(ts / tu) × (T / Tmed)


Where:
- `θ`: Servo angle (0–180°)
- `θoffset`: Minimum angle (slider: 0–120°, default: 30°)
- `I`: Normalized light intensity (0–1)
- `γ`: Control factor (slider: 0–1, default: 0.75)
- `ts`: Sampling interval in seconds
- `tu`: Upload interval in seconds
- `T`: Current temperature
- `Tmed`: Ideal medicine storage temperature (slider: 10–40°C, default: 30°C)

####  Node-RED Dashboard )

- Slider to set **minimum angle (θoffset)**
- Slider to adjust **control factor (γ)**
- Slider to set **ideal storage temperature (Tmed)**

---
###  Wokwi Simulation

<img width="1274" height="631" alt="Screenshot 2025-07-29 192828" src="https://github.com/user-attachments/assets/3a00d5c0-095b-4188-8d76-9219f7308c76" />


---

## 🔧 Tech Stack & Tools

- **ESP32**
- **Arduino IDE**
- **Wokwi** (Online simulation)
- **Node-RED** (Dashboard and data communication)
- **DHT22**, **LDR**, **Servo Motor**, **OLED Display**, **Buzzer**, **Push Buttons**

---




