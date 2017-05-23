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
/*******************************************************************************
 *                              INCLUDES
 ******************************************************************************/
/* Standard Defines */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
#include <file.h>
#include <mspble.h>


/* TI Defines */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/SDSPI.h>
#include <ti/display/Display.h>
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/power/PowerMSP432.h>
#include <ti/sap/sap.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <third_party/fatfs/ffcio.h>
#include <unistd.h>


/* Local Defines */
#include "simple_gatt_profile.h"
#include "Board.h"
#include "data_service.h"
#include "manager_service.h"
#include "stream_service.h"

//#include ".h"


/*******************************************************************************
 *                             LOCAL VARIABLES
 ******************************************************************************/
/* Port mapper configuration register */
const uint8_t port_mapping[] =
{
    /* Port P2 */
    PM_TA1CCR1A, PM_TA1CCR2A, PM_TA1CCR3A, PM_NONE, PM_NONE,
    PM_NONE, PM_NONE, PM_NONE
};

/* Used to block SNP calls during a synchronous transaction. */
Event_Handle MSPBLEEvent;

/* Project Zero State Machine */
MSPBLE_States_t MSPBLEState;

/* Used for log messages */
extern Display_Handle displayOut;


/* Task configuration */
Task_Struct MSPBLETask;
char MSPBLETaskStack[MSPBLE_TASK_STACK_SIZE];

Task_Struct SensorTask;
char SensorTaskStack[MSPBLE_TASK_STACK_SIZE];

/* SAP Parameters for opening serial port to SNP */
SAP_Params sapParams;

/* GAP - SCAN RSP data (max size = 31 bytes) */
static uint8_t scanRspData[] =
{
    /* Complete Name */
    0x0F, /* Length */
    SAP_GAP_ADTYPE_LOCAL_NAME_COMPLETE, 'M', 'S', 'P', ' ', 'S', 'D', ' ',
    'd', 's', 't', ' ', 'M', 'A', 'I',

    /* Connection interval range */
    0x05, /* Length */
    0x12, /* SAP_GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE */
    LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
    HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
    LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),
    HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),

    /* TX power level */
    0x02, /* Length */
    0x0A, /* SAP_GAP_ADTYPE_POWER_LEVEL */
    0     /* 0dBm */
};

/* SNP Device Name */
uint8_t updateSNPDevName[] = { 'M', 'S', 'P', ' ', 'S', 'D', ' ',
                               'd', 's', 't', ' ', 'M', 'A', 'I',};

/* GAP - Advertisement data (max size = 31 bytes, though this is
   best kept short to conserve power while advertising) */
static uint8_t advertData[] =
{
    0x02, /* Length */
    SAP_GAP_ADTYPE_FLAGS,
    DEFAULT_DISCOVERABLE_MODE | SAP_GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

    /* Manufacturer specific advertising data */
    0x06, 0xFF, /* SAP_GAP_ADTYPE_MANUFACTURER_SPECIFIC */
    LO_UINT16(TI_COMPANY_ID), HI_UINT16(TI_COMPANY_ID),
    TI_ST_DEVICE_ID,
    TI_ST_KEY_DATA_ID, 0x00 /* Key state */
};

/* Connection Handle - only one device currently allowed to connect to SNP */
static uint16_t connHandle = MSPBLE_DEFAULT_CONN_HANDLE;

/* BD Addr of the NWP */
static char nwpstr[] = "NWP:  0xFFFFFFFFFFFF";
#define nwpstrIDX       8

/* BD Addr of peer device in connection */
static char peerstr[] = "Peer: 0xFFFFFFFFFFFF";
#define peerstrIDX       8

/* Placeholder variable for characteristic initialization */
uint8_t initVal[40] = {0};
uint8_t initString[] = "This is a pretty long string, isn't it!";



Timer_Handle hc_timer;
unsigned int seconds = 0;
unsigned long logminute = 0;
double hc_elapsed;
char hc_ready = 0;
char hc_obtained = 0;
char streaming = 0;


char fatfsPrefix[] = "fat";

SDSPI_Handle sdspiHandle;
SDSPI_Params sdspiParams;

/* Variables for the CIO functions */
FILE *src, *dst;

char needToTranslate = 0;
char secToTranslate = 0;

#define MINUTE 60

