#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include "SoftwareSerial.h"
#include <Keypad.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <SPI.h>

char ssid[64];
char password[64];

String chipid = "";

String topic_aux;
int acceso = 0;
String piso_responde = "";
String depto_responde = "";
String piso_llama = "";
String depto_llama = "";

const char ssidConf[] = "IVCAR_CLOUD";
const char passConf[] = "12345678";

const char *mqtt_server = "ivcaralarmas.com";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "0000";

char codigo[3]; // Creamos otra constante, en esta se guardaran los caracteres que presionemos en nuestro teclado matricial 4x4
int cont=0; // Esta es una variable que nos servirá más adelante
int cont1=0; // Esta es una variable que nos servirá más adelante
int codigo_final;

char piso[10];
char depto[10];
int bot_dpto;

long lastMsg = 0;


char contrasena[]="21A";
char contrasena1[]="22A";
char contrasena2[]="23A";
char contrasena3[]="24A";
char contrasena4[]="25A";
char contrasena5[]="26A";
char contrasena6[]="27A";
//--------------------------------------PISO 3-----------------------------------------------
char contrasena7[]="31A";
char contrasena8[]="32A";
char contrasena9[]="33A";
char contrasena10[]="34A";
char contrasena11[]="35A";
char contrasena12[]="36A";
char contrasena13[]="37A";
//--------------------------------------PISO 4-----------------------------------------------
char contrasena14[]="41A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena15[]="42A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena16[]="43A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena17[]="44A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena18[]="45A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena19[]="46A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena20[]="47A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 5-----------------------------------------------
char contrasena21[]="51A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena22[]="52A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena23[]="53A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena24[]="54A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena25[]="55A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena26[]="56A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena27[]="57A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 6-----------------------------------------------
char contrasena28[]="61A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena29[]="62A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena30[]="63A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena31[]="64A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena32[]="65A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena33[]="66A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena34[]="67A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 7-----------------------------------------------
char contrasena35[]="71A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena36[]="72A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena37[]="73A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena38[]="74A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena39[]="75A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena40[]="76A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 8-----------------------------------------------
char contrasena41[]="81A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena42[]="82A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena43[]="83A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena44[]="84A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena45[]="85A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena46[]="86A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena47[]="87A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 9-----------------------------------------------
char contrasena48[]="91A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena49[]="92A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena50[]="93A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena51[]="94A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena52[]="95A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena53[]="96A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena54[]="97A"; // En esta linea creamos la constante de cadena llamada contraseña
//--------------------------------------PISO 10-----------------------------------------------
char contrasena55[]="01A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena56[]="02A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena57[]="03A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena58[]="04A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena59[]="05A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena60[]="05A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena61[]="07A"; // En esta linea creamos la constante de cadena llamada contraseña

//--------------------------------------PISO 11-----------------------------------------------
char contrasena62[]="11A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena63[]="12A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena64[]="13A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena65[]="14A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena66[]="15A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena67[]="16A"; // En esta linea creamos la constante de cadena llamada contraseña
char contrasena68[]="17A"; // En esta linea creamos la constante de cadena llamada contraseña


const byte ROWS = 4; //Este código se estable para definir que nuestro teclado tiene 4 filas
const byte COLS = 4; //Este código se estable para definir que nuestro teclado tiene 4 columnas

char hexaKeys[ROWS][COLS] = //En el siguiente matriz se agregan en orden las teclas que posee nuestro teclado
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {D7, D6, D5, D4}; // Estos son los pines de conexión de Arduino de las columnas
byte colPins[COLS] = {D3, D2, D1, D0}; //Estos son los pines de conexión de Arduino de las filas

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //En este punto el teclado se inicia y realiza el mapeo de sus teclas con respecto a la matriz colocada arriba



// variables globales

String serie_device = "";

String redes_escaneadas = "";
String seleccionar_red = "";
String footer = "";


// Instancia a la clase Ticker
Ticker ticker;

