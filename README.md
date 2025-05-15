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
