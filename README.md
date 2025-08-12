🏗 Structural Health Monitoring (SHM) System

A smart IoT-based Structural Health Monitoring (SHM) system designed to track critical parameters such as displacement, vibration, and temperature in real-time for infrastructure health assessment. This project integrates custom-built sensors, cloud data logging, and visualization dashboards to ensure proactive maintenance and prevent structural failures.

📌 Overview
The SHM system continuously monitors structural parameters using a network of IoT sensors, transmitting data to the cloud for real-time analysis and visualization. It helps detect anomalies, enabling timely intervention and improving safety and lifespan of infrastructure such as bridges, buildings, and industrial setups.

🔹 Features
Multi-sensor integration:
LVDT – Measures linear displacement
MPU6050 – Detects vibrations and tilt
BME280 – Monitors temperature and atmospheric pressure
Wireless communication via ESP32 microcontroller
Real-time cloud logging using Google Sheets
Interactive dashboards using Power BI for data visualization
Modular design for easy scaling and deployment

⚙️ Technical Specifications
Controller: ESP32 (Wi-Fi enabled)
Sensors: LVDT, MPU6050, BME280
Cloud Platform: Google Sheets API for data logging
Visualization Tool: Microsoft Power BI
Communication Protocol: HTTP/HTTPS requests to cloud APIs

🚀 How It Works
Sensors collect displacement, vibration, and temperature data.
ESP32 processes the data and sends it to the cloud via Wi-Fi.
Data is stored in Google Sheets in real time.
Power BI dashboard fetches and visualizes the data for analysis.
Alerts or warnings can be configured for abnormal readings.

📈 Applications
Bridge and flyover safety monitoring
High-rise building health tracking
Industrial structure maintenance planning
Earthquake-prone zone infrastructure monitoring
