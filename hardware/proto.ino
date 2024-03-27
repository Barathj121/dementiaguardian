#include <WiFi.h>
#include <HTTPClient.h>

// Replace these values with your actual WiFi credentials
const char* ssid = "vivo Y100";
const char* password = "12345678";

// Replace these values with your actual API endpoint details
const char* apiUrl = "https://iot-0agm.onrender.com/alert";

// Home location coordinates
const float homeLat = 12.3456;
const float homeLng = 78.9101;

// Threshold distance in meters
const float thresholdDistance = 1000; // 1 km

// Function to calculate distance between two coordinates
float distance(float lat1, float lon1, float lat2, float lon2) {
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float a = sin(dLat / 2) * sin(dLat / 2) +
            cos(radians(lat1)) * cos(radians(lat2)) *
            sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float d = 6371000 * c; // Radius of Earth in meters
  return d;
}

void sendAlert(float lat, float lng, float homeLat, float homeLng, int userId) {
  HTTPClient http;

  // Construct the JSON payload
  String payload = "{\"lat\": " + String(lat, 6) + ", \"lng\": " + String(lng, 6) + ", \"homeLat\": " + String(homeLat, 6) + ", \"homeLng\": " + String(homeLng, 6) + ", \"userId\": " + String(userId) + "}";

  http.begin(apiUrl); // Specify the URL
  http.addHeader("Content-Type", "application/json"); // Specify content-type header

  // Send the request and capture the response code
  int httpResponseCode = http.POST(payload);

  // Check for successful response
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String response = http.getString(); // Get the response payload
    Serial.println(response); // Print the response payload
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Close connection
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Hardcoded latitude and longitude for demonstration
  float currentLat = 100.3457;
  float currentLng = 78.9102;

  // Calculate distance from home location
  float dist = distance(currentLat, currentLng, homeLat, homeLng);
  Serial.print("Distance from home: ");
  Serial.print(dist);
  Serial.println(" meters");

  // Check if the distance is beyond threshold
  if (dist > thresholdDistance) {
    // Send alert to API with home location
    sendAlert(currentLat, currentLng, homeLat, homeLng, 12345); // Replace "12345" with your actual user ID
    Serial.println("Alert sent!");
  }

  delay(10000); // 10-second delay
}
