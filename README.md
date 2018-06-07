# Conectar ESP8266 a IBM IoT Platform
Este código muestra como conectar un esp8266 a IBM IoT Platform.

## Modificaciones del código
1- Bucar el código:

//-------- Variables customizables-----------
const char* ssid = "ssid";      //Cambiar por el ssid
const char* password = "password";  //Cambiar por el password

#define ORG "org"           //Cambiar por el org
#define DEVICE_TYPE "type"  //Cambiar por el type
#define DEVICE_ID "id"      //Cambiar por el id
#define TOKEN "token"       //Cambiar por el token

2- Reemplazar los valores:

*ssid = Nombre de la red de internet
*password = La contraseña de la red de internet
*ORG = Este nombre es asignado por Bluemix, verlo Arriba de todo a la Derecha como ID:(ORG), lo que está dentro de esos paréntesis se debe        copiar y pegar en este parámetro en su código.
*DEVICE_TYPE = El tipo de dispositivo que crearon, el mio es ESP8266.
*DEVICE_ID = El ID que le dieron, el mio es Instructables1.
*TOKEN = Es la señal de autenticación que utilizamos anteriormente y obtuvimos cuando generamos el API.
