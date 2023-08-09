# Sensor_ESP32

[![Version](https://img.shields.io/badge/Version-1.3-blue.svg)](https://github.com/Superjulien/Sensor_ESP32) [![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://choosealicense.com/licenses/mit/) [![ESP32](https://img.shields.io/badge/Microcontrollers-ESP32-blue.svg)](https://en.wikipedia.org/wiki/ESP32) [![Arduino](https://img.shields.io/badge/Arduino-9D0600?&logo=Arduino&logoColor=white.svg)](https://www.arduino.cc/) [![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=c%2B%2B&logoColor=white.svg)](https://en.wikipedia.org/wiki/C++)

This script is for an ESP32 sensor system that monitors environmental parameters like temperature, humidity, sound, gas, fire, and motion. It uses various sensors and connects to WiFi to send email alerts and display real-time data on a web server.

## Features

The provided script is designed to be used with an ESP32 board to create a sensor detection device. It utilizes multiple sensors including the DHT22 temperature and humidity sensor, sound sensor, fire sensor, gas sensor, and motion sensor to monitor different environmental aspects. Based on sensor readings, the device can trigger alerts and send email notifications.

Here is a brief overview of the main features of the script:

- Wi-Fi Connection: The script establishes a Wi-Fi connection using the provided SSID and password.

- Web Server: An embedded web server is created to display real-time sensor data. Accessing the ESP32's IP address in a web browser allows viewing the sensor information.

- Sensors: The script utilizes the ESP_Mail_Client, EasyBuzzer, and DHT libraries to interface with the sensors. GPIO pins on the ESP32 are utilized to read sensor values.

- Alerts: If sensor values exceed predefined thresholds, alerts are triggered. LEDs (green, yellow, red) and a buzzer indicate alerts. Additionally, email notifications are sent to inform the user of detected alerts.

- Alert History: The most recent alerts and their timestamps are recorded and displayed on the web server.

- Automatic time synchronization using an NTP server.

- Restart function using a physical button.

- Fahrenheit-Celsius Conversion: By making the necessary modifications, temperature values can be displayed in Fahrenheit instead of Celsius.

Make sure to customize parameters such as Wi-Fi SSID and password, SMTP server address, sender and recipient email addresses, as well as other sensor thresholds according to your requirements.

## Required Libraries

This project relies on the following libraries:

- [ESP_Mail_Client](https://github.com/mobizt/ESP-Mail-Client): Enables sending emails from the ESP32 using the SMTP protocol.

- [EasyBuzzer](https://github.com/evert-arias/EasyBuzzer): Facilitates controlling a buzzer with the ESP32, providing functions for generating sounds.

- [DHT](https://github.com/adafruit/DHT-sensor-library): Allows interacting with the DHT22 temperature and humidity sensor, offering functions for reading sensor data.

- WiFi: Enables the ESP32 to connect to a Wi-Fi network, providing functions for Wi-Fi configuration and management.

- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP): Provides an asynchronous implementation of the TCP protocol for the ESP32, utilized in conjunction with ESPAsyncWebServer.

- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer): Facilitates the creation of an asynchronous web server on the ESP32, offering functions for handling HTTP requests and sending responses.

## Electronic components:

- 1 ESP32 board
- 1 push button (but_restart)
- 3 LEDs (led_red, led_green, led_yellow)
- 1 sound sensor (sound_digital)
- 1 fire sensor (Fire_analog)
- 1 motion sensor (Sensor)
- 1 gas sensor (Gas_analog)
- 1 DHT22 temperature and humidity sensor (DHTPIN, DHTTYPE)
- 1 buzzer (pin)
- 4 resistors 10k ohms
- 5 resistors 220 ohms

## Sensor Connection

- DHT22 Sensor:
  
 Data Pin: Connect the data pin of the DHT22 sensor (pin 4) to a digital pin of the ESP32. Also, make sure to connect a 10k ohm resistor between the data pin and the 3.3V of the ESP32.
 
- Sound Sensor:
  
Signal Pin: Connect the signal pin of the sound sensor (pin 33) to a digital pin of the ESP32. To protect the sensor and prevent interference, you can connect a 220-ohm resistor in series with the signal pin.
  
- Fire Sensor:
  
Signal Pin: Connect the signal pin of the fire sensor (pin 32) to an analog pin of the ESP32. To limit the current passing through the sensor, connect a 10k ohm resistor between the signal pin and the 3.3V of the ESP32.

- Gas Sensor:
  
Signal Pin: Connect the signal pin of the gas sensor (pin 34) to an analog pin of the ESP32. To limit the current passing through the sensor, connect a 10k ohm resistor between the signal pin and the 3.3V of the ESP32.
  
- Motion Sensor:

Signal Pin: Connect the signal pin of the motion sensor (pin 26) to a digital pin of the ESP32. To avoid pin floating issues, you can connect a 10k ohm resistor between the signal pin and the 3.3V of the ESP32.

- LED:
  
Green LED: Connect the anode of the green LED (pin 21) to a digital pin of the ESP32. To limit the current passing through the LED, connect a 220-ohm resistor in series with the LED.

Yellow LED: Connect the anode of the yellow LED (pin 23) to a digital pin of the ESP32. To limit the current passing through the LED, connect a 220-ohm resistor in series with the LED.

Red LED: Connect the anode of the red LED (pin 22) to a digital pin of the ESP32. To limit the current passing through the LED, connect a 220-ohm resistor in series with the LED.

- Buzzer:
  
Connect the control pin of the buzzer (pin 27) to a digital pin of the ESP32. To limit the current passing through the buzzer, connect a 220-ohm resistor in series with the control pin.

- Push button:

Connect the control pin of the push button (pin 19) to a digital pin of the ESP32.

## Configuration

Before uploading the code to your ESP32, make the following modifications:

Replace the following values with the appropriate information:
    
- SMTP_HOST: Replace with the hostname of the SMTP server.
        
- SMTP_PORT: Replace with the SMTP server port.
        
- AUTHOR_EMAIL: Replace with the sender's email address.
        
- AUTHOR_PASSWORD: Replace with the sender's email account password.
        
- RECIPIENT_NAME: Replace with the recipient's name.

- RECIPIENT_EMAIL: Replace with the recipient's email address.

- ssid: Replace with your Wi-Fi network SSID.
    
- password: Replace with your Wi-Fi network password.
    
- ntpServer: Replace with the NTP server hostname.
    
- gmtOffset_sec: Replace with the GMT offset in seconds.
    
 - daylightOffset_sec: Replace with the daylight saving offset in seconds. 

 ### Adjusting Alert Thresholds in `void detecteur()`

To customize the alert thresholds in the `void detecteur()` function of this code, follow these steps to modify the threshold values for specific sensors such as fire, gas, temperature, humidity, and heat index:

1. **Fire Sensor Threshold**:
   
   Locate the section in the code that checks the fire sensor reading:

   ```cpp
   if (val_fire > 1000) {
       // Fire alert condition
   }
   ```

   Adjust the `1000` value to your desired threshold. This value represents the analog reading from the fire sensor above which an alert is triggered. Modify it based on the sensitivity of your fire sensor and the conditions you want to monitor.

2. **Gas Sensor Threshold**:

   Find the section that monitors the gas sensor reading:

   ```cpp
   if (val_gas > 1500) {
       // Gas alert condition
   }
   ```

   Change the `1500` value to the desired gas sensor threshold. This value represents the analog reading beyond which a gas alert is triggered. Adjust it according to the gas levels you want to detect.

3. **Temperature Threshold**:

   Locate the temperature sensor check:

   ```cpp
   if (te > 40 || te < 10) {
       // Temperature alert condition
   }
   ```

   Modify the `40` and `10` values to set the upper and lower temperature thresholds. These values define the temperature range within which no alert is triggered. Adjust them to suit your temperature monitoring requirements.

4. **Humidity Threshold**:

   Find the humidity sensor condition:

   ```cpp
   if (h > 70) {
       // Humidity alert condition
   }
   ```

   Change the `70` value to set the humidity threshold above which an alert is triggered. This value represents the percentage of humidity. Adjust it based on your desired humidity monitoring level.

5. **Heat Index Threshold**:

   Locate the heat index sensor check:

   ```cpp
   if (hic > 40 || hic < 10) {
       // Heat index alert condition
   }
   ```

   Modify the `40` and `10` values to set the upper and lower heat index thresholds. These values determine the acceptable range for the heat index before an alert is triggered. Adjust them as needed for accurate heat index monitoring.

Be sure to consider the sensor characteristics, environmental conditions, and your monitoring goals while setting the new thresholds. Continuous testing and calibration may be necessary to achieve optimal alerting performance.

### Customizable Functions

You can customize the behavior of the script by modifying the following functions:

- alarm_melody: Defines the alarm melody played when alerts are triggered. Modify the frequencies and delays to customize the melody.

- handleRoot: Defines the HTML response sent by the web server. Modify the HTML content to customize the appearance and displayed information.

- send_email: Called when an alert is triggered to send an email notification. Customize the email content by modifying the htmlMsg variable.

### Celsius to Fahrenheit Conversion

To convert temperature from Celsius to Fahrenheit in the script, make the following modifications:

In the detecteur function, change the line 
```
te = dht.readTemperature();
```
to 
```
f = dht.readTemperature(true);
```


Change the line 
```
String val_temperature_mess = String(te);
```
to 
```
String val_temperature_mess = String(f);
```


In the handleRoot function, change the line
```
<p> Temperature sensor: "+ val_temperature_mess + " °C </p>
```
 to
```
 <p> Temperature sensor: "+ val_temperature_mess + " °F </p>
```


This will convert temperature readings from the DHT22 sensor to Fahrenheit and update the displayed values on the web page accordingly.

Remember to adjust the alert trigger thresholds accordingly if you switch from Celsius to Fahrenheit.

## Uploading the Code

 - Connect your ESP32 board to your computer using a USB cable.
 - Select your ESP32 board in the Arduino IDE.
 - Click the "Upload" button to compile and upload the code to your ESP32.

Ensure that your ESP32 is properly connected and the correct serial port is selected in the Arduino IDE before uploading the code.

That's it! Your ESP32 Sensor Device is now ready to be used. You can view sensor data and receive email alerts through the integrated web server.

## Usage

- Open the serial monitor to view the debug information and sensor readings.
- Connect to the WiFi network specified in the code.
- Access the web interface by entering the ESP32's IP address in a web browser.
- The web interface displays the sensor readings and alerts.
- If an alert is triggered, an email will be sent to the specified recipient.

### Web Interface

The web interface provides information about the sensor readings and alerts. It includes the following sections:

- Alert: Shows the current active alert, if any.
- Last alert: Displays the last triggered alert along with the timestamp.
- ESP32 information: Shows the readings from each sensor.
- Mail error: Displays the number of email sending errors, if any.

## Sponsoring

This software is provided to you free of charge, with the hope that if you find it valuable, you'll consider making a donation to a charitable organization of your choice :

- SPA (Society for the Protection of Animals): The SPA is one of the oldest and most recognized organizations in France for the protection of domestic animals. It provides shelters, veterinary care, and works towards responsible adoption.

  [![SPA](https://img.shields.io/badge/Sponsoring-SPA-red.svg)](https://www.la-spa.fr/)

- French Popular Aid: This organization aims to fight against poverty and exclusion by providing food aid, clothing, and organizing recreational activities for disadvantaged individuals.

  [![SPF](https://img.shields.io/badge/Sponsoring-Secours%20Populaire%20Français-red.svg)](https://www.secourspopulaire.fr)

- Doctors Without Borders (MSF): MSF provides emergency medical assistance to populations in danger around the world, particularly in conflict zones and humanitarian crises.

  [![MSF](https://img.shields.io/badge/Sponsoring-Médecins%20Sans%20Frontières-red.svg)](https://www.msf.fr)

- Restaurants of the Heart : Restaurants of the Heart provides meals, emergency accommodation, and social services to the underprivileged.

  [![RDC](https://img.shields.io/badge/Sponsoring-Restaurants%20du%20Cœur-red.svg)](https://www.restosducoeur.org)

- French Red Cross: The Red Cross offers humanitarian aid, emergency relief, first aid training, as well as social and medical activities for vulnerable individuals.

   [![CRF](https://img.shields.io/badge/Sponsoring-Croix%20Rouge%20Française-red.svg)](https://www.croix-rouge.fr)

Every small gesture matters and contributes to making a real difference.

## Support

For support email : 

[![Gmail: superjulien](https://img.shields.io/badge/Gmail-Contact%20Me-purple.svg)](mailto:contact.superjulien@gmail.com) [![Tutanota: superjulien](https://img.shields.io/badge/Tutanota-Contact%20Me-green.svg)](mailto:contacts.superjulien@tutanota.com)

## License

This project is licensed under the MIT License. Please refer to the LICENSE file for more information.
