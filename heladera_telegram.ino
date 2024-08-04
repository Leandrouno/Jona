#include "CTBot.h"

#define HELADERA_ON_CALLBACK "HeladeraON"//enciedo
#define HELADERA_OFF_CALLBACK "HeladeraOFF"
#define ESTADO_HELADERA_CALLBACK "Estado"

#define LED D1     //si queres podes ponerle un led para saber si esta concetado al wifi
#define HELADERA D2 // pin que activa el rele
#define PULSADOR  D7 // pin para el pulsador que activa o desactiva el rele

int BanderaBoton=0;
volatile bool BanderaBotonPresionado = false;

CTBot myBot;
CTBotReplyKeyboard myKbd;
bool isKeyboardActive;

String ssid = "";
String pass = "";
String token = "";
String estadoHeladeraTexto = "";
int estadoRele = 0;
long id = ;  //chat_id sin comillas de telegram

void setup(){

  Serial.begin(9600);
  Serial.println("Iniciando TelegramBot...");

  pinMode(LED, OUTPUT);
  pinMode(HELADERA, OUTPUT);
  pinMode(PULSADOR, INPUT_PULLUP);// resistencia pull up

  attachInterrupt(PULSADOR, BotonPresionado, FALLING); // si detecta una interrupcion (pulsador) llama a la funcion BotonPresionado

  digitalWrite(HELADERA, HIGH); //si el rele se activa con GND (bajo), ponelo en HIGH o viceverza

  myBot.wifiConnect(ssid, pass);

  myBot.setTelegramToken(token);

  if (myBot.testConnection()){
    Serial.println("Conectado a la Red Wifi");
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("Error al Conectar a la Red");
    digitalWrite(LED, LOW);
  }

 //aca armas el teclado en telegram 
 myKbd.addButton("HeladeraON");
 myKbd.addButton("HeladeraOFF");
 myKbd.addRow();
 myKbd.addButton("Estado");
 myKbd.enableResize();
 isKeyboardActive = false;
//fin teclado
myBot.sendMessage(id, "Sistema iniciado");
Serial.println("Mensaje enviado a telegram");
}

void loop(){

  if ( BanderaBotonPresionado ) {  cambio_estado ();}  //si la bandera del boton esta en 1 o TRUE llamo a la funcion cambio_estado       
  
  TBMessage msg;

  if (myBot.getNewMessage(msg)){
    Serial.print("Mensaje recibido : ");
    Serial.println(msg.text);

    if(msg.sender.id == id){

    if (msg.messageType == CTBotMessageText){

      if (msg.text.equalsIgnoreCase("Teclado")){

        myBot.sendMessage(msg.sender.id, "Teclado", myKbd);
        isKeyboardActive = true;

      }

      if (msg.text.equalsIgnoreCase("HeladeraON")){

        digitalWrite(HELADERA, LOW);
        myBot.sendMessage(msg.sender.id, "Heladera encendida");
        estadoHeladeraTexto = "Heladera encendida";
        estadoRele = 1;

      }

      if (msg.text.equalsIgnoreCase("HeladeraOFF")){

        digitalWrite(HELADERA, HIGH);
        myBot.sendMessage(msg.sender.id, "Heladera apagadaa");
        estadoHeladeraTexto = "Heladera apagada";
        estadoRele = 0;

      }

      if (msg.text.equalsIgnoreCase("Estado")){

        myBot.sendMessage(msg.sender.id, estadoHeladeraTexto);

      }


    }

    else if (msg.messageType == CTBotMessageQuery) {

      if (msg.callbackQueryData.equals(HELADERA_ON_CALLBACK)){

        digitalWrite(HELADERA, LOW);
        estadoHeladeraTexto = "Heladera encendida";
        myBot.endQuery(msg.callbackQueryID, estadoHeladeraTexto, true);
      }

      if (msg.callbackQueryData.equals(HELADERA_OFF_CALLBACK)){

        digitalWrite(HELADERA, HIGH);
        estadoHeladeraTexto = "Heladera apagada";
        myBot.endQuery(msg.callbackQueryID, estadoHeladeraTexto, true);
      }

      if (msg.callbackQueryData.equals(ESTADO_HELADERA_CALLBACK)){

        myBot.endQuery(msg.callbackQueryID, estadoHeladeraTexto, true);
      }

    }

  } else{
    myBot.sendMessage(msg.sender.id, "SALI CABEZA DE MONO");
  }

  }

  delay(500);
}

ICACHE_RAM_ATTR  void BotonPresionado() {    
                        int button = digitalRead(PULSADOR);
                        if(button == LOW && BanderaBoton == 0){
                          BanderaBotonPresionado = true; //aca levanto la bandera para avisar que el boton fue presionado
                          BanderaBoton = 1;
                          Serial.println("Boton Presionado");
                          }                                                    
                        return;
} // FIN  BOTON PRESIONADO 

void cambio_estado (){
                        Serial.println("Cambiando estado.....");  
                        BanderaBotonPresionado = false;
                        BanderaBoton = 0;

                        if(estadoRele == 0){
                            estadoRele = 1;
                            digitalWrite(HELADERA, LOW);
                            estadoHeladeraTexto = "Heladera encendida";
                        } else{
                            estadoRele = 0;
                            digitalWrite(HELADERA, HIGH);
                            estadoHeladeraTexto = "Heladera apagada";
                        }

                        myBot.sendMessage(id, estadoHeladeraTexto);
                        Serial.println("Mensaje Enviado a Telegram");
                        Serial.println(" "); 
                                         
} // FIN CAMBIO ESTADO  
