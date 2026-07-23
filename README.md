# ESP-32-Humidity-Controlled-Fan-with-IFTTT-Flask

## Project Overview

This system continuously monitors local humidity using a humidity sensor connected to an ESP32. When the measured humidity rises above the level determined by the potentiometer , the the ESP32 sends a signal to the MOSFET gate which acts as an electronic switch and allows for current to flow through the 5V fan withoug damaging any GPIO pins on the ESP32. Once humidity falls below the threshold, the fan is switched off automatically.

When the fan is switched on initially, the ESP32 sends a GET request to an IFTTT webhook which reponds by sending a email to a user inbox. Edge-detection is utilized to ensure that only 1 email is sent when the fan is turned on.

The ESP32 communicates a Python Flask server. When 30 seconds have elasped the ESP32 creates a POST request with a JSON body that contains sensor data and sends the request to the Flask server. The Flask server uses the "/log" endpoint to process and store the JSON data in a SQLite database that it connects to via SQLAlchemy.

The stored can be accessed by using the "/data" endpoint in the Flask server which retrieves the sensor data, converts it to JSON data, and adds it to a web dashboard. The "/" endpoint in the Flask server is used to render a HTML page that shows a tabulated form of sensor data.




The goal of the project is to provide a simple, low-cost environmental control that operates without manual intervention, alerts users automatically, and updates a browser with real-time data through backend communication.

## Features

* Real-time humidity monitoring
* Automatic fan activation
* Digital presentation of temperature and humidity on OLED display
* Adjustable humidity thresholds
* Python Flask backend communication
* API endpoint for data logging and retrieval

## Hardware Used

| Component                      | Quantity |
| ------------------------------ | -------- |
| ESP32 Microcontrolller         | 1        |
| .96 SSD1306 OLED Display       | 1        |
| DHT11 or DHT22 Humidity Sensor | 1        |
| Bare Wire 5V Fan               | 1        |
| MOSFET or Transistor Driver    | 1        |
| 220 Ω Resistor                 | 1        |
| 10 kΩ Resistor                 | 1        |
| Power Supply                   | 1        |
| Jumper Wires                   | 19       |
| Breadboard (prototype stage)   | 1        |
| Potentiometer                  | 1        |

## Wiring

A complete wiring diagram is provided in the repository documentation. Refer to the diagrams folder for detailed wiring diagrams, pin assignments, and component connections.

The wiring diagram illustrates:

* Humidity sensor connections
* MOSFET connections
* Resistor placement
* OLED display connections
* Poteintometer connections
* ESP32 pin assignments
* Flyback diode placement


## How It Works

1. The humidity sensor measures the surrounding air humidity.
2. The ESP32 reads the sensor value.
3. A threshold is determined by the position of a potentiometer knob
4. If humidity exceeds the threshold:
   * Fan turns ON.
5. If humidity does not exceed the threshold:
   * Fan turns OFF.
6. The cycle repeats continuously.


## Software

The project is programmed using the Arduino IDE.

Required libraries:

* DHT Sensor Library
* Adafruit Unified Sensor Library
* Adafruit GFX Library
* Wire Library
* WiFi Library
* HTTPClient

## Installation

1. Clone this repository.
2. Open the `.ino` file in Arduino IDE.
3. Install required libraries.
4. Input personal WiFi name and password
5. Connect the hardware according to the wiring diagram.
6. Configure an applet on IFTTT with an "if this" as "Recieve a webhook request" and a "then that" as "Send me an email".
7. Paste webhook url given by IFTTT into http.GET()
8. Upload the sketch to the ESP32 board.

## Results

The system successfully controlled a fan based on humidity readings and automatically sent email alerts, demonstrating automated environmental regulation with minimal hardware and allowed for safe power consumption through the use of a MOSFET.

## Future Improvements

* Adjustable threshold via buttons
* Mobile app integration
* Pulse Width Modulation for accurate control

## Lessons Learned

Through this project I learned:

* Reading environmental sensors with ESP32
* Controlling higher-current devices using MOSFETs
* Formatting and displaying data on OLED displays
* Implementing threshold-based automation
* Utilizing potentiometers and mapping analog values to adjust values
* ESP32 WiFi connection capabilities
* Hardware troubleshooting and circuit design
