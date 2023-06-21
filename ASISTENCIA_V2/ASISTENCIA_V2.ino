
/* REFERENCIA:

  0 == AUSENTE
  1 == PRESENTE
  2 == TARDANZA
  3 == AUSENTE CON PERMANENCIA EN EL AULA


*/

#include <SPI.h>
#include <SD.h>
File myFile;
const byte chipSelect = 4;
#include <MFRC522.h>

String Preceptor[8][2] = {
  {"Lopez R.", "60 0B 8D 65"},
  {"Pesoa", "60 8C 28 2B"},
  {"Gonzalez J", "80 13 41 D5"},
  {"Gonzalez M", "63 C7 27 2B"},
  {"Victor", "9D 70 0F 2B"},
  {"Natalia", "80 84 8C 65"},
  {"Villasanti", "31 F9 8C 65"},
  {"Davila S.", "F6 53 2E 2B"}
};


bool flag_salida = 0;
String dataString;      // Cadena para almacenar datos leídos del archivo
String names, id;        // Cadena para almacenar nombres e IDs
const byte numRows = 30; // Número de filas de la matriz
const byte numCols = 2;  // Número de columnas de la matriz
byte encoderPin = 3;
boolean septimo;
boolean sexto;
boolean quinto;
boolean cuarto;
boolean tarde;
boolean manana;
boolean archivoCursoSelected = 0;
boolean curso_seleccionado = false;
String alumno_estado;
String dataArray[numRows][numCols];
MFRC522 rfid(53, 5);
byte Indice;
byte Indice_2 = -1;
boolean prueba = 0;
boolean  alerta = 0;
///////////////////////////////////////////////
#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(26, 24, 22); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);

char Ano[20];
char Mes[20];
char Dia[20];
char Hora[20];
char Minuto[20];

char Fecha [20];
char Tiempo [20];
char datestring[20];
boolean alumnos_main = 0;
byte VARH ;
byte VARM ;

byte hora;
byte minut;
//////////////////////////////////////////////

///////////LIBRERIAS//////////////////////////////////////////

#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>


//////////////////////////////////////////////////////////////
LiquidCrystal_I2C lcd(0x27, 20, 4);

//ENCODER
#define outputA 7
#define outputB 6
#define sw 2



byte OPEN = 11;
byte CLOSE = 12;
////////////////////////////////////////////////////////////////

byte incremento = 0;
byte aState;
byte aLastState;
unsigned long startime;
unsigned long startime1;
unsigned long startime2;
unsigned long msjt1;
unsigned long msjt2;
int TiempoAhora = 0;

byte fcarrera1 = 10;
byte fcarrera2 = 8;
//int pulsadorsd = 4;

/////////////////////
byte ESTADO = 0;
byte pres ;

byte ALUMNOS[40] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

byte Salida_alumn[40] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

String FALTANTES[40] = {
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null",
  "null", "null"
};



////////////////////

boolean protec = false;
boolean cosita = 0;
boolean sd_state = 1;

//////////////////////////LINEAS PRINCIPAL/////////////////////////////
LiquidLine linea1_P(1, 0, "PRINCIPAL:  ", VARH, ":", VARM);
LiquidLine linea1(1, 1, "Asistencia");
LiquidLine linea6(1, 2, "Curso");
LiquidLine linea2(1, 3, "Turno");
LiquidLine linea5(1, 6, "Archivos y SD");
LiquidLine linea3(1, 4, "Extras");
LiquidLine linea4(1, 5, "Modo Ahorro");


LiquidScreen pantalla1;

/////////////////////////LINEAS EXTRA/////////////////////////////////////////////////

LiquidLine RESET(1, 0, "Resetear archivo");
LiquidLine atras1(1, 1, "<<Atras");
LiquidScreen pantalla2;



///////////////LINEAS Turno ////////////////////////////////
LiquidLine Turnos(1, 0, "Elige un turno:");
LiquidLine Turnos_T(1, 1, "Tarde");
LiquidLine Turnos_M(1, 2, "Manana");
LiquidLine T_Atras(1, 3, "<<Atras");
LiquidScreen pantalla6;


///////////LINEAS SD///////////////////////


LiquidLine SDMENU1(1, 0, "Liberar SD ");
LiquidLine SDMENU2(1, 1, "Generar archivo");
LiquidLine SDMENU3(1, 2, "<<Atras");
LiquidScreen pantalla3;


//////////////////////////LINEAS AHORRO////////////////////////////////////

LiquidLine OnLight(1, 0, "Desactivar  ");
LiquidLine OffLight(1, 1, "Activar ");
LiquidLine atras(1, 2, "<<Atras");
LiquidScreen pantalla4;

///////////////LINEAS LECTOR///////////////////////////////////////////
LiquidLine lec1(1, 0, "Habilitar entrada");
LiquidLine lec2(1, 1, "Habilitar salida");
LiquidLine lec3(1, 2, "<<Atras");
LiquidScreen pantalla5;




///////////////LINEAS CURSO ////////////////////////////////
LiquidLine CURSOS(1, 0, "Elige un curso:");
LiquidLine CURSOS_4(1, 1, "Cuarto U");
LiquidLine CURSOS_5(1, 2, "Quinto U");
LiquidLine CURSOS_6(1, 3, "Sexto U");
LiquidLine CURSOS_7(1, 4, "Septimo U");
LiquidLine CURSO_ATRAS(1, 5, "<<Atras");
LiquidScreen pantalla7;




/////////////////////////////////////////////////////////////////////
LiquidMenu menu(lcd);

void setup() {


  Serial.begin(57600);

  myFile = SD.open("ej.txt", FILE_WRITE);
  myFile.close();


  //////////////////////////RTC///////////////////////////

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);
  pinMode(encoderPin, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  pinMode(fcarrera1, INPUT_PULLUP);
  pinMode(fcarrera2, INPUT_PULLUP);
  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected())
  {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning())
  {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }



  /////////////////////////SD//////////////////////////

  // Open serial communications and wait for port to open:
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  lcd.init();
  lcd.backlight();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(3, 1);
    lcd.print("BIENVENIDOS!");
    lcd.setCursor(3, 2);
    lcd.print("ELECTRONICOS");
  }

  limpia();

  startime = millis();
  while ((millis() - startime) < 2000) {
    lcd.setCursor(0, 1);
    lcd.print("INICIANDO SISTEMA...");
  }
  limpia();

  //////////////////////CASO ESPECIAL si la sd no esta al iniciar////////////////////////////

  while (!SD.begin(4)) {
boolean flag = 0;

    lcd.setCursor(0, 0);
    lcd.print("TARJETA INEXISTENTE");
    lcd.setCursor(5, 1);
    lcd.print("INSERTE SD");
    lcd.setCursor(4, 2);
    lcd.print("PARA INICIAR");
    
    while (digitalRead(fcarrera1) == HIGH) {
     
      analogWrite(OPEN, 140);
    
    }

    if (digitalRead(fcarrera1) == LOW and flag == 0) {
      //puerta abierta
      analogWrite(OPEN, 0); 


  }
  }
