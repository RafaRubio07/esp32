#include <wifi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 
#define DHTTYPE DHT 

//Configuração Wi-Fi
const char* ssid = "";
const char* password ="";

//conexão ThingSpeak
const char* serverUrl = "https://www.thingspeak.mathwors.com/channels/2954614";
const char* apiKey = "YBYGJDXWAISSTTNZ";

DHT dht (DHTPIN, DHTTYPE);


void setup() {
  Serial.begin ();
  dht.begin();

  //conectando no Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-fi");
  while (WiFi.Status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado!");
}

void loop() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Erro ao ler sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperatura: ");
  Serial.println(temperature);

  if (Wifi.status()== WL_CONNECTED){
    HTTPClient http;
    String url = String(serverUrl) + "?api_key=" + apikey + "&field=" + String(temperature);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0){
      Serial.println("Dados enviados com sucesso");
      Serial.println(http.getString());
    } else{
      Serial.println("Erro ao enviar os dados");
    }

    http.end();
  } else {
    Serial.println("Wi-Fi desconectado");
  }

  delay(15000); //Atualiza a cada minuto
}
