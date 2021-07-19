#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include "Adafruit_FONA.h"
#include "SoftwareSerial.h"
#include <EEPROM.h>
#include <Ticker.h>
#include <SPI.h>

char ssid[64];
char password[64];

char topic_monitoreo[25];
char msj_monitoreo[25];

String chipid = "";
String topic_monitoreo_aux = "alarma/monitoreo";
String msj_monitoreo_aux = "alarma/monitoreo";


const char ssidConf[] = "IVCAR_CLOUD";
const char passConf[] = "12345678";

const char *mqtt_server = "ivcaralarmas.com";
const int mqtt_port = 1883;
const char *mqtt_user = "web_client";
const char *mqtt_pass = "0000";

String serie_device = "";

String redes_escaneadas = "";
String seleccionar_red = "";
String footer = "";


// Instancia a la clase Ticker
Ticker ticker;

int contconexion = 0;
short cont1=0;
short cont=0;
short cont2=0;
short cont3=0;
// Pin LED rojo
byte digital_1 = 14; // act_desact
byte digital_2 = 12; // sirenas
byte digital_3 = 04; // interior
byte digital_4 = 13; // aberturas
byte TXIvcar = 02;
byte salAux = 05;
byte connectionWiFi = 15;
byte pulsadorFlash = 16;

// ---- CENTRAL MONITOREO ---------
char phone[50];
char phone_ivcar[50];
char mensaje[100];
char mensaje_ivcar[100];
String mje;
String mje_ivcar;

// CONFIGURACION DE UART GSM
#define CONFIG_GSM_RXPIN 5
#define CONFIG_GSM_TXPIN 4
#define CONFIG_GSM_BAUDRATE 9600

// CONFIGURACION DEL NUMERO DESTINO DEL MENSAJE SMS
#define CONFIG_GSM_SMS_DESTINATION phone

// PUERTO SERIE EMULADO: EL ARDUINO UNO TIENE SOLO 1 UART
// EN EL CONSTRUCTOR DE SOFTWARESERIAL SE ESPECIFICAN LOS PINES PARA RX Y TX
SoftwareSerial swseri = SoftwareSerial(CONFIG_GSM_RXPIN, CONFIG_GSM_TXPIN);

// OBJETO ADAFRUIT_FONA USADO PARA COMUNICARSE CON EL SIM800L
Adafruit_FONA fona = Adafruit_FONA(10);
//---------GSM--------------------------------------


WiFiClient espClient;
ESP8266WebServer server(80);
PubSubClient client(espClient);

long lastMsg = 0;
int temp1 = 0;
int temp2 = 1;
int volts = 2;

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
void enviarSMS(char* phone, char* mensaje);
void llamadaTel(char* phone);
void llamadaTelSet0(char* phone);


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
  pinMode(digital_3, OUTPUT);
  pinMode(digital_4, INPUT);
  pinMode(TXIvcar, OUTPUT);
  pinMode(salAux, INPUT);
  pinMode(connectionWiFi, OUTPUT);

  pinMode(pulsadorFlash, INPUT);

  swseri.begin(CONFIG_GSM_BAUDRATE);
  Serial.begin(115200);
  EEPROM.begin(512);

  Serial.print("Serie: ");
  serie_device += String(ESP.getChipId());
  Serial.println(serie_device);

  if (digitalRead(pulsadorFlash) == 0) {
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
  String name = getValues(incoming,',',0);
  String phone_str = getValues(incoming,',',1);
  String serie = getValues(incoming,',',2);
  String alias = getValues(incoming,',',3);

  String phone_ivcar_str = "+543385448580";

  phone_ivcar_str.toCharArray(phone_ivcar, 50);

  phone_str.toCharArray(phone, 50);
// ********* SMS ACTIVACION ***********************
  if(str_topic == "sms/act"){

    mje = alias + ": ACTIVADA";
    mje.toCharArray(mensaje, 100);
    Serial.println(mje);
    enviarSMS(phone, mensaje);
    delay(3000);
}

// ********* SMS DESACTIVACION ***********************
if(str_topic == "sms/desact"){

  mje =  alias + ": DESACTIVADA";
  mje.toCharArray(mensaje, 100);
  Serial.println(mje);
  enviarSMS(phone, mensaje);
  delay(1000);
}

// ********* SMS ROBO ***********************
if(str_topic == "sms/robo"){

  msj_monitoreo_aux = "1";
  topic_monitoreo_aux.toCharArray(topic_monitoreo, 25);
  msj_monitoreo_aux.toCharArray(msj_monitoreo, 25);

  client.publish(topic_monitoreo, msj_monitoreo);

  mje =  alias + ": ROBO";
  mje_ivcar =  "ROBO: " + name;
  mje.toCharArray(mensaje, 100);
  mje_ivcar.toCharArray(mensaje_ivcar, 100);
  Serial.println("monitoreo dice: " + msj_monitoreo_aux);

  enviarSMS(phone, mensaje);
  delay(3000);
  llamadaTel(phone);
  delay(3000);
  llamadaTel(phone_ivcar);
  delay(3000);
  enviarSMS(phone_ivcar, mensaje_ivcar);
  delay(3000);
}


// ********* SMS RESTAURACION ***********************
if(str_topic == "sms/res"){

  mje =  alias + ": RESTAURADA";
  mje_ivcar =  "RESTAURADA: " + name;
  mje.toCharArray(mensaje, 100);
  mje_ivcar.toCharArray(mensaje_ivcar, 100);
  enviarSMS(phone, mensaje);
  delay(1000);
  enviarSMS(phone_ivcar, mensaje_ivcar);
  delay(1000);
  }


  if(str_topic == "sms/desc"){

    mje =  alias + ": SIN CONEXION";
    mje_ivcar =  "SIN CONEXION: " + name;
    mje.toCharArray(mensaje, 100);
    mje_ivcar.toCharArray(mensaje_ivcar, 100);
    enviarSMS(phone, mensaje);
    delay(1000);
    //enviarSMS(phone_ivcar, mensaje_ivcar);
    delay(1000);


  }

  if(str_topic == "sms/conn"){

    mje =  alias + ": RECUPERO CONEXION";
    mje_ivcar =  "RECUPERO CONEXION: " + name;
    mje.toCharArray(mensaje, 100);
    mje_ivcar.toCharArray(mensaje_ivcar, 100);
    enviarSMS(phone, mensaje);
    delay(1000);
    //enviarSMS(phone_ivcar, mensaje_ivcar);
    delay(1000);


  }

}