char hc_data[MINUTE][STREAM_LEN];
char sendData[MINUTE][STREAM_LEN];

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void MSPBLE_init(void);

static void MSPBLE_initServices(void);
static void MSPBLE_asyncCB(uint8_t cmd1, void *pParams);
static void MSPBLE_taskFxn(UArg arg0, UArg arg1);
static void SensorLoop(UArg arg0, UArg arg1);

static void MSPBLE_processDataServiceCB(uint8_t charID);
static void MSPBLE_processDataServicecccdCB(uint8_t charID,
        uint16_t value);
static void MSPBLE_processManagerServiceCB(uint8_t charID);
static void MSPBLE_processManagerServicecccdCB(uint8_t charID,
        uint16_t value);
static void MSPBLE_processStreamServiceCB(uint8_t charID);
static void MSPBLE_processStreamServicecccdCB(uint8_t charID,
        uint16_t value);

static void hc_setReady(Timer_Handle handle);

/*******************************************************************************
 *                                 PROFILE CALLBACKS
 ******************************************************************************/

/*
 * Data Characteristic value change callback
 */
BLEProfileCallbacks_t MSPBLE_DataServiceCBs =
{
    MSPBLE_processDataServiceCB,
    MSPBLE_processDataServicecccdCB
};

/*
 * Manager Characteristic value change callback
 */
BLEProfileCallbacks_t MSPBLE_ManagerServiceCBs =
{
    MSPBLE_processManagerServiceCB,
    MSPBLE_processManagerServicecccdCB
};

/*
 * Stream Characteristic value change callback
 */
BLEProfileCallbacks_t MSPBLE_StreamServiceCBs =
{
    MSPBLE_processStreamServiceCB,
    MSPBLE_processStreamServicecccdCB
};

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/
/*******************************************************************************
 * @fn      MSPBLE_createTask
 *
 * @brief   Task creation function for the Simple BLE Peripheral.
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
void MSPBLE_createTask(void)
{
    Task_Params taskParams;

    Task_Params_init(&taskParams);
    taskParams.stack = MSPBLETaskStack;
    taskParams.stackSize = MSPBLE_TASK_STACK_SIZE;
    taskParams.priority = MSPBLE_TASK_PRIORITY;

    Task_construct(&MSPBLETask, MSPBLE_taskFxn, &taskParams, NULL);



    Task_Params_init(&taskParams);
    taskParams.stack = SensorTaskStack;
    taskParams.stackSize = MSPBLE_TASK_STACK_SIZE;
    taskParams.priority = MSPBLE_TASK_PRIORITY;

    Task_construct(&SensorTask, SensorLoop, &taskParams, NULL);



}


/*
 * Function for translate integer/double to char string.
 * Due to some reasons sprintf does not work with double.
 */

void ntoa(char *buff, double number, int n)
{
    int i = 0;
    int j = 0;
    int point;
    unsigned long high = (unsigned long)number;
    double low = number - (unsigned long)number;

    while (high > 0 && i < n) {
        buff[i] = high%10 + '0';
        high = high/10;
        i++;
    }
    point = i;
    for (j = 0; j < i/2; j++) {
        char temp;

        temp = buff[j];
        buff[j] = buff[i-j-1];
        buff[i-j-1] = temp;
    }
    if (number < 1) {
        buff[0] = '0';
        point++;
    }
    buff[point] = 0;
    if (low > 0) {
        char lowstr[128];

        i = 0;
        buff[point] = '.';
        buff[point+1] = 0;
        while (low > 0 && i+point < n) {
            low = low*10.0;
            lowstr[i] = (char)low + '0';
            low = low - (unsigned long)low;
            i++;
        }
        lowstr[i] = 0;
        strcat(buff, lowstr);
    }
}


/*******************************************************************************
 * @fn      MSPBLE_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
static void MSPBLE_init(void)
{
    /* Create RTOS Event */
    MSPBLEEvent = Event_create(NULL, NULL);

    /* Zeroing out initial value array */
    memset(initVal, 0x00, 40);

    /* Write to the UART. */
    Display_print0(displayOut, 0, 0,"--------- Project Zero Example ---------");
    Display_print0(displayOut,0,0,
            "Initializing the user task, hardware, BLE stack and services.");



     ManagerService_registerAppCBs(&MSPBLE_ManagerServiceCBs);

     DataService_registerAppCBs(&MSPBLE_DataServiceCBs);

     StreamService_registerAppCBs(&MSPBLE_StreamServiceCBs);
}



