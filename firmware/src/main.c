#include <asf.h>
#include <time.h>
#include <string.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

//Definindo tudo do LED da placa:
#define LED_PIO           PIOC                 // periferico que controla o LED
// #
#define LED_PIO_ID        ID_PIOC                 // ID do periférico PIOC (controla LED)
#define LED_PIO_IDX       8                    // ID do LED no PIO
#define LED_PIO_IDX_MASK  (1 << LED_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Definindo tudo do botão 1:
#define BUT1_PIO PIOD
#define BUT1_PIO_ID ID_PIOD
#define BUT1_PIO_IDX 28
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX) // esse já está pronto.

// Definindo tudo do LED 1:
#define LED1_PIO           PIOA                 // periferico que controla o LED
// #
#define LED1_PIO_ID        ID_PIOA                 // ID do periférico PIOC (controla LED)
#define LED1_PIO_IDX       0                    // ID do LED no PIO
#define LED1_PIO_IDX_MASK  (1 << LED1_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Definindo tudo do botão 2:
#define BUT2_PIO PIOC
#define BUT2_PIO_ID ID_PIOC
#define BUT2_PIO_IDX 31
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX) // esse já está pronto.

// Definindo tudo do LED 2:
#define LED2_PIO           PIOC                 // periferico que controla o LED
// #
#define LED2_PIO_ID        ID_PIOC                 // ID do periférico PIOC (controla LED)
#define LED2_PIO_IDX       30                    // ID do LED no PIO
#define LED2_PIO_IDX_MASK  (1 << LED2_PIO_IDX)   // Mascara para CONTROLARMOS o LED

// Definindo tudo do botão 3:
#define BUT3_PIO PIOA
#define BUT3_PIO_ID ID_PIOA
#define BUT3_PIO_IDX 19
#define BUT3_PIO_IDX_MASK (1u << BUT3_PIO_IDX) // esse já está pronto.

// Definindo tudo do LED 3:
#define LED3_PIO           PIOB                 // periferico que controla o LED
// #
#define LED3_PIO_ID        ID_PIOB                 // ID do periférico PIOC (controla LED)
#define LED3_PIO_IDX       2                    // ID do LED no PIO
#define LED3_PIO_IDX_MASK  (1 << LED3_PIO_IDX)   // Mascara para CONTROLARMOS o LED


volatile char but1_flag; // variável global
volatile char but2_flag; // variável global
volatile char but3_flag; // variável global
volatile int deu_o_tempo = 0;


volatile uint32_t alarm_min, alarm_sec;

void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);
void pin_toggle(Pio *pio, uint32_t mask);

void but1_callback(void)
{
	but1_flag = 1;
}

void but2_callback(void) {
	but2_flag = 1;
}

void but3_callback(void) {
	but3_flag = 1;
}

void draw_animation(void){
	for(int j=70;j<=120;j+=2){
		
		gfx_mono_draw_rect(j, 20, 2, 10, GFX_PIXEL_SET);
		delay_ms(10);
		
	}
	
	for(int j=120;j>=70;j-=2){
		
		gfx_mono_draw_rect(j, 20, 2, 10, GFX_PIXEL_CLR);
		delay_ms(10);
		
	}
}

void TC1_Handler(void) {
	/**
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	* Isso é realizado pela leitura do status do periférico
	**/
	volatile uint32_t status = tc_get_status(TC0, 1);

	/** Muda o estado do LED (pisca) **/

	if (alarm_sec <= 0 && alarm_min <= 0){
		alarm_sec = 0;
		alarm_min = 0;
		deu_o_tempo = 1;
		
	}
	else if (alarm_sec == 0 && alarm_min != 0){
		alarm_sec = 59;
		alarm_min -= 1;
	} else{
		alarm_sec -= 1;
	}

}

void TC2_Handler(void) {

	volatile uint32_t status = tc_get_status(TC0, 2);

	/** Muda o estado do LED (pisca) **/
	pin_toggle(LED_PIO, LED_PIO_IDX_MASK);  
}


void TC0_Handler(void) {
	volatile uint32_t status = tc_get_status(TC0, 0);
	pin_toggle(LED3_PIO, LED3_PIO_IDX_MASK);
}


void pin_toggle(Pio *pio, uint32_t mask) {
	if(pio_get_output_data_status(pio, mask))
	pio_clear(pio, mask);
	else
	pio_set(pio,mask);
}


void pisca_led (int n, int t) {
	for (int i=0;i<n;i++){
		pio_clear(LED3_PIO, LED3_PIO_IDX_MASK);
		delay_ms(t);
		pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
		delay_ms(t);
	}
}

void configure_output(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask, const uint32_t ul_attribute, uint32_t ul_id ){
	pmc_enable_periph_clk(ul_id);
	pio_set_output(p_pio, ul_mask, 1, 1, 1 );

	//pio_configure(p_pio, ul_type, ul_mask, ul_attribute);
}

void configure_input(Pio *p_pio, const pio_type_t ul_type, const uint32_t ul_mask, const uint32_t ul_attribute, uint32_t ul_id){
	pmc_enable_periph_clk(ul_id);
	pio_configure(p_pio, ul_type, ul_mask, ul_attribute);
	pio_set_debounce_filter(p_pio, ul_mask, 60);
}

void configure_interruption(Pio *p_pio, uint32_t ul_id, uint32_t ul_mask, uint32_t ul_attr, void (*p_handler) (uint32_t, uint32_t)){
	pio_handler_set(p_pio, ul_id, ul_mask, ul_attr, p_handler);
	pio_enable_interrupt(p_pio, ul_mask);
	pio_get_interrupt_status(p_pio);
	NVIC_EnableIRQ(ul_id);
	NVIC_SetPriority(ul_id, 4);
}


