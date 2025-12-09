// GPIO_config.c
#include "GPIO_config.h"

static void configure_LED_pin(int pin)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIOA->MODER &= ~(3 << (2 * pin));   // clear mode
    GPIOA->MODER |=  (1 << (2 * pin));   // output mode

    GPIOA->OTYPER &= ~(1 << pin);        // push-pull
    GPIOA->PUPDR  &= ~(3 << (2 * pin));  // no pull
}


void configure_LED_pa0(void) { configure_LED_pin(0); }
void configure_LED_pa1(void) { configure_LED_pin(1); }
void configure_LED_pa6(void) { configure_LED_pin(6); }
void configure_LED_pa7(void) { configure_LED_pin(7); }
void configure_LED_pa4(void) { configure_LED_pin(4); }

void turnON_LED_pa0(void) { GPIOA->ODR |= (1 << 0); }
void turnON_LED_pa1(void) { GPIOA->ODR |= (1 << 1); }
void turnON_LED_pa6(void) { GPIOA->ODR |= (1 << 6); }
void turnON_LED_pa7(void) { GPIOA->ODR |= (1 << 7); }
void turnON_LED_pa4(void) { GPIOA->ODR |= (1 << 4); }

void turnOFF_LED_pa0(void) { GPIOA->ODR &= ~(1 << 0); }
void turnOFF_LED_pa1(void) { GPIOA->ODR &= ~(1 << 1); }
void turnOFF_LED_pa6(void) { GPIOA->ODR &= ~(1 << 6); }
void turnOFF_LED_pa7(void) { GPIOA->ODR &= ~(1 << 7); }
void turnOFF_LED_pa4(void) { GPIOA->ODR &= ~(1 << 4); }

// ---- BUTTON CONFIG ----

// Blue user button on PC13, with EXTI interrupt
void configure_Button_pc13(void)
{
    // GPIOC clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    // SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // PC13 input
    GPIOC->MODER &= ~(3 << (2 * 13));

    // Pull-up (button pulls pin low when pressed)
    GPIOC->PUPDR &= ~(3 << (2 * 13));
    GPIOC->PUPDR |=  (1  << (2 * 13));

    // EXTI13 ← PC13
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;

    // Unmask EXTI13
    EXTI->IMR1 |= (1 << 13);

    // Falling-edge trigger (button press)
    EXTI->FTSR1 |= (1 << 13);
    EXTI->RTSR1 &= ~(1 << 13);

    // NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// Optional: external button on PC2 as simple input
void configure_Button_pc2(void)
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    GPIOC->MODER &= ~(3 << (2 * 2)); // input
    // Example: pull-down (external switch pulls to VCC)
    GPIOC->PUPDR &= ~(3 << (2 * 2));
    GPIOC->PUPDR |=  (2 << (2 * 2)); // pull-down

}
