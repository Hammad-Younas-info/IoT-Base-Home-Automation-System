# 🏠 IoT-Base Home Automation System using Arduino

A simple time-based home automation system built with **Arduino Uno**, **16x2 LCD**, **DS3231 RTC Module**, **Relay**, **Buttons**, and a **Bulb**. This project allows scheduled and manual control of a home appliance (like a bulb) and displays real-time status on an LCD.

---

## 📦 Components Used

- Arduino Uno
- 16x2 LCD Display
- DS3231 Real Time Clock (RTC) Module
- Relay Module (5V)
- Push Buttons ×2
- AC Bulb
- 10kΩ Potentiometer (for LCD contrast)
- Breadboard, jumper wires, resistors, power supply

---

## ⚙️ Features

- Real-time clock-based control using DS3231
- Manual time setting via push buttons
- Appliance control (bulb) via relay
- Time and status display on LCD
- EEPROM saves schedule on power loss

---

## 🔌 Connections

| Component        | Arduino Pin       |
|------------------|-------------------|
| LCD RS           | D2                |
| LCD EN           | D3                |
| LCD D4–D7        | D4, D5, D6, D7    |
| RTC SDA/SCL      | A4 (SDA), A5 (SCL)|
| Set Time Button  | A0                |
| Increment Button | A1                |
| Relay IN         | D8                |

⚠️ **Safety Note:** Use opto-isolated relays and proper insulation when working with AC appliances.

---

## 🧠 Working Principle

- Reads time from the DS3231 RTC module.
- Displays the current time and relay (bulb) status on LCD.
- Button A0 enters time-setting mode.
- Button A1 increments hours/minutes.
- At scheduled time, relay activates bulb.
- EEPROM stores the set schedule permanently.

---

## 🚀 Future Upgrades

- Add Bluetooth/Wi-Fi control via app
- Auto control based on sensors (light/temperature)
- OLED screen for compact setup
- Battery backup for RTC module

---

## 👨‍💻 Author

**Hammad Younas**  
📧 hammad.younas.info@gmail.com  
🔌 Electrical Engineer 