//////////////////////////////////////////////////////////////////////////////////
    if (SD.begin(4) and sd_state == 0) {
      msjsd();
      sd_state = 1;
    }

    //////////////////////////////////////////////////////////////////////////

    lcd.clear();

    delay(150);
    while (digitalRead(fcarrera2) == HIGH && SD.begin(4) ) {//cerrando puertas
      //servo.write(180);

      lcd.setCursor(6, 1);
      lcd.print("CERRANDO");
      lcd.setCursor(6, 2);
      lcd.print("COMPUERTA");
      analogWrite(CLOSE, 150);
    }


    if (digitalRead(fcarrera2) == LOW) {
      //puerta cerrada
      analogWrite(CLOSE, 0);
    }

    delay(250);
    COMclose();









  
  limpia();
  msjsd();
  limpia();
  ////////////////////////////////////////////////////////////////
  if (SD.begin(4)) {
    Serial.println("initialization correct!");
  }
  //////////////////////////////////////////////////

  // pinMode(pulsadorsd, INPUT_PULLUP);
  ///////////////////////////PANTALLA PRINCIPAL///////////////////////////////////////
  pantalla1.add_line(linea1_P);
  pantalla1.add_line(linea1);
  pantalla1.add_line(linea6);
  pantalla1.add_line(linea2);
  pantalla1.add_line(linea5);
  pantalla1.add_line(linea3);
  pantalla1.add_line(linea4);

  linea1_P.set_focusPosition(Position::CUSTOM);
  linea1.set_focusPosition(Position::LEFT);
  linea6.set_focusPosition(Position::LEFT);
  linea2.set_focusPosition(Position::LEFT);
  linea5.set_focusPosition(Position::LEFT);
  linea3.set_focusPosition(Position::LEFT);
  linea4.set_focusPosition(Position::LEFT);


  linea1_P.attach_function(1, fn_atras );
  linea1.attach_function(1, LECTOR);
  linea2.attach_function(1, TURNO);
  linea3.attach_function(1, EXTRAS);
  linea4.attach_function(1, MenuAhorro);
  linea5.attach_function(1, EXSD);
  linea6.attach_function(1, curso_selector);
  menu.add_screen(pantalla1);
  ////////////////////////////PANTALLA EXTRAS/////////////////////////////////////////
  pantalla2.add_line(RESET);
  pantalla2.add_line(atras1);
  RESET.set_focusPosition(Position::LEFT);
  atras.set_focusPosition(Position::RIGHT);
  RESET.attach_function(1, RES);
  atras1.attach_function(1, fn_atras);
  menu.add_screen(pantalla2);
  ///////////////////////////PANTALLA SD//////////////////////////////////////
  pantalla3.add_line(SDMENU1);
  pantalla3.add_line(SDMENU2);
  pantalla3.add_line(SDMENU3);
  SDMENU1.set_focusPosition(Position::LEFT);
  SDMENU2.set_focusPosition(Position::LEFT);
  SDMENU3.set_focusPosition(Position::RIGHT);
  SDMENU1.attach_function(1, SERVOSD);
  SDMENU2.attach_function(1, GEN);
  SDMENU3.attach_function(1, fn_atras);
  menu.add_screen(pantalla3);
  ////////////////////////////////PANTALLA AHORRO///////////////////////////////
  pantalla4.add_line(OnLight);
  pantalla4.add_line(OffLight);
  pantalla4.add_line(atras);
  OnLight.set_focusPosition(Position::LEFT);
  OffLight.set_focusPosition(Position::LEFT);
  atras.set_focusPosition(Position::RIGHT);
  OnLight.attach_function(1, ON);
  OffLight.attach_function(1, OFF);
  atras.attach_function(1, fn_atras);
  menu.add_screen(pantalla4);
  ///////////////////////////PANTALLA LECTOR///////////////////////////////
  pantalla5.add_line(lec1);
  pantalla5.add_line(lec2);
  pantalla5.add_line(lec3);
  lec1.set_focusPosition(Position::LEFT);
  lec2.set_focusPosition(Position::LEFT);
  lec3.set_focusPosition(Position::RIGHT);
  lec1.attach_function(1, lec_f);
  lec2.attach_function(1, LEC_SALIDA);
  lec3.attach_function(1, fn_atras);
  menu.add_screen(pantalla5);
  ///////////////////////////PANTALLA TURNO///////////////////////////////////////
  pantalla6.add_line(Turnos);
  pantalla6.add_line(Turnos_T);
  pantalla6.add_line(Turnos_M);
  pantalla6.add_line(T_Atras);
  Turnos.set_focusPosition(Position::CUSTOM);
  Turnos_T.set_focusPosition(Position::LEFT);
  Turnos_M.set_focusPosition(Position::LEFT);
  T_Atras.set_focusPosition(Position::LEFT);
  Turnos.attach_function(1, V);
  Turnos_T.attach_function(1, T_tarde);
  Turnos_M.attach_function(1, T_man);
  T_Atras.attach_function(1, fn_atras);
  menu.add_screen(pantalla6);
  ///////////////////////////PANTALLA CURSOS////////////
  pantalla7.add_line(CURSOS);
  pantalla7.add_line(CURSOS_4);
  pantalla7.add_line(CURSOS_5);
  pantalla7.add_line(CURSOS_6);
  pantalla7.add_line(CURSOS_7);
  pantalla7.add_line(CURSO_ATRAS);
  CURSOS.set_focusPosition(Position::CUSTOM);
  CURSOS_4.set_focusPosition(Position::LEFT);
  CURSOS_5.set_focusPosition(Position::LEFT);
  CURSOS_6.set_focusPosition(Position::LEFT);
  CURSOS_7.set_focusPosition(Position::LEFT);
  CURSO_ATRAS.set_focusPosition(Position::LEFT);
  CURSOS.attach_function(1, V);
  CURSOS_4.attach_function(1, CURSOS_cuarto);
  CURSOS_5.attach_function(1, CURSOS_quinto);
  CURSOS_6.attach_function(1, CURSOS_sexto);
  CURSOS_7.attach_function(1, CURSOS_septimo);
  CURSO_ATRAS.attach_function(1, fn_atras);
  menu.add_screen(pantalla7);
  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);
  pantalla3.set_displayLineCount(3);
  pantalla4.set_displayLineCount(3);
  pantalla5.set_displayLineCount(3);
  pantalla6.set_displayLineCount(4);
  pantalla7.set_displayLineCount(4);
  menu.init();
  menu.set_focusedLine(0);
  menu.update();
  SPI.begin();
  rfid.PCD_Init();
}
boolean msj = 0;
boolean sdarchive = false;
String file ; //////////nombre del archivo
String carpeta; //carpeta principal
String dataFile ; //////////nombre del archivo con los datos de los alumnos
boolean var = 0;
boolean FOLDER_EXIST = false;
boolean FOLDER_EXIST2 = false;
String ruta;
void loop() {
  digitalWrite(encoderPin, HIGH);
  RtcDateTime now = Rtc.GetDateTime();
  printDateTimeDia(now);
  printDateTimeFecha(now);
  printDateTimeTiempo(now);
  String contenedor = String(Fecha); //// guarda dato rtc (char) en variable string
  if ( carpeta == contenedor) { ////////////// si el nombre del archivo = dato rtc inicia creacion del files sd
    if (sdarchive == false) { //////crea archivo si aun no fue creado
      if (!FOLDER_EXIST) {
        if (SD.mkdir("/" + carpeta)) {
          Serial.println("dia creada correctamente");
          FOLDER_EXIST = true;
        } else {
          Serial.println("Error al crear dia");
        }
      }
      Serial.println(carpeta);
      delay(20);
      if (curso_seleccionado == true) {
        Serial.println("CURSO SELECCIONADO");
        if (septimo == 1) {
          file = "7mo.txt";
          if (tarde == 1) {
            ruta = "/" + carpeta + "/" + "7mo" + "/" + "tarde";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("tarde correctamente");
                FOLDER_EXIST2 = true;
              }
            }
          }
          else if (manana == 1) {
            ruta = "/" + carpeta + "/" + "7mo" + "/" + "manana";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("mañana correctamente");
                FOLDER_EXIST2 = true;
              }
            }
          }
          myFile = SD.open(ruta + "/" + file, FILE_WRITE);
          myFile.print("Asistencia 7To 'U'---");
          myFile.println(Ano);
          myFile.println(" ");
          myFile.close();
          Serial.println("Archivo 7mo ya creado");
          FOLDER_EXIST2 = false;
        }

        else  if (sexto == 1) {
          file = "6to.txt";
          if (tarde == 1) {
            ruta = "/" + carpeta + "/" + "6to" + "/" + "tarde";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("tarde correctamente");
                FOLDER_EXIST2 = true;
              }
            }
          }

          else if (manana == 1) {
            ruta = "/" + carpeta + "/" + "6to" + "/" + "manana";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("mañana correctamente");
                FOLDER_EXIST2 = true;
              }

            }
          }


          myFile = SD.open(ruta + "/" + file, FILE_WRITE);
          myFile.print("Asistencia 6To 'U'---");
          myFile.println(Ano);
          myFile.println(" ");
          myFile.close();
          Serial.println("Archivo 6to ya creado");
        }

        else if (quinto == 1) {
          if (tarde == 1) {
            ruta = "/" + carpeta + "/" + "5to" + "/" + "tarde";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("tarde correctamente");
                FOLDER_EXIST2 = true;
              }
            }
          }

          else if (manana == 1) {
            ruta = "/" + carpeta + "/" + "5to" + "/" + "manana";
            if (!FOLDER_EXIST2) {
              if (SD.mkdir(ruta)) {
                Serial.println("mañana correctamente");
                FOLDER_EXIST2 = true;
              }

            }
          }
          file = "5to.txt";
          myFile = SD.open(ruta + "/" + file, FILE_WRITE);
          myFile.print("Asistencia 5To 'U'---");
          myFile.println(Ano);
          myFile.println(" ");
          myFile.close();
          Serial.println("Archivo 5to ya creado");
        }
        else if (cuarto == 1) {
          file = "4to.txt";
          myFile = SD.open(ruta + "/" + file, FILE_WRITE);
          myFile.print("Asistencia 4To 'U'---");
          myFile.println(Ano);
          myFile.println(" ");
          myFile.close();
          Serial.println("Archivo 4to  ya creado");
        }
      } else {
        Serial.println("SIN CURSO");
      }
      /*
        myFile = SD.open("/" + file + "/" + "7mo", FILE_WRITE);
        myFile.print("Asistencia 7To 'U'---");
        myFile.println(Ano);
        myFile.println(" ");
        myFile.close();*/


      //file.remove(2, 4); ///////////remueve el .txt para hacer la comparacion posterior
      sdarchive = true; ///////variable que indica que el archivo fue creado
      protec = false; //permite la generacion de una nueva lista
      resetoide();


    }

    else if (sdarchive == true) /////////verifica si ya se creo un archivo
    {
      if (msj == 0) {


        msj = 1;////////impide que el mensaje se repita mucho en en serial
      }
    }

  }
  else if (carpeta != contenedor) { // verifica si el dia ha cambiado

    carpeta = contenedor; //vuelve a igualar file name al dato rtc

    Serial.println("Fecha actualizada");
    FOLDER_EXIST = false; // habilita la creacion de una nueva carpeta del dia
    msj = 0; //////// msj no importante
    archivoCursoSelected = 0 ;
    sdarchive = false; /////////////// vuelve a permitir la creacion de un archivo devolviendo la variable a false

  }



  ////////////////
  selectOption();

  aState = digitalRead(outputA);
  if (aState != aLastState) {

    if (digitalRead(outputB) != aState) {

      incremento++;
      if (incremento > 1) {
        incremento = 0;
        menu.switch_focus(false);
      }

    } else {

      incremento++;
      if (incremento > 1) {
        incremento = 0;
        menu.switch_focus(true);
      }

    }
    menu.softUpdate();;
    aLastState = aState;
  }



  printDateTimeMinuto(now);
  printDateTimeHora(now);


  hora = String(Hora).toInt();
  minut = String(Minuto).toInt();


  if (VARM != minut) {
    VARM = minut;
    menu.softUpdate();
  }


  if (VARH != hora) {
    VARH = hora;
    menu.softUpdate();
  }

}






