# ⌚ Wearable Stress Monitoring Device

This project documents the development of a wearable device designed to monitor stress levels in real-time. The device utilizes a combination of sensors to track key physiological indicators, including heart rate, skin conductivity (GSR), and body temperature. 
---
**Key Features:**

* **Real-time Stress Monitoring:** Continuously tracks heart rate, skin conductivity, and body temperature.
* **Wireless Data Transmission:** Transmits data wirelessly to a Telegram bot for remote monitoring and analysis.
* **Compact and Wearable:** 3D-printed casing ensures a comfortable and portable design.
* **User-Friendly:** OLED display provides immediate feedback on stress levels.
* **Rechargeable Battery:** Powered by a rechargeable Li-ion battery for extended usage.
---
**Components:**

* **Arduino R4 Wi-Fi:** Microcontroller for data processing and communication.
* **MAX30105 Heart Rate Sensor:** Measures heart rate using photoplethysmography.
* **BME280 Temperature Sensor:** Measures ambient temperature.
* **GSR Sensor:** Measures skin conductivity.
* **OLED Display:** Displays real-time data.
* **Li-ion Battery:** Provides power to the device.
* **3D-Printed Casing:** Houses all components.
* **Telegram Bot:** Enables remote data monitoring.
---
**Hardware Setup:**

1. **Sensor Integration:**
   - Connect the MAX30105, BME280, and GSR sensors to the Arduino R4 Wi-Fi board using I2C communication.
   - Connect the OLED display to the Arduino for data visualization.
2. **Power Supply:**
   - Connect the Li-ion battery to the Arduino and integrate the charging circuit (TP4056).
3. **Casing Assembly:**
   - Assemble the 3D-printed casing and securely mount all components.
---
**Software Development:**

1. **Arduino Code:**
   - Write Arduino code to:
     - Read sensor data from the MAX30105, BME280, and GSR sensors.
     - Process sensor data and calculate stress indices (optional).
     - Display real-time data on the OLED screen.
     - Transmit data wirelessly to the Telegram bot using the UniversalTelegramBot library.
2. **Telegram Bot Setup:**
   - Create a Telegram bot using BotFather.
   - Obtain the bot's API key and your Telegram chat ID.
   - Integrate the API key and chat ID into the Arduino code.
---
**Usage:**

1. **Charge the device:** Use the provided charging circuit.
2. **Wear the device:** Position the device comfortably on your wrist.
3. **Monitor data:** View real-time stress data on the OLED display and receive updates on your Telegram account.
---
**Future Work:**

* **Machine Learning Integration:** Implement machine learning algorithms to predict stress levels more accurately.
* **Data Visualization:** Develop a more sophisticated dashboard for data visualization and analysis.
* **User Interface Enhancements:** Improve the user interface for easier interaction with the device.
* **Stress Management Features:** Integrate stress-reducing exercises or mindfulness techniques into the device.
---

## 🤝 Contributions  
- **Aswin Deivanayagam S**: [GitHub](https://github.com/Ashprogrammer29)  
- **Kishore Muruganantham**: [GitHub](https://github.com/KishoreMuruganantham)  