/*******************************************************************************
 * @fn      MSPBLE_taskFxn
 *
 * @brief   Application task entry point for the Simple BLE Peripheral.
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 ******************************************************************************/
//void *MSPBLE_taskFxn(void *arg0)
void MSPBLE_taskFxn(UArg arg0, UArg arg1)
{
    uint32_t events;
    uint8_t enableAdv = 1;
    uint8_t disableAdv = 0;

    /* Initialize State Machine */
    MSPBLEState = MSPBLE_RESET;

    /* Initialize application */
    MSPBLE_init();

    /* Application main loop */
    while(1)
    {
        switch (MSPBLEState)
        {
        case MSPBLE_RESET:
        {
            /* Make sure CC26xx is not in BSL */
            GPIO_write(Board_DP0, Board_LED_OFF);
            GPIO_write(Board_DP2, Board_LED_ON);

            usleep(10000);
            GPIO_write(Board_DP0, Board_LED_ON);

            /* Initialize UART port parameters within SAP parameters */
            SAP_initParams(SAP_PORT_REMOTE_UART, &sapParams);

            sapParams.port.remote.mrdyPinID = Board_MRDY;
            sapParams.port.remote.srdyPinID = Board_SRDY;
            sapParams.port.remote.boardID = Board_UART1;

            /* Setup NP module */
            SAP_open(&sapParams);

            /* Register Application thread's callback to receive asynchronous 
                requests from the NP. */
            SAP_setAsyncCB(MSPBLE_asyncCB);

            /* Reset the NP, and await a powerup indication.
               Clear any pending power indications received prior to this reset
               call */
            Event_pend(MSPBLEEvent, MSPBLE_NONE,
                       MSPBLE_EVT_PUI, 1000);
            SAP_reset();

            events = Event_pend(MSPBLEEvent, MSPBLE_NONE,
            MSPBLE_EVT_PUI + MSPBLE_EVT_BSL_BUTTON,
                    BIOS_WAIT_FOREVER);

            /* Read BD ADDR */
            SAP_setParam(SAP_PARAM_HCI, SNP_HCI_OPCODE_READ_BDADDR, 0,
                    NULL);

            /* Setup Services - Service creation is blocking so no need
               to pend */
            MSPBLE_initServices();
            MSPBLEState = MSPBLE_START_ADV;
        }
            break;

        case MSPBLE_START_ADV:
        {

            /* Set advertising data. */
            SAP_setServiceParam(SNP_GGS_SERV_ID, SNP_GGS_DEVICE_NAME_ATT,
                                   sizeof(updateSNPDevName), updateSNPDevName);

            SAP_setParam(SAP_PARAM_ADV, SAP_ADV_DATA_NOTCONN,
                    sizeof(advertData), advertData);

            /* Set Scan Response data. */
            SAP_setParam(SAP_PARAM_ADV, SAP_ADV_DATA_SCANRSP,
                    sizeof(scanRspData), scanRspData);

            /* Enable Advertising and await NP response */
            SAP_setParam(SAP_PARAM_ADV, SAP_ADV_STATE, 1, &enableAdv);
            Event_pend(MSPBLEEvent, MSPBLE_NONE,
                       MSPBLE_EVT_ADV_ENB, BIOS_WAIT_FOREVER);

            /* Wait for connection or button press to cancel advertisement */
            events = Event_pend(MSPBLEEvent, MSPBLE_EVT_ADV_END,
                                MSPBLE_EVT_CONN_EST,
                                30000);

            MSPBLEState = (events & MSPBLE_EVT_CONN_EST) ?
                     MSPBLE_CONNECTED : MSPBLE_CANCEL_ADV;
        }
            break;

        case MSPBLE_CONNECTED:

            /* Turn on user LED to indicate connection */
            GPIO_write(Board_LED0, Board_LED_ON);

            /* Before connecting, NP will send the stop ADV meesage */
            Event_pend(MSPBLEEvent, MSPBLE_NONE,
                       MSPBLE_EVT_ADV_END, BIOS_WAIT_FOREVER);

            /* Events that can happen during connection - Client Disconnection
                                                        - AP Disconnection */
            events = Event_pend(MSPBLEEvent, MSPBLE_NONE,
                                MSPBLE_EVT_CONN_TERM, BIOS_WAIT_FOREVER);

            if (events & MSPBLE_EVT_CONN_TERM)
            {
                /* Client has disconnected from server */
                SAP_setParam(SAP_PARAM_CONN, SAP_CONN_STATE, sizeof(connHandle),
                        (uint8_t *) &connHandle);
                Event_pend(MSPBLEEvent, MSPBLE_NONE,
                           MSPBLE_EVT_CONN_TERM, BIOS_WAIT_FOREVER);

                MSPBLEState = MSPBLE_CANCEL_ADV;
            }
            break;

        case MSPBLE_CANCEL_ADV:
            /* Cancel Advertisement */
            SAP_setParam(SAP_PARAM_ADV, SAP_ADV_STATE, 1, &disableAdv);
            Event_pend(MSPBLEEvent, MSPBLE_NONE,
                       MSPBLE_EVT_ADV_END, BIOS_WAIT_FOREVER);

            MSPBLEState = MSPBLE_IDLE;
            break;

        case MSPBLE_IDLE:
            /* Turn off user LED to indicate stop advertising */
            GPIO_write(Board_LED0, Board_LED_OFF);
            MSPBLEState = MSPBLE_START_ADV;
            break;

        default:
            break;
        }

    }
}