void enviarSMS(char* phone, char* mensaje){
  if (fona.begin(swseri))
  Serial.println(F("MODULO GSM OK"));
else
{
  Serial.println(F("NO SE ENCUENTRA MODULO GSM"));
}
if (!fona.sendSMS(phone, mensaje)) {
  Serial.println(F("ENVIADO"));
} else {
  Serial.println(F("ERROR"));
}
}

void llamadaTel(char* phone){
if (fona.begin(swseri))
    Serial.println(F("MODULO GSM OK"));

Serial.println("Realizando llamada...");

        Serial.print(F("Call #"));
Serial.print(F("Calling ")); Serial.println(phone);
        if (!fona.callPhone(phone)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }
delay(20000); // wait for 20 seconds...
if (!fona.hangUp()) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("OK!"));
        }
}

//*****************************
//***    CONEXION MQTT      ***
//*****************************
void reconnectMQTT() {

	while (!client.connected()) {

		Serial.print("Intentando conexión Mqtt...");
		// Creamos un cliente ID
		String clientId = "CENTRALMONITOREO_";
		clientId += String(random(0xffff), HEX);

		// Intentamos conectar
		if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
			Serial.println("Conectado!");

      client.publish("alarma/monitoreo", "0");// estoy disponible
      client.publish("alarma/monitoreo", "2");// estoy conectado

      // Nos suscribimos a comandos
        char topic_act[25];
        char topic_desact[25];
        char topic_robo[25];
        char topic_res[25];
        char topic_desc[25];
        char topic_conn[25];

        String topic_aux_act = "sms/act";
        String topic_aux_desact = "sms/desact";
        String topic_aux_robo = "sms/robo";
        String topic_aux_res = "sms/res";
        String topic_aux_desc = "sms/desc";
        String topic_aux_conn = "sms/conn";

        topic_aux_act.toCharArray(topic_act,25);
        topic_aux_desact.toCharArray(topic_desact,25);
        topic_aux_robo.toCharArray(topic_robo,25);
        topic_aux_res.toCharArray(topic_res,25);
        topic_aux_desc.toCharArray(topic_desc,25);
        topic_aux_conn.toCharArray(topic_conn,25);

        client.subscribe(topic_act);
        client.subscribe(topic_desact);
        client.subscribe(topic_robo);
        client.subscribe(topic_res);
        client.subscribe(topic_desc);
        client.subscribe(topic_conn);

		} else {
			Serial.print("falló :( con error -> ");
			Serial.print(client.state());
			Serial.println(" Intentamos de nuevo en 5 segundos");

			delay(5000);
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

//*****************************
//***    GUARDAR EEPROM      ***
//*****************************
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
  grabar(64,server.arg("pass"));
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