//Funciones:::::
void selectOption() {
  if (digitalRead(sw) == LOW) {
    menu.call_function(1);
    delay(300);
  }
}
////////////LECTOR//////////////////////////////////
void LECTOR() {

  if (archivoCursoSelected == 1) {
    printDatosMatriz();
    menu.change_screen(5);
    menu.set_focusedLine(0);
  } else {
    lcd.clear();
    startime = millis();
    while ((millis() - startime) < 3000) {
      lcd.setCursor(6, 0);
      lcd.print("CURSO NO");
      lcd.setCursor(4, 1);
      lcd.print("SELECCIONADO");
      lcd.setCursor(1, 2);
      lcd.print("PRIMERO SELECCIONE");
      lcd.setCursor(6, 3);
      lcd.print("UN CURSO");

    }
    menu.change_screen(7);
    menu.set_focusedLine(0);
  }
}

boolean rfid_main;
boolean rfid_main2 = 0;
void lec_f() {
  limpia();
  startime = millis();
  auto_asis();
  if (cosita == 0) { // si aun no es la hora se requiere si o si tarjeta maestra
    while ((millis() - startime) < 5000 and rfid_main == 0 and alerta == 0) {

      lcd.setCursor(3, 0);
      lcd.print("Esperando al");
      lcd.setCursor(4, 1);
      lcd.print("Preceptor");

      RFID_FUNCTION();


    }
  }
  else {
    rfid_main2 = 1;
  }
  ///////////////////
  if ( alerta ==  1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 4000) {
      lcd.setCursor(0, 0);
      lcd.print("Tarjeta incorrecta");
      lcd.setCursor(0, 1);
      lcd.print("ALUMNO NO AUTORIZADO");
      lcd.setCursor(0, 2);
      lcd.print(dataArray[Indice_2][0]);
    }
  }
  ////////////////////

  if (rfid_main == 1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 5000) {
      lcd.setCursor(0, 0);
      lcd.print("Tarjeta Correcta");
      lcd.setCursor(0, 1);
      lcd.print("Bienvenido/a ");
      lcd.setCursor(0, 2);
      lcd.print(Preceptor[Indice][0]);

    }
    RtcDateTime now = Rtc.GetDateTime();
    printDateTimeTiempo(now);
    String tiempito = String(Tiempo);

    myFile = SD.open(ruta + "/" + file, FILE_WRITE);
    myFile.print("El preceptor/a ");
    myFile.print(Preceptor[Indice][0]);
    myFile.print(" habilito la asistencia a las: ");
    myFile.println(tiempito);
    myFile.println(" ");
    //////////////

    Serial.print("El/La preceptor/a ");
    Serial.print(Preceptor[Indice][0]);
    Serial.print(" habilito la asistencia a las: ");
    Serial.println(tiempito);
    Serial.println(" ");

    /////////////
    myFile.close();
  }
  Indice = 0;
  Indice_2 = 0;
  alumnos_main = 0;
  if ( rfid_main == 1 or rfid_main2 == 1 ) {

    limpia();
    startime = millis();
    lcd.setCursor(0, 0);
    lcd.print("Esperando alumnos");
    lcd.setCursor(0, 1);
    lcd.print("apoyar tarjetas");

    while ((millis() - startime) < 5000 and prueba == 0) {


      lcd.setCursor(0, 0);
      lcd.print("Esperando alumnos");
      lcd.setCursor(0, 1);
      lcd.print("apoya tarjetas");
      alumnos();
      /////////////
      if ( alumnos_main == 1) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 3000) {
          lcd.setCursor(0, 0);
          lcd.print("Tarjeta Correcta");
          lcd.setCursor(0, 1);
          lcd.print("Bienvenido/a ");
          lcd.setCursor(5, 2);
          lcd.print(dataArray[Indice_2][0]);

        }
        alumnos_main = 0;
        startime = millis();
        limpia();
        Indice_2 = 0;
      }
      ////////////
    }
    if ( prueba == 1) {
      limpia();
      startime = millis();
      while ((millis() - startime) < 4000) {
        lcd.setCursor(0, 0);
        lcd.print("Tarjeta incorrecta");
        lcd.setCursor(0, 1);
        lcd.print("Se requiere tarjeta");
        lcd.setCursor(0, 2);
        lcd.print("de alumno");
      }
    }

  }
  if ( rfid_main == 0 and rfid_main2 == 0 ) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 3000 ) {
      Nodetec();
    }
  }
  Indice = 0;
  Indice_2 = -1;
  rfid_main = 0;
  rfid_main2 = 0;
  cosita = 0;
  prueba = 0;
  alerta = 0;
  lcd.clear();
  menu.change_screen(5);
  menu.set_focusedLine(0);

}

