#include <WiFi.h>
#include <DHT.h>
#include <ESP32Servo.h>

const char* ssid = "MicheliniDeco";
const char* password = "michelini1910";

WiFiServer server(80);

//Servo Motor
Servo motorgaragem;
int posicaoAtual = 0; // Servo começa fechado

// DHT11
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ-2
#define GasD 26
#define Buzzer 25

//LDR
#define LDR 34

//PIR
#define PIR_SENSOR 32
bool sensorPresencaAtivo = false;
bool presencaDetectada = false;

// Piso Inferior
const int luzSala = 2;
const int luzGaragem = 4;
const int luzWcInferior = 5;
const int luzCozinha = 18;
const int luzServico = 19;

// Piso Superior
const int luzSuiteJr = 21;
const int luzWcJr = 22;
const int luzMaster = 23;
const int luzWcMaster = 13;
const int luzCloset = 12;
const int luzFachada = 14;

bool estadosLuzes[10] = { false };

const int pinosLuzes[] = {
  luzSala, luzGaragem, luzWcInferior, luzCozinha, luzServico,
  luzSuiteJr, luzWcJr, luzMaster, luzWcMaster, luzCloset
};

const char* nomesLuzes[] = {
  "Sala", "Garagem", "WC Inferior", "Cozinha", "Área Serviço",
  "Suíte Jr.", "WC Jr.", "Suíte Master", "WC Master", "Closet"
};

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(GasD, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(luzFachada, OUTPUT);
  pinMode(PIR_SENSOR, INPUT);
  // Anexa o servo ao pino 33 e move para 0 graus
  motorgaragem.attach(15); //anexa o servo ao pino 15 do ESP
  motorgaragem.write(180); // Começa fechado
  motorgaragem.detach(); //desanexa o servo ao pino 15 do ESP para evitar interferências

  for (int i = 0; i < 10; i++) {
    pinMode(pinosLuzes[i], OUTPUT);
    digitalWrite(pinosLuzes[i], LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");

  unsigned long inicioTentativa = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicioTentativa < 15000) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    server.begin();
  } else {
    Serial.println("\nErro ao conectar ao Wi-Fi.");
    // Aqui você pode reiniciar ou entrar em modo AP, se quiser
  }
}

