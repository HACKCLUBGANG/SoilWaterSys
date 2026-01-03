#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASS";

int sensorPin = A0;
int threshold = 400;

// Twilio WhatsApp
String accountSID = "YOUR_SID";
String authToken = "YOUR_TOKEN";
String fromNumber = "whatsapp:+14155238886";
String toNumber = "whatsapp:+YOUR_NUMBER";

void setup() {
  Serial.begin(115200);
  int soilValue = analogRead(sensorPin);
  Serial.println(soilValue);

  if (soilValue < threshold) {
    connectWiFi();
    sendWhatsApp("Alert! Plant soil is dry.");
  }

  // Go to deep sleep for 15 minutes (900,000,000 microseconds)
  esp_deep_sleep_start();
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    attempts++;
  }
}

void sendWhatsApp(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.twilio.com/2010-04-01/Accounts/" + accountSID + "/Messages.json";
    String postData = "From=" + fromNumber + "&To=" + toNumber + "&Body=" + message;

    http.begin(url);
    http.setAuthorization(accountSID.c_str(), authToken.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) Serial.println("WhatsApp alert sent!");
    else Serial.println("Error sending message");

    http.end();
  }
}
