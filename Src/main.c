#include "stm32l476xx.h"
#include "SysTick_Timer.h"
#include "GPIO_config.h"

#define NUM_LEDS      5
#define TARGET_INDEX  2   // index in led_pins[] for target LED

// Wired PA PINs:
static const int led_pins[NUM_LEDS] = { 0, 1, 6, 7, 4 }; // static because it reduces risk of name collisions in other .c files

// Mask for all LED bits: PA0, PA1, PA6, PA7, PA6
#define LED_MASK  ( (1<<0) | (1<<1) | (1<<6) | (1<<7) | (1<<4) )

volatile unsigned int pinNum = 0;      // 0 to 4 index into led_pins[], unsigned because we dont use negative index values
volatile int game_running = 1;
volatile int button_pressed = 0;
volatile int speed_index = 0;
//#define speed_number 3
static const int speeds[3] = {8000000, 4500000, 1000000};
volatile int difficulty = 0;

// SysTick: step LEDs
void SysTick_Handler(void)
{
	if (!game_running)
		return;

	// turn all LEDs off
	GPIOA->ODR &= ~LED_MASK;
	pinNum++;
	if (pinNum >= NUM_LEDS)
		pinNum = 0;
	// turn on the current LED
	int pin = led_pins[pinNum];
	GPIOA->ODR |= (1 << pin);

	// advance index

}

// PC13 button ISR
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR1 & EXTI_PR1_PIF13)
	{
		EXTI->PR1 = EXTI_PR1_PIF13;
		if (game_running) {
			//moved to here
			//game starts when we press this
		__disable_irq();
			SysTick_Init(difficulty);
			game_running = 0;
			button_pressed = 1;
			__enable_irq();
		}
	}
}

void EXTI2_IRQHandler(void) {
	// PR (Pending Register): Check if the interrupt is triggered by EXTI13, as EXTI 10-15 share this interrupt vector
	if ((EXTI->PR1 & EXTI_PR1_PIF2) == EXTI_PR1_PIF2) {
		// cleared by writing a 1 to this bit
		EXTI->PR1 |= EXTI_PR1_PIF2;
		if ((GPIOC->IDR & (1<<2)) == (1<<2)){ //check if push button is pressed
			// disable Systick and EXTI interrupts because we want the below lines to run for sure without conflict
			__disable_irq();
			// WIN if current index matches target index
			int win = (pinNum == TARGET_INDEX);

			GPIOA->ODR &= ~LED_MASK;

			if (win) {
				// Light target LED solid: use its PA pin
				int target_pin = led_pins[TARGET_INDEX];
				//toggles immediately
				GPIOA->ODR ^= (1 << target_pin);
				if (speed_index == 2) speed_index = -1;
				speed_index++;
				for (int l = 0; l < 6; l++) {
					GPIOA->ODR ^= (1 << target_pin);
					// simple delay
					for (volatile int d = 0; d < 200000; d++);
				}
			} else {
				// Blink all LEDs a few times
				for (int l = 0; l < 6; l++) {
					GPIOA->ODR ^= LED_MASK;
					if(speed_index != 0) speed_index--;
					// simple delay
					for (volatile int d = 0; d < 200000; d++);
				}
				GPIOA->ODR &= ~LED_MASK;
			}
			SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
			__enable_irq();
			difficulty = speeds[speed_index];
			SysTick_Init(difficulty);

		}
	}
}

void configure_EXTI(void){

	//1. Enable the EXTI line 2 interrupt in the NVIC
	NVIC_EnableIRQ(EXTI2_IRQn);

	//2. Configure the SYSCFG module
	// link EXTI line 2 to GPIO PC2
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;				// Enable the clock to SYSCFG
	//exti line 2, clear and set
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;     	// Clear the EXTI2 bits in SYSCFG's EXTICR1 register.
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI2_PC; 	// Set PC (0010) as the EXTI2 source in SYSCFG_EXTICR1.

	// 3. Enable (unmask) the EXTI2 interrupt by setting its corresponding bit in the EXTI's IMR.
	// EXTI IMR
	EXTI->IMR1 |= (1<<2);		//Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)

	//4. Enable interrupt trigger for both rising (button release) and falling (button press) edges.
	EXTI->RTSR1 |= (1<<2);  //Rising trigger selection register (RTSR):0 = disabled, 1 = enabled

}

int main(void)
{
	// Configure LEDs on PA0,1,6,7,4
	configure_LED_pa0();
	configure_LED_pa1();
	configure_LED_pa6();
	configure_LED_pa7();
	configure_LED_pa4();

	GPIOA->ODR &= ~LED_MASK;

	configure_Button_pc13();
	configure_Button_pc2();
	configure_EXTI();

	pinNum = NUM_LEDS - 1;
	game_running = 1;
	button_pressed = 0;
	difficulty = speeds[speed_index];

	while (1)
	{
		if (button_pressed)
		{
			__disable_irq();
			button_pressed = 0;
			game_running = 1;
			SysTick_Init(difficulty);
			//difficulty = speeds[speed_index];
			__enable_irq();
		}

	}
}
