// config.h - Definições de pinos e variáveis globais do projeto de automação residencial

#ifndef CONFIG_H
#define CONFIG_H

#include <DHT.h>
#include <ESP32Servo.h>

// =========================
// Rede Wi-Fi
// =========================
const char *ssid = "MicheliniDeco";
const char *password = "michelini1910";

// =========================
// Servo motor da garagem
// =========================
Servo motorgaragem;
int posicaoAtual = 0; // Começa fechado (posição 0)

// =========================
// Sensor de temperatura e umidade (DHT11)
// =========================
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// =========================
// Sensor de gás (MQ-2) e buzzer
// =========================
#define GasD 26
#define Buzzer 25

// =========================
// Sensor de luminosidade (LDR)
// =========================
#define LDR 34

// =========================
// Sensor de presença (PIR)
// =========================
#define PIR_SENSOR 32
bool sensorPresencaAtivo = false;
bool presencaDetectada = false;

// =========================
// Luzes da casa - Pinos por ambiente
// =========================
const int luzSala = 2;
const int luzGaragem = 4;
const int luzWcInferior = 5;
const int luzCozinha = 18;
const int luzServico = 19;

const int luzSuiteJr = 21;
const int luzWcJr = 22;
const int luzMaster = 23;
const int luzWcMaster = 13;
const int luzCloset = 12;
const int luzFachada = 14; // LDR controla automaticamente

// =========================
// Arrays com pinos e nomes das luzes
// =========================
const int pinosLuzes[] = {
    luzSala, luzGaragem, luzWcInferior, luzCozinha, luzServico,
    luzSuiteJr, luzWcJr, luzMaster, luzWcMaster, luzCloset};

const char *nomesLuzes[] = {
    "Sala", "Garagem", "WC Inferior", "Cozinha", "Área Serviço",
    "Suíte Jr.", "WC Jr.", "Suíte Master", "WC Master", "Closet"};

const int numLuzes = sizeof(pinosLuzes) / sizeof(pinosLuzes[0]);
bool estadosLuzes[numLuzes] = {false};

#endif
// Fim do arquivo config.h