void LEC_SALIDA() {
  limpia();
  startime = millis();
  while ((millis() - startime) < 5000 and rfid_main == 0 and alerta == 0) {

    lcd.setCursor(3, 0);
    lcd.print("Esperando al");
    lcd.setCursor(4, 1);
    lcd.print("Preceptor");

    RFID_FUNCTION();


  }


  ///////////////////
  if ( alerta ==  1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 4000) {
      lcd.setCursor(0, 0);
      lcd.print("Tarjeta incorrecta");
      lcd.setCursor(0, 1);
      lcd.print("ALUMNO NO AUTORIZADO");
      lcd.setCursor(0, 2);
      lcd.print(dataArray[Indice_2][0]);
    }
  }
  ////////////////////

  if (rfid_main == 1) {
    flag_salida = 1;
    limpia();
    startime = millis();
    while ((millis() - startime) < 5000) {
      lcd.setCursor(0, 0);
      lcd.print("Tarjeta Correcta");
      lcd.setCursor(0, 1);
      lcd.print("Salida habilitada");
      lcd.setCursor(0, 2);
      lcd.print(Preceptor[Indice][0]);

    }
    RtcDateTime now = Rtc.GetDateTime();
    printDateTimeTiempo(now);
    String tiempito = String(Tiempo);

    myFile = SD.open(ruta + "/" + file, FILE_WRITE);
    myFile.print("El preceptor/a ");
    myFile.print(Preceptor[Indice][0]);
    myFile.print(" habilito la SALIDA a las: ");
    myFile.println(tiempito);
    myFile.println(" ");
    //////////////

    Serial.print("El/La preceptor/a ");
    Serial.print(Preceptor[Indice][0]);
    Serial.print(" habilito la SALIDA a las: ");
    Serial.println(tiempito);
    Serial.println(" ");

    /////////////
    myFile.close();
  }
  Indice = 0;
  Indice_2 = 0;
  alumnos_main = 0;
  if ( rfid_main == 1 or rfid_main2 == 1 ) {

    limpia();
    startime = millis();
    lcd.setCursor(0, 0);
    lcd.print("Esperando alumnos");
    lcd.setCursor(0, 1);
    lcd.print("apoyar tarjetas");

    while ((millis() - startime) < 5000 and prueba == 0) {


      lcd.setCursor(0, 0);
      lcd.print("Esperando alumnos");
      lcd.setCursor(0, 1);
      lcd.print("apoya tarjetas");
      alumnos_salida();
      /////////////
      if ( alumnos_main == 1) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 3000) {
          lcd.setCursor(0, 0);
          lcd.print("Tarjeta Correcta");
          lcd.setCursor(0, 1);
          lcd.print("ADIOS");
          lcd.setCursor(5, 2);
          lcd.print(dataArray[Indice_2][0]);

        }
        alumnos_main = 0;
        startime = millis();
        limpia();
        Indice_2 = 0;
      }
      ////////////
    }
    if ( prueba == 1) {
      limpia();
      startime = millis();
      while ((millis() - startime) < 4000) {
        lcd.setCursor(0, 0);
        lcd.print("Tarjeta incorrecta");
        lcd.setCursor(0, 1);
        lcd.print("Se requiere tarjeta");
        lcd.setCursor(0, 2);
        lcd.print("de alumno");
      }
    }

  }
  if ( rfid_main == 0 and rfid_main2 == 0 ) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 3000 ) {
      Nodetec();
    }
  }
  Indice = 0;
  Indice_2 = -1;
  rfid_main = 0;
  rfid_main2 = 0;
  cosita = 0;
  prueba = 0;
  alerta = 0;
  lcd.clear();
  menu.change_screen(5);
  menu.set_focusedLine(0);

}
//////////////Lector preceptores////////////////////////////////

