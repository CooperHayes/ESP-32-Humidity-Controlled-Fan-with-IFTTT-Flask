# ESP-32-Humidity-Controlled-Fan-with-IFTTT-Flask

## Project Overview

This system continuously monitors local humidity using a humidity sensor connected to an ESP32. When the measured humidity rises above the level determined by the potentiometer knob, the the ESP32 sends a signal to the MOSFET gate which acts as an electronic switch and allows for current to flow through the 5V fan and be controlled by the ESP32. Once humidity falls below the threshold, the fan is switched off automatically.

When the fan is switched on initially, the ESP32 sends a GET request to an IFTTT webhook which reponds by sending a email to a user inbox. Edge-detection is utilized to ensure that only 1 email is sent when the fan is turned on.

The ESP32 communicates a Python Flask server. When 30 seconds have elasped the ESP32 creates a POST request with a JSON body that contains sensor data and sends the request to the Flask server. The Flask server uses the "/log" endpoint to process and store the JSON data from a POST request in a SQLite database that it connects to via SQLAlchemy.

The stored data can be accessed by using the "/data" endpoint in the Flask server which causes the Flaks server to retrieve the sensor data, converts it to JSON data, and adds it to a web dashboard. The "/" endpoint in the Flask server is used to render a HTML page that shows a tabulated form of sensor data.




The goal of the project is to provide a simple, low-cost environmental control that operates without manual intervention, alerts users automatically, and updates a browser with real-time data through backend communication.

## Features

* Real-time humidity monitoring
* Automatic fan activation
* Digital presentation of temperature and humidity on OLED display
* Fan control via MOSFET driver
* Adjustable humidity thresholds
* Email alerts with IFTTT
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
   * If the fan is transitioning from OFF to ON:
       * An email is sent to inbox via IFTTT
5. If humidity does not exceed the threshold:
   * Fan turns OFF.
6. If 30 seconds have passed since the last POST request to the Flask server:
    * POST request containing sensor data is sent to Flask and stored in SQLite database
8. A browser makes GET requests with the route "/data" for the Flask to retrieve stored sensor data
9. The cycle repeats continuously.


## Software

The ESP32 portion of the project is programmed using the Arduino IDE.

Required libraries:

* DHT Sensor Library
* Adafruit Unified Sensor Library
* Adafruit GFX Library
* Wire Library
* WiFi Library
* HTTPClient

The backend is written in Python using Flask

The backend uses:

* Python
* Flask
* SQLAlchemy
* SQLite
* HTML

The Flask server is responsible for accepting POST requests containing sensor data, storing data in the SQLite database, and retrieving stored data when it receives GET requests from a web browser.

## Installation

1. Clone this repository.
2. Open the `.ino` file in Arduino IDE.
3. Install required libraries.
4. Input personal WiFi name and password
5. Connect the hardware according to the wiring diagram.
6. Configure an applet on IFTTT with an "if this" as "Recieve a webhook request" and a "then that" as "Send me an email".
7. Paste webhook url given by IFTTT into http.begin() when preparing a connection between ESP32 and IFTTT webhook
8. Add the Flask server IPv4 address to http.begin() when perparing a connection between ESP32 and Flask server
9. Upload the sketch to the ESP32 board.

 ## Flask Server Setup


* Install Python
* Navigate to the Flask backend directory
* Install the required Python dependencies
* Type "pip install flask flask-sqlalchemy" in Windows powershell or terminal
* Start the Flask server. by typing "python app.py" in the terminal
* Record the second IP address displayed in the terminal response and add the address to the ESP32 code as the flaskServer variable with "/log" added at the end
* Ensure that the ESP32 and Flask server are connected to the appropriate network
* Configure the ESP32 to send POST requests to the Flask server's /log endpoint by using the form "FlaskIPAddress/log"
* Open the Flask server's web interface in a browser to view the recorded data in JSON form by using the "/data" endpoint or in tabulated form by using "/" endpoint



## Results

The system successfully controlled a fan based on humidity readings and automatically sent email alerts, demonstrating automated environmental regulation with minimal hardware and allowed for safe power consumption through the use of a MOSFET. The ESP32 was capable of establishing a connection with the Pyhon Flask server and sending POST requests that were accepted by the Flask server and stored in the SQLite database.

The Flask server was able to query data from the SQLite database when GET requests routed with the endpoint "/" or "/data" were sent from browser


## Lessons Learned

Through this project I learned:

* Reading environmental sensors with ESP32
* Safely controlling higher-current devices using MOSFETs
* Formatting and displaying data on OLED displays
* Implementing threshold-based automation
* Utilizing potentiometers and mapping analog values to adjust values
* ESP32 WiFi connection capabilities
* Hardware troubleshooting and circuit design
* Utilizing Python Flask servers to recieve and retreive data stored in SQLite database
