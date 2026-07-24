
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>


// ---------- HELPER FUNCTION PROTOTYPES  ----------
void readSensor();
void fanControl();
void sendEmailAlert();

void uploadFlask();
void updateOLED();


// ---------- OLED INITIALIZATION ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------- DHT22 INITIALIZATION ----------
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float hum = 55;
float temp = 24;

// ---------- FAN INITIALIZATION ----------
#define FAN_PIN 18


// ---------- POTENTIOMETER INITIALIZATION ----------
const int potPin = 32;
float potValue = 0;
int HUMIDITY_THRESHOLD = 60;



// ---------- WIFI INITIALIZATION ----------
const char* ssid = "..."; // Add personal WiFi name
const char* password = "..."; // Add personal WiFi password


// ---------- EDGE-DETECTION INITIALIZATION ----------
bool alertSent = false;
bool fanON = false;



// ---------- PYTHON FLASK SERVER INITIALIZATION ----------
unsigned long lastUpload = 0;
const char* flaskServer = ".../log"; // replace with flask server IP

void setup() {
	Serial.begin(115200);

	// I2C communication for OLED
	Wire.begin(OLED_SDA, OLED_SCL);

	// OLED start
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		Serial.println("OLED failed");
		while (true);
	}

	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);

	// DHT start
	dht.begin();

	// Fan setup
	pinMode(FAN_PIN, OUTPUT);
	digitalWrite(FAN_PIN, LOW); // fan OFF at boot


	// Connect to WiFi
	WiFi.begin( ssid , password );
  
  Serial.print("Connecting to WiFi");

  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Waiting...");
    delay(200);
  }
  Serial.println("Connected.");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());


	

} // END SETUP

void loop() {


	readSensor();


	fanControl();


	sendEmailAlert();


	updateOLED(); 


	if(millis() - lastUpload > 30000){
		
		uploadFlask();

		lastUpload = millis();
	}
	
	
	// ---------- SERIAL DISPLAY ----------
	Serial.print("Temp: ");
	Serial.print(temp);
	Serial.print(" C  Hum: ");
	Serial.println(hum);

	if (WiFi.status() == WL_CONNECTED) {
  	Serial.println("WiFi OK");
	}
	else {
  	Serial.println("WiFi Lost");
	}


	delay(2000);

} // END LOOP

// HELPER FUNCTIONS

void readSensor(){
	// Data collection
	temp = dht.readTemperature();
	hum = dht.readHumidity();

	if (isnan(temp) || isnan(hum)) {
		Serial.println("DHT read failed");
		delay(2000);
		return;
	}

} // End readSensor()

void fanControl(){

	potValue = analogRead(potPin);

	HUMIDITY_THRESHOLD = map(potValue , 0 , 4095 , 60 , 90);

	// ---------- FAN CONTROL ----------
	if (hum > HUMIDITY_THRESHOLD) {


		if(fanON == false){
			
			fanON = true;
			digitalWrite(FAN_PIN, HIGH); // ON

		}

	}
	else{
		if(fanON == true){ 
			
			fanON = false;
			digitalWrite(FAN_PIN , LOW); // OFF

		}

		alertSent = false;
	}

}// End fanControl()

void sendEmailAlert(){

	if (hum > HUMIDITY_THRESHOLD){
			if(alertSent == false){ // Send email alert
				HTTPClient http;
				http.begin("..."); // Add URL given by IFTTT
				int httpResponseCode = http.GET();

  			Serial.print("IFTTT HTTP Response Code: ");
  			Serial.println(httpResponseCode);

				alertSent = true;
  			http.end();
		}
	}

	else{
		alertSent = false;
	}

} // End sendEmailAlert


void updateOLED(){

	display.clearDisplay();

	display.setTextSize(1);
	display.setCursor(0, 0);
	display.print("Temp:");
	display.print(temp, 1);

	display.setCursor(0, 15);
	display.print("Hum:");
	display.print(hum, 1);
	display.print("%");

	display.setCursor(0, 30);
	display.print("Threshold:");
	display.print(HUMIDITY_THRESHOLD);
	display.print("%");

	display.setCursor(0,45);
	if (hum > HUMIDITY_THRESHOLD) {
		display.print("Fan: ON");
	} else {
		display.print("Fan: OFF");
	}

	display.display();
} // End updateOLED()


void uploadFlask(){

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi disconnected");
    return;
  }

  HTTPClient http;


  http.begin(flaskServer); // Add IPv4 address associated with Flask server
  http.addHeader("Content-Type", "application/json");

	// build JSON body
  String json = "{";
  json += "\"temperature\":" + String(temp,1) + ",";
  json += "\"humidity\":" + String(hum,1) + ",";
  json += "\"threshold\":" + String(HUMIDITY_THRESHOLD) + ",";
  json += "\"fan\":" + String(fanON ? "true" : "false");
  json += "}";

  int response = http.POST(json);

  Serial.print("Python Flask HTTP Response: ");
  Serial.println(response);

  http.end();
}

