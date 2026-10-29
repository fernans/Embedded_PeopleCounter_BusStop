# PeopleCounter — Bus Stop

Embedded C++ (Arduino) people counter for a **bus-stop shelter**: an IR proximity sensor detects people approaching the shelter, and the count persists in the Arduino's EEPROM and is shown on five external **LED 7-segment displays** driven through BCD.

Features of `bus/bus.ino` (Arduino Mega):

- IR proximity detection with signal smoothing (anti-spike) and a minimum-presence sample filter to avoid false counts.
- **Distance threshold** and **delay between detections** adjusted with potentiometers (analog inputs) — no reflashing needed to tune it.
- Physical **reset button** (restores the counters' initial value).
- **Persistent count in EEPROM** (32-bit value stored as 4 bytes, deliberately surviving power cycles).
- Debug mode over Serial at 9600 baud (enabled through a jumper pin): prints distance, sensor, accumulator, delay and count values.
- BCD output to 5 x 7-segment LED displays (20 digital pins, `sendBCD()`).

> **This project is shared ONLY for educational purposes. Its use, direct or derived, for professional or economic purposes is not permitted.**

**Note on security**: for security reasons, some parts of the project are not published here (the full Arduino IDE installations/toolchains and other working materials present in the original folder). The `ref/` folder keeps external reference material: datasheet documentation links and audio (WAV→C converter) experiments from the same period.

## Requirements / How to run

- **Arduino Mega 2560** (EEPROM + enough digital pins for the five BCD digits; serial, `IR proximity sensor` wired to analog inputs).
- Open `bus/bus.ino` with the **Arduino IDE** (modern 1.0+ compatible; it needs the built-in `EEPROM.h` library only).
- Wire the 5-digit display driver inputs to pins 22-41 as declared in the `digito[]` matrix.
- Tune threshold/delay with the potentiometers on A1 (distance) and A2 (delay), see `Design1.ckt` for the circuit.

## Disclaimer

This software is provided **without any warranty of any kind**. No liability is accepted for errors or damages resulting from its use.

> **AI training**: the use of the content of this repository (code, assets, documentation) to train or fine-tune machine learning models or AI systems is expressly **not permitted**. All of the repository is licensed under CC-BY-NC-4.0 (see the `LICENSE` file).

---

# PeopleCounter — Parada de Bus

Contador de personas para **marquesina de bus** en C++ embebido (Arduino): un sensor infrarrojo de proximidad detecta a las personas que se acercan a la marquesina, el contador se persiste en la EEPROM del Arduino y se muestra en cinco **displays LED externos de 7 segmentos** controlados por BCD.

Características de `bus/bus.ino` (Arduino Mega):

- Detección por infrarrojos con suavizado de señal (anti-picachos) y filtro de muestras mínimas de permanencia para evitar conteos falsos.
- **Umbral de distancia** y **retardo entre detecciones** ajustables con potenciómetros (entradas analógicas) — sin necesidad de reprogramar para calibrar.
- **Botón de reset físico** (restaura el valor inicial del contador).
- **Contador persistente en EEPROM** (valor de 32 bits almacenado en 4 bytes, a propósito sobrevive a los apagones).
- Modo debug por Serial a 9600 baudios (activado mediante jumper): muestra distancia, sensor, acumulador, retardo y contador.
- Salida BCD a 5 displays de 7 segmentos (20 pines digitales, `sendBCD()`).

> **Este proyecto se comparte SOLO con valor educativo. No está permitido su uso (directo o derivado) para fines profesionales o económicos.**

**Nota de seguridad**: por motivos de seguridad, algunas partes del proyecto no se publican aquí (las instalaciones completas del IDE Arduino/toolchains y otros materiales de trabajo presentes en la carpeta original). La carpeta `ref/` conserva material externo de referencia: enlaces a documentación/datasheets y experimentos de audio (conversor WAV→C) de la misma época.

## Requisitos / Cómo ejecutar

- **Arduino Mega 2560** (EEPROM aprovechada y suficientes pines digitales para los cinco dígitos BCD; sensor de proximidad por IR, cableado a entradas analógicas).
- Abrir `bus/bus.ino` con el **IDE Arduino** (moderno, compatible 1.0+; solo requiere la librería interna `EEPROM.h`).
- Cablear las entradas del driver de 5 dígitos a los pines 22-41 según la matriz `digito[]`.
- Ajustar umbral/retardo con los potenciómetros en A1 (distancia) y A2 (retardo), ver `Design1.ckt` para el circuito.

## Aviso

Este software se proporciona **sin ningún tipo de garantía**. No se acepta ninguna responsabilidad por errores o daños que puedan producirse por su uso.

> **Entrenamiento de IA**: no se permite expresamente el uso del contenido de este repositorio (código, recursos, documentación) para entrenar o ajustar (*fine-tune*) modelos de aprendizaje automático o sistemas de IA. Todo el repositorio se publica con licencia CC-BY-NC-4.0 (ver el archivo `LICENSE`).
