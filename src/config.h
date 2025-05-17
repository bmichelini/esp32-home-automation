// config.h - Definições de pinos e variáveis globais do projeto de automação residencial

#ifndef CONFIG_H
#define CONFIG_H

#include <DHT.h>        // Biblioteca para o sensor DHT
#include <ESP32Servo.h> // Biblioteca para controle de servo motores

// =========================
// Servo motor da garagem
// =========================
Servo motorgaragem;   // Cria objeto Servo para controlar o motor da garagem
int posicaoAtual = 0; // Começa fechado (posição 0)

// ========================================
// Sensor de temperatura e umidade (DHT11)
// ========================================
#define DHTPIN 27         // Pino onde o DHT11 está conectado
#define DHTTYPE DHT11     // Tipo do sensor DHT (DHT11 ou DHT22)
DHT dht(DHTPIN, DHTTYPE); // Objeto DHT para ler temperatura e umidade

// ==============================
// Sensor de gás (MQ-2) e buzzer
// ==============================
#define GasD 26   // Pino do sensor de gás
#define Buzzer 25 // Pino do buzzer

// =============================
// Sensor de luminosidade (LDR)
// =============================
#define LDR 34 // Pino do LDR (sensor de luminosidade)

// =========================
// Sensor de presença (PIR)
// =========================
#define PIR_SENSOR 32             // Pino do sensor PIR
bool sensorPresencaAtivo = false; // Variável para verificar se o sensor de presença está ativo
bool presencaDetectada = false;   // Variável para verificar se a presença foi detectada

// ===================================
// Luzes da casa - Pinos por ambiente
// ===================================
const int luzSala = 2;       // Pino da luz da sala
const int luzGaragem = 4;    // Pino da luz da garagem
const int luzWcInferior = 5; // Pino da luz do banheiro inferior
const int luzCozinha = 18;   // Pino da luz da cozinha
const int luzServico = 19;   // Pino da luz da área de serviço
const int luzSuiteJr = 21;   // Pino da luz da suíte júnior
const int luzWcJr = 22;      // Pino da luz do banheiro júnior
const int luzMaster = 23;    // Pino da luz da suíte master
const int luzWcMaster = 13;  // Pino da luz do banheiro master
const int luzCloset = 12;    // Pino da luz do closet
const int luzFachada = 14;   // Pino da luz da fachada

unsigned long ultimaLeitura = 0;      // Armazena o tempo da última leitura dos sensores
const unsigned long intervalo = 2000; // Intervalo de tempo entre as leituras dos sensores (em milissegundos)

// ===================================
// Arrays com pinos e nomes das luzes
// ===================================
const int pinosLuzes[] = {
    luzSala, luzGaragem, luzWcInferior, luzCozinha, luzServico,
    luzSuiteJr, luzWcJr, luzMaster, luzWcMaster, luzCloset};

const char *nomesLuzes[] = {
    "Sala", "Garagem", "WC Inferior", "Cozinha", "Área Serviço",
    "Suíte Jr.", "WC Jr.", "Suíte Master", "WC Master", "Closet"};

const int numLuzes = sizeof(pinosLuzes) / sizeof(pinosLuzes[0]); // Calcula automaticamente o número de luzes
bool estadosLuzes[numLuzes] = {false};                           // Armazena o estado (ligado/desligado) de cada luz

#endif
// Fim do arquivo config.h