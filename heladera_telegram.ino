#include "CTBot.h"

#define HELADERA_ON_CALLBACK "HeladeraON"
#define HELADERA_OFF_CALLBACK "HeladeraOFF"
#define ESTADO_HELADERA "Estado"

#define LED D1     //si queres podes ponerle un led para saber si esta concetado al wifi
#define HELADERA D5 // pin que activa el rele
#define PULSADOR  D4 // pin para el pulsador que activa o desactiva el rele

int BanderaBoton=0;
volatile bool BanderaBotonPresionado = false;

CTBot myBot;
CTBotReplyKeyboard myKbd;
bool isKeyboardActive;

String ssid = "NOMBREWIFI";
String pass = "CONTRASEÑAWIFI";
String token = "TOKENDETELEGRAM";
long id = ;  //chat_id sin comillas de telegram

void setup(){

  Serial.begin(9600);
  Serial.println("Iniciando TelegramBot...");

  pinMode(LED, OUTPUT);
  pinMode(HELADERA, OUTPUT);
  pinMode(PULSADOR, INPUT_PULLUP);

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

String estadoHeladeraTexto = "";
int estadoRele = 0;

}

void loop(){

  if ( BanderaBotonPresionado ) {  cambio_estado ();}  //si la bandera del boton esta en 1 o TRUE llamo a la funcion cambio_estado       
  
  TBMessage msg;

  if (myBot.getNewMessage(msg)){

    if (msg.messageType == CTBotMessageText){

      if (msg.text.equalsIgnoreCase("Teclado")){

        myBot.sendMessage(msg.sender.id, "Teclado", myKbd);
        isKeyboardActive = true;

      }

      if (msg.text.equalsIgnoreCase("HeladeraON")){

        digitalWrite(Heladera, LOW);
        myBot.sendMessage(msg.sender.id, "Heleadera encendida");
        estadoHeladeraTexto = "Heladera encendida";
        estadoRele = 1;

      }

      if (msg.text.equalsIgnoreCase("HeladeraOFF")){

        digitalWrite(Heladera, HIGH);
        myBot.sendMessage(msg.sender.id, "Heleadera apagadaa");
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

      if (msg.callbackQueryData.equals(HELADERA_OF_CALLBACK)){

        digitalWrite(HELADERA, HIGH);
        estadoHeladeraTexto = "Heladera apagada";
        myBot.endQuery(msg.callbackQueryID, estadoHeladeraTexto, true);
      }

      if (msg.callbackQueryData.equals(ESTADO_CALLBACK)){

        myBot.endQuery(msg.callbackQueryID, estadoHeladeraTexto, true);
      }

    }

  }

  delay(500);
}

void BotonPresionado() {    
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
