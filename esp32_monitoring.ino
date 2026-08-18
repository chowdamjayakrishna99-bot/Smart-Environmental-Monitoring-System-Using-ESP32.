#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// -------------------- Pin Definitions --------------------
#define DHTPIN      4
#define DHTTYPE     DHT22
#define SOIL_PIN    34
#define LED_PIN     2

// -------------------- Objects --------------------
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 oled(128, 64, &Wire, -1);

// -------------------- Setup --------------------
void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  Wire.begin(21,22);

  lcd.init();
  lcd.backlight();

  if(!oled.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    Serial.println("OLED not found");
    while(true);
  }

  oled.clearDisplay();
  oled.display();

  lcd.setCursor(0,0);
  lcd.print("SMART FARM");

  lcd.setCursor(0,1);
  lcd.print("SYSTEM READY");

  delay(2000);
}

// -------------------- Loop --------------------
void loop() {

  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  int soilRaw = analogRead(SOIL_PIN);

  int soilPercent = map(soilRaw,4095,0,0,100);
  soilPercent = constrain(soilPercent,0,100);

  // LED indication
  if(soilPercent < 30){
    digitalWrite(LED_PIN,HIGH);
  }
  else{
    digitalWrite(LED_PIN,LOW);
  }

  // -------- LCD --------
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temperature,1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(humidity,0);
  lcd.print("%");

  // -------- OLED --------
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  oled.setCursor(0,0);
  oled.println("Smart Env Monitor");

  oled.setCursor(0,18);
  oled.print("Temp: ");
  oled.print(temperature,1);
  oled.println(" C");

  oled.setCursor(0,32);
  oled.print("Hum : ");
  oled.print(humidity,0);
  oled.println(" %");

  oled.setCursor(0,46);
  oled.print("Soil: ");
  oled.print(soilPercent);
  oled.println(" %");

  oled.display();

  // -------- Serial Monitor --------
  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil Moist. : ");
  Serial.print(soilPercent);
  Serial.println(" %");

  Serial.println("-----------------------");

  delay(2000);
}