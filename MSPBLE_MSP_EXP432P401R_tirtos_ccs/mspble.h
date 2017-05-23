/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
#ifndef MSPBLE_H
#define MSPBLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *                                FUNCTIONS
 ******************************************************************************/
/*
 * Task creation function for the Application Processor.
 */
extern void MSPBLE_createTask(void);

/*******************************************************************************
 *                            CONSTANTS
 ******************************************************************************/
/* Advertising interval when device is discoverable (units 625us, 160=100ms)) */
#define DEFAULT_ADVERTISING_INTERVAL          160

/* Limited discoverable mode advertises for 30.72s, and then stops
   General discoverable mode advertises indefinitely */
#define DEFAULT_DISCOVERABLE_MODE             SAP_GAP_ADTYPE_FLAGS_GENERAL

/* Minimum connection interval (units of 1.25ms, 80=100ms) if automatic
   parameter update request is enabled */
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     80

/* Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic
   parameter update request is enabled */
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     800

/**
 * Project Zero States.
 */
typedef enum
{
    MSPBLE_RESET,
    MSPBLE_IDLE,
    MSPBLE_START_ADV,
    MSPBLE_CONNECTED,
    MSPBLE_CANCEL_ADV,
    MSPBLE_UPDATE_SNP
} MSPBLE_States_t;

/* Task configuration */
#define MSPBLE_TASK_PRIORITY                     1
#define MSPBLE_TASK_STACK_SIZE                   2048

/* Application Events */
#define MSPBLE_NONE                Event_Id_NONE
#define MSPBLE_EVT_PUI             Event_Id_00
#define MSPBLE_EVT_ADV_ENB         Event_Id_01
#define MSPBLE_EVT_ADV_END         Event_Id_02
#define MSPBLE_EVT_CONN_EST        Event_Id_03
#define MSPBLE_EVT_CONN_TERM       Event_Id_04
#define MSPBLE_EVT_BSL_BUTTON      Event_Id_24
#define MSPBLE_EVT_BUTTON_RESET    Event_Id_25
#define MSPBLE_EVT_BUTTON_RIGHT    Event_Id_30
#define MSPBLE_ERROR               Event_Id_31

/* How often to perform periodic event (in msec) */
#define MSPBLE_PERIODIC_EVT_PERIOD               5000
#define MSPBLE_DEFAULT_CONN_HANDLE               0xFFFF

/* Company Identifier: Texas Instruments Inc. (13) */
#define TI_COMPANY_ID                        0x000D
#define TI_ST_DEVICE_ID                      0x03
#define TI_ST_KEY_DATA_ID                    0x00



/* Period Units for PWM */
#define PWM_PERIOD_MAX 2400

#ifdef __cplusplus
}
#endif

#endif /* MSPBLE_H */
