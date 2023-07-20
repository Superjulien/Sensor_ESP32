// Sensor ESP32
// by superjulien 
// > https://github.com/Superjulien
// > https://framagit.org/Superjulien
// V1.3

#include "ESP_Mail_Client.h"
#include "EasyBuzzer.h"
#include "DHT.h"
#include "WiFi.h"
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "time.h"

// ###################################################

#define SMTP_HOST "SMTP_HOST"
#define SMTP_PORT SMTP_PORT
#define AUTHOR_EMAIL "AUTHOR_EMAIL"
#define AUTHOR_PASSWORD "AUTHOR_PASSWORD"
#define RECIPIENT_NAME "RECIPIENT_NAME"
#define RECIPIENT_EMAIL "RECIPIENT_EMAIL"
const char* ssid = "ssid";
const char* password =  "password";
const char* ntpServer = "ntpServer";
const long  gmtOffset_sec = gmtOffset_sec;
const int   daylightOffset_sec = daylightOffset_sec;

// ###################################################

String version = "1.3";

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
SMTPSession smtp;
int but_restart = 19;
int but_restartState = 0;
int Gas_analog = 34;
int sound_digital = 33;
int Fire_analog = 32;
int led_red = 22;
int led_green = 21;
int led_yellow = 23;
int pin = 27;
int Sensor = 26; 
int alert_sound = 0;
int alert_fire = 0;
int alert_move = 0;
int alert_gas = 0;
int alert_temperature = 0;
int alert_humidi = 0;
int alert_heat = 0;
int tps;
float h;
float te;
float f;
float hif;
float hic;
int val_sound;
int val_fire;
int val_move;
int val_gas;
int start;
String alert;
String last_alert;
String timeN;
String last_time;
int mail_error = 0;
int mail_env = 0;
int mail_succ = 0;
int nmb_alarm = 15;
AsyncWebServer server(80);
#define wifi_max_try 10

void print_wifi_error(){
  switch(WiFi.status()) 
  {
    case WL_IDLE_STATUS : Serial.println("WL_IDLE_STATUS"); break;
    case WL_NO_SSID_AVAIL : Serial.println("WL_NO_SSID_AVAIL"); break;
    case WL_CONNECT_FAILED : Serial.println("WL_CONNECT_FAILED"); break;
    case WL_DISCONNECTED : Serial.println("WL_DISCONNECTED"); break;
    default : Serial.printf("No know WiFi error"); break;
  }
}

