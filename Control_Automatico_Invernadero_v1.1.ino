/* Control Automatico de Invernadero
   Creditos: Urtzi del Val
   Fecha: 07/09/2016                 */

/*-----( Librerias )-----*/

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>

/*-----( Declaraciones )-----*/

LiquidCrystal_I2C lcd(0x27, 16, 2); // Direccion del LCD 0x27 para pantalla de 16 caracteres y 2 lineas
int HumedadTierra = A0;

/*-----( Declaramos el Pin del DHT )-----*/
#define DHTPIN 10                   //Seleccionamos el pin en el que se conectará el sensor
#define DHTTYPE DHT22              //Se selecciona el DHT22
DHT dht(DHTPIN, DHTTYPE);  //Se inicia una variable que será usada por Ard

/*----( Configuracion Inicial )----*/

void setup()
{
  Serial.begin(9600);      //Activa el Serial en 9600
  lcd.begin();             //Enciende la pantalla
  lcd.backlight();         //Enciende la luz de la pantalla
  dht.begin();             //Se inicia el DHT22

  //pinMode(10, INPUT);      //Pin 10 para Sensor DHT22 (Humedad y Temperatura)
  pinMode(11, OUTPUT);     //Pin 11 para Rele 2 (Ventilacion Caliente Fuera)
  pinMode(12, OUTPUT);     //Pin 12 para Rele 4 (Ventilacion Fresco Dentro)
  pinMode(13, OUTPUT);     //Pin 13 para Rele 3 (Bomba del Agua)

}
/*--( Fin de Configuracion )---*/

/*----( LOOP: Funciona Constantemenete )----*/

void loop()
{
  float hume = dht.readHumidity(); //Se lee la humedad
  float temp = dht.readTemperature(); //Se lee la temperatura

/*----( Muestra en Pantalla )----*/

lcd.setCursor(0, 0);

delay(3000);
lcd.clear();
  
lcd.print("Temp:     ");
lcd.print(temp);
lcd.print("c");

lcd.setCursor(0, 1);

lcd.print("Hume.Rel:  ");
lcd.print(hume);
lcd.print("%");

delay(3000);
lcd.clear();

/*  Condicionales para el Riego   */

if (analogRead(A0) >= 807 && analogRead(A0) <= 1023) //1023 corresponde al 0% de Humedad
{
  digitalWrite(13, LOW);
  lcd.setCursor(0, 0);
  lcd.print("H.Tierra:  0-25");
  lcd.print("%");
}
else if (analogRead(A0) >= 588 && analogRead(A0) <= 806)
{
  digitalWrite(13, LOW);
  lcd.setCursor(0, 0);
  lcd.print("H.Tierra: 25-50");
  lcd.print("%");
}
else if (analogRead(A0) >= 369 && analogRead(A0) <= 587)
{
  digitalWrite(13, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("H.Tierra: 50-75");
  lcd.print("%");
}
else if (analogRead(A0) >= 150 && analogRead(A0) <= 368) //150 aprox corresponde al 100% de Humedad
{
  digitalWrite(13, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("HTierra: 75-100");
  lcd.print("%");
}

lcd.setCursor(0, 1);
lcd.print("Valor:       ");
lcd.print(analogRead(A0)); //lectura analógica

delay(3000);
lcd.clear();

/* Fin Condicionales Riego*/

/* Condicionales para Ventilacion   */

if (temp >= 27) // Si la temperatura es superior a 27º se activa el ventilador
{
  digitalWrite(12, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Ventilacion:  ON");
}
else if (temp <= 26) // Si la temperatura es inferior a 26º se desactiva el ventilador
{
  digitalWrite(12, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Ventilacion: OFF");
}

/* Fin Condicionales para Ventilacion */

/* Condicionales para Apertura de Ventana */

if (temp >= 27) // Si la temperatura es superior a 27º se abre la ventana
{
  lcd.setCursor(0, 1);
  lcd.print("Ventana: Abierta");
}

else if (temp <= 26) // Si la temperatura es inferior a 26º se cierra la ventana
{
  lcd.setCursor(0, 1);
  lcd.print("Ventana: Cerrada");
}

/* Fin Condicionales para Apertura de Ventana */


/*----( Muestra en el Monitor Serie )----

  Serial.print("Temperatura (C): ");
  Serial.println(temp);

  Serial.print("Humedad (%): ");
  Serial.println(temp);

  Serial.println(digitalRead(10)); //lectura digital

  delay(1000);
  }
*/


/* Condicionales para la Ventilacion

  /* --( Final de Condicionales Ventilacion )-- */

/* --( Final del LOOP )-- */
}

