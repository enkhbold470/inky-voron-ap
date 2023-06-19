/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://192.168.4.1/H to turn the LED on or http://192.168.4.1/L to turn it off
     OR
     Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with 192.168.4.1 as IP address and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 2  // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.
const char *ssid = "inky-voron";
const char *password = "123!@#qwe";

WiFiServer server(80);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            // client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            // client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");
            String html = "<!DOCTYPE html>";
            html += "<html>";
            html += "<head>";
            html += "<title>Welcome to ESP32 Server</title>";
            html += "</head>";
            html += "<body>";
            html += "<h1>Welcome to ESP32 Server</h1>";
            html += "<p>This is a simple introduction page served by an ESP32 server.</p>";
            html += "<p>You can use this server to host web applications and interact with various sensors and actuators.</p>";
            html += "<h2>Getting Started</h2>";
            html += "<ol>";
            html += "<li>Connect to the ESP32 server by connecting to its Wi-Fi network.</li>";
            html += "<li>Open a web browser and enter the IP address of the server.</li>";
            html += "<li>You will see this introduction page, indicating that you are connected to the server.</li>";
            html += "<li>From here, you can navigate to different pages or interact with the server's functionality.</li>";
            html += "</ol>";
            html += "<h2>Server Information</h2>";
            html += "<p>The server is running on an ESP32 microcontroller, which is a powerful embedded platform.</p>";
            html += "<p>It is programmed using the C++ language and the Arduino framework.</p>";
            html += "<p>The ESP32 server can handle client requests and provide responses accordingly.</p>";
            html += "<h2>Next Steps</h2>";
            html += "<p>Now that you are connected to the server, you can start building your own web applications.</p>";
            html += "<p>Feel free to explore the provided examples and experiment with different functionalities.</p>";
            html += "</body>";
            html += "</html>";


            client.print(html);
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);  // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);  // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
