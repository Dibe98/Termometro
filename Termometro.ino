// Includo libreria LCD
#include <LiquidCrystal.h>
// Inizializzo l'LCD 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Dichiaro le variabili di tipo intero
const int sensorTemp = A0; // Sensore temperatura
const int piezo = 10; // Piezo
const int reset = 6; // Pin per il reser
const int GREEN = 9; // LED Verde
const int YELLOW = 8; // LED Giallo
const int RED = 7; // LED Rosso
int val_Adc = 0; // Variabile per leggere 
const float tempOkLimit = 27.0; // Limite massimo per definire la temperatura OK
const float tempNormalLimit = 37.0; // Limite massimo per definire la temperatura nella norma (NORMALE)
const float tempCriticLimit = 43.5; // Limite massimo per definire la temperatura CRITICA superato questo si passa a PERICOLO
float temp = 0; // variabile per calcolare la temperatura

void(* Riavvia)(void) = 0; // Funzione del reset

void tempOk(float); // Funzione per settare i componenti ad indicare che la temperatura è OK

void tempNormal(float);  // Funzione per settare i componenti ad indicare che la temperatura è NORMALE

void tempCritic(float);  // Funzione per settare i componenti ad indicare che la temperatura è CRITICA

void tempDanger(float);   // Funzione per settare i componenti ad indicare che la temperatura è PERICOLOSA


void setup()
{
  // ciclo per settare i pin 7,8,9 in modalità OUTPUT e livello LOW
 for(int pinNumber = RED; pinNumber <= GREEN; pinNumber++)
 {
  pinMode(pinNumber, OUTPUT);
  digitalWrite(pinNumber, LOW);
 }

 pinMode(reset, INPUT); // setto il pin 6 in modalità INPUT
 lcd.begin(16,2); // Inizializzo l' LCD
 Serial.begin(9600); //init seriale
 //utilizzando l'istruzione analogReference
 //indico al convertitore AD che deve impiegare
 //la tensione presente sul pin AREF come
 //valore di riferimento per la conversione
 analogReference(EXTERNAL);

 // TESTO TUTTI I COMPONENTI
 tempOk(tempOkLimit);
 delay(1500);
 tempNormal(tempNormalLimit);
 delay(1500);
 tempCritic(tempCriticLimit);
 delay(1500);
 tempDanger(tempCriticLimit);
 delay(1000);
}

void loop()
{
 //ritardo un secondo
 delay(1000);
 // Controllo se il pulsante per il reset è premuto
 if(digitalRead(reset) == HIGH){
   // Se è premuto riavvio Arduino
   Riavvia();
 }
 //init variabile
 val_Adc = 0;
 //eseguo un ciclo
 for(byte Ciclo = 0; Ciclo<100; Ciclo++)
 {
   //acquisisco il valore e lo sommo alla variabile
   val_Adc += analogRead(sensorTemp);
   //questo ritardo serve per dare il tempo all' ADC di eseguire correttamente la prossima acquisizione
   delay(10);
 }

 //eseguo la media dei 100 valori letti
 val_Adc /= 100;
 //calcolo la temperatura in °C
 temp = ((val_Adc * 0.0032) - 0.5) / 0.01;
 //invio i dati al computer
 Serial.println(temp);
 if (temp <= tempOkLimit){
   tempOk(temp);
 } else {
  if (temp <= tempNormalLimit){
    tempNormal(temp);
  } else {
    if (temp <= tempCriticLimit){
      tempCritic(temp);
    }
    else {
      tempDanger(temp);
    }
  }
 }

}

void tempOk(float temp){
  // Pulisco l'LCD
  lcd.clear();
  // Imposto il cursore sull'angolo in altro a sinistra
  lcd.setCursor(0,0);
  // Stampo sull'LCD la parola 'Temperatura'
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print("C OK");
  digitalWrite(GREEN, HIGH); // LED verde acceso
  digitalWrite(YELLOW, LOW); // LED giallo spento
  digitalWrite(RED, LOW); // LED rosso spento
  lcd.setCursor(0,1);
  // Invio i dati all'lcd
  lcd.print(temp);
}

void tempNormal(float temp){
  // Pulisco l'LCD
  lcd.clear();
  // Imposto il cursore sull'angolo in altro a sinistra
  lcd.setCursor(0,0);
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print("C NORM.");
  digitalWrite(GREEN, LOW); // LED verde spento
  digitalWrite(YELLOW, HIGH); // LED giallo acceso
  digitalWrite(RED, LOW); // LED rosso spento
  lcd.setCursor(0,1);
  // Invio i dati all'lcd
  lcd.print(temp);
}

void tempCritic(float temp){
  // Pulisco l'LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print("C CRITICO");
  digitalWrite(GREEN, LOW); // LED verde spento
  digitalWrite(YELLOW, LOW); // LED giallo spento
  digitalWrite(RED, HIGH); // LED rosso acceso
  lcd.setCursor(0,1);
  // Invio i dati all'lcd
  lcd.print(temp);
}

void tempDanger(float temp){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print("C PERICOLO");
  digitalWrite(GREEN, HIGH); // LED verde acceso
  digitalWrite(YELLOW, HIGH); // LED giallo acceso
  digitalWrite(RED, HIGH); // LED rosso acceso
  lcd.setCursor(0,1);
  // Invio i dati all'lcd
  lcd.print(temp);
  tone(piezo,8200,450); // Emetto un suono col piezo
  delay(500);
  tone(piezo,8200,450);
}