void wifi_conn() {
  WiFi.begin(ssid, password);
  Serial.println("############################");
  Serial.print("# Connecting");
  int count_try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count_try++;
    if ( count_try >= wifi_max_try ) {
      Serial.println("\n");
      Serial.println("# Impossible to establish WiFi connexion");
      print_wifi_error();
      count_try = 0;
      delay(100);
    }
  }
  Serial.println();
  Serial.print("# Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(but_restart, INPUT_PULLUP);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(sound_digital, INPUT);
  pinMode(Fire_analog, INPUT);
  pinMode(Sensor, INPUT);
  Serial.begin(115200);
  digitalWrite(led_yellow, HIGH);
  wifi_conn();
  digitalWrite(led_yellow, LOW);
  dht.begin();
  EasyBuzzer.setPin(pin);
  start = 1;
  Serial.print("# Version:");
  Serial.println(version);
  last_alert = "None";
  last_time = "None";
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void alarm_melody() {
  EasyBuzzer.beep(2850);
  delay(80);
  EasyBuzzer.stopBeep();
  delay(80);
  EasyBuzzer.beep(3600);
  delay(80);
  EasyBuzzer.stopBeep();
}

void alarm() {
  tps = nmb_alarm;
  while (tps != 0) {
    alarm_melody();
    tps = tps - 1;
  }
}

void handleRoot(AsyncWebServerRequest *request) {
   String val_fire_mess = String(val_fire);
   String val_gas_mess = String(val_gas);
   String val_move_mess = String(val_move);
   String val_sound_mess = String(val_sound);
   String val_temperature_mess = String(te);
   String val_humidi_mess = String(h); 
   String val_heat_mess = String(hic);
   String mail_error_mess = String(mail_error);
   String page = "<!DOCTYPE html>";
    page += "<html lang=en>";
    page += "";
    page += "<head>";
    page += "  <meta charset=UTF-8>";
    page += "  <meta name=viewport content=width=device-width, initial-scale=1.0>";
    page += "  <title>Sensor ESP32 - v" + version + "</title>";
    page += "  <link rel=stylesheet href=https://fonts.googleapis.com/css2?family=Roboto:wght@400;700&display=swap>";
    page += "  <style>";
    page += "    body {";
    page += "      font-family: 'Roboto', sans-serif;";
    page += "      background-color: #f2f2f2;";
    page += "      margin: 0px;";
    page += "      font-size: 22px;";
    page += "      padding: 0;";
    page += "      display: flex;";
    page += "      flex-direction: column;";
    page += "      align-items: center;";
    page += "      width: 100%;";
    page += "    }";
    page += "";
    page += "    h1 {";
    page += "      text-align: center;";
    page += "      color: #333;";
    page += "      font-size: 38px;";
    page += "      font-family: 'Roboto', sans-serif;";
    page += "    }";
    page += "";
    page += "    .logo {";
    page += "      text-align: center;";
    page += "      margin-bottom: 2px;";
    page += "      margin-top: 20px;";
    page += "    }";
    page += "";
    page += "    .tab { ";
    page += "       font-family: 'Roboto', sans-serif;";
    page += "       font-size: 16px;";     
    page += "       margin-left: 70px;";
    page += "    }";
    page += "";    
    page += "    .tabza { ";
    page += "       font-family: 'Roboto', sans-serif;";     
    page += "       margin-left: 80px;";
    page += "    }";
    page += ""; 
    page += "    .sensor-data {";
    page += "      background-color: #fff;";
    page += "      border-radius: 5px;";
    page += "      box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);";
    page += "      padding: 20px;";
    page += "      margin-bottom: 20px;";
    page += "    }";
    page += "";
    page += "    .sensor-data p {";
    page += "      margin: 0;";
    page += "      padding: 5px 0;";
    page += "    }";
    page += "";
    page += "    .alert {";
    page += "      font-size: 28px;";
    page += "      color: red;";
    page += "      font-weight: bold;";
    page += "    }";
    page += "";
    page += "    .sensor-info {";
    page += "      display: flex;";
    page += "      align-items: center;";
    page += "    }";
    page += "";
    page += "    .vers {";
    page += "      text-align: center;";
    page += "      font-size: 16px;";
    page += "      font-family: 'Roboto', sans-serif;";
    page += "    }";
    page += "";
    page += "    .sensor-info img {";
    page += "      margin-right: 10px;";
    page += "      background-color: transparent;";
    page += "    }";
    page += "    @media (max-width: 767px) {";
    page += "    body {";
    page += "      font-size: 15px;";
    page += "    }";
    page += "    h1 {";
    page += "      font-size: 25px;";
    page += "    }";
    page += "    .tab {";
    page += "      font-size: 13px;";
    page += "    }";
    page += "    .tabza {";
    page += "      margin-left: 35px;";
    page += "    }";
    page += "    .sensor-data p {";
    page += "      padding: 2px 0;";
    page += "    }";
    page += "    .alert {";
    page += "      font-size: 18px;";
    page += "    }";
    page += "    .vers {";
    page += "      font-size: 12px;";
    page += "    }";
    page += "    .sensor-info img {";
    page += "      margin-right: 6px;";
    page += "    }";
    page += "}";
    page += "  </style>";
    page += "</head>";
    page += "";
    page += "<body>";
    page += "  <div class=logo>";
    page += "    <img src=http://ttspl.co.in/img/fire-logo.jpg alt=Logo width=150>";
    page += "  </div>";
    page += "";
    page += "  <h1>Sensor ESP32<br></h1>";
    page += "  <div class=vers>";
    page += "  v" + version;
    page += "  </div>";
    page += "";
    page += "    <p></p>";
    page += "  <div class=sensor-data>";
    page += "    <p class=alert>Alert :  [ "+ alert + " ]</p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";    
    page += "      <p class=sensor-info>";
    page += "      <img src=https://icones.pro/wp-content/uploads/2021/03/symbole-de-l-horloge-rouge.png alt=Last width=50 style=background-color: transparent;>";
    page += "  Last alert : </p><p><div class=tabza>  [ "+ last_alert + " : " + last_time +" ]</div></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/317/PNG/512/calendar-icon_34471.png alt=Fire Sensor width=40 style=background-color: transparent;>";
    page += "      - Time : " +timeN;
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://external-content.duckduckgo.com/iu/?u=https%3A%2F%2Fstatic.vecteezy.com%2Fsystem%2Fresources%2Fpreviews%2F000%2F223%2F904%2Foriginal%2Ffire-alarm-icon-isolated-illustration-vector.jpg&f=1&nofb=1&ipt=4fc8599ddd53145105d3368b4af8568ef4a721c53ad3889e4cc09038487221f5&ipo=images alt=Fire Sensor width=40 style=background-color: transparent;>";
    page += "      - Fire sensor :  "+ val_fire_mess + " / 1000 </p><div class=tab><p>Number of alerts : "+ alert_fire +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/2265/PNG/512/gas_mask_icon_140415.png alt=Gas Sensor width=40 style=background-color: transparent;>";
    page += "      - Gas sensor :  "+ val_gas_mess + " / 1500 </p><div class=tab><p>Number of alerts : "+ alert_gas +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/957/PNG/128/sound_icon-icons.com_74449.png alt=Sound Sensor width=40 style=background-color: transparent;>";
    page += "      - Sound sensor :  "+ val_sound_mess + " / 1 </p><div class=tab><p>Number of alerts : "+ alert_sound +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/1603/PNG/512/real-setate-house-home-apartment-earthquake_108646.png alt=Move Sensor width=40 style=background-color: transparent;>";
    page += "      - Move sensor :  "+ val_move_mess + " / 1 </p><div class=tab><p>Number of alerts : "+ alert_move +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/3191/PNG/512/temperature_thermometer_cold_warm_hot_icon_194240.png alt=Temperature Sensor width=40 style=background-color: transparent;>";
    page += "      - Temperature sensor :  "+ val_temperature_mess + " °C </p><div class=tab><p>Number of alerts : "+ alert_temperature +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/3609/PNG/512/temperature_climate_forecast_weather_rain_water_humidity_icon_226614.png alt=Humidity Sensor width=40 style=background-color: transparent;>";
    page += "      - Humidity sensor :  "+ val_humidi_mess + " % </p><div class=tab><p>Number of alerts : "+ alert_humidi +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/159/PNG/256/sensor_thermometer_22332.png alt=Heat Sensor width=40 style=background-color: transparent;>";
    page += "      - Heat sensor :  "+ val_heat_mess + " °C </p><div class=tab><p>Number of alerts : "+ alert_heat +"</p></div>";
    page += "    </p>";
    page += "    <p></p>";
    page += "    <p class=sensor-info>";
    page += "      <img src=https://cdn.icon-icons.com/icons2/944/PNG/512/medical-25_icon-icons.com_73900.png alt=Mail width=40 style=background-color: transparent;>";
    page += "      - Mail error :  "+ mail_error_mess;
    page += "    </p>";
    page += "    <p></p>";
    page += "  </div>";
    page += "</body>";
    page += "";
    page += "</html>";
   request->send(200, "text/html", page);
}

void start_server(){
  if (start == 1){
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_red, HIGH);
    delay(100);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      handleRoot(request);
    });
    server.onNotFound([](AsyncWebServerRequest *request){
      request->send(404, "text/plain", "404: Not found");
    });
    server.begin();
    Serial.println("# Active web server");
    Serial.println("############################");
    Serial.println("");
    start = 0 ;
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_red, LOW);
  }
}