int contconexion = 0;
byte digital_1 = 14; // act_desact
byte digital_2 = 12; // sirenas
byte digital_3 = 04; // interior
byte digital_4 = 13; // aberturas
byte TXIvcar = 02;

byte RXIvcar = 03;
byte Buzzer = 10;
byte Rele = 15;

byte out = 05;
byte connectionWiFi = 1;
byte pulsadorFlash = 16;

WiFiClient espClient;
ESP8266WebServer server(80);
PubSubClient client(espClient);

char msg[50];


String paginaconf = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=utf-8 name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>CONFIGURACION DE WIFI</title>"
"<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>"
"<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>"
"</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>"
"<style>  h1 { font-weight: 999; color: #00008B; font-size:255%;}</style>"
"<h1>IVCAR CLOUD</h1>"
"<form method='get' action='saveConfWiFi'><input id='s' name='ssid' length=32 placeholder='Nombre de red'><br/><input id='p' name='pass' length=64 type='password' placeholder='Contraseña'><br/>"
"<br/><button type='submit'>Guardar</button></form><br>"
"<a href='escanear'><button  type='submit'>Escanear</button></a><br>"
"</div>";

String paginaconfin = "</body>"
"</html>";

//--------------------------------PAGINA 2---------------------------------
String paginaconfsave = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=utf-8 name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>CONFIGURACION DE WIFI</title>"
"<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>"
"<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>"
"</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>"
"<style>  h1 { font-weight: 999; color: #00008B; font-size:255%;}</style>"
"<h1>RED CONFIGURADA</h1>"
"<font face='arial' size='4'>Su configuracion se ha<strong> guardado exitosamente</strong>.El dispositivo comenzará a buscar la red. Ingrese a la pagina <a href='http://alarmasivcar.com/'> http://alarmasivcar.com</a> con su nombre de usuario y contraseña y para agregar el <strong>dispositivo</strong> con el siguiente número de serie: </font>"
"</div>";
String paginaconfsavefin = "</body>"
"</html>";


//*****************************
//*** DECLARACION FUNCIONES ***
//*****************************
void callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void setup_wifi();
void parpadeoLed();
void modoconf();
void pageSetConfWiFi();
void pageSaveConfWiFi();
void escanear();
void grabar(int addr, String a);
void saveConfWiFi();
void searchSerieDevice();

//-----------------Función para leer la EEPROM------------------------
String leer(int addr) {
  byte lectura;
  String strlectura;
  for (int i = addr; i < addr+50; i++) {
    lectura = EEPROM.read(i);
    if (lectura != 255) {
      strlectura += (char)lectura;
    }
  }
  return strlectura;
}

