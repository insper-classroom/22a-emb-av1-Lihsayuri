/*
 * abs.h
 *
 * Created: 30/09/2021 20:46:09
 *  Author: Glados
 */ 


#ifndef ABS_H_
#define ABS_H_

#include "asf.h"
#include "arm_math.h"

// LED
#define LED_PIO      PIOC
#define LED_PIO_ID   ID_PIOC
#define LED_IDX      8
#define LED_IDX_MASK (1 << LED_IDX)

#define RODA_DENTE_PIO      PIOD
#define RODA_DENTE_ID   ID_PIOD
#define RODA_DENTE_IDX  30
#define RODA_DENTE_MASK (1 << RODA_DENTE_IDX)

// Botão
#define BUT_PIO      PIOA
#define BUT_PIO_ID   ID_PIOA
#define BUT_IDX  11
#define BUT_IDX_MASK (1 << BUT_IDX)

#define ANTILOCK_PIO      PIOC
#define ANTILOCK_PIO_ID   ID_PIOC
#define ANTILOCK_IDX  13
#define ANTILOCK_IDX_MASK (1 << ANTILOCK_IDX)

#define vel_default 1
#define n_dentes 10


/* A reference setting for USART */
/** USART Interface */
#define CONF_UART              USART1
/** Baudrate setting */
#define CONF_UART_BAUDRATE     115200
/** Character length setting */
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
/** Parity setting */
#define CONF_UART_PARITY       US_MR_PAR_NO
/** Stop bits setting */
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT

void TC6_Handler(void);
void roda_fonica_init(void);
// void but_callback(uint32_t id, uint32_t mask);
// float tc_set_new_vel(float vel);
// void abs_init(void);
 void pin_toggle(Pio *pio, uint32_t mask);

#endif /* ABS_H_ */