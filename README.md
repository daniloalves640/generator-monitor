⚡ ESP32 Generator Vibration Monitor (Slack Alerts)

This project uses an ESP32 and an SW-420 vibration sensor to monitor when equipment (e.g. a generator or motor) turns ON and OFF based on vibration.
It sends real-time notifications to Slack via a webhook.

The system avoids false triggers by requiring 1 minute of continuous vibration before confirming that the equipment is running.

🚀 Features

🟢 Slack message when ESP32 comes online

⏳ Confirms equipment is ON only after 1 minute of continuous vibration

✅ Slack alert when equipment turns ON

🔴 Slack alert when equipment turns OFF

🔁 Automatic re-arming after shutdown

🖥 Serial monitor debug output

🌐 Wi-Fi enabled (no extra servers required)

🧰 Hardware Required

ESP32 (tested with LilyGO T-Display / ESP32 Dev Module)

SW-420 vibration sensor module

Jumper wires

Stable 5V power supply

(Recommended) 100µF capacitor between 5V and GND

🔌 Wiring
SW-420	ESP32
VCC	5V
GND	GND
DO	GPIO 27

⚠️ Power the SW-420 from 5V, not 3.3V.
The output pin (DO) is safe to connect to ESP32 GPIO.

📡 Logic

ESP32 boots

Connects to Wi-Fi

Sends online status to Slack

Waits for vibration

Starts timer when vibration begins

If vibration continues for 60s → Equipment ON alert

When vibration stops → Equipment OFF alert

System resets state and waits again

🧠 State Machine
IDLE → VIBRATION → (60s) → RUNNING → STOPPED → IDLE