String getValues(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup(){
  pinMode(digital_1, INPUT);
  pinMode(digital_2, INPUT);
  pinMode(digital_3, INPUT);
  pinMode(digital_4, INPUT);
  pinMode(TXIvcar, OUTPUT);
  pinMode(out, OUTPUT);
  pinMode(connectionWiFi, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Rele, OUTPUT);  //RELE

  pinMode(pulsadorFlash, INPUT);

  Serial.begin(115200);
  EEPROM.begin(512);

  digitalWrite(TXIvcar,LOW);

  pinMode(D6, INPUT);  //RX FLASH
  int lectura = digitalRead(D6);
  // float temp=lectura*3,3/1024;
  Serial.println(lectura);

  Serial.print("Serie: ");
  serie_device += String(ESP.getChipId());
  Serial.println(serie_device);

  if (lectura == 0){
    modoconf();
  }

  leer(0).toCharArray(ssid, 50);
  leer(50).toCharArray(password, 50);

  setup_wifi();

  randomSeed(micros());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop(){

  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("se perdió la conexion a: ");
    Serial.println(ssid);
    reconnectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  char customKey = customKeypad.getKey(); //esta linea indica que el carácter presionado se guardara en la variable customKey

  if (customKey != NO_KEY) //Con este ciclo preguntamos si se presiono una tecla, si se presiono entonce el ciclo continua
  {
    codigo[cont]=customKey; //se guardaran los caracteres presionados en codigo[cont]
    Serial.print(codigo[cont]); //Se imprime en nuestro monitor serial lo que este guardado en codigo[cont]
    cont=cont+1; //incrementamos la variable cont (esto con el fin de tener los 4 dígitos que requerimos)
    digitalWrite(10, HIGH);
    delay(30);
    digitalWrite(10, LOW);


    if(cont==3) //Ahora comienza otro siclo if, en el cual se pregunta si ya hay 4 datos guardados y si es así entonce el siclo continua

    //--------------------------------------PISO 2-----------------------------------------------

    {
      if(codigo[0]==contrasena[0]&&codigo[1]==contrasena[1]&&codigo[2]==contrasena[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/A";
        codigo_final = 21;
      }
      if(codigo[0]==contrasena1[0]&&codigo[1]==contrasena1[1]&&codigo[2]==contrasena1[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/B";
        codigo_final = 22;
      }
      if(codigo[0]==contrasena2[0]&&codigo[1]==contrasena2[1]&&codigo[2]==contrasena2[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/C";
        codigo_final = 23;
      }

      if(codigo[0]==contrasena3[0]&&codigo[1]==contrasena3[1]&&codigo[2]==contrasena3[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/D";
        codigo_final = 24;
      }

      if(codigo[0]==contrasena4[0]&&codigo[1]==contrasena4[1]&&codigo[2]==contrasena4[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/E";
        codigo_final = 25;

      }

      if(codigo[0]==contrasena5[0]&&codigo[1]==contrasena5[1]&&codigo[2]==contrasena5[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/F";
        codigo_final = 26;
      }
      if(codigo[0]==contrasena6[0]&&codigo[1]==contrasena6[1]&&codigo[2]==contrasena6[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/2/G";
        codigo_final = 27;
      }
      //--------------------------------------PISO 3-----------------------------------------------
      if(codigo[0]==contrasena7[0]&&codigo[1]==contrasena7[1]&&codigo[2]==contrasena7[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/A";
        codigo_final = 31;

      }
      if(codigo[0]==contrasena8[0]&&codigo[1]==contrasena8[1]&&codigo[2]==contrasena8[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/B";
        codigo_final = 32;

      }
      if(codigo[0]==contrasena9[0]&&codigo[1]==contrasena9[1]&&codigo[2]==contrasena9[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/C";
        codigo_final = 33;

      }
      if(codigo[0]==contrasena10[0]&&codigo[1]==contrasena10[1]&&codigo[2]==contrasena10[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/D";
        codigo_final = 34;
      }
      if(codigo[0]==contrasena11[0]&&codigo[1]==contrasena11[1]&&codigo[2]==contrasena11[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/E";
        codigo_final = 35;
      }
      if(codigo[0]==contrasena12[0]&&codigo[1]==contrasena12[1]&&codigo[2]==contrasena12[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/F";
        codigo_final = 36;
      }

      if(codigo[0]==contrasena13[0]&&codigo[1]==contrasena13[1]&&codigo[2]==contrasena13[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/3/G";
        codigo_final = 37;
      }
      //--------------------------------------PISO 4-----------------------------------------------
      if(codigo[0]==contrasena14[0]&&codigo[1]==contrasena14[1]&&codigo[2]==contrasena14[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/A";
        codigo_final = 41;
      }
      if(codigo[0]==contrasena15[0]&&codigo[1]==contrasena15[1]&&codigo[2]==contrasena15[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/B";
        codigo_final = 42;

      }

      if(codigo[0]==contrasena16[0]&&codigo[1]==contrasena16[1]&&codigo[2]==contrasena16[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/C";
        codigo_final = 43;

      }
      if(codigo[0]==contrasena17[0]&&codigo[1]==contrasena17[1]&&codigo[2]==contrasena17[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/D";
        codigo_final = 44;
      }
      if(codigo[0]==contrasena18[0]&&codigo[1]==contrasena18[1]&&codigo[2]==contrasena18[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/E";
        codigo_final = 45;
      }
      if(codigo[0]==contrasena19[0]&&codigo[1]==contrasena19[1]&&codigo[2]==contrasena19[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/F";
        codigo_final = 46;
      }

      if(codigo[0]==contrasena20[0]&&codigo[1]==contrasena20[1]&&codigo[2]==contrasena20[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/4/G";
        codigo_final = 47;
      }
      //--------------------------------------PISO 5-----------------------------------------------
      if(codigo[0]==contrasena21[0]&&codigo[1]==contrasena21[1]&&codigo[2]==contrasena21[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/A";
        codigo_final = 51;
      }
      if(codigo[0]==contrasena22[0]&&codigo[1]==contrasena22[1]&&codigo[2]==contrasena22[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/B";
        codigo_final = 52;
      }

      if(codigo[0]==contrasena23[0]&&codigo[1]==contrasena23[1]&&codigo[2]==contrasena23[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/C";
        codigo_final = 53;
      }

      if(codigo[0]==contrasena24[0]&&codigo[1]==contrasena24[1]&&codigo[2]==contrasena24[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/D";
        codigo_final = 54;
      }


      if(codigo[0]==contrasena25[0]&&codigo[1]==contrasena25[1]&&codigo[2]==contrasena25[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/E";
        codigo_final = 55;
      }

      if(codigo[0]==contrasena26[0]&&codigo[1]==contrasena26[1]&&codigo[2]==contrasena26[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/F";
        codigo_final = 56;
      }

      if(codigo[0]==contrasena27[0]&&codigo[1]==contrasena27[1]&&codigo[2]==contrasena27[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/5/G";
        codigo_final = 57;
      }
      //--------------------------------------PISO 6-----------------------------------------------


      if(codigo[0]==contrasena28[0]&&codigo[1]==contrasena28[1]&&codigo[2]==contrasena28[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/A";
        codigo_final = 61;
      }
      if(codigo[0]==contrasena29[0]&&codigo[1]==contrasena29[1]&&codigo[2]==contrasena29[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/B";
        codigo_final = 62;
      }

      if(codigo[0]==contrasena30[0]&&codigo[1]==contrasena30[1]&&codigo[2]==contrasena30[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/C";
        codigo_final = 63;
      }

      if(codigo[0]==contrasena31[0]&&codigo[1]==contrasena31[1]&&codigo[2]==contrasena31[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/D";
        codigo_final = 64;
      }

      if(codigo[0]==contrasena32[0]&&codigo[1]==contrasena32[1]&&codigo[2]==contrasena32[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/E";
        codigo_final = 65;
      }

      if(codigo[0]==contrasena33[0]&&codigo[1]==contrasena33[1]&&codigo[2]==contrasena33[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/F";
        codigo_final = 66;
      }

      if(codigo[0]==contrasena34[0]&&codigo[1]==contrasena34[1]&&codigo[2]==contrasena34[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/6/G";
        codigo_final = 67;
      }
      //--------------------------------------PISO 7-----------------------------------------------

      if(codigo[0]==contrasena35[0]&&codigo[1]==contrasena35[1]&&codigo[2]==contrasena35[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/7/A";
        codigo_final = 71;
      }

      if(codigo[0]==contrasena36[0]&&codigo[1]==contrasena36[1]&&codigo[2]==contrasena36[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/7/B";
        codigo_final = 72;
      }

      if(codigo[0]==contrasena37[0]&&codigo[1]==contrasena37[1]&&codigo[2]==contrasena37[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/master/7/C";
        codigo_final = 73;
      }

      if(codigo[0]==contrasena38[0]&&codigo[1]==contrasena38[1]&&codigo[2]==contrasena38[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/7/D";
        codigo_final = 74;
      }

      if(codigo[0]==contrasena39[0]&&codigo[1]==contrasena39[1]&&codigo[2]==contrasena39[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/7/E";
        codigo_final = 75;
      }

      if(codigo[0]==contrasena40[0]&&codigo[1]==contrasena40[1]&&codigo[2]==contrasena40[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/7/F";
        codigo_final = 76;
      }

      //--------------------------------------PISO 8-----------------------------------------------


      if(codigo[0]==contrasena41[0]&&codigo[1]==contrasena41[1]&&codigo[2]==contrasena41[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/A";
        codigo_final = 81;
      }

      if(codigo[0]==contrasena42[0]&&codigo[1]==contrasena42[1]&&codigo[2]==contrasena42[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/B";
        codigo_final = 82;
      }

      if(codigo[0]==contrasena43[0]&&codigo[1]==contrasena43[1]&&codigo[2]==contrasena43[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/C";
        codigo_final = 83;
      }

      if(codigo[0]==contrasena44[0]&&codigo[1]==contrasena44[1]&&codigo[2]==contrasena44[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/D";
        codigo_final = 84;
      }

      if(codigo[0]==contrasena45[0]&&codigo[1]==contrasena45[1]&&codigo[2]==contrasena45[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/E";
        codigo_final = 85;
      }

      if(codigo[0]==contrasena46[0]&&codigo[1]==contrasena46[1]&&codigo[2]==contrasena46[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/F";
        codigo_final = 86;
      }

      if(codigo[0]==contrasena47[0]&&codigo[1]==contrasena47[1]&&codigo[2]==contrasena47[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/8/G";
        codigo_final = 87;
      }

      //--------------------------------------PISO 9-----------------------------------------------


      if(codigo[0]==contrasena48[0]&&codigo[1]==contrasena48[1]&&codigo[2]==contrasena48[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/A";
        codigo_final = 91;
      }

      if(codigo[0]==contrasena49[0]&&codigo[1]==contrasena49[1]&&codigo[2]==contrasena49[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/B";
        codigo_final = 92;
      }

      if(codigo[0]==contrasena50[0]&&codigo[1]==contrasena50[1]&&codigo[2]==contrasena50[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/C";
        codigo_final = 93;
      }

      if(codigo[0]==contrasena51[0]&&codigo[1]==contrasena51[1]&&codigo[2]==contrasena51[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/D";
        codigo_final = 94;
      }

      if(codigo[0]==contrasena52[0]&&codigo[1]==contrasena52[1]&&codigo[2]==contrasena52[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/E";
        codigo_final = 95;
      }

      if(codigo[0]==contrasena53[0]&&codigo[1]==contrasena53[1]&&codigo[2]==contrasena53[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/F";
        codigo_final = 96;
      }

      if(codigo[0]==contrasena54[0]&&codigo[1]==contrasena54[1]&&codigo[2]==contrasena54[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/9/G";
        codigo_final = 97;
      }

      //--------------------------------------PISO 10-----------------------------------------------

      if(codigo[0]==contrasena55[0]&&codigo[1]==contrasena55[1]&&codigo[2]==contrasena55[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/A";
        codigo_final = 98;
      }

      if(codigo[0]==contrasena56[0]&&codigo[1]==contrasena56[1]&&codigo[2]==contrasena56[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/B";
        codigo_final = 99;
      }

      if(codigo[0]==contrasena57[0]&&codigo[1]==contrasena57[1]&&codigo[2]==contrasena57[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/C";
        codigo_final = 100;
      }

      if(codigo[0]==contrasena58[0]&&codigo[1]==contrasena58[1]&&codigo[2]==contrasena58[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/D";
        codigo_final = 101;
      }

      if(codigo[0]==contrasena59[0]&&codigo[1]==contrasena59[1]&&codigo[2]==contrasena59[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/E";
        codigo_final = 102;
      }

      if(codigo[0]==contrasena60[0]&&codigo[1]==contrasena60[1]&&codigo[2]==contrasena60[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/F";
        codigo_final = 103;
      }

      if(codigo[0]==contrasena61[0]&&codigo[1]==contrasena61[1]&&codigo[2]==contrasena61[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/10/G";
        codigo_final = 104;
      }

      //--------------------------------------PISO 11-----------------------------------------------

      if(codigo[0]==contrasena62[0]&&codigo[1]==contrasena62[1]&&codigo[2]==contrasena62[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/A";
        codigo_final = 105;
      }

      if(codigo[0]==contrasena63[0]&&codigo[1]==contrasena63[1]&&codigo[2]==contrasena63[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/B";
        codigo_final = 106;
      }

      if(codigo[0]==contrasena64[0]&&codigo[1]==contrasena64[1]&&codigo[2]==contrasena64[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/C";
        codigo_final = 107;
      }

      if(codigo[0]==contrasena65[0]&&codigo[1]==contrasena65[1]&&codigo[2]==contrasena65[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/D";
        codigo_final = 108;
      }

      if(codigo[0]==contrasena66[0]&&codigo[1]==contrasena66[1]&&codigo[2]==contrasena66[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/E";
        codigo_final = 109;
      }

      if(codigo[0]==contrasena67[0]&&codigo[1]==contrasena67[1]&&codigo[2]==contrasena67[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/F";
        codigo_final = 110;
      }

      if(codigo[0]==contrasena68[0]&&codigo[1]==contrasena68[1]&&codigo[2]==contrasena68[2])
      {
        cont1=1;
        topic_aux = "elmirador/master/11/G";
        codigo_final = 111;
      }

      if (cont1==1) //empieza un nuevo ciclo, esta vez indicamos si el código es diferente que la contraseña siga con el siclo
      {
        Serial.println("Password correcta"); //si la contraseña es correcta entonces se imprime ""Password correcta")

        Serial.println("acceso: " + acceso);

        piso_llama = getValues(topic_aux,'/',2);
        depto_llama = getValues(topic_aux,'/',3);

        char topic[50];
        topic_aux.toCharArray(topic,50);
        client.publish(topic, "abrir");
        Serial.println("Solicita acceso: " + piso_llama + depto_llama);

        digitalWrite(D8, LOW);
        digitalWrite(10, HIGH); //BUZZER
        delay(650);
        digitalWrite(10, LOW);//BUZZER

        for (int i = 0; i<=30; i++) {
          client.loop();

          if (acceso == 1 && piso_llama == piso_responde && depto_llama == depto_responde){

                digitalWrite(15, HIGH);//RELE
                for (int i = 0; i<=5; i++)
                {
                  digitalWrite(10, HIGH);
                  delay(600);
                  digitalWrite(10, LOW);
                  delay(200);
                }
                digitalWrite(15, LOW);//RELE
                i = 30;

          }else{
            Serial.println("Acceso denegado....");
          }

          delay(1000);
        }


        Serial.println("Cierre de acceso: " + topic_aux);
        client.publish(topic, "close");
        acceso = 0;
        piso_llama = "";
        depto_llama = "";
        depto_responde = "";
        piso_responde = "";

        codigo_final = 0;
        cont1=0;
      }
      else
      {
        Serial.println("Password incorrecta");
        digitalWrite(10, HIGH); //BUZZER
        delay(80);
        digitalWrite(10, LOW);//BUZZER
        delay(20);
        digitalWrite(10, HIGH); //BUZZER
        delay(80);
        digitalWrite(10, LOW);//BUZZER
        delay(20);
        cont=0;
      }
      cont=0; //resetear a 0 la variable cont

    }
  }
}

//*****************************
//***    CONEXION WIFI      ***
//*****************************

void reconnectWiFi(){
  // Reintento conexion
  Serial.print("Reintentando conexión e 60 segundos");
  delay(60000);
  WiFi.begin(ssid, password);

  ticker.attach(0.2, parpadeoLed);
  // Verifico estado de conexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print("Error conexion ->");
    Serial.println("Reintentando conexión e 60 segundos");
    WiFi.begin(ssid, password);
    delay(60000);
  }

  ticker.detach();
  digitalWrite(connectionWiFi, HIGH);
  Serial.print("Conexión reestablecida");

}

void modoconf() {

  delay(100);
  digitalWrite(connectionWiFi, HIGH);
  delay(100);
  digitalWrite(connectionWiFi, LOW);
  delay(100);
  digitalWrite(connectionWiFi, HIGH);
  delay(100);
  digitalWrite(connectionWiFi, LOW);

  WiFi.softAP(ssidConf, passConf);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP del acces point: ");
  Serial.println(myIP);
  Serial.println("WebServer iniciado...");

  server.on("/", pageSetConfWiFi);
  server.on("/", pageSaveConfWiFi);
  server.on("/saveConfWiFi", saveConfWiFi);
  server.on("/escanear", escanear);
  server.on("/searchSerieDevice", searchSerieDevice);
  server.begin();

  while (true) {
    server.handleClient();
  }
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion <50) {
    ++contconexion;
    delay(250);
    Serial.print(".");
    digitalWrite(connectionWiFi, HIGH);
    delay(250);
    digitalWrite(connectionWiFi, LOW);
  }
  if (contconexion <50) {
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    digitalWrite(connectionWiFi, HIGH);
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
    digitalWrite(15, LOW);
    reconnectWiFi();
  }
}

void pageSetConfWiFi() {
  server.send(200, "text/html", paginaconf + seleccionar_red + redes_escaneadas + paginaconfin);
}
void pageSaveConfWiFi() {
  server.send(200, "text/html", paginaconfsave + chipid + footer + paginaconfsavefin);
}

//******************************************
//***    CUANDO ME LLEGA UN MENSAJE      ***
//******************************************
void callback(char* topic, byte* payload, unsigned int length){
  String incoming = "";
  Serial.print("Mensaje recibido desde -> ");
  Serial.print(topic);
  Serial.println("");

  for (int i = 0; i < length; i++) {
    incoming += (char)payload[i];
  }
  incoming.trim();
  Serial.println("Mensaje -> " + incoming);
  String str_topic(topic);

    acceso = 1;
    piso_responde = getValues(incoming,',',0);
    depto_responde = getValues(incoming,',',1);

    Serial.println("Responde -> " + piso_responde + depto_responde);
}

//*****************************
//***    CONEXION MQTT      ***
//*****************************
void reconnectMQTT() {

  while (!client.connected()) {

    Serial.print("Intentando conexión Mqtt...");
    // Creamos un cliente ID
    String clientId = "esp8266_";
    clientId += String(random(0xffff), HEX);

    // Intentamos conectar
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("Conectado!");

      // Nos suscribimos a comandos


      client.subscribe("elmirador/boton/2/A");
      client.subscribe("elmirador/boton/2/B");
      client.subscribe("elmirador/boton/2/C");
      client.subscribe("elmirador/boton/2/D");
      client.subscribe("elmirador/boton/2/E");
      client.subscribe("elmirador/boton/2/F");
      client.subscribe("elmirador/boton/2/G");

      client.subscribe("elmirador/boton/3/A");
      client.subscribe("elmirador/boton/3/B");
      client.subscribe("elmirador/boton/3/C");
      client.subscribe("elmirador/boton/3/D");
      client.subscribe("elmirador/boton/3/E");
      client.subscribe("elmirador/boton/3/F");
      client.subscribe("elmirador/boton/3/G");

      client.subscribe("elmirador/boton/4/A");
      client.subscribe("elmirador/boton/4/B");
      client.subscribe("elmirador/boton/4/C");
      client.subscribe("elmirador/boton/4/D");
      client.subscribe("elmirador/boton/4/E");
      client.subscribe("elmirador/boton/4/F");
      client.subscribe("elmirador/boton/4/G");

      client.subscribe("elmirador/boton/5/A");
      client.subscribe("elmirador/boton/5/B");
      client.subscribe("elmirador/boton/5/C");
      client.subscribe("elmirador/boton/5/D");
      client.subscribe("elmirador/boton/5/E");
      client.subscribe("elmirador/boton/5/F");
      client.subscribe("elmirador/boton/5/G");

      client.subscribe("elmirador/boton/6/A");
      client.subscribe("elmirador/boton/6/B");
      client.subscribe("elmirador/boton/6/C");
      client.subscribe("elmirador/boton/6/D");
      client.subscribe("elmirador/boton/6/E");
      client.subscribe("elmirador/boton/6/F");
      client.subscribe("elmirador/boton/6/G");

      client.subscribe("elmirador/boton/7/A");
      client.subscribe("elmirador/boton/7/B");
      client.subscribe("elmirador/boton/7/C");
      client.subscribe("elmirador/boton/7/D");
      client.subscribe("elmirador/boton/7/E");
      client.subscribe("elmirador/boton/7/F");
      client.subscribe("elmirador/boton/7/G");

      client.subscribe("elmirador/boton/8/A");
      client.subscribe("elmirador/boton/8/B");
      client.subscribe("elmirador/boton/8/C");
      client.subscribe("elmirador/boton/8/D");
      client.subscribe("elmirador/boton/8/E");
      client.subscribe("elmirador/boton/8/F");
      client.subscribe("elmirador/boton/8/G");

      client.subscribe("elmirador/boton/9/A");
      client.subscribe("elmirador/boton/9/B");
      client.subscribe("elmirador/boton/9/C");
      client.subscribe("elmirador/boton/9/D");
      client.subscribe("elmirador/boton/9/E");
      client.subscribe("elmirador/boton/9/F");
      client.subscribe("elmirador/boton/9/G");

      client.subscribe("elmirador/boton/9/A");
      client.subscribe("elmirador/boton/9/B");
      client.subscribe("elmirador/boton/9/C");
      client.subscribe("elmirador/boton/9/D");
      client.subscribe("elmirador/boton/9/E");
      client.subscribe("elmirador/boton/9/F");
      client.subscribe("elmirador/boton/9/G");

      client.subscribe("elmirador/boton/10/A");
      client.subscribe("elmirador/boton/10/B");
      client.subscribe("elmirador/boton/10/C");
      client.subscribe("elmirador/boton/10/D");
      client.subscribe("elmirador/boton/10/E");
      client.subscribe("elmirador/boton/10/F");
      client.subscribe("elmirador/boton/10/G");

      client.subscribe("elmirador/boton/11/A");
      client.subscribe("elmirador/boton/11/B");
      client.subscribe("elmirador/boton/11/C");
      client.subscribe("elmirador/boton/11/D");
      client.subscribe("elmirador/boton/11/E");
      client.subscribe("elmirador/boton/11/F");
      client.subscribe("elmirador/boton/11/G");





    } else {
      Serial.print("falló :( con error -> ");
      Serial.print(client.state());
      Serial.println(" Intentamos de nuevo en 5 segundos");

      delay(5000);
    }
  }
}


//*****************************
//***    GUARDAR EEPROM      ***
//*****************************
//----------------Función para grabar en la EEPROM-------------------
void grabar(int addr, String a) {
  int tamano = a.length();
  char inchar[50];
  a.toCharArray(inchar, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, inchar[i]);
  }
  for (int i = tamano; i < 50; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}


//*****************************
//***    ESCANEAR REDES     ***
//*****************************
void escanear() {
  int n = WiFi.scanNetworks(); //devuelve el número de redes encontradas
  Serial.println("escaneo terminado");
  if (n == 0) { //si no encuentra ninguna red
    Serial.println("no se encontraron redes");
    redes_escaneadas = "no se encontraron redes";
  }
  else
  {
    Serial.print(n);
    Serial.println(" redes encontradas");
    redes_escaneadas = "";
    seleccionar_red = "<h3>Seleccione su red:</h3>";

    for (int i = 0; i < n; ++i)
    {
      // agrega al STRING "mensaje" la información de las redes encontradas
      redes_escaneadas = (redes_escaneadas) + "<p>" + String(i + 1) + ": " "<a href='#p' onclick='c(this)'>" + WiFi.SSID(i)  + "</a></p>\r\n";


      delay(10);
    }
    Serial.println(redes_escaneadas);
    pageSetConfWiFi();
  }
}


//*****************************
//***  GUARDAR CONF EEPROM  ***
//*****************************
void saveConfWiFi() {

  Serial.println(server.arg("ssid"));//Recibimos los valores que envia por GET el formulario web
  grabar(0,server.arg("ssid"));
  Serial.println(server.arg("pass"));
  grabar(50,server.arg("pass"));
  searchSerieDevice();
  pageSaveConfWiFi();
  delay(6000);
  ESP.restart();

}

//*****************************
//***    BUSCAR SERIE       ***
//*****************************
void searchSerieDevice() {

  chipid  =  "<br><strong>" + String(ESP.getChipId()) + "</strong>";
  footer="<br><br><h2>IVCAR ALARMAS</h2>";
  pageSaveConfWiFi();
}

void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(connectionWiFi);
  digitalWrite(connectionWiFi, !estado);
}