void SensorLoop(UArg arg0, UArg arg1)
{
    Timer_Params timerParams;
    char *name;
    char *buff;
    int i;

    name = (char *)calloc(64, sizeof(char));
    buff = (char *)calloc(512, sizeof(char));

    SDSPI_init();

    add_device(fatfsPrefix, _MSA, ffcio_open, ffcio_close, ffcio_read,
            ffcio_write, ffcio_lseek, ffcio_unlink, ffcio_rename);

    SDSPI_Params_init(&sdspiParams);
    sdspiHandle = SDSPI_open(Board_SDSPI0, 0, &sdspiParams);
    if (sdspiHandle == NULL) {
        Display_printf(displayOut, 0, 0, "Error starting the SD card\n");
        while (1);
    }
    else {
        Display_printf(displayOut, 0, 0, "Drive %u is mounted\n", 0);
    }


    src = fopen("fat:0:minutes.txt", "r");
    if (src == NULL)
    {
        Display_printf(displayOut, 0, 0, "error open file");
        return;
    }
    int size = 0;
    size = fread(buff, 1, 64, src);
    for (i = 0; i < size; i++)
    {
        logminute *= 10;
        logminute += buff[i] - '0';
    }
    fclose(src);


    Timer_Params_init(&timerParams);
    timerParams.period = 1;
    timerParams.periodUnits = TIMER_PERIOD_HZ;
    timerParams.timerCallback = hc_setReady;
    timerParams.timerMode = TIMER_CONTINUOUS_CB;
    hc_timer = Timer_open(0, &timerParams);

    Timer_start(hc_timer);

    seconds = 0;
    hc_obtained = 0;
    hc_ready = 0;




    while (1)
    {
        if (secToTranslate > 0)
            usleep(100);
        else
            usleep(500000);


        if (needToTranslate)
        {
            char bufftranslate[8] = { 0 };
            int len;
            int i;
            long min = 0;

            DataService_getParameter(DS_STRING_ID, bufftranslate);
            len = strlen(bufftranslate);
            for (i = 0; i < len; i++)
            {
                min *= 10;
                min += bufftranslate[i] - '0';
            }
            if (min >= 0 && min < logminute || secToTranslate != 0)
            {
                if (secToTranslate == 0) // at first second load minute
                {
                    strcpy(name, "fat:0:log");
                    sprintf(buff, "%u", min);
                    strcat(name, buff);
                    strcat(name, ".txt");

                    src = fopen(name, "r");
                    if (src == NULL)
                    {
                        strcpy(buff, "error");
                        DataService_setParameter(DS_STRING_ID, strlen(buff), buff);
                        Display_printf(displayOut, 0, 0, "error open file");
                        return;
                    }
                    for (i = 0; i < MINUTE; i++)
                    {
                        fread(sendData[i], 1, STREAM_LEN, src);
                    }
                    fclose(src);

                }

                DataService_setParameter(DS_STREAM_ID, STREAM_LEN, sendData[secToTranslate]);
                secToTranslate++;
                if (secToTranslate >= 60)
                    secToTranslate = 0;
                needToTranslate = 0;

            }
            else
            {
                strcpy(buff, "error");
                DataService_setParameter(DS_STRING_ID, strlen(buff), buff);
                Display_printf(displayOut, 0, 0, "wrong minute: %u", min);
            }

            needToTranslate = 0;
        }


        if (hc_ready)
        {
            GPIO_write(HC_TRIG, Board_LED_OFF);
            usleep(2);
            GPIO_write(HC_TRIG, Board_LED_ON);
            usleep(10);
            GPIO_write(HC_TRIG, Board_LED_OFF);


            //uint32_t baseAddress = GPIO_PORT_TO_BASE[6];
            GPIO_write(HC_ECHO, Board_LED_ON);
            usleep(1);

            while(HWREG16(0x40004C41) & 16)
            {
                hc_elapsed++;
            }
            hc_elapsed = hc_elapsed / 58 * 6;
            // 5330 - miss

            hc_ready = 0;
            seconds++;

            Display_printf(displayOut, 0, 0, "%d, %lf", seconds, hc_elapsed);
            memset(buff, 0x00, sizeof(buff));
            ntoa(buff, hc_elapsed, STREAM_LEN);
            memcpy(hc_data[seconds], buff, STREAM_LEN);

            if (streaming)
                StreamService_setParameter(STREAM_LEN, buff);

            hc_elapsed = 0;
        }

        if (seconds >= MINUTE-1) // write down this minute
        {
            strcpy(name, "fat:0:log");
            sprintf(buff, "%u", logminute);
            strcat(name, buff);
            strcat(name, ".txt");
            ManagerService_setParameter(strlen(buff), buff); // old minute completed


            dst = fopen(name, "w");
            for (i = 0; i < MINUTE; i++)
            {
                fwrite(hc_data[i], 1, STREAM_LEN, dst);
            }
            fflush(dst);
            fclose(dst);

            dst = fopen("fat:0:minutes.txt", "w");
            sprintf(buff, "%u", logminute);
            fwrite(buff, 1, strlen(buff), dst);
            fflush(dst);
            fclose(dst);


            Display_printf(displayOut, 0, 0, "%u minute!", logminute);
            seconds = 0;
            logminute++;
        }

    }
}


