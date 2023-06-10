#include "Fs.h" // Adds menu item to Tools menu
#include <Wire.h> // Allows to communicate with I2c devices
#include "MAX30100_PulseOximeter.h" // Used to include PulseOximeter Library
#define CAYENNE_PRINT Serial // Defining Cayenne_print as serial so that we can get values in cayenne IOT dircetly using WI-FI 
#include <CayenneMQTTESP8266.h> // Including Cayenne Library and Its Functions
#include <ESP8266WiFi.h> // Including the library for nodemcu_esp8266 - wifi connection.

#define REPORTING_PERIOD_MS     1000 // Setting Minimum time to record as 1000

char ssid[] = "IITRPR"; // Your WiFi credentials.
char pass[] = "V#6qF?pyM!bQ$%NX"; // Your wifi Password
char username[] = "bc669600-7177-11ed-b193-d9789b2af62b"; // Cayenne username
char password[] = "57d68fe68c238452911c6d564e2e6ada97e13c28"; // Cayenne password
char clientID[] = "cdf02df0-7177-11ed-8d53-d7cd1025126a"; // Cayenne Auth Token

PulseOximeter pox; // Create a PulseOximeter object
float bpm = 0,spo2 = 0; // Initiating bpm and spo2 as 0
uint32_t tsLastReport = 0; // Time at which the last beat occurred

void onBeatDetected() 
{
    Serial.println("♥ Beat!"); // Callback routine is executed when a pulse is detected
}

void setup() 
{
    Serial.begin(115200); // Setting Baud_Rate to 115200
    Cayenne.begin(username,password,clientID,ssid,pass);  // Setting up cayenne cloud with username,passowrd,clientID,ssid,pass
    Serial.print("Initializing pulse oximeter..");
    // Initialize sensor
    if (!pox.begin()) // If sensor hasn't started working
    {
        Serial.println("FAILED");// Print failed
        for(;;);
    } 
    else 
    {
        Serial.println("SUCCESS");// Print Success
    }
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);  // Configure sensor to use 7.6mA for LED drive
    pox.setOnBeatDetectedCallback(onBeatDetected); // Register a callback routine
}

void loop() 
{
    pox.update(); // Read from the sensor
    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) // If time is greater than our minimum required time to Read value
    {
        Serial.print("Heart rate:");
        bpm = pox.getHeartRate(); // Get value of bpm in bpm 
        spo2 = pox.getSpO2(); // get value of spo2 in spo2
        Serial.print(pox.getHeartRate()); // Printing Value of heart beat (bpm)
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2()); // Printing value of spo2(spo2)
        Serial.println("%");

        Cayenne.virtualWrite(0,bpm,"counter","p"); // Writing the value of bpm in cayenne suitable widget
        Cayenne.virtualWrite(1,spo2,"O2","p"); // Writing the value of spo2 in cayenne suitable widget
        tsLastReport = millis(); // Storing current time in tsLastReport
    }
}