void RFID_FUNCTION() {
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }
  String tag = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    tag.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(rfid.uid.uidByte[i], HEX));
  }

  tag.toUpperCase();
  if (tag.substring(1) != dataArray[Indice_2][1])
  {
    Indice_2++;
  }
  if (tag.substring(1) != Preceptor[Indice][1])
  {
    Indice++;
  }
  if (tag.substring(1) == Preceptor[Indice][1])
  {
    Serial.println("Access Granted " + Preceptor[Indice][0]);

    rfid_main = 1;
    rfid_main2 = 1;
  }

  if (tag.substring(1) == dataArray[Indice_2][1])
  {
    Serial.println("Acceso denegado, alumno no autorizado : " + dataArray[Indice_2][0]);

    alerta = 1;
  }
  else {
    Serial.println(".");
  }
}
//////////////////////////Lector alumnos////////////////////////////////////////


void alumnos() {
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }
  String tag = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    tag.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(rfid.uid.uidByte[i], HEX));
  }

  tag.toUpperCase();

  if (tag.substring(1) == Preceptor[0][1] or tag.substring(1) == Preceptor[1][1] ) {
    prueba = 1;
  }
  if (tag.substring(1) != dataArray[Indice_2][1])
  {
    Indice_2++;
  }

  if (tag.substring(1) == dataArray[Indice_2][1])
  {

    asistencia();////////////////////SETEA ESTADO DE ALUMNO ENTRADA

    ALUMNOS[Indice_2] = pres;

    Serial.println("Alumno registrado: " + dataArray[Indice_2][0]);

    SET_ESTADO();

    Serial.println(dataArray[Indice_2][0] + " " + alumno_estado);

    alumnos_main = 1;

  }
  /*else {
    Serial.println(".");
    }*/

}

//////////////////////////////////////////////////////
void alumnos_salida() {
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }
  String tag = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    tag.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " "));
    tag.concat(String(rfid.uid.uidByte[i], HEX));
  }

  tag.toUpperCase();

  if (tag.substring(1) == Preceptor[0][1] or tag.substring(1) == Preceptor[1][1] ) {
    prueba = 1;
  }
  if (tag.substring(1) != dataArray[Indice_2][1])
  {
    Indice_2++;
  }

  if (tag.substring(1) == dataArray[Indice_2][1])
  {
    Salida_alumn[Indice_2] = 1; /////////////////SETEA ESTADO ALUMNO SALIDA

    Serial.println("Alumno registrado: " + dataArray[Indice_2][0] + " PRESENTE A LA SALIDA");

    alumnos_main = 1;

  }


}

/////////////////////////SD FUNCTIONS///////////////////////////

void EXSD() {
  menu.change_screen(3);
  menu.set_focusedLine(0);

}
//////////////////////////////////////////////////////////////////
void SERVOSD() {
  lcd.clear();
  limpia();
  startime = millis();

  while ((millis() - startime) < 4000 and rfid_main == 0 and alerta == 0) {

    lcd.setCursor(3, 0);
    lcd.print("Esperando al");
    lcd.setCursor(4, 1);
    lcd.print("Preceptor");

    RFID_FUNCTION();

  }
  //////////////////////////////////////////////
  if (rfid_main == 1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 3000 ) {
      lcd.setCursor(3, 0);
      lcd.print("Tarjeta leida");
      lcd.setCursor(4, 1);
      lcd.print("Correcta!");
    }

    limpia();
    while (digitalRead(fcarrera1) == HIGH) {
      lcd.setCursor(3, 0);
      lcd.print("ABRIENDO");
      lcd.setCursor(3, 1);
      lcd.print("COMPUERTA");

      analogWrite(OPEN, 140);
      //servo.write(0);
    }

    if (digitalRead(fcarrera1) == LOW) {
      //puerta abierta
      analogWrite(OPEN, 0);
    }
    delay(150);
    COMopen();

    ///////////////////////////////////////////////////////////////////////////////////////////

    lcd.clear();
    startime2 = millis();
    while ((millis() - startime2) < 10000 and SD.begin(4))// verifica sd y tiempo de espera
    {
      lcd.setCursor(5, 1);
      lcd.print("EXTRAIGA");
      lcd.setCursor(8, 2);
      lcd.print("SD");
    }
    lcd.clear();
    if (SD.begin(4)) {
      lcd.clear();
      startime = millis();
      while ((millis() - startime) < 3000 )//
      {
        lcd.setCursor(6, 1);
        lcd.print("TARJETA");
        lcd.setCursor(4, 2);
        lcd.print("NO EXTRAIDA");
      }
    }
    while (!SD.begin(4)) {// entra aca si  no esta la sd
      msj1();
      msj2();
      sd_state = 0;
    }
    ////////////////////////////////////////////////////////////////////////// mensaje siguiente si se reinserto la sd
    delay(150);
    if (SD.begin(4) and sd_state == 0) {
      msjsd();
      sd_state = 1;
    }

    //////////////////////////////////////////////////////////////////////////

    lcd.clear();

    delay(150);
    while (digitalRead(fcarrera2) == HIGH && SD.begin(4) ) {//cerrando puertas
      //servo.write(180);

      lcd.setCursor(6, 1);
      lcd.print("CERRANDO");
      lcd.setCursor(6, 2);
      lcd.print("COMPUERTA");
      analogWrite(CLOSE, 150);
    }


    if (digitalRead(fcarrera2) == LOW) {
      //puerta cerrada
      analogWrite(CLOSE, 0);
    }

    delay(250);
    COMclose();

    lcd.clear();

    menu.change_screen(1);
    menu.set_focusedLine(0);
  }
  rfid_main = 0;
}
//////////////////////////////////////////////////////////////


//////////////FUNCIONES AHORRO////////////////////////////////////////
void MenuAhorro() {
  menu.change_screen(4);
  menu.set_focusedLine(0);

}

void OFF() {
  lcd.noBacklight();
}

void ON() {
  lcd.backlight();

}
/////////////// FUNCIONES EXTRA ///////////////////////////////////////////

void EXTRAS() {
  menu.change_screen(2);
  menu.set_focusedLine(0);
}

void Credits() {

}


/////////////////////////////////////////////////////////////////////////


//////////////////////////ATRAS /////////////////////////////////////////////
void fn_atras() {
  menu.change_screen(1);
  menu.set_focusedLine(0);
}


//////////////////////////////////////////////////////////////////////
void msj1() {
  lcd.clear();
  msjt1 = millis();
  while ((millis() - msjt1) < 1000) {
    lcd.setCursor(6, 1);
    lcd.print("TARJETA");
    lcd.setCursor(6, 2);
    lcd.print("EXTRAIDA");

  }

}

