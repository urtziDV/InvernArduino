/* Control Automatico de Invernadero
   Creditos: Urtzi del Val
   Fecha: 25/12/2016               */

/*-----( Librerias )-----*/

#include <LiquidCrystal_I2C.h>      // Libreria de la pantalla LCD
#include <DHT.h>                    // Libreria del sensor de Humedad y Temperatura DHT22
#include <Wire.h>                   // Libreria para comunicaciones I2C (Utilizado en el conector entre el LCD y el Arduino UNO)

/*-----( Declaraciones )-----*/

//SoftwareSerial BT1(4, 2);           // Nombre Bluetooth y pines RX, TX recorder que se cruzan

LiquidCrystal_I2C lcd(0x27, 16, 2);   // Direccion del LCD 0x27 para pantalla de 16 caracteres y 2 lineas
#define pinLDR A1                     // Pin analógico al que conectamos el LDR (A1)
            
int temp;
int luminosidad;                  
int bluetooth;                       
int humetierra = A0;                  // Resultado de la lectura del pin Analógico (A0)

/*-----( Declaramos el Pin del DHT )-----*/
#define DHTPIN 10                  //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22              //Se selecciona el DHT22
DHT dht(DHTPIN, DHTTYPE);          //Se inicia una variable que será usada por Arduino

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
  luminosidad = analogRead(pinLDR);      // Lee el valor del LDR
  char bluetooth = Serial.read();
  humetierra = map(humetierra, 0, 1023, 0, 100);       // Utiliza la funcion de mapeo para establecer que el valor de lectura 0 corresponde al 100% y el 1023 al 0%
  luminosidad = map(luminosidad, 0, 1023, 100, 0);       // Utiliza la funcion de mapeo para establecer que el valor de lectura 0 corresponde al 100% y el 1023 al 0%


    /*----( Muestra en Pantalla )----*/

    lcd.setCursor(0, 0);

    lcd.print("Temp:     ");           // Muestra el texto entre las comillas en el LCD
    lcd.print(temp);                   // Muestra el valor que corresponde a la variable en el LCD
    lcd.print("c");                    // Muestra el texto entre las comillas en el LCD

    lcd.setCursor(0, 1);

    lcd.print("Hume.Rel: ");           // Muestra el texto entre las comillas en el LCD
    lcd.print(hume);                   // Muestra el valor que corresponde a la variable en el LCD
    lcd.print("%");                    // Muestra el texto entre las comillas en el LCD

    delay(2000);                       // Hace una pausa de 2s
    lcd.clear();                       // Borra la pantalla

    /*  Condicionales para el Riego   */


    if (humetierra >= 0 && humetierra <= 50)          // Entre el 0% y 50% de humedad, se activa el riego
    {
      digitalWrite(13, LOW);                          // Activa el pin Digital 13
      lcd.setCursor(0, 0);                            // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
      lcd.print("H.Tierra:   ");                      // Muestra el texto entre las comillas en el LCD
      lcd.print(humetierra);
      lcd.print("%");
    }
    else if (humetierra >= 51 && humetierra <= 100)    // Entre el 51% y 100% de humedad, se desactiva el riego
    {
      digitalWrite(13, HIGH);                          // Desactiva el pin Digital 13
      lcd.setCursor(0, 0);                             // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
      lcd.print("H.Tierra:   ");                       // Muestra el texto entre las comillas en el LCD
      lcd.print(humetierra);
      lcd.print("%");
    }
 

  /* Fin Condicionales Riego*/


  /* Condicionales para Iluminación LED */

  // Niveles: 1023 Oscuridad - 0 Luz

  if (luminosidad <= 20)           // Si la luminosidad es inferior al 20% se encienden los LED
  {
    digitalWrite(11, LOW);         // Activa el pin Digital 11
    lcd.setCursor(0, 1);           // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Luz LED: ON");      // Muestra el texto entre las comillas en el LCD
  }

  else if (luminosidad >= 21)      // Si la luminosidad es superior al 21% se apagan los LED
  {
    digitalWrite(11, HIGH);        // Desactiva el pin Digital 11
    lcd.setCursor(0, 1);           // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Luz LED: OFF");     // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Iluminación LED */


  delay(2000);  // Hace una pausa de 2s
  lcd.clear();  // Borra la pantalla


  /* Condicionales para Ventilacion   */
  
  if (temp >= 21)                       // Si la temperatura es superior a 27º se activa el ventilador
  {
    digitalWrite(12, LOW);              // Activa el pin Digital 12
    lcd.setCursor(0, 0);                // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("Ventilacion:  ON");      // Muestra el texto entre las comillas en el LCD
  }
  else if (temp <= 20)              // Si la temperatura es inferior a 26º se desactiva el ventilador
  {
    digitalWrite(12, HIGH);             // Desactiva el pin Digital 12
    lcd.setCursor(0, 0);                // Se situa en el Caracter 0 de la Fila 0 para la escritura en el LCD
    lcd.print("Ventilacion: OFF");      // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Ventilacion */

  /* Condicionales para Apertura de Ventana */

  if (temp >= 21)                       // Si la temperatura es superior a 27º se abre la ventana
  {
    lcd.setCursor(0, 1);                // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Ventana: Abierta");      // Muestra el texto entre las comillas en el LCD
  }

  else if (temp <= 20)                  // Si la temperatura es inferior a 26º se cierra la ventana
  {
    lcd.setCursor(0, 1);                // Se situa en el Caracter 0 de la Fila 1 para la escritura en el LCD
    lcd.print("Ventana: Cerrada");      // Muestra el texto entre las comillas en el LCD
  }

  /* Fin Condicionales para Apertura de Ventana */

  delay(2000);  // Hace una pausa de 2s
  lcd.clear();  // Borra la pantalla

  /* Lectura Serial */


  Serial.println("Humedad Tierra: ");
  Serial.println(humetierra);

  Serial.println("Humedad Tierra Mapeo: ");
  Serial.println(humetierra);
  
  Serial.println("Humedad Ambiental: ");
  Serial.println(hume);

  Serial.println("Temperatura: ");
  Serial.println(temp);

  Serial.println("Luminosidad: ");
  Serial.println(luminosidad);

  Serial.println("--------------------------");
  
  delay(1); 

  /* --( Final del LOOP )-- */
}

