/*
 * Copyright (c) 2015-2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** ============================================================================
 *  @file       MSP_EXP432P401R.h
 *
 *  @brief      MSP_EXP432P401R Board Specific APIs
 *
 *  The MSP_EXP432P401R header file should be included in an application as
 *  follows:
 *  @code
 *  #include <MSP_EXP432P401R.h>
 *  @endcode
 *
 *  ============================================================================
 */
#ifndef __MSP_EXP432P401R_H
#define __MSP_EXP432P401R_H

#ifdef __cplusplus
extern "C" {
#endif

/* LEDs on MSP_EXP432P401R are active high. */
#define MSP_EXP432P401R_LED_OFF (0)
#define MSP_EXP432P401R_LED_ON  (1)

/*!
 *  @def    MSP_EXP432P401R_ADCName
 *  @brief  Enum of ADC channels on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_ADCName {
    MSP_EXP432P401R_ADC0 = 0,
    MSP_EXP432P401R_ADC1,

    MSP_EXP432P401R_ADCCOUNT
} MSP_EXP432P401R_ADCName;

/*!
 *  @def    MSP_EXP432P401R_ADCBufName
 *  @brief  Enum of ADCBuf channels on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_ADCBufName {
    MSP_EXP432P401R_ADCBUF0 = 0,

    MSP_EXP432P401R_ADCBUFCOUNT
} MSP_EXP432P401R_ADCBufName;

/*!
 *  @def    MSP_EXP432P401R_CaptureName
 *  @brief  Enum of Capture timer names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_CaptureName {
    MSP_EXP432P401R_CAPTURE_TA1 = 0,
    MSP_EXP432P401R_CAPTURE_TA2,
    MSP_EXP432P401R_CAPTURE_TA3,

    MSP_EXP432P401R_CAPTURECOUNT
} MSP_EXP432P401R_CaptureName;

/*!
 *  @def    MSP_EXP432P401R_GPIOName
 *  @brief  Enum of GPIO names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_GPIOName {

    MSP_EXP432P401R_S1 = 0,
    MSP_EXP432P401R_S2,
    MSP_EXP432P401R_SRDY,
    MSP_EXP432P401R_BL,
    HC_ECHO,
    MSP_EXP432P401R_LED1,
    MSP_EXP432P401R_LED_RED,
    MSP_EXP432P401R_RESET,
    HC_TRIG,
    MSP_EXP432P401R_LED_GREEN,
    MSP_EXP432P401R_LED_BLUE,
    MSP_EXP432P401R_GPIOCOUNT
} MSP_EXP432P401R_GPIOName;

/*!
 *  @def    MSP_EXP432P401R_I2CName
 *  @brief  Enum of I2C names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_I2CName {
    MSP_EXP432P401R_I2CB0 = 0,

    MSP_EXP432P401R_I2CCOUNT
} MSP_EXP432P401R_I2CName;

/*!
 *  @def    MSP_EXP432P401R_I2CSlaveName
 *  @brief  Enum of I2CSlave names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_I2CSlaveName {
    MSP_EXP432P401R_I2CSLAVEB0 = 0,

    MSP_EXP432P401R_I2CSLAVECOUNT
} MSP_EXP432P401R_I2CSlaveName;

/*!
 *  @def    MSP_EXP432P401R_PWMName
 *  @brief  Enum of PWM names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_PWMName {
    MSP_EXP432P401R_PWM_TA1_1 = 0,
    MSP_EXP432P401R_PWM_TA1_2,
    MSP_EXP432P401R_PWM_TA1_3,

    MSP_EXP432P401R_PWMCOUNT
} MSP_EXP432P401R_PWMName;

/*!
 *  @def    MSP_EXP432P401R_SDSPIName
 *  @brief  Enum of SDSPI names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_SDSPIName {
    MSP_EXP432P401R_SDSPIB0 = 0,

    MSP_EXP432P401R_SDSPICOUNT
} EMSP_EXP432P401R_SDSPIName;

/*!
 *  @def    MSP_EXP432P401R_SPIName
 *  @brief  Enum of SPI names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_SPIName {
    MSP_EXP432P401R_SPIB0 = 0,
    MSP_EXP432P401R_SPIB2,

    MSP_EXP432P401R_SPICOUNT
} MSP_EXP432P401R_SPIName;

/*!
 *  @def    MSP_EXP432P401R_TimerName
 *  @brief  Enum of Timer names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_TimerName {
    MSP_EXP432P401R_TIMER_T32_0 = 0,
    MSP_EXP432P401R_TIMER_T32_1,
    MSP_EXP432P401R_TIMER_TA_1,
    MSP_EXP432P401R_TIMER_TA_2,
    MSP_EXP432P401R_TIMER_TA_3,

    MSP_EXP432P401R_TIMERCOUNT
} MSP_EXP432P401R_TimerName;

/*!
 *  @def    MSP_EXP432P401R_UARTName
 *  @brief  Enum of UART names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_UARTName {
    MSP_EXP432P401R_UARTA0 = 0,
    MSP_EXP432P401R_UARTA2,

    MSP_EXP432P401R_UARTCOUNT
} MSP_EXP432P401R_UARTName;

/*!
 *  @def    MSP_EXP432P401R_WatchdogName
 *  @brief  Enum of Watchdog names on the MSP_EXP432P401R dev board
 */
typedef enum MSP_EXP432P401R_WatchdogName {
    MSP_EXP432P401R_WATCHDOG = 0,

    MSP_EXP432P401R_WATCHDOGCOUNT
} MSP_EXP432P401R_WatchdogName;

/*!
 *  @brief  Initialize the general board specific settings
 *
 *  This function initializes the general board specific settings.
 */
extern void MSP_EXP432P401R_initGeneral(void);

#ifdef __cplusplus
}
#endif

#endif /* __MSP_EXP432P401R_H */