void msj2() {
  lcd.clear();
  msjt2 = millis();
  while ((millis() - msjt2) < 1000) {
    lcd.setCursor(5, 0);
    lcd.print("REINSERTE");
    lcd.setCursor(6, 1);
    lcd.print("TARJETA");
    lcd.setCursor(5, 2);
    lcd.print("POR FAVOR");
  }
}

///////////////////////////////////////////////////////////////////////////
void COMclose() {
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 2000) {
    lcd.setCursor(5, 1);
    lcd.print("COMPUERTA");
    lcd.setCursor(6, 2);
    lcd.print("CERRADA");

  }

}
///////////////////////////////////////////////////////////////////////////
void COMopen() {
  lcd.clear();
  startime1 = millis();
  while ((millis() - startime1) < 2000) {
    lcd.setCursor(5, 1);
    lcd.print("COMPUERTA");
    lcd.setCursor(6, 2);
    lcd.print("ABIERTA");
  }

}

//////////////////////////////////////////////////////////////////////////////
void  msjsd() {
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(6, 1);
    lcd.print("TARJETA");
    lcd.setCursor(5, 2);
    lcd.print("INSERTADA!");
    sd_state = 1;
  }

}

/////////////////////////////////// ASISTENCIA /////////////////////////////////////////////////////

void tardanza() {
  if (manana == 1) {
    if (hora <= 7 ) {//los valores son de prueba
      ESTADO = 1;
    }
  } else if (tarde == 1) {
    if (hora <= 14 ) {//los valores son de prueba
      ESTADO = 1;
    }
  }
  else if (tarde == 1 and septimo == 1) {
    if (hora <= 17 ) {//los valores son de prueba
      ESTADO = 1;
    }
  }


}

void veriftime() {
  if (manana == 1) {
    if (hora == 7 && minut > 45 and minut < 59) { //los valores son de prueba
      ESTADO = 2;
    }
  } else if (tarde == 1) {
    if (hora == 14 && minut > 15 and minut < 35) { //los valores son de prueba
      ESTADO = 2;
    }
  } else if (tarde == 1 and septimo == 1) {
    if (hora == 17 && minut > 15 and minut < 35) { //los valores son de prueba
      ESTADO = 2;
    }
  }
}

void asistencia() {
  if (manana == 1) {
    if (hora > 7) {
      ESTADO = 3;
    }
  }
  if (tarde == 1) {
    if (hora > 14) {
      ESTADO = 3;
    }
  }
  if (tarde == 1 and septimo == 1) {
    if (hora > 17) {
      ESTADO = 3;
    }
  }

  tardanza();
  veriftime();

  if (ESTADO == 1) {
    pres = 1;
  }
  else if (ESTADO == 2) {
    pres = 2;
  }
  else if (ESTADO == 3) {
    pres = 3;
  }
  ESTADO = 0;

}


///////////////////////////////////////////////////////////////////////////////////////
void Nodetec() {
  lcd.setCursor(0, 0);
  lcd.print("Tarjeta Maestra");
  lcd.setCursor(0, 1);
  lcd.print("No encontrada!");
}



void limpia() {
  lcd.setCursor(0, 0);
  lcd.print("                           ");
  lcd.setCursor(0, 1);
  lcd.print("                           ");
  lcd.setCursor(0, 2);
  lcd.print("                           ");
  lcd.setCursor(0, 3);
  lcd.print("                           ");
}
///////////////////////////RTC VOIDs//////////////////////////////////////////////////
#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime & dt)
{
  //char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );
  Serial.print(datestring);
  return datestring;
}

void printDateTimeTiempo(const RtcDateTime & dt)
{
  //char datestring[20];

  snprintf_P(Tiempo,
             countof(Tiempo),
             PSTR("%02u:%02u:%02u"),
             dt.Hour(),
             dt.Minute(),
             dt.Second() );

  return Tiempo;

}

void printDateTimeFecha(const RtcDateTime & dt) {



  snprintf_P(Fecha,
             countof(Fecha),
             PSTR("%02u-%02u"),
             dt.Day(),
             dt.Month()
            );

  return Fecha;

}

void printDateMes(const RtcDateTime & dt)
{
  //char datestring[20];

  snprintf_P(Mes,
             countof(Mes),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month() );



  return Mes;
}

void printDateTimeDia(const RtcDateTime & dt)

{
  //char datestring[20];

  snprintf_P(Dia,
             countof(Dia),
             PSTR("%02u"),
             dt.Day());


  return Dia;
}

void printDateTimeHora(const RtcDateTime & dt)
{
  //char datestring[20];

  snprintf_P(Hora,
             countof(Hora),
             PSTR("%02u"),
             dt.Hour());

  return Hora;
}

void printDateTimeMinuto(const RtcDateTime & dt)
{
  //char datestring[20];

  snprintf_P(Minuto,
             countof(Minuto),
             PSTR("%02u"),
             dt.Minute());

  return Minuto;
}


