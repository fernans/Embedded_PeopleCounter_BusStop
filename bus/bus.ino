int debugPin=12;
boolean debugMode=false;
unsigned long lastTime=0; // evitar time-loop (cada 50 dias aprox)

int sensorPin = A0;
int sensorValue=0;
int sensorValueOld=0;
int distancePin = A1;
int distanceValue=0;
int delayPin = A2;
int ledPin=13;
int resetPin=11;

unsigned long delayCall=1000;
unsigned long nextCall=0;
boolean near=false;

float sensorStabilize=0.8; //0=direct ... 1=fixed
unsigned int sensorNeededSamples=10;
unsigned int sensorAcum=0;

boolean prevReset=false;
unsigned long resetValue=530;
unsigned long contador=0;

//* *********************** SOLO Aduino mega con EEPROM
// eeprom
#include <EEPROM.h>
//*/
// store a long (32-bit)=(4x 8-bit char)
unsigned int aSet=0; // flag if memory is set (must be 530)
unsigned int aHH=1;
unsigned int aHL=2;
unsigned int aLH=3;
unsigned int aLL=4;

int digito[5][4] = {{22,23,24,25},{26,27,28,29},{30,31,32,33},{34,35,36,37},{38,39,40,41}};

void setup() {
  pinMode(debugPin, INPUT);
  digitalWrite(debugPin, HIGH); // pull-up
  debugMode=(digitalRead(debugPin)==LOW);
  if (debugMode) Serial.begin(9600);
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  pinMode(resetPin,INPUT);
  digitalWrite(resetPin,HIGH); // pull-up
  randomSeed(analogRead(A7));
  
  for (int dig=0;dig<5;dig++)
    for (int pin=0;pin<4;pin++)
      pinMode(digito[dig][pin], INPUT);
      
  pinMode(sensorPin, INPUT);
  pinMode(distancePin, INPUT);
  pinMode(delayPin, INPUT);
      
  
  contador=resetValue;
  //* *********************** SOLO Aduino mega con EEPROM
  // read contador (32bit) from EEPROM
  int vSet = EEPROM.read(aSet); // test if memory is set (must be 530)
  if (vSet==130) {
    unsigned long vHH = EEPROM.read(aHH);
    unsigned long vHL = EEPROM.read(aHL);
    unsigned long vLH = EEPROM.read(aLH);
    unsigned long vLL = EEPROM.read(aLL);
    contador=(vHH<<24)+(vHL<<16)+(vLH<<8)+vLL;
  } else {
    // ignore EEPROM and continue with resetValue;
    contador=resetValue;
    // save and set memory flag
    guardarContador(contador);
    EEPROM.write(aSet,130);
  }
  //*/
}

void guardarContador(unsigned long c) {
    //s* *********************** SOLO Aduino mega con EEPROM
  // split 32-bit unsigned long into 4 unsigned ints
  unsigned long vLL = (c & 255); c=c>>8;
  unsigned long vLH = (c & 255); c=c>>8;
  unsigned long vHL = (c & 255); c=c>>8;
  unsigned long vHH = (c & 255); c=c>>8;
  // store value
  EEPROM.write(aHH,vHH);
  EEPROM.write(aHL,vHL);
  EEPROM.write(aLH,vLH);
  EEPROM.write(aLL,vLL);
  //*/
}

void loop() {
  if (lastTime>millis()) nextCall=millis()+(nextCall-lastTime); //timeloop (nextCall=actual + (lo que faltaba de tiempo antes)
  lastTime=millis();
  
  sensorValue = analogRead(sensorPin);
  // estabilizar sensor value
  //sensorValue = (int) ((float)sensorValue*(sensorStabilize))+((1-sensorStabilize)*(float)analogRead(sensorPin));
  if ((sensorValue-sensorValueOld)>100) sensorValue=(sensorValueOld+sensorValue)*0.5; // suavizar incremento a las subidas
  sensorValueOld=sensorValue;
  
  distanceValue = analogRead(distancePin);

  if (!near) { // si no activo
    if (sensorValue>distanceValue) { // cerca
        sensorAcum++; // acumular permanecer
        if ((sensorAcum>sensorNeededSamples)) { // si superado el tiempo margen, contar //(millis()>nextCall)
            near=true; // activado
            if (debugMode) Serial.print("----- deteccion ----\r\n");
            contar(false); // contar normal (no-reset)
        } // else ignorar (tiene que permanecer)
    } else {  // lejos
        sensorAcum=0; // reset acum permanecer
    }
  } else { // si activo
      if (sensorAcum>0) { // acaba de contar
          if (sensorValue<distanceValue) { // se ha alejado
            //calcular tiempo de espera para bajar flag          
            delayCall=analogRead(delayPin);
            delayCall=map(delayCall,0,1023,0,10000);
            nextCall=millis()+delayCall; // iniciar tiempo margen entre detecciones
            sensorAcum=0; // reset acum (indicar que ya se aparto antes)
        }
      } else { // ya se apartó antes
          // comprobar tiempo limite para liberar
          if (millis()>nextCall) near=false; // librerar
      }
  }

//  digitalWrite(ledPin,(millis()<=nextCall)?HIGH:LOW);
  digitalWrite(ledPin,(near)?HIGH:LOW);
  
  if (digitalRead(resetPin)==LOW) {
    if (!prevReset) {
      contar(true); // conectado a gnd (contar-reset)
      prevReset=true;
    }
  } else prevReset=false;
  
  if (debugMode) {
    Serial.print("d=");
    Serial.print(distanceValue);
    Serial.print("\ts=");
    Serial.print(sensorValue);
    Serial.print("\tac=");
    Serial.print(sensorAcum);
    Serial.print("\tt=");
    delayCall=analogRead(delayPin); // actualizar cada debug-loop
    Serial.print((delayCall/1000)); 
    if (millis()<=nextCall) Serial.print("*");
    Serial.print("\tc=");
    Serial.print(contador);
    if (digitalRead(resetPin)==LOW) Serial.print("(r)");
    Serial.print("\r\n");
  }
  mostrarDigitos();
 
  /*
  // contar automatico (para test only)
  contador+=11111;
  if (contador>99999) contador=0;
  delay(250);
  //*/
}

void contar(boolean reset) {
  if (reset) contador=resetValue;
  else contador++;
  guardarContador(contador);
  //digitalWrite(ledPin,HIGH);
  //delay(100); // mostrar led medio segundo
  //digitalWrite(ledPin,LOW);
  //delay(100);
}

void mostrarDigitos() {
  unsigned long c=contador;
  char val=0;
  for (int dig=0;dig<5;dig++) {
    val=(c%10);  c=c/10;
    sendBCD(digito[dig],val);
  }
}

void sendBCD(int pines[4], char value) {
   digitalWrite(pines[0],!(value & B0001)); // A
   digitalWrite(pines[1],!(value & B0010)); // B
   digitalWrite(pines[2],!(value & B0100)); // C
   digitalWrite(pines[3],!(value & B1000)); // D
}


