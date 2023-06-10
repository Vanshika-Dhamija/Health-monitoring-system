const int GSR= A0; // Setting A0 as the pin for connecting GSR
int sensorValue=0; // Initiating the sensor value to zero
int gsr_average=0; // Initiating the gsr_average to zero
#define CAYENNE_PRINT Serial // Defining Cayenne_print as serial so that we can get values in cayenne IOT dircetly using WI-FI
#include <CayenneMQTTESP8266.h> // Including Libraries related to Cayenne
#include <ESP8266WiFi.h> // Including the library for nodemcu_esp8266 - wifi connection.

char ssid[] = "IITRPR"; // Your WiFi credentials.
char pass[] = "V#6qF?pyM!bQ$%NX"; // Your wifi Password
char username[] = "bc669600-7177-11ed-b193-d9789b2af62b"; // Cayenne username   
char password[] = "57d68fe68c238452911c6d564e2e6ada97e13c28";// Cayenne password
char clientID[] = "c1c6cf40-718e-11ed-8d53-d7cd1025126a";// Cayenne Auth Token
uint32_t tsLastReport = 0;
float human_resistance=0; // Initiating human_resistance to zero

void setup()
{
    Serial.begin(9600); // Setting Baud_Rate to 9600
    Cayenne.begin(username,password,clientID,ssid,pass); // Setting up cayenne cloud with username,passowrd,clientID,ssid,pass
}

void loop()
{
        long sum=0;// Initiating sum to zero after each loop
        for(int i=0;i<10;i++) // Average the 10 measurements to remove the variations and have a steady value 
        {
            sensorValue=analogRead(GSR);// Reading the value from GSR into sensorvalue variable
            //Serial.println(sensorValue);
            sum += sensorValue;// Adding the current sensor value to sum
            delay(5);// Delay of 5ms between reading values
        }
        sensorValue = analogRead(A0);// Reading value form A0 into sensor value
        gsr_average = sum/10;// Getting the average over 10 measurements
        gsr_average = gsr_average; 
        human_resistance = ((1024+2*gsr_average)*10000)/(512-gsr_average);// Applying the relation between human resistance and gsr_average value
        
        Serial.print("GSR VALUE- ");
        Serial.print(gsr_average);// Printing the gsr average
        Serial.println("  V");
        Serial.print("Human resistance using gsr value- ");// Printing the Human Resistance
        Serial.print(human_resistance);
        Serial.println(" Ohm");
        Cayenne.virtualWrite(0,gsr_average,"analog_sensor","null");// Writing the value of gsr in cayenne suitable widget
        Cayenne.virtualWrite(1,human_resistance,"counter","null");// Writing the value of human_resistance in cayenne suitable widget
        delay(1000);// Adding delay of 1000 between outputs
}
