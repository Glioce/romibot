/**********************************
  Robot Siguelíneas (ROMIBOT) V2

  Historial de versiones
  v1 probar celda de carga
  v2 agregar LCD modo 4 bits
********************************/

// LCD
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//BASCULA
#include "HX711.h" //https://github.com/bogde/HX711
//HX711 balanza;
HX711 bascula;
const int DOUT = A1;
const int CLK = A0;
long lecturaBascula;

//MOTORES
// Definición de variables y constantes relacionadas con los motores
const int IZQ1 = 13;  // Pin digital 13 para controlar sentido giro motor izquierdo
const int IZQ2 = 12;  // Pin digital 12 para controlar sentido giro motor izquierdo

//SENSOR
// Definición de variables y constantes relacionadas con los sensores IR
int lecturaSensorIzq; // Almacena el valor de la lectura del sensor izquierdo
int lecturaSensorDer; //Valores para el sen. Der
const int sensorIzqPin = A1; // El sensor izq irá conectado al pin analógico A0
const int sensorDerPin = A0; //es a donde va conectado el sensor

void setup() {
  Serial.begin(9600);

  // LCD
  lcd.begin(16, 2); //16 columnas, 2 filas
  lcd.print("ROMIBOT LISTO"); //mensaje inicial

  //MOTOR
  // Se declaran todos los pines como salidas
  // Pines asociados a los motores
  pinMode (IZQ1, OUTPUT);
  pinMode (IZQ2, OUTPUT);
  pinMode(sensorIzqPin  , INPUT) ;
  pinMode(sensorDerPin  , INPUT) ;
  Serial.begin(9600); // Se inicia el puerto de comunicaciones en serie

  //BASCULA
  bascula.begin(DOUT, CLK);
  Serial.print("Lectura del valor del ADC:t");
  lecturaBascula = bascula.read();
  Serial.println(lecturaBascula);
  //Serial.println("No colocar objetos sobre la placa");
  //Serial.println("Destarando...");
  //balanza.set_scale(); //Escala = 1
  //balanza.tare(20);  //Tara
  //Serial.println("Coloque un peso conocido:");
  // imprimir lectura en LCD
  lcd.setCursor(0, 1);
  lcd.print(lecturaBascula);
}

void loop() {
  lecturaSensorIR(); // Se lee el valor de los sensores IR
  // Se analiza el resultado de los sensores para hacer que el robot siga la línea negra

  // Si el resultado de ambos sensores es 0 (zona blanca) el robot sigue se para
  if (lecturaSensorIzq == 0 && lecturaSensorDer == 0)
  {
    robotParar(); // El robot para

  }
  // Si el izquierdo retorna 0 (zona blanca) y el derecho 1 (negra) el robot gira derecha
  if (lecturaSensorIzq == 0 && lecturaSensorDer == 1)
  {
    robotDerecha();
    // El robot gira a la derecha

  }
  // Si el izquierdo retorna 1 (zona negra) y el derecho 0 (blanca) el robot gira izquierda
  if (lecturaSensorIzq == 1 && lecturaSensorDer == 0)
  {
    robotIzquierda();

  }
  // Si ambos sensores retornan 0 (zona negra) el robot sigue recto
  if (lecturaSensorIzq == 1 && lecturaSensorDer == 1)
  {
    robotAvance(); // El robot avanza
    Serial.println("robot avanza");
  }

  // Obtener nueva lectura de celda de carga
  lecturaBascula = bascula.read();
  lcd.setCursor(0, 1);
  lcd.print(lecturaBascula);
  delay(100);
}
/*
  Función lecturaSensorIR: leerá el valor del sensor de infrarrojos TCRT5000
  y lo almacena en una variable. Dicho sensor retornará el valor 0 (LOW) si
  el sensor está en zona blanca y el valor 1 (HIGH) si el sensor está en zona
  negra.
*/
void lecturaSensorIR()
{
  lecturaSensorIzq = digitalRead(sensorIzqPin); // Almacena la lectura del sensor izquierdo
  lecturaSensorDer = digitalRead(sensorDerPin); // Almacena la lectura del sensor derecho

  Serial.println("El valor del sensor izquierdo es ");
  Serial.println(lecturaSensorIzq);

  Serial.println("El valor del sensor derecho es ");
  Serial.println(lecturaSensorDer);

}

void lecturaBalanza()
{
  //lecturaBalanza = digitalRead(DOUT
}
/*
  Función robotAvance: esta función hará que ambos motores se activen a máxima potencia
  por lo que el robot avanzará hacia delante
*/
void robotAvance()
{
  // Motor izquierdo
  // Al mantener un pin HIGH y el otro LOW el motor gira en un sentido
  digitalWrite (IZQ1, HIGH);
  digitalWrite (IZQ2, LOW);

}
/*
  Función robotRetroceso: esta función hará que ambos motores se activen a máxima potencia
  en sentido contrario al anterior por lo que el robot avanzará hacia atrás
*/
void robotRetroceso()
{
  // Motor izquierdo
  // Al mantener un pin LOW y el otro HIGH el motor gira en sentido contrario al anterior
  digitalWrite (IZQ1, LOW);
  digitalWrite (IZQ2, HIGH);

}

/*
  Función robotDerecha: esta función acccionará el motor izquierdo y parará el derecho
  por lo que el coche girará hacia la derecha (sentido horario)
*/
void robotDerecha()
{
  //  Motor izquierdo
  // Se activa el motor izquierdo
  digitalWrite (IZQ1, HIGH);
  digitalWrite (IZQ2, LOW);

}
/*
  Función robotIzquierda: esta función acccionará el motor derecho y parará el izquierdo
  por lo que el coche girará hacia la izquierda (sentido antihorario)
*/
void robotIzquierda ()
{
  //  Motor izquierdo
  // Se para el motor izquierdo
  digitalWrite (IZQ1, LOW);
  digitalWrite (IZQ2, HIGH);

}
/*
  Función robotParar: esta función parará ambos motores
  por lo que el robot se parará.
*/
void robotParar()
{
  // Motor izquierdo
  // Se para el motor izquierdo
  digitalWrite (IZQ1, LOW);
  digitalWrite (IZQ2, LOW);

  //BASCULA
  Serial.print("Valor de lectura: t");
  Serial.println(bascula.get_value(10), 0);
  delay(500);
}
