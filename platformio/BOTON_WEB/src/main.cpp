#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Ticker.h>

char ssid[64];
char password[64];
String topic_pub_str = "elmirador/boton/11/E";
String topic_sub_str = "elmirador/master/11/E";
String msg_pub_str = "11,E";
char topic_pub[50];
char topic_sub[50];
char msg_pub[10];

String chipid = "";

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
byte Pulsador = 14; // act_desact
byte digital_2 = 12; // sirenas
byte digital_3 = 04; // interior
byte digital_4 = 13; // aberturas
byte TXIvcar = 02;
byte salAux = 05;
byte connectionWiFi = 15;
byte pulsadorFlash = 16;

WiFiClient espClient;
ESP8266WebServer server(80);
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];


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
void abrirPuerta();
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


void setup(){
  pinMode(Pulsador, INPUT);
  pinMode(digital_2, INPUT);
  pinMode(digital_3, INPUT);
  pinMode(digital_4, INPUT);
  pinMode(TXIvcar, OUTPUT);
  pinMode(salAux, OUTPUT);
  pinMode(connectionWiFi, OUTPUT);

  pinMode(pulsadorFlash, INPUT);

  Serial.begin(115200);
  EEPROM.begin(512);

  digitalWrite(TXIvcar,LOW);

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
  topic_pub_str.toCharArray(topic_pub, 50);

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

  if (incoming == "abrir"){
  abrirPuerta();
}




}


void abrirPuerta(){

  topic_pub_str.toCharArray(topic_pub,50);
  msg_pub_str.toCharArray(msg_pub,10);

  for (int i = 0; i<=30; i++)
{

      digitalWrite(salAux, HIGH);
      delay(600);
      digitalWrite(salAux, LOW);
      delay(250);
      int pulsa=digitalRead(14);
  if(  pulsa==0)
            {
      client.publish(topic_pub, msg_pub);

  for (int j = 0; j<=30; j++)
                      {
      digitalWrite(salAux, HIGH);
      delay(30);
      digitalWrite(salAux, LOW);
      delay(4);

            }
            i=30;

      }
  }


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

      // Nos suscribimos a comando
				topic_sub_str.toCharArray(topic_sub,50);
				client.subscribe(topic_sub);

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
