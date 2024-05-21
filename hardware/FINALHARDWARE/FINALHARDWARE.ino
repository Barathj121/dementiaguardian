#include <ArduinoJson.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <XTronical_ST7735.h> // Hardware-specific library
#include <TinyGPSPlus.h>
#include <SPI.h>
#include <HTTPClient.h> // For making HTTP requests
#include <WiFi.h>
#include <math.h> // For math functions

#define TFT_DC     2       // Data/Command pin
#define TFT_RST    4       // Reset pin
#define TFT_CS     5       // Chip select pin
#define BUTTON_PIN 34       // Pin connected to the button


#define HOME_LAT   200.34   // Latitude of home location
#define HOME_LONG  56.78   // Longitude of home location
#define ALERT_DISTANCE 100 // Distance threshold for sending alert (in meters)

#define WIFI_SSID "vivo Y100"    // Your WiFi SSID
#define WIFI_PASSWORD "12345678"  // Your WiFi password



Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);  // Initialize TFT
TinyGPSPlus gps; // Initialize GPS object

double haversine(double lat1, double lon1, double lat2, double lon2) {
  // Convert degrees to radians
  lat1 = lat1 * M_PI / 180.0;
  lon1 = lon1 * M_PI / 180.0;
  lat2 = lat2 * M_PI / 180.0;
  lon2 = lon2 * M_PI / 180.0;

  // Differences
  double dlat = lat2 - lat1;
  double dlon = lon2 - lon1;

  double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));

  // Radius of Earth in meters
  double R = 6371000;

  // Calculate distance
  double distance = R * c;
  return distance; // in meters
}

bool alertSent = false; // Flag to track if alert has been sent
int alertCount = 0; // Variable to track the number of alerts sent
volatile bool buttonAlertFlag = false;
volatile bool getlocationflag = false;
volatile bool medicineAlertFlag = false;

int alertTime;
String medicine_name;

void checkDistance() {
  if (gps.location.isValid()) {
    double currentLat = gps.location.lat();
    double currentLon = gps.location.lng();
    /*double currentLat = 76.905421;
    double currentLon = 10.902515;*/
    double distance = haversine(HOME_LAT, HOME_LONG, currentLat, currentLon);

    if (distance > ALERT_DISTANCE && alertCount < 2) { // Check if distance exceeds threshold and alerts haven't been sent twice
      sendAlert("Distance");
      displayAlert();
      delay(10000);
      displayDefaultText();
      alertSent = true; // Set flag to true after sending alert
      alertCount++; // Increment alert count
    } else if (distance <= ALERT_DISTANCE) {
      alertSent = false; // Reset flag if user is inside the perimeter
      alertCount = 0; // Reset alert count if user is within the perimeter
    }
  }
}

void IRAM_ATTR handleButtonPress() {

  buttonAlertFlag=true;
}

hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer() {
  getlocationflag=true;
}

hw_timer_t * medicineTimer = NULL;

void IRAM_ATTR onMedicineTimer() {
  medicineAlertFlag = true;
}


void setup() {
  tft.init();   // Initialize the display
  Serial.begin(115200); // Initialize Serial communication
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // Initialize GPS communication
  pinMode(BUTTON_PIN, INPUT); // Set button pin as input with pull-up resistor


  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonPress, HIGH);

  connectWiFi();
  displayDefaultText();
  checkDistance();
  getMedicineAlert();
  getMedicineAlert();
  Serial.println(alertTime);
  Serial.println(medicine_name);
  
  // Initialize hardware timer
  timer = timerBegin(0, 80, true); // Timer 0, divider 80 (for 1MHz), count up

  // Attach interrupt
  timerAttachInterrupt(timer, &onTimer, true);

  // Set timer to trigger every minute (60 seconds * 1,000,000 microseconds)
  timerAlarmWrite(timer, 120000000, true);

  // Start timer
  timerAlarmEnable(timer);

  medicineTimer = timerBegin(1, 80, true); // Timer 1, divider 80 (for 1MHz), count up

  // Attach interrupt
  timerAttachInterrupt(medicineTimer, &onMedicineTimer, true);

  // Set timer to trigger every `time` seconds
  timerAlarmWrite(medicineTimer, alertTime, true);

  // Start timer
  timerAlarmEnable(medicineTimer);


}



void loop() {
  while (Serial2.available() > 0) {
    if (gps.encode(Serial2.read())) {
      checkDistance();
    }
  }
  if(buttonAlertFlag){
    sendAlert("Button");
    displayAlert();
    delay(10000);
    displayDefaultText();
    buttonAlertFlag=false;
  }
  if(getlocationflag){
    sendLocation();
    getlocationflag=false;
  }
  if(medicineAlertFlag){
    displayMedicineAlert();
    delay(10000);
    displayDefaultText();
    medicineAlertFlag = false;
  }
  
}

void displayDefaultText() {
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  tft.setCursor(0, 0); // Set cursor position
  tft.setTextColor(ST7735_WHITE); // Set text color
  tft.setTextSize(2); // Set text size

  tft.println("Click button if you forgot where you are!"); // Display default text
}

void displayAlert() {
  // Display alert message on the screen
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  tft.setCursor(0, 0); // Set cursor position
  tft.setTextColor(ST7735_WHITE); // Set text color
  tft.setTextSize(2); // Set text size

  tft.print("Alert Sent with location! Source: ");
  // Display alert message

}

void sendAlert(String source) {
  // Create JSON object with current location, timestamp, and source of alert
  String jsonData = "{\"location_lat\": " + String(gps.location.lat(), 6) + ", \"location_long\": " + String(gps.location.lng(), 6) + ", \"source\": \"" + String(source) + "\"}";

  // Send HTTP POST request with JSON data to external API
  HTTPClient http;
  http.begin("https://iot-0agm.onrender.com/sensor"); // Replace with your API endpoint
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonData);
  String response = http.getString();
  Serial.println("HTTP Response code: " + String(httpResponseCode));
  Serial.println("Server response: " + response);
  http.end();
}

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}

void sendLocation() {


  String jsonData = "{\"lat\": " + String(gps.location.lat(), 6) + ", \"lon\": " + String(gps.location.lng(), 6) + "}";
  // Send HTTP POST request with JSON data to external API
  HTTPClient http;
  http.begin("https://dementian-location.onrender.com/update-location"); // Replace with your API endpoint
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonData);
  String response = http.getString();
  Serial.println("HTTP Response code: " + String(httpResponseCode));
  Serial.println("Server response: " + response);
  http.end();
}



void getMedicineAlert() {
  HTTPClient http;
  http.begin("https://iot-0agm.onrender.com/medicine_alerts");  // Replace with your API endpoint

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String response = http.getString();

    // Parse the JSON response
    StaticJsonDocument<200> doc;
    deserializeJson(doc, response);

    // Store the data in the variables
    alertTime = doc["time"];
    medicine_name = doc["medicine_name"].as<String>();
    

  } else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}

void displayMedicineAlert() {
  tft.fillScreen(ST7735_BLACK); // Clear the screen
  tft.setCursor(0, 0); // Set cursor position
  tft.setTextColor(ST7735_WHITE); // Set text color
  tft.setTextSize(2); // Set text size

  tft.println("Take medicine: " + medicine_name); // Display medicine alert

  // Display alert message
}
 