//////////////////////
void GEN() {
  //
  if (archivoCursoSelected == 1) {
    if (flag_salida == 1) {
      limpia();
      startime = millis();

      while ((millis() - startime) < 4000 and rfid_main == 0 and alerta == 0) {

        lcd.setCursor(3, 0);
        lcd.print("Esperando al");
        lcd.setCursor(4, 1);
        lcd.print("Preceptor");

        RFID_FUNCTION();


      }

      //////////////////////////////////////////////
      if (rfid_main == 1) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 3000 ) {

          lcd.setCursor(3, 0);
          lcd.print("Tarjeta leida");
          lcd.setCursor(4, 1);
          lcd.print("Correcta!");


        }
      }
      ////////////////////////////////////////////////


      if (protec == false and rfid_main == 1 ) {///escritura de la asistencia

        Indice_2 = -1;
        Indice = -1;

        myFile = SD.open(ruta + "/" + file, FILE_WRITE);
        Serial.println("ENTRADA: ");
        myFile.println("ENTRADA:   ");
        for (byte i = 0; i < 25; i ++ ) {
          Indice_2++;
          SET_ESTADO();
          myFile.print(dataArray[Indice_2][0]);
          myFile.print("  ");

          myFile.println(alumno_estado);
          myFile.println(" ");

          Serial.print(dataArray[Indice_2][0]);
          Serial.print("  ");
          //1V32

          Serial.println(alumno_estado);
          Serial.println(" ");

        }
        myFile.println("//////////////////////////////////////////////");
        myFile.println("SALIDA: ");

        Serial.println("//////////////////////////////////////////////");
        Serial.println("SALIDA: ");
        entrance_compare();

        myFile.println("//////////////////////////////////////////////");
        myFile.println("ALUMNOS FALTANTES A LA SALIDA:");

        Serial.println("//////////////////////////////////////////////");
        Serial.println("ALUMNOS FALTANTES A LA SALIDA:");

        print_faltantes();
        Serial.println("");
        Serial.println("/////////////ASISTENCIA CERRADA//////////////");

        myFile.println("");
        myFile.println("/////////////ASISTENCIA CERRADA//////////////");
        myFile.close();

        protec = true;
        Indice_2 = -1;
        Indice = -1;

      }
      else if (protec == true and rfid_main == 1) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 4000) {
          lcd.setCursor(0, 0);
          lcd.print("Ya ha generado el");
          lcd.setCursor(0, 1);
          lcd.print("archivo de hoy");
        }
      }

      if ( rfid_main == 0 and alerta == 0 ) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 3000 ) {
          Nodetec();
        }
      }

      if ( alerta ==  1) {
        limpia();
        startime = millis();
        while ((millis() - startime) < 4000) {
          lcd.setCursor(0, 0);
          lcd.print("Tarjeta incorrecta");
          lcd.setCursor(0, 1);
          lcd.print("ALUMNO NO AUTORIZADO");
          lcd.setCursor(0, 2);
          lcd.print(dataArray[Indice_2][0]);
        }
      }

      lcd.clear();
      rfid_main = 0;
      rfid_main2 = 0;
      alerta = 0;
      Indice_2 = -1;
      Indice = -1;
    }  else {
      lcd.clear();
      startime = millis();
      while ((millis() - startime) < 3000) {
        lcd.setCursor(6, 0);
        lcd.print("NO HA");
        lcd.setCursor(4, 1);
        lcd.print("HABILITADO");
        lcd.setCursor(5, 2);
        lcd.print("LA SALIDA");
      }
      menu.change_screen(5);
      menu.set_focusedLine(0);
    }
  } else {
    lcd.clear();
    startime = millis();
    while ((millis() - startime) < 3000) {
      lcd.setCursor(6, 0);
      lcd.print("CURSO NO");
      lcd.setCursor(4, 1);
      lcd.print("SELECCIONADO");
      lcd.setCursor(1, 2);
      lcd.print("PRIMERO SELECCIONE");
      lcd.setCursor(6, 3);
      lcd.print("UN CURSO");

    }
    menu.change_screen(7);
    menu.set_focusedLine(0);
  }

}
////////////////////////////////////////////////////void para activacion sin tarjeta maestra despues de cierta hora
void auto_asis() {
  if (tarde == 1) {
    if (hora == 2 && minut > 30 and minut < 59) {
      cosita = 1; // boolean para operar  en el void del lector
    }
  }

  if (tarde == 1 and septimo == 1) {
    if (hora == 5 && minut > 20 and minut < 59) {
      cosita = 1; // boolean para operar  en el void del lector
    }
  }
  if (manana == 1) {
    if (hora == 7 && minut > 50 and minut < 59) {
      cosita = 1; // boolean para operar  en el void del lector
    }
  }
}
////////////////////////RESET FILE////////////////////////////////
//resetea manualmente el archivo
void RES() {
  limpia();
  startime = millis();

  while ((millis() - startime) < 4000 and rfid_main == 0 and alerta == 0) {

    lcd.setCursor(3, 0);
    lcd.print("Esperando al");
    lcd.setCursor(4, 1);
    lcd.print("Preceptor");

    RFID_FUNCTION();


  }
  ////////////////////
  if (rfid_main == 1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 3000 ) {

      lcd.setCursor(3, 0);
      lcd.print("Tarjeta leida");
      lcd.setCursor(4, 1);
      lcd.print("Correcta!");

    }
  }
  /////////////

  Indice = 0;
  delay(400);
  if (rfid_main == 1) {
    limpia();
    startime = millis();
    rfid_main = 0;

    while ((millis() - startime) < 6000 and rfid_main == 0 ) {

      lcd.setCursor(3, 0);
      lcd.print("Se requiere");
      lcd.setCursor(4, 1);
      lcd.print("confirmacion!");
      RFID_FUNCTION();


    }
  }
  Indice = 0;

  limpia();
  startime = millis();
  if (rfid_main == 1 ) {
    protec = false;
    sdarchive = false;
    msj = 0;
    SD.remove(ruta + "/" + file);

    if (!SD.exists(ruta + "/" + file)) {
      Serial.println("archivo borrado");
    }
    else {
      Serial.println("archivo no borrado");
    }

    while ((millis() - startime) < 4000 ) {

      lcd.setCursor(3, 0);
      lcd.print("Reseteo realizado");
      lcd.setCursor(3, 1);
      lcd.print("archivo removido!");
    }

  }
  ////////////////////////////////////////////
  if ( rfid_main == 0 and alerta == 0 ) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 3000 ) {
      Nodetec();
    }
  }
  ////////////////////////////////////

  if ( alerta ==  1) {
    limpia();
    startime = millis();
    while ((millis() - startime) < 4000) {
      lcd.setCursor(0, 0);
      lcd.print("Tarjeta incorrecta");
      lcd.setCursor(0, 1);
      lcd.print("ALUMNO NO AUTORIZADO");
      lcd.setCursor(0, 2);
      lcd.print(dataArray[Indice_2][0]);
    }
  }

  lcd.clear();
  rfid_main = 0;
  rfid_main2 = 0;
  Indice = 0;
  Indice_2 = -1;
  alerta = 0;
}
///////////////////FUNCION PARA REINICIAR EL ESTADO DE LOS ALUMNOS////////////////

void resetoide() {
  Indice_2 = -1;
  for (byte i = 0; i < 40 ; i++) {
    Indice_2++;
    ALUMNOS[Indice_2] = 0;
    Salida_alumn[Indice_2] = 0;
    FALTANTES[Indice_2] = " ";
  }
  Indice_2 = -1;

}




/////////////////////////TURNO MENUS///////////////////////////




void TURNO() {

  if (archivoCursoSelected == 1) {
    menu.change_screen(6);
    menu.set_focusedLine(0);
  } else {
    lcd.clear();
    startime = millis();
    while ((millis() - startime) < 3000) {
      lcd.setCursor(6, 0);
      lcd.print("CURSO NO");
      lcd.setCursor(4, 1);
      lcd.print("SELECCIONADO");
      lcd.setCursor(1, 2);
      lcd.print("PRIMERO SELECCIONE");
      lcd.setCursor(6, 3);
      lcd.print("UN CURSO");

    }
    menu.change_screen(7);
    menu.set_focusedLine(0);
  }
}



void T_tarde() {
  manana = 0;
  tarde = 1;
  sdarchive = false;
  lcd.clear();
  startime = millis();
  Serial.println("Turno tarde");
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("Turno tarde");
    lcd.setCursor(0, 1);
    lcd.print("seleccionado");

  }
  menu.change_screen(5);
  menu.set_focusedLine(0);

}

void T_man() {
  manana = 1;
  tarde = 0;
  sdarchive = false;
  Serial.println("Turno manana");
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("Turno manana");
    lcd.setCursor(0, 1);
    lcd.print("seleccionado");

  }
  menu.change_screen(5);
  menu.set_focusedLine(0);

}

