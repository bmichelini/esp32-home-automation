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
   - DHT sensor library. Para mais informações [clique aqui](https://github.com/adafruit/DHT-sensor-library).
   - Adafruit Unified Sensor. Para mais informações [clique aqui](https://github.com/adafruit/DHT-sensor-library).
   - WiFiManager. Para mais informações [clique aqui](https://github.com/tzapu/WiFiManager)
   - ESP32Servo. Para mais informações [clique aqui](https://madhephaestus.github.io/ESP32Servo/annotated.html).
4. Abra o arquivo `automacao_esp32_refatorado.ino` e envie para o ESP32. Garanta que todos os arquivos `.h` estejam na mesma pasta.
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

Este projeto está licenciado sob a **[Licença GNU GPL v3](LICENSE)**. Consulte o arquivo para mais informações.

---

## 👨‍🎓 Créditos

Projeto desenvolvido como parte de um **Trabalho de Conclusão de Curso (TCC)** voltado à demonstração de conceitos práticos de automação residencial e Internet das Coisas (IoT), para a conclusão do curso de **Bacharelado em Ciência da Computação da UNEMAT** - Universidade do Estado de Mato grosso, vinculado ao **Núcleo Pedagógico de Rondonópolis**. Este trabalho foi desenvolvido pelo discente Bruno João Michelin, orientado pelo professor [Max Robert Marinho](http://lattes.cnpq.br/3542593918203653) e contou com a contribuição do professor [Lucas Kriesel Sperotto](http://lattes.cnpq.br/2335475405623609), do professor [Fernando Yoiti Obana](http://lattes.cnpq.br/1633968449319130) e do aluno de pós graduação [Edinaldo Serra Cardoso Júnior](http://lattes.cnpq.br/6990476825546492).

Código desenvolvido por [bmichelini](https://github.com/bmichelini) e refatorado por [Lucas-Sperotto](https://github.com/Lucas-Sperotto).