void hc_setReady(Timer_Handle handle)
{
    hc_ready = 1;
}

/*******************************************************************************
 * @fn      MSPBLE_initServices
 *
 * @brief   Configure SNP and register services.
 *
 * @param   None.
 *
 * @return  None.
 ******************************************************************************/
static void MSPBLE_initServices(void)
{
    ManagerService_setParameter(MAN_LEN, initVal);
    ManagerService_setParameter(1, "0");
    ManagerService_addService();

    DataService_setParameter(DS_STRING_ID, DS_STRING_LEN, initVal);
    DataService_setParameter(DS_STREAM_ID, DS_STREAM_LEN, initVal);
    DataService_addService();

    StreamService_setParameter(STREAM_LEN, initVal);
    StreamService_addService();

}

/*
 * This is a callback operating in the NPI task.
 * These are events this application has registered for.
 */
static void MSPBLE_processSNPEventCB(uint16_t event,
        snpEventParam_t *param)
{
    switch (event)
    {
    case SNP_CONN_EST_EVT:
    {
        snpConnEstEvt_t * connEstEvt = (snpConnEstEvt_t *) param;

        /* Update peer address string */
        connHandle = connEstEvt->connHandle;
        ProfileUtil_convertBdAddr2Str(&peerstr[peerstrIDX], connEstEvt->pAddr);

        /* Notify state machine of established connection */
        Event_post(MSPBLEEvent, MSPBLE_EVT_CONN_EST);

        Display_printf(displayOut, 0, 0, "connection established!");
    }
        break;

    case SNP_CONN_TERM_EVT:
    {
        connHandle = MSPBLE_DEFAULT_CONN_HANDLE;
        /* Notify state machine of disconnection event */
        Event_post(MSPBLEEvent, MSPBLE_EVT_CONN_TERM);

        Display_printf(displayOut, 0, 0, "connection closed");
    }
        break;

    case SNP_ADV_STARTED_EVT:
    {
        snpAdvStatusEvt_t *advEvt = (snpAdvStatusEvt_t *) param;
        if (advEvt->status == SNP_SUCCESS)
        {
            /* Notify state machine of Advertisement Enabled */
            Event_post(MSPBLEEvent, MSPBLE_EVT_ADV_ENB);
        } else
        {
            Event_post(MSPBLEEvent, MSPBLE_ERROR);
        }
    }
        break;

    case SNP_ADV_ENDED_EVT:
    {
        snpAdvStatusEvt_t *advEvt = (snpAdvStatusEvt_t *) param;
        if (advEvt->status == SNP_SUCCESS)
        {
            /* Notify state machine of Advertisement Disabled */
            Event_post(MSPBLEEvent, MSPBLE_EVT_ADV_END);
        }
    }
        break;

    default:
        break;
    }
}