///////////////comparacion entrada-salida////////////////////////////
void entrance_compare() {
  Indice_2 = -1;
  String nameF;
  for (int j = 0; j < 40; j ++ ) {
    Indice_2++;

    if (ALUMNOS[Indice_2] == 1 and Salida_alumn[Indice_2] == 1) {
      Serial.println(dataArray[Indice_2][0] + "PRESENTE A LA SALIDA");
      myFile.println(dataArray[Indice_2][0] + "PRESENTE A LA SALIDA");

    }
    else if (ALUMNOS[Indice_2] == 0 and Salida_alumn[Indice_2] == 0 and dataArray[Indice_2][0] != "") {
      Serial.println(dataArray[Indice_2][0] + " ALUMNO AUSENTE");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO AUSENTE");

    }


    else if (ALUMNOS[Indice_2] == 2 and Salida_alumn[Indice_2] == 0) {//<----------
      Serial.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      nameF = dataArray[Indice_2][0];
      FALTANTES[Indice_2] = nameF;
    }
    else if (ALUMNOS[Indice_2] == 3 and Salida_alumn[Indice_2] == 0) {//<----------
      Serial.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      FALTANTES[Indice_2] = nameF;
    }
    else if (ALUMNOS[Indice_2] == 1 and Salida_alumn[Indice_2] == 0) {//<----------
      Serial.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO ESCAPADO");
      nameF = dataArray[Indice_2][0];
      Serial.println(nameF);

      FALTANTES[Indice_2] = nameF;
    }
    else if (ALUMNOS[Indice_2] == 2 and Salida_alumn[Indice_2] == 1) {
      Serial.println(dataArray[Indice_2][0] + " ALUMNO PRESENTE");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO PRESENTE");
    }
    else if (ALUMNOS[Indice_2] == 3  and Salida_alumn[Indice_2] == 1) {
      Serial.println(dataArray[Indice_2][0] + " ALUMNO AUSENTE C/ PERMANENCIA EN EL AULA ");
      myFile.println(dataArray[Indice_2][0] + " ALUMNO AUSENTE C/ PERMANENCIA EN EL AULA ");
    }


  }
  Indice_2 = -1;
}

void print_faltantes() {
  Indice_2 = -1;
  for (byte i = 0; i < 40; i ++ ) {
    Indice_2++;
    if (FALTANTES[Indice_2] == "o") {
      Serial.print("a");
      myFile.print("a");
    }
    else {
      Serial.print(FALTANTES[Indice_2]);
      Serial.print("-");

      myFile.print(FALTANTES[Indice_2]);
      myFile.print("-");

    }
  }
  Indice_2 = -1;

}
/////////////////////////Curso MENUS///////////////////////////



void curso_selector() {
  menu.change_screen(7);
  menu.set_focusedLine(0);

}

void CURSOS_septimo() {
  septimo = 1;
  curso_seleccionado = true;

  cuarto = quinto = sexto = 0;
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("CURSO: SEPTIMO");
    lcd.setCursor(1, 1);
    lcd.print("seleccionado");

  }
  file_selector();
  menu.change_screen(6);
  menu.set_focusedLine(0);

}

void CURSOS_cuarto() {
  cuarto = 1;
  curso_seleccionado = true;

  septimo = quinto = sexto = 0;
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("CURSO: CUARTO");
    lcd.setCursor(1, 1);
    lcd.print("seleccionado");

  }
  file_selector();
  menu.change_screen(6);
  menu.set_focusedLine(0);

}

void CURSOS_quinto() {
  quinto = 1;
  curso_seleccionado = true;

  cuarto = septimo = sexto = 0;
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("CURSO: QUINTO");
    lcd.setCursor(1, 1);
    lcd.print("seleccionado");

  }
  file_selector();
  menu.change_screen(6);
  menu.set_focusedLine(0);

}

void CURSOS_sexto() {
  sexto = 1;
  curso_seleccionado = true;

  cuarto = quinto = septimo = 0;
  lcd.clear();
  startime = millis();
  while ((millis() - startime) < 3000) {
    lcd.setCursor(0, 0);
    lcd.print("CURSO: SEXTO");
    lcd.setCursor(1, 1);
    lcd.print("seleccionado");

  }

  file_selector();
  menu.change_screen(6);
  menu.set_focusedLine(0);

}

void V() {

}



void SET_ESTADO() {
  if (ALUMNOS[Indice_2] == 1) {
    alumno_estado = "PRESENTE";
  }
  else  if (ALUMNOS[Indice_2] == 2) {
    alumno_estado = "TARDANZA";
  }
  else  if (ALUMNOS[Indice_2] == 3) {
    alumno_estado = "AUSENTE C/PERMANENCIA EN EL AULA";
  }
  else  if (ALUMNOS[Indice_2] == 0) {
    alumno_estado = "AUSENTE";
  }
}


void file_selector() {
  eliminarDatosMatriz();
  if (septimo == 1) {
    myFile = SD.open("LF_7mo.txt");
    Serial.println("archivo septimo");
  } else if ( cuarto == 1 ) {
    myFile = SD.open("4to.txt");
    Serial.println("archivo cuarto");
  } else if ( quinto == 1 ) {
    myFile = SD.open("5to.txt");
    Serial.println("archivo quinto");
  } else if ( sexto == 1 ) {
    myFile = SD.open("6to.txt");
    Serial.println("archivo sexto");
  }
  archivoCursoSelected = 1;

  // Leer los datos del archivo y colocarlos en la matriz
  if (myFile) {
    byte row = 0; // Fila actual de la matriz
    while (myFile.available() && row < numRows) {
      // Leer la siguiente línea de datos del archivo
      dataString = myFile.readStringUntil('\n');

      // Separar los nombres y los IDs utilizando la función de división de cadena
      byte separatorIndex = dataString.indexOf(',');
      names = dataString.substring(0, separatorIndex);
      id = dataString.substring(separatorIndex + 1);

      // Colocar los nombres y los IDs en la matriz
      dataArray[row][0] = names;
      dataArray[row][1] = id;

      row++; // Avanzar a la siguiente fila de la matriz
    }

    // Cerrar el archivo de datos
    myFile.close();
  }
  Serial.println("TERMINO LA LECTURA");
  printDatosMatriz();
  flag_salida = 0;
}

void eliminarDatosMatriz() {
  for (byte row = 0; row < numRows; row++) {
    dataArray[row][0] = ""; // Elimina los datos asignando una cadena vacía
    dataArray[row][1] = ""; // Elimina los datos asignando una cadena vacía
  }
  Serial.println("Matriz reiniciada");
}

void printDatosMatriz() {
  for (byte row = 0; row < numRows; row++) {
    Serial.print(dataArray[row][0]);
    Serial.println(dataArray[row][1]);
  }
}
