// Inclusão das bibliotecas básicas para conexão Wi-Fi, sensor de temperatura/umidade e controle de servo motor.
#include <WiFi.h>         // Conecta o ESP32 à rede Wi-Fi
#include <DHT.h>          // Biblioteca para ler sensores DHT (temperatura/umidade)
#include <ESP32Servo.h>   // Biblioteca para controlar servo motores

// Inclusão dos arquivos de cabeçalho personalizados do projeto
#include "config.h"       // Constantes, definições de pinos, arrays de luzes, etc.
#include "sensores.h"     // Funções que lidam com sensores
#include "atuadores.h"    // Funções que controlam buzzer, luzes, servo motor, etc.
#include "interface.h"    // Interface HTML e tratamento de requisições HTTP

// Criação de um servidor web na porta 80 (padrão HTTP)
WiFiServer server(80);


void setup() {
  Serial.begin(115200);      // Inicia a comunicação serial (monitor serial do Arduino)
  dht.begin();               // Inicia o sensor de temperatura/umidade DHT

  pinMode(GasD, INPUT);      // Define pino do sensor de gás como entrada
  pinMode(Buzzer, OUTPUT);   // Define pino do buzzer como saída
  pinMode(luzFachada, OUTPUT); // Define pino da luz da fachada como saída
  pinMode(PIR_SENSOR, INPUT);  // Define pino do sensor de presença como entrada

  // Configura os pinos das luzes e garante que todas iniciem desligadas
  for (int i = 0; i < numLuzes; i++) {
    pinMode(pinosLuzes[i], OUTPUT);
    digitalWrite(pinosLuzes[i], LOW);
  }

  // Prepara o motor da garagem para iniciar fechado
  motorgaragem.attach(15);     // Liga o motor ao pino 15
  motorgaragem.write(180);     // 180 graus = posição de fechado
  motorgaragem.detach();       // Desconecta para evitar ruído ou sobreaquecimento

  // Tenta conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");

  unsigned long inicioTentativa = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicioTentativa < 15000) {
    delay(1000);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());  // Mostra o IP no monitor serial
    server.begin();                  // Inicia o servidor web
  } else {
    Serial.println("\\nErro ao conectar ao Wi-Fi.");
    // Pode ser melhorado com fallback para modo Access Point (AP)
  }
}

void loop() {
  WiFiClient client = server.available();  // Verifica se há cliente (navegador) conectado

  verificarGas();           // Lê sensor de gás e aciona o alarme se necessário
  atualizarLuzFachada();    // Liga/desliga a luz da fachada com base na claridade
  verificarPresenca();      // Lê sensor PIR e aciona buzzer se detectar movimento

  if (client) {
    // Se houver cliente, processa a requisição HTTP
    String request = lerRequisicao(client);        // Lê a solicitação HTTP
    processarRequisicao(client, request);          // Responde ao cliente com HTML e ações
  }

  delay(100);  // Pequeno atraso para estabilidade (pode ser removido em versões assíncronas)
}













