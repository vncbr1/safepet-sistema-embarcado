#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configurações do Display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BUZZER_PIN 33

// Coordenadas base (Ex: Casa)
float lat = -8.0880;
float lon = -34.8775;
bool petFugiu = false;

void setup() {
  Serial.begin(115200);
  
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN); // Garante que o som inicie 100% desligado

  // Inicializa o Display OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha no display OLED"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 20);
  display.println("SafePet Iniciado!");
  display.display();
  
  // Imprime instruções no terminal preto do Wokwi
  Serial.println("=================================");
  Serial.println("SISTEMA SAFEPET LIGADO (SEGURO)");
  Serial.println("Para simular a fuga do pet:");
  Serial.println("Clique nesta tela preta e aperte ENTER");
  Serial.println("=================================");
  
  delay(2000); 
}

void loop() {
  // Lê se você apertou ENTER no Terminal
  if (Serial.available() > 0) {
    String comando = Serial.readString(); 
    
    petFugiu = true;
    lat -= 0.0500; // Simula a quebra da cerca virtual
    lon += 0.0500;
    
    Serial.println("--> ATENCAO: Evento de fuga simulado!");
  }

  display.clearDisplay();
  
  if (!petFugiu) {
    // Estado Normal: Pet em casa
    display.setCursor(0, 0);
    display.println("Status: SEGURO");
    display.setCursor(0, 20);
    display.print("Lat: "); display.println(lat, 4);
    display.setCursor(0, 30);
    display.print("Lon: "); display.println(lon, 4);
    display.display(); // Atualiza a tela
    
    noTone(BUZZER_PIN); // Garante o silêncio absoluto
    delay(100);
    
  } else {
    // Estado de Alerta: Pet saiu da Cerca Virtual
    display.setCursor(0, 0);
    display.println("ALERTA: PET FUGIU!");
    display.setCursor(0, 20);
    display.print("Lat: "); display.println(lat, 4);
    display.setCursor(0, 30);
    display.print("Lon: "); display.println(lon, 4);
    display.display(); // Atualiza a tela antes do alarme tocar
    
    // Sirene manual (corrige o problema do hardware ESP32)
    tone(BUZZER_PIN, 1000); // LIGA o som a 1000Hz
    delay(300);             // Fica tocando por 300 milissegundos
    noTone(BUZZER_PIN);     // DESLIGA o som
    delay(300);             // Fica em silêncio por 300 milissegundos (efeito de bipe intermitente)
  }
}