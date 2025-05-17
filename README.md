# 🏠 ESP32 Home Automation - Maquete

Projeto de **automação residencial de baixo custo** utilizando um **ESP32**, sensores acessíveis e uma **maquete em MDF** cortada a laser. O sistema foi desenvolvido como parte de um **Trabalho de Conclusão de Curso (TCC)**, visando aplicar conceitos de IoT, eletrônica embarcada e desenvolvimento web embarcado.

---

## 🚀 Funcionalidades

- 🌡️ Leitura de **temperatura e umidade** (sensor DHT11)
- 🧪 Detecção de **gás inflamável** (sensor MQ-2)
- 🌞 Medição de **luminosidade ambiente** (sensor LDR)
- 🚶‍♂️ Detecção de **movimento** (sensor PIR)
- 🚪 Controle de **portão automatizado** com servo motor (SG90)
- 💡 Controle de **10 luzes internas** e **1 luz externa automática**
- 🌐 Interface **web responsiva**, com **monitoramento em tempo real** via JavaScript

---

## 🧩 Tecnologias utilizadas

- Microcontrolador **ESP32**
- Plataforma **Arduino IDE**
- **Linguagem C++**
- Bibliotecas:
  - `WiFi.h` (conexão web)
  - `WiFiManager.h` (conexão Wi-Fi)
  - `DHT.h` (sensor de temperatura)
  - `ESP32Servo.h` (servo motor)
  - `Preferences.h` (guardar configurações)
- Sensores:
  - DHT11
  - MQ-2
  - LDR
  - PIR
  - Servo SG90

---

## 📁 Estrutura do Projeto

```
├── src/
│   ├── automacao_esp32_refatorado.ino   # Código principal
│   ├── config.h                         # Definições de pinos e variáveis globais
│   ├── sensores.h                       # Funções de leitura dos sensores
│   ├── atuadores.h                      # Funções de controle dos atuadores
│   ├── interface.h                      # Interface web e rotas
├── images/                              # Imagens da maquete e esquemas de ligação
└── docs/                                # Documentação extra (manual, esquemas, etc.)
```

---

## 🧪 Como executar

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software)
2. Configure o suporte ao ESP32:  
   👉 [Tutorial oficial](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
3. Instale as bibliotecas necessárias:
   - DHT sensor library. Instale buscando por [DHT sensor library](https://docs.arduino.cc/libraries/dht-sensor-library/).
   - Adafruit Unified Sensor.
   - WiFiManager.
   - ESP32Servo. Instale buscando por [ESP32Servo](https://docs.arduino.cc/libraries/esp32servo/).
4. Abra o arquivo `automacao_esp32_refatorado.ino` e envie para o ESP32.
5. Acesse o IP exibido no monitor serial para abrir a interface web.

---

## ⚠️ Requisitos de Hardware

- ESP32 DevKit
- Sensor DHT11
- Sensor MQ-2
- Sensor PIR
- Sensor LDR
- Servo motor SG90
- Resistores, jumpers e protoboard
- Maquete de MDF cortada a laser

---

## 📝 Licença

Este projeto está licenciado sob a **Licença GNU GPL v3**. Consulte o arquivo `LICENSE` para mais informações.

---

## 👨‍🎓 Créditos

Projeto desenvolvido como parte de um **Trabalho de Conclusão de Curso (TCC)** voltado à demonstração de conceitos práticos de automação residencial e Internet das Coisas (IoT).


<!--
# ESP32 Home Automation - Maquete

Este projeto consiste em um sistema de automação residencial de baixo custo utilizando um ESP32 e sensores comuns, implementado em uma maquete de MDF cortada a laser.

## Funcionalidades
- Leitura de temperatura e umidade com DHT11/DHT22
- Monitoramento de gás (sensor MQ)
- Medição de luminosidade ambiente (sensor LDR)
- Detecção de movimento (sensor PIR)
- Controle de servo motor para abertura de porta/portão

## Tecnologias utilizadas
- ESP32
- Arduino IDE
- Sensores: DHT11, MQ-x, LDR, PIR
- Servo motor SG90

## Estrutura do Projeto
- `src/`: Código-fonte Arduino
- `images/`: Imagens da maquete e esquemas de ligação
- `docs/`: Documentação adicional (manual de montagem, esquemas eletrônicos, etc.)

## Como executar
1. Instale a [Arduino IDE](https://www.arduino.cc/en/software)
2. Configure a IDE para o ESP32, seguindo [este tutorial oficial](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
3. Baixe as bibliotecas necessárias:
   - DHT sensor library
   - Adafruit Unified Sensor
   - Servo.h
4. Faça o upload do arquivo `src/27.04maquete_final.ino` para o ESP32.

## Licença
Este projeto está licenciado sob a Licença GNU GPL v3. Veja o arquivo LICENSE para mais detalhes.

## Créditos
Projeto desenvolvido como parte de um trabalho escolar para demonstração de automação residencial.
-->
