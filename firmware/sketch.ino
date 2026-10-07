/**
 * SafePet - demonstrador academico ESP32 / Wokwi.
 * Equipe: Marcos Vinicius, Jailton Alves, Guilherme Monteiro, Gabriel Cordeiro.
 * Evolucao do esboco da equipe: https://wokwi.com/projects/476617832747866113
 * POSICAO SIMULADA via Serial. MPU6050 mede aceleracao, NAO latitude/longitude.
 * Nao implementa GPS, rede, aplicativo nem rastreamento real.
 * Comandos terminados em Enter: c (casa), f (fora), p LAT LON, s (sem posicao), h.
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "geofence.h"

constexpr uint8_t BUZZER_PIN=33;
constexpr uint32_t SAMPLE_MS=200, DISPLAY_MS=250, BEEP_MS=300;
constexpr uint32_t POSITION_TIMEOUT_MS=30000; // exige renovar a entrada simulada
Adafruit_SSD1306 display(128,64,&Wire,-1);
Adafruit_MPU6050 mpu;
bool displayOk=false, sensorOk=false, positionValid=false, escaped=false, sounding=false;
double latitude=SafePet::BASE_LAT, longitude=SafePet::BASE_LON, distanceM=0;
float accelerationDelta=0;
uint32_t lastPosition=0, lastSample=0, lastDisplay=0, lastBeep=0;
char command[80];
size_t commandLength=0;
bool commandOverflow=false;

// Atualiza a cerca somente depois de receber uma posicao valida.
void updatePosition(double lat,double lon) {
  latitude=lat; longitude=lon;
  distanceM=SafePet::distanceMeters(lat,lon);
  escaped=SafePet::nextAlert(escaped,distanceM);
  positionValid=true; lastPosition=millis();
  Serial.printf("SIM: %.6f %.6f | %.1f m | %s\n",lat,lon,distanceM,escaped?"FORA":"DENTRO");
}
void printHelp() {
  Serial.println("SAFEPET DEMO | localizacao SIMULADA, sem GPS");
  Serial.println("c=casa | f=fora | p LAT LON | s=sem posicao | h=ajuda");
  Serial.println("Envie Enter. Posicao expira em 30 s. Cerca: sai >100m, volta <=80m.");
}
// Entradas desconhecidas nao alteram o estado nem simulam fuga.
void executeCommand(const char* line) {
  if (!strcmp(line,"c")) updatePosition(SafePet::BASE_LAT,SafePet::BASE_LON);
  else if (!strcmp(line,"f")) updatePosition(SafePet::BASE_LAT+0.002,SafePet::BASE_LON);
  else if (!strcmp(line,"s")) { positionValid=false; Serial.println("SEM POSICAO"); }
  else if (!strcmp(line,"h")) printHelp();
  else {
    double lat,lon;
    if (SafePet::parsePosition(line,lat,lon)) updatePosition(lat,lon);
    else Serial.println("Comando invalido. Use h. Estado preservado.");
  }
}
// Buffer limitado e leitura sem readString()/timeout. CRLF nao gera dois comandos.
void readCommands() {
  while (Serial.available()) {
    char c=(char)Serial.read();
    if (c=='\r') continue;
    if (c=='\n') {
      command[commandLength]='\0';
      if (commandOverflow) Serial.println("Comando longo demais: descartado.");
      else if (commandLength) executeCommand(command);
      commandLength=0; commandOverflow=false;
    } else if (!commandOverflow) {
      if (commandLength<sizeof(command)-1) command[commandLength++]=c;
      else commandOverflow=true;
    }
  }
}
// Sensor complementar: desvio da magnitude de aceleracao em relacao a 1 g.
// Indicador didatico, sem diagnostico de saude ou classificacao validada de atividade.
void readMotion(uint32_t now) {
  if (!sensorOk || uint32_t(now-lastSample)<SAMPLE_MS) return;
  lastSample=now;
  Wire.beginTransmission(0x68);
  if (Wire.endTransmission()!=0) { sensorOk=false; Serial.println("MPU6050 desconectado"); return; }
  sensors_event_t a,g,t;
  if (!mpu.getEvent(&a,&g,&t)) { sensorOk=false; Serial.println("Falha na leitura MPU6050"); return; }
  accelerationDelta=fabsf(sqrtf(a.acceleration.x*a.acceleration.x + a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z)-9.80665f);
}
// Alarme local intermitente, sem delay bloqueante. Sem posicao nao significa seguro.
void updateBuzzer(uint32_t now) {
  bool alarm=positionValid && escaped;
  if (!alarm) { if (sounding) noTone(BUZZER_PIN); sounding=false; lastBeep=now; return; }
  if (uint32_t(now-lastBeep)>=BEEP_MS) {
    lastBeep=now; sounding=!sounding;
    if (sounding) tone(BUZZER_PIN,1000); else noTone(BUZZER_PIN);
  }
}
// OLED ASCII para compatibilidade com a fonte padrao da biblioteca.
void updateDisplay(uint32_t now) {
  if (!displayOk || uint32_t(now-lastDisplay)<DISPLAY_MS) return;
  lastDisplay=now; display.clearDisplay(); display.setCursor(0,0);
  display.println("SafePet | DEMO");
  display.println(!positionValid?"SEM POSICAO":(escaped?"FORA DA CERCA":"DENTRO DA CERCA"));
  if (positionValid) {
    display.print("Dist: "); display.print(distanceM,1); display.println(" m");
    display.print("Lat:"); display.println(latitude,5);
    display.print("Lon:"); display.println(longitude,5);
  } else {
    display.println("Envie c, f ou p"); display.println("GPS: SIMULADO"); display.println("Sem garantia local");
  }
  if (!sensorOk) display.println("MPU: FALHA");
  else { display.print("Delta a:"); display.print(accelerationDelta,1); display.println("m/s2"); }
  display.println("Posicao simulada"); display.display();
}
void setup() {
  Serial.begin(115200); pinMode(BUZZER_PIN,OUTPUT); noTone(BUZZER_PIN);
  Wire.begin(21,22);
  displayOk=display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  if (displayOk) { display.clearDisplay(); display.setTextSize(1); display.setTextColor(SSD1306_WHITE); }
  else Serial.println("Falha OLED: use o terminal para diagnostico.");
  sensorOk=mpu.begin(0x68,&Wire);
  if (sensorOk) { mpu.setAccelerometerRange(MPU6050_RANGE_4_G); mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); }
  else Serial.println("Falha MPU6050: confira ligacoes e reinicie.");
  printHelp(); // Inicia SEM POSICAO: nunca presume que o animal esta em casa.
}
void loop() {
  readCommands(); uint32_t now=millis();
  if (positionValid && uint32_t(now-lastPosition)>=POSITION_TIMEOUT_MS) {
    positionValid=false; Serial.println("SEM POSICAO: entrada expirou (30 s).");
  }
  readMotion(now); updateBuzzer(now); updateDisplay(now);
}
