/* Control Automatico de Invernadero
   Creditos: Urtzi del Val
   Fecha: 2016
*/

/*-----( Librerias Necesarias )-----*/
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1302.h>
#include <Time.h>

/*-----( Declarar )-----*/

LiquidCrystal_I2C lcd(0x27, 16, 2); // Direccion del LCD 0x27 para pantalla de 16 caracteres y 2 lineas
dht11 DHT11;
DS1302 rtc(4,3,2);
int HumedadTierra = A0;

 /*-----( Declaramos el Pin del DHT )-----*/
#define DHT11PIN 11

/*----( Configuracion Inicial )----*/
void setup()
{
  Serial.begin(9600);      //Activa el Serial en 9600
  pinMode(10, INPUT);      //Pin 10 para Sensor DHT11 (Humedad y Temperatura)
  pinMode(11, OUTPUT);     //Pin 11 para Rele 4 (Servo Motores)
  pinMode(12, OUTPUT);     //Pin 12 para Rele 3 (Ventilacion)
  pinMode(13, OUTPUT);     //Pin 13 para Rele 2 (Bomba del Agua)  
  lcd.begin();             //Enciende la pantalla
  lcd.backlight();         //Enciende la luz de la pantalla  
  rtc.halt(false);         //Enciende el Reloj
  rtc.writeProtect(false); //Desactiva la proteccion contra escritura del Reloj

  //rtc.setTime(17, 12, 0); // Set the time to 12:00:00 (24hr format) // Configura la Hora del Reloj, tras cargarlo hay que comentarlo
  //rtc.setDate(07, 4, 2016); // Set the date to August 6th, 2010     // Configura la Fecha del Reloj, tras cargarlo hay que comentarlo
  //set_time();
  
}/*--( Fin de Configuracion )---*/

/*----( LOOP: Funciona Constantemenete )----*/

void loop()
{
  
  Serial.print(rtc.getTimeStr());   //Imprime la Hora por Serial
  Serial.print(" ");                //Imprime un Espacio
  Serial.println(rtc.getDateStr()); //Imprime la Fecha
    
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Lectura del Sensor: ");
  switch (chk)
  {
    case 0: Serial.println("CORRECTO"); break;
    case -1: Serial.println("ERROR Suma de Comprobacion"); break;
    case -2: Serial.println("ERROR Tiempo Agotado"); break;
    default: Serial.println("ERROR Desconocido"); break;
  }

  /*----( Muestra en Pantalla )----*/

  lcd.setCursor(0, 0);

  lcd.print("Temperatura: ");
  lcd.print((float)DHT11.temperature, 0);
  lcd.print("c");

  lcd.setCursor(0, 1);

  lcd.print("Humedad: ");
  lcd.setCursor(13, 1);
  lcd.print((float)DHT11.humidity, 0);
  lcd.print("%");

  delay(3000);
  lcd.clear();

  lcd.setCursor(0, 0);

  /*  Condicionales para el Riego   */

  if (analogRead(A0) >= 807 && analogRead(A0) <= 1023) //1023 corresponde al 0% de Humedad
  {
    digitalWrite(13, LOW);
    lcd.print("H.Tierra:  0-25");
    lcd.print("%");
  }
  else if (analogRead(A0) >= 588 && analogRead(A0) <= 806)
  {
    digitalWrite(13, LOW);
    lcd.print("H.Tierra:  25-50");
    lcd.print("%");
  }
  else if (analogRead(A0) >= 369 && analogRead(A0) <= 587)
  {
    digitalWrite(13, HIGH);
    lcd.print("H.Tierra:  50-75");
    lcd.print("%");
  }
  else if (analogRead(A0) >= 150 && analogRead(A0) <= 368) //150 aprox corresponde al 100% de Humedad
  {
    digitalWrite(13, HIGH);
    lcd.print("HTierra: 75-100");
    lcd.print("%");
  }
  /* Fin Condicionales Riego*/
  
  /* Condicionales para Ventilacion   */

  if (DHT11.temperature >= 27) // Si la temperatura es superior a 27º se activa el ventilador
  {
    digitalWrite(12, LOW);
    lcd.print("Ventilacion:  ON");
  }
  else if (DHT11.temperature <= 26) // Si la temperatura es inferior a 26º se desactiva el ventilador
  {
    digitalWrite(12, HIGH);
    lcd.print("Ventilacion:  OFF");
  }
  
  /* Fin Condicionales para Ventilacion */

  /* Condicionales para Apertura de Ventana */

  if (DHT11.temperature >= 27) // Si la temperatura es superior a 27º se activa el ventilador
  {
    digitalWrite(11, LOW);
    lcd.print("Ventana: Abierta");
  }
  else if (DHT11.temperature <= 26) // Si la temperatura es inferior a 26º se desactiva el ventilador
  {
    digitalWrite(11, HIGH);
    lcd.print("Ventana: Cerrada");
  }


  /* Fin Condicionales para Apertura de Ventana */

  lcd.setCursor(0, 1);
  lcd.print("Valor:      ");
  lcd.print(analogRead(A0)); //lectura analógica


  /*----( Muestra en el Monitor Serie )----*/

  Serial.print("Temperatura (C): ");
  Serial.println((float)DHT11.temperature, 2);

  Serial.print("Humedad (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.println(digitalRead(10)); //lectura digital

  delay(1000);
  lcd.clear();

  /* Condicionales para la Ventilacion 
  
  if Ventilar //Entre las 12:00 y las 20:00 mantiene abierta la ventana
  {
    digitalWrite(12, HIGH);
  }
  else // Antes hasta las desde las 20:00 hasta las 12:00 mantiene cerrada la ventana
  {
    digitalWrite(12, LOW);
  }

      /* Defino entre las 12:00 y las 20:00 mantiene abierta la Ventana*/
          
          //Para hacer las comparaciones de cuando empezar y cuando terminar, lo paso todo a minutos.

          Time t;
          t=getTime();
            
    if (t.hour >= 11 && t.hour <= 20) {      
        Serial.print("Ventilacion: ON");      
        digitalWrite(12 , HIGH);
        Serial.print(" "); 
        
    } else {
      
        Serial.print("Ventilacion: OFF");
        Serial.print(" "); 
        digitalWrite(12 , LOW) ;
    }
  
  /* --( Final de Condicionales Ventilacion )-- */

}/* --( Final del LOOP )-- */

