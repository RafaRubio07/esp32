#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "My_House_1";
const char* password = "rafa2210";
const char* server = "api.thingspeak.com";
const char* apiKey = "1N1X0EGTW2JQAAZI";

void setup(){
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado");
}

void loop(){
  float t = dht.readTemperature();

  if (isnan(t)){
    Serial.println("Falha ao ler o sensor");
    return;
  }
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    String url = String("http://api.thingspeak.com/update?api_key=") + apiKey + "&field1=" + String(t);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0){
      String payload = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(payload);
    } else{
      Serial.print("Erro na requisição: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Erro na conexão");
  }

  delay(15000);
}
