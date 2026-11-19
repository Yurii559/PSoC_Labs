/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

/* Варіант 6: Clock_1 = 1 МГц (тік = 1 мкс) -> 5000 тіків = 5 мс (Period = 5000 - 1) */
#define TIMER_PERIOD_TICKS  (4999u)
#define MAIN_DELAY_MS       (8u)

volatile uint32_t isr_count = 0u;

/* Обробник переривання таймера (Завдання 1) */
CY_ISR(Timer_ISR_Handler)
{
    Pin_ISR_Write((uint8)(1u - Pin_ISR_Read()));      /* Інверсія стану піна (toggle) */
    isr_count++;
    Timer_1_ClearInterrupt(Timer_1_INTR_MASK_TC);     /* Обов'язкове скидання прапорця TC */
}

/* Обробник переривання кнопки P0.7 (Завдання 2) */
CY_ISR(Button_ISR_Handler)
{
    Pin_LED_Write((uint8)(1u - Pin_LED_Read()));      /* Вмикання/вимикання світлодіода */
    Pin_Button_ClearInterrupt();                      /* Обов'язкове скидання прапорця GPIO */
}

int main(void)
{
    CyGlobalIntEnable;

    /* Запуск тактування і таймера */
    Clock_1_Start();
    Timer_1_Start();
    Timer_1_WritePeriod(TIMER_PERIOD_TICKS);
    Timer_1_SetInterruptMode(Timer_1_INTR_MASK_TC);

    /* Реєстрація обробників переривань */
    isr_Timer_StartEx(Timer_ISR_Handler);
    isr_Button_StartEx(Button_ISR_Handler);

    for (;;)
    {
        Pin_Main_Write(1u);
        CyDelay(MAIN_DELAY_MS);   /* 8 мс — високий рівень */
        Pin_Main_Write(0u);
        CyDelay(MAIN_DELAY_MS);   /* 8 мс — низький рівень (повний період ~16 мс) */
    }
}