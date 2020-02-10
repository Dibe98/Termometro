#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int val_Adc = 0;
float temp = 0;
const int GREEN = 9;
const int YELLOW = 8;
const int RED = 7;
const float tempOkLimit = 20.0;
const float tempCriticLimit = 30.0;
const float tempDanger = 38.0;
const int piezo = 10;
const int reset = 6;

void(* Riavvia)(void) = 0;

void setup()
{
 for(int pinNumber = RED; pinNumber <= GREEN; pinNumber++)
 {
  pinMode(pinNumber, OUTPUT);
  digitalWrite(pinNumber, LOW);
 }

 pinMode(reset, INPUT);
 lcd.begin(16,2);
 lcd.print("Temperatura");
 lcd.setCursor(6,1);
 lcd.print("C  ");
 //init seriale
 Serial.begin(9600);
 //utilizzando l'istruzione analogReference
 //indico al convertitore AD che deve impiegare
 //la tensione presente sul pin AREF come
 //valore di riferimento per la conversione
 analogReference(EXTERNAL);

 // TESTO TUTTI I COMPONENTI
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST COMP");
  lcd.setCursor(6,1);
  lcd.print("C OK");
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST COMP");
  lcd.setCursor(6,1);
  lcd.print("C NORM.");
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, LOW);
  lcd.setCursor(0,1);
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST COMP");
  lcd.setCursor(6,1);
  lcd.print("C CRITICO");
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH);
  delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TEST COMP");
  lcd.setCursor(6,1);
  lcd.print("C PERICOLO");
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, HIGH);
  lcd.setCursor(0,1);
  tone(piezo,8200,450);
}

void loop()
{
 //ritardo di mezzo secondo
 delay(500);
 if(digitalRead(reset) == HIGH){
  Riavvia();
 }
 //init variabile
 val_Adc = 0;
 //eseguo un ciclo
 for(byte Ciclo = 0; Ciclo<100; Ciclo++)
 {
  //acquisisco il valore e lo sommo alla
  //variabile
  val_Adc += analogRead(0);
  //questo ritardo serve per dare il tempo
  //all' ADC di eseguire correttamente
  //la prossima acquisizione
  delay(10);
 }

 //eseguo la media dei 100 valori letti
 val_Adc /= 100;
 //calcolo la temperatura in °C
 temp = ((val_Adc * 0.0032) - 0.5) / 0.01;
 //invio i dati al computer
 Serial.println(temp);


 if (temp <= tempOkLimit){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperatura");
  lcd.setCursor(6,1);
  lcd.print("C OK");
  digitalWrite(GREEN, HIGH);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
  lcd.setCursor(0,1);
  // Invio i dati all'lcd
  lcd.print(temp);
 } else {
  if (temp <= tempCriticLimit){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura");
    lcd.setCursor(6,1);
    lcd.print("C NORM.");
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
    lcd.setCursor(0,1);
    // Invio i dati all'lcd
    lcd.print(temp);
  } else {
    if (temp <= tempDanger){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temperatura");
      lcd.setCursor(6,1);
      lcd.print("C CRITICO");
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(RED, HIGH);
      lcd.setCursor(0,1);
      // Invio i dati all'lcd
      lcd.print(temp);
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temperatura");
      lcd.setCursor(6,1);
      lcd.print("C PERICOLO");
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(RED, HIGH);
      lcd.setCursor(0,1);
      // Invio i dati all'lcd
      lcd.print(temp);
      tone(piezo,8200,450);
    }
  }
 }

 
}