void send_email() {
  digitalWrite(led_yellow, HIGH);
  mail_env = mail_env + 1;
  ESP_Mail_Session session;
  char contenu_message[100];
  SMTP_Message message;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  message.sender.name = "ESP32";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "WARNING-ESP32 : "+alert+" !!";
  message.addRecipient(RECIPIENT_NAME, RECIPIENT_EMAIL);
  String val_fire_mess = String(val_fire);
  String val_gas_mess = String(val_gas);
  String val_move_mess = String(val_move);
  String val_sound_mess = String(val_sound);
  String val_temperature_mess = String(te);
  String val_humidi_mess = String(h); 
  String val_heat_mess = String(hic);
  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>ESP32 - WARNING !</h1><p>v" + version + "<p><h2>ALERT : [ " + alert + " ]</h2><p><h2><p> ESP32 information :</h2><p> Time : "+ timeN + "<p> Fire sensor : "+ val_fire_mess + "<p> Gas sensor : "+ val_gas_mess +"<p> Sound sensor : "+ val_sound_mess + "<p> Move sensor : "+ val_move_mess +"<p>""<p> Temperature sensor : "+ val_temperature_mess +"<p>""<p> Humidity sensor : "+ val_humidi_mess +"<p>""<p> Heat sensor : "+ val_heat_mess +"<p>";
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_high;
  if (!smtp.connect(&session))
    return;
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error while sending the email, " + smtp.errorReason());
}

