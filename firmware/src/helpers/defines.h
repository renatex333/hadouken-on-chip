#ifndef DEFINES_H_   /* Include guard */
#define DEFINES_H_

#include <asf.h>

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// Entrada da leitura do potenciometro que vai alterar o volume
#define AFEC_POT AFEC0
#define AFEC_POT_ID ID_AFEC0
#define AFEC_POT_CHANNEL 0 // Canal do pino PD30

// Bot�o Azul 1 - PC31
#define BUT_PIO_BLUE_1 PIOC
#define BUT_PIO_ID_BLUE_1 ID_PIOC
#define BUT_IDX_BLUE_1 31
#define BUT_IDX_MASK_BLUE_1 (1 << BUT_IDX_BLUE_1)
// Bot�o Azul 2 - PA19
#define BUT_PIO_BLUE_2 PIOA
#define BUT_PIO_ID_BLUE_2 ID_PIOA
#define BUT_IDX_BLUE_2 19
#define BUT_IDX_MASK_BLUE_2 (1 << BUT_IDX_BLUE_2)
// Bot�o Azul 3 - PB3
#define BUT_PIO_BLUE_3 PIOB
#define BUT_PIO_ID_BLUE_3 ID_PIOB
#define BUT_IDX_BLUE_3 3
#define BUT_IDX_MASK_BLUE_3 (1 << BUT_IDX_BLUE_3)
// Bot�o Azul 4 - PB2
#define BUT_PIO_BLUE_4 PIOB
#define BUT_PIO_ID_BLUE_4 ID_PIOB
#define BUT_IDX_BLUE_4 2
#define BUT_IDX_MASK_BLUE_4 (1 << BUT_IDX_BLUE_4)

// Bot�o Vermelho 5 - PC19
#define BUT_PIO_RED_5 PIOC
#define BUT_PIO_ID_RED_5 ID_PIOC
#define BUT_IDX_RED_5 19
#define BUT_IDX_MASK_RED_5 (1 << BUT_IDX_RED_5)

// Bot�o Vermelho 6 - PC13
#define BUT_PIO_RED_6 PIOC
#define BUT_PIO_ID_RED_6 ID_PIOC
#define BUT_IDX_RED_6 13
#define BUT_IDX_MASK_RED_6 (1 << BUT_IDX_RED_6)
// Bot�o Vermelho 7 - PA6
#define BUT_PIO_RED_7 PIOA
#define BUT_PIO_ID_RED_7 ID_PIOA
#define BUT_IDX_RED_7 6
#define BUT_IDX_MASK_RED_7 (1 << BUT_IDX_RED_7)
// Bot�o Vermelho 8 - PD11
#define BUT_PIO_RED_8 PIOD
#define BUT_PIO_ID_RED_8 ID_PIOD
#define BUT_IDX_RED_8 11
#define BUT_IDX_MASK_RED_8 (1 << BUT_IDX_RED_8)

// Bot�o START
#define BUT_PIO_START PIOD
#define BUT_PIO_ID_START ID_PIOD
#define BUT_IDX_START 27
#define BUT_IDX_MASK_START (1 << BUT_IDX_START)

// Bot�o START
#define BUT_PIO_COIN PIOA
#define BUT_PIO_ID_COIN ID_PIOA
#define BUT_IDX_COIN 21
#define BUT_IDX_MASK_COIN (1 << BUT_IDX_COIN)

// Bot�o Vermelho 8 - PD11
#define BUT_PIO_RED_8 PIOD
#define BUT_PIO_ID_RED_8 ID_PIOD
#define BUT_IDX_RED_8 11
#define BUT_IDX_MASK_RED_8 (1 << BUT_IDX_RED_8)

// N�mero de LEDs na fita
#define LEDS_NUMBER 44

//LED DATA PIN - PD28
#define PIN_DATA      PIOD
#define PIN_DATA_ID   ID_PIOD
#define PIN_DATA_IDX  28
#define PIN_DATA_IDX_MASK (1 << PIN_DATA_IDX)

// JOYSTICK
// Primeira entrada
#define JOY1_PIO PIOD
#define JOY1_PIO_ID ID_PIOD
#define JOY1_IDX 22
#define JOY1_IDX_MASK (1 << JOY1_IDX)

#define JOY2_PIO PIOA
#define JOY2_PIO_ID ID_PIOA
#define JOY2_IDX 24
#define JOY2_IDX_MASK (1 << JOY2_IDX)

#define JOY3_PIO PIOA
#define JOY3_PIO_ID ID_PIOA
#define JOY3_IDX 5
#define JOY3_IDX_MASK (1 << JOY3_IDX)

#define JOY4_PIO PIOD
#define JOY4_PIO_ID ID_PIOD
#define JOY4_IDX 12
#define JOY4_IDX_MASK (1 << JOY4_IDX)

// #endregion

// usart (bluetooth ou serial)
// Descomente para enviar dados
// pela serial debug

//#define DEBUG_SERIAL

#ifdef DEBUG_SERIAL
#define USART_COM USART1
#define USART_COM_ID ID_USART1
#else
#define USART_COM USART0
#define USART_COM_ID ID_USART0
#endif

#endif