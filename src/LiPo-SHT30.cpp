#include <Homie.h>
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);

const unsigned long TEMPERATURE_INTERVAL = 60000;
const unsigned long SLEEP_TIME = 60001;

unsigned long lastTemperatureSent = 0;


HomieNode temperatureNode("temp", "ESH:Temperature");
HomieNode humidityNode("hum", "ESH:Humidity");
HomieNode vccNode("vcc", "ESH:Battery");


void setupHandler() {
  temperatureNode.setProperty("unit").send("C");
  humidityNode.setProperty("unit").send("%");
}

void loopHandler() {
  if ((millis() - lastTemperatureSent) >= TEMPERATURE_INTERVAL || lastTemperatureSent == 0) {
    Homie.getLogger() << TEMPERATURE_INTERVAL;
    Homie.getLogger() << lastTemperatureSent;
    sht30.get();
    float temperature = sht30.cTemp;; // Fake temperature here, for the example
    Homie.getLogger() << "Temperature: " << temperature << " °C" << endl;
    temperatureNode.setProperty("degrees").send(String(temperature));
    float humidity = sht30.humidity;; // Fake temperature here, for the example
    Serial.print(humidity);
    Homie.getLogger() << "Humidity: " << humidity << " %" << endl;
    humidityNode.setProperty("percent").send(String(humidity));
    lastTemperatureSent = millis();
    Homie.prepareToSleep();
    ESP.deepSleep(SLEEP_TIME);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Homie_setFirmware("lipo-sht30", "1.0.0");

  temperatureNode.advertise("unit");
  temperatureNode.advertise("degrees");

  humidityNode.advertise("unit");
  humidityNode.advertise("percent");

  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}


void loop() {
  Homie.loop();
}