void smtpCallback(SMTP_Status status)
{
  Serial.println(status.info());
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    mail_succ =  status.completedCount();
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
  }
}

void warn() {
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, HIGH);
  alarm();
  send_email();
  mail_error = mail_env - mail_succ;
  digitalWrite(led_yellow, LOW);
  delay(1000);
}

void detecteur() {
  alert = "None";
  digitalWrite(led_green, HIGH);
  digitalWrite(led_red, LOW);
  delay(100);
  h = dht.readHumidity();
  te = dht.readTemperature();
  f = dht.readTemperature(true);
  if (isnan(h) || isnan(te) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  hif = dht.computeHeatIndex(f, h);
  hic = dht.computeHeatIndex(te, h, false);
  val_sound = digitalRead(sound_digital);
  val_fire = analogRead(Fire_analog);
  val_move = digitalRead (Sensor);
  val_gas = analogRead(Gas_analog);
  Serial.print("Sound :");
  Serial.println(val_sound);
  Serial.print("Fire :");
  Serial.println(val_fire); 
  Serial.print("Move :");
  Serial.println(val_move);
  Serial.print("Gas :");
  Serial.println(val_gas);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(te);
  Serial.print(F("°C  Heat index: "));
  Serial.print(hic);
  Serial.println(F("°C "));
  if (val_sound == HIGH) {
    Serial.println("Warning : Sound");
    alert = "Sound";
    last_alert = alert;
    last_time = timeN;
    alert_sound = alert_sound+1;
    warn();
  }
  if (val_move == HIGH) {
    Serial.println("Warning : Move");
    alert = "Move";
    last_alert = alert;
    last_time = timeN;
    alert_move = alert_move+1;
    warn();
  }
  if (val_fire > 1000) {
    Serial.println("Warning : Fire");
    alert = "Fire";
    last_alert = alert;
    last_time = timeN;
    alert_fire = alert_fire+1;
    warn();
  }
  if (val_gas > 1500) {
    Serial.println("Warning : Gas");
    alert = "Gas";
    last_alert = alert;
    last_time = timeN;
    alert_gas = alert_gas+1;
    warn();
  }
  if (h > 70) {
    Serial.println("Warning : Humidity");
    alert = "Humidity";
    last_alert = alert;
    last_time = timeN;
    alert_humidi = alert_humidi+1;
    warn();
  }
  if (te > 40 || te < 10) {
    Serial.println("Warning : Temperature");
    alert = "Temperature";
    last_alert = alert;
    last_time = timeN;
    alert_temperature = alert_temperature+1;
    warn();
  }
  if (hic > 40 || hic < 10) {
    Serial.println("Warning : Heat index");
    alert = "Heat";
    last_alert = alert;
    last_time = timeN;
    alert_heat = alert_heat+1;
    warn();
  }
}

void btn_state(){
  but_restartState = digitalRead(but_restart);
  if (but_restartState == LOW) {
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_red, LOW);
    digitalWrite(led_green, LOW);
    delay(100);
    ESP.restart();
  }
}

void loop() {
  time_t now = time(nullptr);
  struct tm* timeinfo;
  timeinfo = localtime(&now);
  timeN = asctime(timeinfo);
  start_server();
  Serial.print("Time : ");
  Serial.println(timeN);
  btn_state();
  detecteur();
  smtp.callback(smtpCallback);
}