void loop() {
  WiFiClient client = server.available();
  int ValorGas = digitalRead(GasD);

  //Verifica valors do Sensor se presença PIR
  if (sensorPresencaAtivo) {
    presencaDetectada = digitalRead(PIR_SENSOR);
    if (presencaDetectada) {
      Serial.println("PRESENÇA DETECTADA!");
      tone(Buzzer, 1500);
      delay(500);
      tone(Buzzer, 1500);
      delay(500);
      tone(Buzzer, 1500);
      delay(500);
      tone(Buzzer, 1500);
      delay(500);
      noTone(Buzzer);
    } else {
      Serial.println("Sensor de Presença: Ativo (Sem presença)");
    }
  } else {
    presencaDetectada = false;
  }

  // Verifica o valor do LDR e comando da luz da fachada/Jardin
  int ValorLDR = analogRead(LDR);
  //Serial.println(ValorLDR);
  bool statusFachada = ValorLDR < 300;
  digitalWrite(luzFachada, statusFachada ? HIGH : LOW);
  //delay(500);

  // Verifica os valores do sensor de Gas MQ-2
  if (ValorGas == 0) {
    Serial.println("Gás: ALERTA Gas Detectado!!");
    tone(Buzzer, 600);
    delay(1000);
    tone(Buzzer, 2112);
    delay(1000);
    noTone(Buzzer);
  } else {
    Serial.println("Gás: OK");
    noTone(Buzzer);
  }


  if (client) {
    Serial.println("Novo cliente conectado.");
    Serial.print("IP HTML: ");
    Serial.println(WiFi.localIP());

    String request = "";
    unsigned long timeout = millis() + 2000;

    while (client.connected() && millis() < timeout) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n' && request.endsWith("\r\n\r\n")) break;
      }
    }

    // Verifica os comandos para ligar ou desligar cada luz, exceto a luz da Fachada e Jardin
    for (int i = 0; i < 10; i++) {
      if (request.indexOf("GET /ligar" + String(i)) >= 0) {
        estadosLuzes[i] = true;
        digitalWrite(pinosLuzes[i], HIGH);
        Serial.println("Luz " + String(nomesLuzes[i]) + " LIGADA.");
      }
      if (request.indexOf("GET /desligar" + String(i)) >= 0) {
        estadosLuzes[i] = false;
        digitalWrite(pinosLuzes[i], LOW);
        Serial.println("Luz " + String(nomesLuzes[i]) + " DESLIGADA.");
      }
    }

    // Verifica os valores do sensor de Gas MQ-2
    if (ValorGas == 0) {
      Serial.println("Gás: ALERTA Gas Detectado!!");
      tone(Buzzer, 600);
      delay(1000);
      tone(Buzzer, 2112);
      delay(1000);
      noTone(Buzzer);
    } else {
      Serial.println("Gás: OK");
      noTone(Buzzer);
    }

    // Verifica o valor do PIR
    if (request.indexOf("GET /ligarPIR") >= 0) {
      sensorPresencaAtivo = true;
      Serial.println("Sensor de presença ATIVADO");
    }
    if (request.indexOf("GET /desligarPIR") >= 0) {
      sensorPresencaAtivo = false;
      Serial.println("Sensor de presença DESATIVADO");
    }

    // Verifica o valor de abertura ou fechamento do portão
    if (request.indexOf("GET /abrir") >= 0) {
      Serial.println("Abrindo garagem...");
      moverServo(90);
    }
    else if (request.indexOf("GET /fechar") >= 0) {
      Serial.println("Fechando garagem...");
      moverServo(0);
    }


    // Rotas dinâmicas
    if (request.indexOf("GET /status_gas") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(ValorGas == 0 ? "ALERTA! Vazamento de gás detectado" : "OK");
      client.flush();
      client.stop();
      return;
    }

    if (request.indexOf("GET /status_dht") >= 0) {
      float temperatura = dht.readTemperature();
      float umidade = dht.readHumidity();

      if (isnan(temperatura) || isnan(umidade)) {
        temperatura = 0;
        umidade = 0;
        Serial.println("Falha ao ler do DHT11.");
      } else {
        Serial.printf("Temp: %.1f°C | Umid: %.1f%%\n", temperatura, umidade);
      }

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(String(temperatura) + "°C / " + String(umidade) + "%");
      client.flush();
      client.stop();
      return;
    }

    if (request.indexOf("GET /status_pir") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();

      if (!sensorPresencaAtivo) {
        client.println("Desativado");
      } else if (presencaDetectada) {
        client.println("ALERTA! Presença detectada!");
      } else {
        client.println("Ativo");
      }

      client.flush();
      client.stop();
      return;
    }

    if (request.indexOf("GET /status_fachada") >= 0) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.println(statusFachada ? "Ligada" : "Desligada");
      client.flush();
      client.stop();
      return;
    }

    // Página HTML
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Automação Residencial</title>";
    html += "<style>";
    html += "body { text-align: center; font-family: Arial, sans-serif; }";
    html += "h1 { font-size: 28px; color: navy; }";
    html += "h2 { font-size: 22px; }";
    html += "p { font-size: 18px; }";
    html += "button { font-size: 18px; padding: 10px; margin: 5px; width: 120px; }";
    html += ".ligado { background: green; color: white; }";
    html += ".desligado { background: red; color: white; }";
    html += "</style></head><body>";

    html += "<h1>CONTROLE RESIDENCIAL</h1>";

    html += "<h2>Monitoramento de Ambiente</h2>";
    html += "<p><b>Temperatura:</b> <span id='statusTemp'>Carregando...</span></p>";
    html += "<p><b>Umidade:</b> <span id='statusUmid'>Carregando...</span></p><hr>";

    html += "<h2>Sensor de Gás</h2>";
    html += "<p>Status: <b id='statusGas'>" + String(ValorGas == 0 ? "ALERTA! Vazamento de gás detectado" : "OK") + "</b></p><hr>";

    html += "<h2>Controle do Portão</h2>";
    html += "<button onclick=\"location.href='/abrir'\">Abrir</button>";
    html += "<button onclick=\"location.href='/fechar'\">Fechar</button>";

    html += "<h2>Sensor de Presença</h2>";
    html += "<p>Status: <b id='statusPIR'>" + String(!sensorPresencaAtivo ? "Desativado" : (presencaDetectada ? "ALERTA! Presença detectada!" : "Ativo")) + "</b></p>";
    html += "<a href='/ligarPIR'><button class='ligado'>Ligar</button></a>";
    html += "<a href='/desligarPIR'><button class='desligado'>Desligar</button></a><hr>";

    html += "<h2>Luzes Externas</h2>";
    html += "<p>Status: <b id='statusFachada'>Carregando...</b></p><hr>";

    html += "<h2>Piso Inferior</h2>";
    for (int i = 0; i < 5; i++) {
      html += "<h3>Luz da " + String(nomesLuzes[i]) + "</h3>";
      html += "<p>Status: <b>" + String(estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>";
      html += "<a href='/ligar" + String(i) + "'><button class='ligado'>Ligar</button></a>";
      html += "<a href='/desligar" + String(i) + "'><button class='desligado'>Desligar</button></a><hr>";
    }

    html += "<h2>Piso Superior</h2>";
    for (int i = 5; i < 10; i++) {
      html += "<h3>Luz da " + String(nomesLuzes[i]) + "</h3>";
      html += "<p>Status: <b>" + String(estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>";
      html += "<a href='/ligar" + String(i) + "'><button class='ligado'>Ligar</button></a>";
      html += "<a href='/desligar" + String(i) + "'><button class='desligado'>Desligar</button></a><hr>";
    }

    //Java Script para atualizaão automática
    html += "<script>";
    html += "function atualizarStatusGas() {fetch('/status_gas').then(r => r.text()).then(t => {document.getElementById('statusGas').innerHTML = t; }); }";
    html += "function atualizarStatusDHT() {fetch('/status_dht').then(r => r.text()).then(t => {let v = t.split(' / ');document.getElementById('statusTemp').innerHTML = v[0]; document.getElementById('statusUmid').innerHTML = v[1]; }); }";
    html += "function atualizarStatusPIR() {fetch('/status_pir').then(r => r.text()).then(t => {document.getElementById('statusPIR').innerHTML = t;});}";
    html += "function atualizarStatusFachada() {fetch('/status_fachada').then(r => r.text()).then(t => {document.getElementById('statusFachada').innerHTML = t;}); }";

    // Atualiza os dados imediatamente ao carregar
    html += "atualizarStatusGas();";
    html += "atualizarStatusDHT();";
    html += "atualizarStatusPIR();";
    html += "atualizarStatusFachada();";

    // Atualiza de forma contínua
    html += "setInterval(atualizarStatusGas, 3000);";
    html += "setInterval(atualizarStatusDHT, 5000);";
    html += "setInterval(atualizarStatusPIR, 2000);";
    html += "setInterval(atualizarStatusFachada, 5000);";
    html += "</script>";

    html += "</body></html>";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.print(html);
    client.flush();
    client.stop();

    //Serial.println("Cliente desconectado.");
  }
  delay(100);
}

//rotaciona no sentido anti horario
void moverServo(int destino) {
  motorgaragem.attach(15);
  int passo = destino > posicaoAtual ? 1 : -1;
  for (int pos = posicaoAtual; pos != destino; pos += passo) {
    motorgaragem.write(pos);
    delay(10); // Ajuste esse valor para suavidade
  }
  motorgaragem.write(destino);
  posicaoAtual = destino;
  delay(200); // tempo para o servo estabilizar
  motorgaragem.detach();
}