/*
 * This is a callback operating in the NPI task. These are asynchronous
 * indications.
 */
 static void MSPBLE_asyncCB(uint8_t cmd1, void *pParams)
{
    switch (SNP_GET_OPCODE_HDR_CMD1(cmd1))
    {
    case SNP_DEVICE_GRP:
    {
        switch (cmd1)
        {
        case SNP_POWER_UP_IND:
            Event_post(MSPBLEEvent, MSPBLE_EVT_PUI);
            break;

        case SNP_HCI_CMD_RSP:
        {
            snpHciCmdRsp_t *hciRsp = (snpHciCmdRsp_t *) pParams;
            switch (hciRsp->opcode)
            {
            case SNP_HCI_OPCODE_READ_BDADDR:
                ProfileUtil_convertBdAddr2Str(&nwpstr[nwpstrIDX],
                        hciRsp->pData);
                break;
            default:
                break;
            }
        }
            break;

        case SNP_EVENT_IND:
            Event_post(MSPBLEEvent, MSPBLE_EVT_ADV_ENB);
            break;

        default:
            break;
        }
    }
        break;

    default:
        break;
    }
}



/*
 * Callbacks for the Data Service
 */
static void MSPBLE_processDataServiceCB(uint8_t charID)
{
    needToTranslate = 1;

}

static void MSPBLE_processDataServicecccdCB(uint8_t charID, uint16_t value)
{

}


/*
 * Callbacks for the Stream Service
 */
static void MSPBLE_processStreamServiceCB(uint8_t charID)
{

}

static void MSPBLE_processStreamServicecccdCB(uint8_t charID, uint16_t value)
{
    streaming = value;
}

/*
 * Callbacks for the Manager Service
 */
static void MSPBLE_processManagerServiceCB(uint8_t charID)
{
    uint8_t value[MAN_LEN] = { 0 };

    switch (PROFILE_ID_CHAR(charID))
    {
    case MAN_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:

            ManagerService_getParameter(&value);

            if (value[0] == 'r')
            {
                // reset counter
                Display_printf(displayOut, 0, 0, "Reset!");
                value[0] = '0';
                value[1] = 0;
                ManagerService_setParameter(MAN_LEN, &value);

                logminute = 0;
                seconds = 0;
            }

            break;

        default:
            break;
        }
        break;
    default:
        break;
    }
}

static void MSPBLE_processManagerServicecccdCB(uint8_t charID, uint16_t value)
{

}

int32_t fatfs_getFatTime(void)
{
    time_t seconds;
    uint32_t fatTime;
    struct tm *pTime;

    /*
     *  TI time() returns seconds elapsed since 1900, while other tools
     *  return seconds from 1970.  However, both TI and GNU localtime()
     *  sets tm tm_year to number of years since 1900.
     */
    seconds = time(NULL);

    pTime = localtime(&seconds);

    /*
     *  localtime() sets pTime->tm_year to number of years
     *  since 1900, so subtract 80 from tm_year to get FAT time
     *  offset from 1980.
     */
    fatTime = ((uint32_t)(pTime->tm_year - 80) << 25) |
        ((uint32_t)(pTime->tm_mon) << 21) |
        ((uint32_t)(pTime->tm_mday) << 16) |
        ((uint32_t)(pTime->tm_hour) << 11) |
        ((uint32_t)(pTime->tm_min) << 5) |
        ((uint32_t)(pTime->tm_sec) >> 1);

    return ((int32_t)fatTime);
}
