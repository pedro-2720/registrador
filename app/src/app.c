/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI



void showDateAndTime( rtc_t * rtc ){
 
   //imprimimos fecha y hora
   
   printf("\n\r");
   printf("Fecha: %d/%d/%d Hora %d:%d:%2d", rtc-> year,
                     rtc-> month,
                     rtc-> mday,
                     rtc-> hour,
                     rtc-> min,
                     rtc-> sec );

}

void escribeHora(rtc_t *rtc)
{
   // Vamos a escribir la fecha y la hora en un display
   char fecha_hora[18];
   
   
   sprintf(fecha_hora , "%d/%2d/%2d  %2d:%2d:%2d" ,
                     rtc-> year,
                     rtc-> month,
                     rtc-> mday,
                     rtc-> hour,
                     rtc-> min,
                     rtc-> sec );
   
    lcdSendStringRaw(fecha_hora);
   
   
}
//prototipo de la funcion format convierte los datos numericos en strimgs.

static void format( float valor, char *dst, uint8_t pos );


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();
   
    /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );
   
   //Inicializar LCD
    lcdInit( 20, 4, 5, 8 ); // Inicializar LCD 20 x 4 caracteres tamaño 5x8

   /* Estructura RTC */
   rtc_t rtc;

   rtc.year = 2018;
   rtc.month = 8;
   rtc.mday = 13;
   rtc.wday = 1;
   rtc.hour = 19;
   rtc.min = 25;
   rtc.sec= 0;

   bool_t val = 0;
   uint8_t i = 0;

   /* Inicializar RTC */
   val = rtcConfig( &rtc );

   delay_t delay1s;
   delayConfig( &delay1s, 1000 );
   printf("seteando RTC \n");
   lcdClear();
   lcdGoToXY(1,1);
   lcdSendStringRaw( "INICIALIZANDO" );
   

   delay(2000); // El RTC tarda en setear la hora, por eso el delay
   
   printf("seteado\n");

   for( i=0; i<10; i++ ){
      /* Leer fecha y hora */
      val = rtcRead( &rtc );
      /* Mostrar fecha y hora en formato "DD/MM/YYYY, HH:MM:SS" */
      showDateAndTime( &rtc );
      delay(1000);
   }

   rtc.year = 2016;
   rtc.month = 7;
   rtc.mday = 3;
   rtc.wday = 1;
   rtc.hour = 14;
   rtc.min = 30;
   rtc.sec= 0;

   /* Establecer fecha y hora */
   val = rtcWrite( &rtc );
  
  //Variables para temp y humedad 
   char buffout[64];
	float hum = 0, temp = 0;
   
   dht11Init(GPIO1); // Inicializar DHT 11 en GPIO 1 por alli vamos a leer los datos.
   uint8_t cont = 0;

 

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {

      
      //lcdGoToXY(1,1);
      //lcdSendStringRaw( "en marcha" );
       lcdGoToXY(2,1);
      
      if( delayRead( &delay1s ) ){
         /* Leer fecha y hora */
         val = rtcRead( &rtc );
         /* Mostrar fecha y hora en formato "DD/MM/YYYY, HH:MM:SS" */
         showDateAndTime( &rtc );
         //printf("muestra hora");
         //Enviamos fecha y hora al lcd
         lcdGoToXY(1,2);
         escribeHora(&rtc);
      }
      
      //**************************************************************
      // Leemos datos de temperatura y humedad
      if( dht11Read( &hum, &temp ) ) {
         
			gpioWrite( LEDG, ON );
			gpioWrite( LEDR, OFF );
         
			printf( "Temperatura: %d gradosC  " , temp);
			format( temp, buffout, 0 );
			

			printf( "Humedad: %2.2f %  ", hum);
			format( hum, buffout, 0 );
         
			
         cont = 0;
         printf("%d",cont);

		} else {
			gpioWrite( LEDG, OFF );
			gpioWrite( LEDR, ON );
         
			printf ("Error al leer DHT11." );         
      
         cont++;
		}   
		delay(1500);
      
      //______________________________________
         
         
       
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/
/* Fumcion que convierte float a string*/

static void format( float valor, char *dst, uint8_t pos ){
	uint16_t val;
	val = 10 * valor;
	val = val % 1000;
	dst[pos] = (val / 100) + '0';
	pos++;
	dst[pos] = (val % 100) / 10 + '0';
	pos++;
	dst[pos] = '.';
	pos++;
	dst[pos] = (val % 10)  + '0';
	pos++;
	dst[pos] = '\0';
}
