#include <ESP8266WiFi.h>

const char* ssid = "test";
const char* password = "test2345";

int ledPin = D4; // GPIO13---D7 of NodeMCU
WiFiServer server(80);

int speedFactor = 1; // Default speed factor

// Morse code timing (will be adjusted by speedFactor)
int dotLength = 200;
int dashLength = 600;
int symbolSpace = 200;
int letterSpace = 600;
int wordSpace = 1400;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void updateTiming() {
  dotLength = 200 / speedFactor;
  dashLength = 600 / speedFactor;
  symbolSpace = 200 / speedFactor;
  letterSpace = 600 / speedFactor;
  wordSpace = 1400 / speedFactor;
}

// Morse code table for A-Z and 0-9
const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",  // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--..",                         // U-Z
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

int charToMorseIndex(char c) {
  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= '0' && c <= '9') return c - '0' + 26;
  return -1;
}

void flashMorseChar(const char* morseCode) {
  for (int i = 0; morseCode[i] != '\0'; i++) {
    if (morseCode[i] == '.') {
      digitalWrite(ledPin, HIGH);
      delay(dotLength);
      digitalWrite(ledPin, LOW);
    } else if (morseCode[i] == '-') {
      digitalWrite(ledPin, HIGH);
      delay(dashLength);
      digitalWrite(ledPin, LOW);
    }
    delay(symbolSpace);  // Space between symbols in the same letter
  }
}

void flashMorseText(String text) {
  for (int i = 0; i < text.length(); i++) {
    char c = toupper(text[i]);
    if (c == ' ') {
      delay(wordSpace);  // Space between words
    } else {
      int index = charToMorseIndex(c);
      if (index != -1) {
        flashMorseChar(morseTable[index]);
        delay(letterSpace);  // Space between letters
      }
    }
  }
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi successfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi, trying to reconnect...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Register event handlers
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  // Initialize WiFi
  initWiFi();

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while(!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  String morseText = "";
  if (request.indexOf("morseText=") != -1) {
    int startIndex = request.indexOf("morseText=") + 10;
    int endIndex = request.indexOf(" ", startIndex);
    morseText = request.substring(startIndex, endIndex);
    morseText.replace("%20", " ");  // Decode spaces
  }

  if (request.indexOf("speed=") != -1) {
    int startIndex = request.indexOf("speed=") + 6;
    int endIndex = request.indexOf(" ", startIndex);
    if (endIndex == -1) endIndex = request.length();
    String speedValue = request.substring(startIndex, endIndex);
    speedFactor = speedValue.toInt();
    if (speedFactor < 1) speedFactor = 1; // Prevent division by zero or negative values
    updateTiming();
  }

  // If morseText is not empty, convert to Morse code
  if (morseText.length() > 0) {
    flashMorseText(morseText);
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<form action=\"/\" method=\"GET\">");
  client.println("Enter text to convert to Morse code: <input type=\"text\" name=\"morseText\"><br>");
  client.println("Set speed (1-10): <input type=\"number\" name=\"speed\" min=\"1\" max=\"10\" value=\"1\"><br>");
  client.println("<input type=\"submit\" value=\"Convert\">");
  client.println("</form>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
}
