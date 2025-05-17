// Inclusão das bibliotecas básicas para conexão Wi-Fi, sensor de temperatura/umidade e controle de servo motor.
#include <WiFi.h>        // Conecta o ESP32 à rede Wi-Fi
#include <DHT.h>         // Biblioteca para ler sensores DHT (temperatura/umidade)
#include <ESP32Servo.h>  // Biblioteca para controlar servo motores
#include <WiFiManager.h> // Biblioteca que gerencia conexão Wi-Fi com portal de configuração
#include <Preferences.h>

// Inclusão dos arquivos de cabeçalho personalizados do projeto
#include "config.h"    // Constantes, definições de pinos, arrays de luzes, etc.
#include "sensores.h"  // Funções que lidam com sensores
#include "atuadores.h" // Funções que controlam buzzer, luzes, servo motor, etc.
#include "interface.h" // Interface HTML e tratamento de requisições HTTP

// Criação de um servidor web na porta 80 (padrão HTTP)
WiFiServer server(80);

// Objeto global para armazenar configurações persistentes
Preferences prefs;

void setup()
{
  Serial.begin(115200); // Inicia a comunicação serial (monitor serial do Arduino)

  // ========== Configuração via WiFiManager ==========
  WiFiManager wm;

  // Adiciona estilo visual ao portal de configuração
  wm.setCustomHeadElement("<style>body{font-family:Arial;text-align:center;}h2{color:#2c3e50;}</style>");

  // Cria um campo extra no portal de configuração (ID, label, valor padrão, tamanho máximo)
  WiFiManagerParameter custom_local("local", "Nome do local", "", 40);
  wm.addParameter(&custom_local); // Adiciona o campo ao portal

  // Tenta conectar-se com credenciais salvas anteriormente
  // Se falhar, cria um Access Point chamado "ESP32-Setup"
  bool conectado = wm.autoConnect("ESP32-Setup");

  if (!conectado)
  {
    Serial.println("❌ Falha na conexão ou tempo esgotado. Reiniciando...");
    delay(3000);
    ESP.restart(); // Reinicia o ESP32 para tentar novamente
  }

  // Conectado com sucesso
  Serial.println("✅ Conectado à rede Wi-Fi!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Lê o valor inserido no campo personalizado (nome do local)
  String nomeLocal = custom_local.getValue();
  Serial.print("Nome do local configurado: ");
  Serial.println(nomeLocal);

  // Salva o nome do local na memória flash para uso futuro
  prefs.begin("config", false);        // Inicia namespace "config"
  prefs.putString("local", nomeLocal); // Grava a string "nomeLocal"
  prefs.end();                         // Encerra o uso da memória

  // ========== INICIALIZAÇÃO DOS SENSORES E ATUADORES ==========

  pinMode(GasD, INPUT);        // Define pino do sensor de gás como entrada
  pinMode(Buzzer, OUTPUT);     // Define pino do buzzer como saída
  pinMode(luzFachada, OUTPUT); // Define pino da luz da fachada como saída
  pinMode(PIR_SENSOR, INPUT);  // Define pino do sensor de presença como entrada

  // Configura os pinos das luzes e garante que todas iniciem desligadas
  for (int i = 0; i < numLuzes; i++)
  {
    pinMode(pinosLuzes[i], OUTPUT);
    digitalWrite(pinosLuzes[i], LOW);
  }

  dht.begin(); // Inicia o sensor de temperatura/umidade DHT

  // Prepara o motor da garagem para iniciar fechado
  motorgaragem.attach(15); // Liga o motor ao pino 15
  motorgaragem.write(180); // 180 graus = posição de fechado
  motorgaragem.detach();   // Desconecta para evitar ruído ou sobreaquecimento

  server.begin(); // Inicia o servidor após conectar à rede

  // Indicação de que o sistema foi iniciado com sucesso
  Serial.println("🏠 Sistema de automação residencial iniciado com sucesso.");
}

void loop()
{
  // unsigned long inicio = millis();
  // Executa leitura de sensores a cada intervalo
  if (millis() - ultimaLeitura >= intervalo)
  {
    verificarGas();           // Lê sensor de gás e aciona o alarme se necessário
    verificarPresenca();      // Lê sensor PIR e aciona buzzer se detectar movimento
    atualizarLuzFachada();    // Liga/desliga a luz da fachada com base na claridade
    ultimaLeitura = millis(); // Atualiza o tempo da última leitura
  }

  WiFiClient client = server.available(); // Verifica se há cliente (navegador) conectado
  if (client)
  {
    // Se houver cliente, processa a requisição HTTP
    //Serial.println("📡 Cliente conectado!");
    String request = lerRequisicao(client); // Lê a solicitação HTTP
    processarRequisicao(client, request);   // Responde ao cliente com HTML e ações
    // delay(1);
    // client.stop();
  }

  // Serial.print("Resposta enviada em: ");
  // Serial.println(millis() - inicio);
  // delay(100);  // Pequeno atraso para estabilidade (pode ser removido em versões assíncronas)
}