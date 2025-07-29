# MediBox
### Smart medicine remainder system

# 💊 Medibox: Smart Medicine Reminder and Storage System

![Medibox Simulation Screenshot](images/medibox_simulation.png)
> *Wokwi Simulation Interface*

## 📘 Overview

This project implements a **Medibox** system using the **ESP32**, combining two progressive assignments aimed at medication schedule management and safe medicine storage. It features an interactive OLED-based alarm and environment monitoring system, enhanced with real-time data handling via **Node-RED**. The project simulates and demonstrates real-world capabilities using **Wokwi** and **physical hardware**.

---

## 🧠 Assignment 1: Smart Alarm-Based Medibox System

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

---

## 🛠️ Assignment 2: Enhanced Medibox with Smart Storage Conditions

### 🔆 Light Intensity Monitoring

- **LDR Sensor** collects light readings every **ts seconds** (default: 5s).
- Readings averaged over **tu seconds** (default: 2 min).
- Values normalized (range 0–1).
- Data sent to **Node-RED dashboard** for visualization.

#### 📊 Node-RED Dashboard (Group: Light Monitor)

- Live numeric display of averaged light intensity.
- Historical chart of light trends.
- Two sliders for adjusting:
  - Sampling interval (`ts`)
  - Upload interval (`tu`)

---

### 🤖 Dynamic Shaded Sliding Window (Servo Motor Control)

- Controls light exposure based on:
  - Light intensity (`I`)
  - Temperature (`T`)
  - User-defined parameters

#### 🔍 Motor Angle Calculation:


