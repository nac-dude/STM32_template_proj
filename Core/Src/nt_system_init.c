
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx.h"
#include "lwprintf/lwprintf.h"
#include "nt_system_init.h"

/*
HCLOCK max with HSE 72MHz
HCLOCK max with HSI 64MHz
*/

bool nt_systemClock_init(nt_PLLSourceEnum_t nt_pll_source, uint32_t hse_clk_freq, nt_PLLDefStruct_t *nt_pll_def)
{
    ErrorStatus status = SUCCESS;

    switch (nt_pll_source)
    {
    case nt_HSE:
        nt_pll_def->nt_HSE_freq = hse_clk_freq;
        nt_pll_def->nt_pll_prediv = LL_RCC_PLLSOURCE_HSE_DIV_2;
        nt_pll_def->nt_pll_mul = LL_RCC_PLL_MUL_16;
        //nt_pll_def->nt_pll_prediv = LL_RCC_PLLSOURCE_HSE_DIV_1;
        //nt_pll_def->nt_pll_mul = LL_RCC_PLL_MUL_9;
        nt_pll_def->nt_AHB_prescaler = LL_RCC_SYSCLK_DIV_1;
        nt_pll_def->nt_APB1_prescaler = LL_RCC_APB1_DIV_2;
        nt_pll_def->nt_APB2_prescaler = LL_RCC_APB2_DIV_1;

        LL_UTILS_PLLInitTypeDef HSE_PLL_struct;
        LL_UTILS_ClkInitTypeDef HSE_CLK_struct;

        HSE_PLL_struct.PLLMul = nt_pll_def->nt_pll_mul;
        HSE_PLL_struct.Prediv = nt_pll_def->nt_pll_prediv;

        HSE_CLK_struct.AHBCLKDivider = nt_pll_def->nt_AHB_prescaler;
        HSE_CLK_struct.APB1CLKDivider = nt_pll_def->nt_APB1_prescaler;
        HSE_CLK_struct.APB2CLKDivider = nt_pll_def->nt_APB2_prescaler;

        status = LL_PLL_ConfigSystemClock_HSE(nt_pll_def->nt_HSE_freq, LL_UTILS_HSEBYPASS_ON, &HSE_PLL_struct, &HSE_CLK_struct);

        LL_Init1msTick(nt_pll_def->nt_HSE_freq);
        LL_RCC_HSE_EnableCSS();

        break;

    case nt_HSI:

        nt_pll_def->nt_pll_mul = LL_RCC_PLL_MUL_16;

        nt_pll_def->nt_AHB_prescaler = LL_RCC_SYSCLK_DIV_1;
        nt_pll_def->nt_APB1_prescaler = LL_RCC_APB1_DIV_2;
        nt_pll_def->nt_APB2_prescaler = LL_RCC_APB2_DIV_1;

        LL_UTILS_PLLInitTypeDef HSI_PLL_struct;
        LL_UTILS_ClkInitTypeDef HSI_CLK_struct;

        HSI_PLL_struct.PLLMul = nt_pll_def->nt_pll_mul;

        HSI_CLK_struct.AHBCLKDivider = nt_pll_def->nt_AHB_prescaler;
        HSI_CLK_struct.APB1CLKDivider = nt_pll_def->nt_APB1_prescaler;
        HSI_CLK_struct.APB2CLKDivider = nt_pll_def->nt_APB2_prescaler;

        status = LL_PLL_ConfigSystemClock_HSI(&HSI_PLL_struct, &HSI_CLK_struct);

        LL_Init1msTick(nt_pll_def->nt_HSE_freq);

        break;

    default:

        break;
    }

    

    return status;
}

