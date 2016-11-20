/* Control Automatico de Invernadero
   Creditos: Urtzi del Val
   Fecha: 20/11/2016               */

/*-----( Librerias )-----*/

#include <LiquidCrystal_I2C.h>      // Libreria de la pantalla LCD
#include <DHT.h>                    // Libreria del sensor de Humedad y Temperatura DHT22
#include <Wire.h>                   // Libreria para comunicaciones I2C (Utilizado en el conector entre el LCD y el Arduino UNO)

/*-----( Declaraciones )-----*/

LiquidCrystal_I2C lcd(0x27, 16, 2); // Direccion del LCD 0x27 para pantalla de 16 caracteres y 2 lineas

int HumedadTierra = A0;             // Resultado de la lectura del pin Analógico (A0)

#define pinLDR A1                   // Pin analógico al que conectamos el LDR (A1)
int valorLDR = 0;                   // Resultado de la lectura del pin Analógico (A1)

/*-----( Declaramos el Pin del DHT )-----*/
#define DHTPIN 10                  //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22              //Se selecciona el DHT22
DHT dht(DHTPIN, DHTTYPE);          //Se inicia una variable que será usada por Ard

/*----( Configuracion Inicial )----*/

void setup()
{
  Serial.begin(9600);         //Activa el monitor serie en 9600
  lcd.begin();                //Enciende la pantalla
  lcd.backlight();            //Enciende la luz de la pantalla
  dht.begin();                //Se inicia el DHT22

  pinMode(10, OUTPUT);        //Pin 10 para Sensor (Humedad y Temperatura)

  //pinMode(9, OUTPUT);       //Pin 9 para Rele 1 (VACÍO para futura ampliación)
  pinMode(11, OUTPUT);        //Pin 11 para Rele 2 (Iluminación LED)
  pinMode(12, OUTPUT);        //Pin 12 para Rele 4 (Ventilacion Entrada Aire Fresco)
  pinMode(13, OUTPUT);        //Pin 13 para Rele 3 (Bomba del Agua)

}
/*--( Fin de Configuracion )---*/

/*----( LOOP: Funciona Constantemenete )----*/

void loop()
{
  float hume = dht.readHumidity();       // Lee la humedad relativa
  float temp = dht.readTemperature();    // Lee la temperatura ambiental
  valorLDR = analogRead(pinLDR);         // Lee el valor del LDR

  /*----( Muestra en Pantalla )----*/

  lcd.setCursor(0, 0);

  lcd.print("Temp:     ");      // Muestra el texto entre las comillas en el LCD
  lcd.print(temp);              // Muestra el valor que corresponde a la variable en el LCD
  lcd.print("c");               // Muestra el texto entre las comillas en el LCD

  lcd.setCursor(0, 1);

  lcd.print("Hume.Rel: ");      // Muestra el texto entre las comillas en el LCD
  lcd.print(hume);              // Muestra el valor que corresponde a la variable en el LCD
  lcd.print("%");               // Muestra el texto entre las comillas en el LCD

  delay(2000);  // Hace una pausa de 2s
  lcd.clear();  // Borra la pantalla

  /*  Condicionales para el Riego   */

  if (analogRead(A0) >= 807 && analogRead(A0) <= 1023) //1023 corresponde al 0% de Humedad
  {
    digitalWrite(13, LOW);            // Activa el pin Digital 13
    lcd.setCursor(0, 0);              // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("H.Tierra:  0-25%");    // Muestra el texto entre las comillas en el LCD
  }
  else if (analogRead(A0) >= 588 && analogRead(A0) <= 806)
  {
    digitalWrite(13, LOW);            // Activa el pin Digital 13
    lcd.setCursor(0, 0);              // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("H.Tierra: 25-50%");    // Muestra el texto entre las comillas en el LCD
  }
  else if (analogRead(A0) >= 369 && analogRead(A0) <= 587)
  {
    digitalWrite(13, HIGH);           // Desactiva el pin Digital 13
    lcd.setCursor(0, 0);              // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("H.Tierra: 50-75%");    // Muestra el texto entre las comillas en el LCD
  }
  else if (analogRead(A0) >= 150 && analogRead(A0) <= 368) //150 aprox corresponde al 100% de Humedad
  {
    digitalWrite(13, HIGH);           // Desactiva el pin Digital 13
    lcd.setCursor(0, 0);              // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("HTierra: 75-100%");    // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales Riego*/

  
  /* Condicionales para Iluminación LED */

  // Niveles: 1023 Oscuridad - 31 Luz

  if (valorLDR >= 500)             // Si el valor del LDR supera los 500 se encienden los LED
  {
    digitalWrite(11, LOW);         // Activa el pin Digital 11
    lcd.setCursor(0, 1);           // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Luz LED: ON");      // Muestra el texto entre las comillas en el LCD
  }

  else if (valorLDR <= 499)        // Si el valor del LDR es inferior a los 500 se apagan los LED
  {
    digitalWrite(11, HIGH);        // Desactiva el pin Digital 11
    lcd.setCursor(0, 1);           // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Luz LED: OFF");     // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Iluminación LED */
  

  delay(2000);  // Hace una pausa de 2s
  lcd.clear();  // Borra la pantalla


  /* Condicionales para Ventilacion   */

  if (temp >= 27)                       // Si la temperatura es superior a 27º se activa el ventilador
  {
    digitalWrite(12, LOW);              // Activa el pin Digital 12
    lcd.setCursor(0, 0);                // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("Ventilacion:  ON");      // Muestra el texto entre las comillas en el LCD
  }
  else if (temp <= 26)              // Si la temperatura es inferior a 26º se desactiva el ventilador
  {
    digitalWrite(12, HIGH);             // Desactiva el pin Digital 12
    lcd.setCursor(0, 0);                // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("Ventilacion: OFF");      // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Ventilacion */

  /* Condicionales para Apertura de Ventana */

  if (temp >= 27)                       // Si la temperatura es superior a 27º se abre la ventana
  {
    lcd.setCursor(0, 1);                // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Ventana: Abierta");      // Muestra el texto entre las comillas en el LCD
  }

  else if (temp <= 26)                  // Si la temperatura es inferior a 26º se cierra la ventana
  {
    lcd.setCursor(0, 1);                // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Ventana: Cerrada");      // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Apertura de Ventana */

  delay(2000);  // Hace una pausa de 2s
  lcd.clear();  // Borra la pantalla
  
  /*----( Muestra en el Monitor Serie )----

    Serial.print("Temperatura (C): ");
    Serial.println(temp);

    Serial.print("Humedad (%): ");
    Serial.println(temp);

    Serial.println(digitalRead(10)); //lectura digital

    delay(1000);
    }

    /* --( Final del LOOP )-- */

}