void io_init(void) {
	// Configura led
	configure_output(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, 1, LED1_PIO_ID);
	configure_output(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, 1, LED2_PIO_ID);
	configure_output(LED3_PIO, PIO_OUTPUT_0, LED3_PIO_IDX_MASK, 1, LED2_PIO_ID);


	// Inicializa clock do periférico PIO responsavel pelo botao
	
	configure_input(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE, BUT1_PIO_ID);
	configure_input(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE, BUT2_PIO_ID);
	configure_input(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE, BUT3_PIO_ID);
	
	configure_interruption(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but1_callback);
	configure_interruption(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but2_callback);
	configure_interruption(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_FALL_EDGE, but3_callback);
}

void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configura o PMC */
	pmc_enable_periph_clk(ID_TC);

	/** Configura o TC para operar em  freq hz e interrupçcão no RC compare */
	tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC, TC_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq);

	/* Configura NVIC*/
	NVIC_SetPriority(ID_TC, 4);
	NVIC_EnableIRQ((IRQn_Type) ID_TC);
	tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);
}


void draw_time (uint32_t current_hour, uint32_t current_min, uint32_t current_sec){
	char tempo[20];
	sprintf(tempo, "%02d:%02d:%02d", current_hour, current_min, current_sec);
	gfx_mono_draw_string(tempo, 5,5, &sysfont);
}

void draw_alarm (uint32_t current_min, uint32_t current_sec){
	char tempo[20];
	sprintf(tempo, "%02d:%02d", current_min, current_sec);
	gfx_mono_draw_string(tempo, 5,18, &sysfont);
}


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();
	
	io_init();
	
	WDT->WDT_MR = WDT_MR_WDDIS;


  // Init OLED
	gfx_mono_ssd1306_init();
	
	int piscando = 0;             
	int esquentado_comida = 0;                                                                                         
	
	while(1) {

		if (but3_flag){
			deu_o_tempo = 0;
			tc_stop(TC0, 0); // para de apitar quando abre a porta do microondas
			pio_set(LED3_PIO, LED3_PIO_IDX_MASK);

			int delay = 300;
			while (!pio_get(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK)){
				alarm_sec+=5;
				if (delay <= 80){
					delay = 50;
				} 
				if (alarm_sec >= 60){ // Deve ser o valor configurado pelo usuário, sendo que o valor máximo deve ser 60 minutos : 60 segundos.
					alarm_sec = 0;
					alarm_min+=1;
				}
				if (alarm_min >= 61){
					alarm_min = 0;
				}
				delay_ms(delay);
				delay-= 20;
				draw_alarm(alarm_min, alarm_sec);
			} 			
	
			but3_flag = 0;
			
			delay_ms(200);
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);

			esquentado_comida = 1;
			TC_init(TC0, ID_TC1, 1, 1);
			tc_start(TC0, 1);

		}
		
		if (but2_flag){
			deu_o_tempo = 0;
			tc_stop(TC0, 0); // para de apitar quando abre a porta do microondas
			pio_set(LED3_PIO, LED3_PIO_IDX_MASK);


			int delay = 300;
			
			while (!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)){
				alarm_sec+=10;
				if (delay <= 80){
					delay = 50;
				}
				if (alarm_sec >= 60){
					alarm_sec = 0;
					alarm_min+=1;
				}
				if (alarm_min >= 61){
					alarm_min = 0;
				}
				delay_ms(delay);
				delay-= 20;
				draw_alarm(alarm_min, alarm_sec);
			}
			
			but2_flag = 0;
			
			delay_ms(200);
			pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
			pio_clear(LED2_PIO, LED2_PIO_IDX_MASK);
			
			esquentado_comida = 1;
			TC_init(TC0, ID_TC1, 1, 1);
			tc_start(TC0, 1);

		}
		if (!(deu_o_tempo) && but1_flag){ // abre a porta do microondas
			
			if (esquentado_comida){
				while (!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)){
					pio_clear(LED1_PIO, LED1_PIO_IDX_MASK); // luz interna acende
					pio_set(LED2_PIO, LED2_PIO_IDX_MASK); // sistema de potência para
					tc_stop(TC0, 1); // contador para

				}
				pio_clear(LED2_PIO, LED2_PIO_IDX_MASK); // sistema de potência continua
				tc_start(TC0, 1); // contador continua
				
			} else{
				pio_set(LED2_PIO, LED2_PIO_IDX_MASK); // sistema de potência para
				pio_set(LED3_PIO, LED3_PIO_IDX_MASK); // sistema de potência para

				while (!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)){
					pio_clear(LED1_PIO, LED1_PIO_IDX_MASK);
				}
				pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			}
			

			but1_flag = 0;
		}
		if (deu_o_tempo){
			 tc_stop(TC0, 1);

			 esquentado_comida = 0;
			 TC_init(TC0, ID_TC0, 0, 10);
			 tc_start(TC0, 0);
			 pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			 pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			 if (but1_flag){
				 esquentado_comida = 0;
				 deu_o_tempo = 0;

				 tc_stop(TC0, 0); // para de apitar quando abre a porta do microondas
				 pio_set(LED3_PIO, LED3_PIO_IDX_MASK);
				 pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
				 but1_flag = 0;
			 }

		}
		
		draw_alarm(alarm_min, alarm_sec);
				
		
		pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);

		
	}  
				
}

// porta fechada e rodando = luz 2
// porta aberta e rodando = luz 1