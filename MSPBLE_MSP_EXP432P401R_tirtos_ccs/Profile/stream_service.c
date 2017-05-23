/*
 * stream_service.c
 *
 *  Created on: 9 мая 2017 г.
 *      Author: Volk
 */


#include <string.h>
#include <ti/display/Display.h>
#include <ti/sap/snp.h>
#include <ti/sap/snp_rpc.h>
#include <ti/sap/snp_rpc_synchro.h>
#include <ti/sap/sap.h>
#include "stream_service.h"

/*******************************************************************************
 *                                   MACROS
 ******************************************************************************/
#define STREAM_NUM_ATTR_SUPPORTED 1

/*******************************************************************************
 *                                  GLOBAL TYPEDEFS
 ******************************************************************************/
/* STREAM_Service Service UUID */
static uint8_t STREAMServiceUUID[SNP_128BIT_UUID_SIZE] =
{ TI_BASE_UUID_128(STREAM_SERVICE_UUID) };

/* STREAM0 UUID */
static uint8_t ss_STREAM0UUID[SNP_128BIT_UUID_SIZE] =
{ TI_BASE_UUID_128(STREAM_UUID) };

/*******************************************************************************
 *                             VARIABLES
 ******************************************************************************/
static BLEProfileCallbacks_t *streamServiceCallback;

static uint16_t connHandle = 0;
static uint8_t cccdFlag = 0;

/* Used for log messages */
extern Display_Handle displayOut;


/*******************************************************************************
 *                              Profile Attributes - TYPEDEFS
 ******************************************************************************/
static SAP_Service_t    STREAMService;
static SAP_CharHandle_t StreamServiceCharHandles[STREAM_NUM_ATTR_SUPPORTED];

/* Profile Service attribute */
static UUIDType_t STREAMServiceDecl =
{ SNP_128BIT_UUID_SIZE, STREAMServiceUUID };

/* Characteristic "STREAM" Value variable */
static uint8_t STREAMVal[STREAM_LEN] = { 0 };


/*******************************************************************************
 *                              Profile Attributes - TABLE
 ******************************************************************************/

static SAP_UserCCCDAttr_t DataCCCD =
{ SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE };


static SAP_Char_t STREAMAttrTable[STREAM_NUM_ATTR_SUPPORTED] =
{
    /* STREAM0 Characteristic */
    {
        { SNP_128BIT_UUID_SIZE, ss_STREAM0UUID },        /* UUID */
        SNP_GATT_PROP_NOTIFICATION,                     /* Properties */
        SNP_GATT_PERMIT_READ,                            /* Permissions */
        NULL,                                       /* User Description */
        &DataCCCD                                       /* CCCD */
    },
};

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void Stream_processSNPEventCB(uint16_t event, snpEventParam_t *param);
static uint8_t StreamService_ReadAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData);
static uint8_t StreamService_CCCDIndCB(void *context, uint16_t conn,
        uint16_t cccdHdl, uint8_t type, uint16_t value);

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 * @fn      STREAMService_addService
 *
 * @brief   Initializes the LedService service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 ******************************************************************************/
uint8_t StreamService_addService(void)
{
    /* Register to receive Connection Established Events */
    SAP_registerEventCB(Stream_processSNPEventCB,
            SNP_CONN_EST_EVT | SNP_CONN_TERM_EVT);

    /* Build Service to register with NP */
    STREAMService.serviceUUID = STREAMServiceDecl;
    STREAMService.serviceType = SNP_PRIMARY_SERVICE;
    STREAMService.charTableLen = STREAM_NUM_ATTR_SUPPORTED;
    STREAMService.charTable = STREAMAttrTable;
    STREAMService.context = NULL;
    STREAMService.charReadCallback = StreamService_ReadAttrCB;
    STREAMService.charWriteCallback = NULL;
    STREAMService.cccdIndCallback = StreamService_CCCDIndCB;
    STREAMService.charAttrHandles = StreamServiceCharHandles;

    Display_print1(displayOut,
            0,0,"StreamService: Registered service, %d attributes",
                    STREAM_NUM_ATTR_SUPPORTED);

    /* Service is set up, register with GATT server on the SNP. */
    return SAP_registerService(&STREAMService);
}

/*******************************************************************************
 * @fn      STREAMService_registerAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  BLE_PROFILE_SUCCESS or BLE_PROFILE_ALREADY_IN_REQ_MODE
 ******************************************************************************/
uint8_t StreamService_registerAppCBs(BLEProfileCallbacks_t *appCallbacks)
{
    if (streamServiceCallback == NULL)
    {
        if (appCallbacks->cccdUpdateCB != NULL
                && appCallbacks->charChangeCB != NULL)
        {
            streamServiceCallback = appCallbacks;
            Display_print0(displayOut, 0, 0,
                    "StreamService: Registered callbacks to application.");
        }
        else
        {
            Display_print0(displayOut, 0, 0,
                    "StreamService: nullptr given for application callbacks.");
            return BLE_PROFILE_FAILURE;
        }
        return BLE_PROFILE_SUCCESS;
    }

    return BLE_PROFILE_ALREADY_IN_REQ_MODE;
}

/*******************************************************************************
 * @fn      STREAMService_setParameter
 *
 * @brief   Set a LedService parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint8_t
 ******************************************************************************/
uint8_t StreamService_setParameter(uint16_t len, void *value)
{
    uint8_t ret = BLE_PROFILE_SUCCESS;
    snpNotifIndReq_t localReq;

    memset(STREAMVal, 0x00, STREAM_LEN);

    if (len == STREAM_LEN)
    {
        memcpy(STREAMVal, value, len);

        localReq.attrHandle = ProfileUtil_getHdlFromCharID(STREAM_CHAR0,
                        StreamServiceCharHandles,
                        STREAM_NUM_ATTR_SUPPORTED);

        localReq.connHandle = connHandle;
        localReq.pData = STREAMVal;
        localReq.authenticate = 0;

        /* Check for whether a notification or indication should be sent.
           Both flags should never be allowed to be set by NWP */
        if (cccdFlag)
        {
           localReq.type = SNP_SEND_NOTIFICATION;
           SNP_RPC_sendNotifInd(&localReq, len);
        }
    }
    else
    {
        ret = BLE_PROFILE_INVALID_RANGE;
    }

    return ret;
}

/*******************************************************************************
 * @fn      STREAMService_getParameter
 *
 * @brief   Get a LedService parameter
 *
 * @param   param - Profile parameter IDz
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  uint8_t
 ******************************************************************************/
uint8_t StreamService_getParameter(void *value)
{
    uint8_t ret = BLE_PROFILE_SUCCESS;

    memcpy(value, STREAMVal, STREAM_LEN);

    return ret;
}

/*******************************************************************************
 * @fn      STREAMService_ReadAttrCB
 *
 * @brief   Read an attribute.
 *
 * @param   context - context used when registering service
 * @param   conn    - connection handle ID
 * @param   charHdl - characteristic value handle
 * @param   offset  - offset of data to be read
 * @param   size    - maximum size of data bytes to be read
 * @param   pLen    - amount of bytes copied into pData
 * @param   pData   - pointer to copy read data
 *
 * @return  BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
static uint8_t StreamService_ReadAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData)
{
    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl,
            StreamServiceCharHandles,
            STREAM_NUM_ATTR_SUPPORTED);
    uint8_t isValid = 0;

    /* Find settings for the characteristic to be read. */
    switch (PROFILE_ID_CHAR(charID))
    {
    case STREAM_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = STREAM_LEN;
            memcpy(pData, STREAMVal, *pLen);
            isValid = 1;
            break;

            /* Other considerations for STREAM0 can be inserted here */
        default:
            break;
        }
        break;

    default:
        break;
    }

    if (isValid)
    {
        return (SNP_SUCCESS);
    }

    /* Unable to find handle - set len to 0 and return error code */
    *pLen = 0;
    return (SNP_UNKNOWN_ATTRIBUTE);
}


/*******************************************************************************
 * @fn      STREAM_processSNPEventCB
 *
 * @brief   This is a callback operating in the NPI task. It will be invoked
 *          whenever an event is received from the SNP that this profile has
 *          registered for
 *
 * @param   event  - event mask
 * @param   pValue - pointer event struct
 *
 * @return  status
 ******************************************************************************/
static void Stream_processSNPEventCB(uint16_t event, snpEventParam_t *param)
{
    switch (event)
    {
    case SNP_CONN_EST_EVT:
    {
    }
        break;

    case SNP_CONN_TERM_EVT:
    {
    }
        break;

    default:
        break;
    }
}


/*******************************************************************************
 * @fn      StreamService_CCCDIndCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message
 *
 * @return  BLE_PROFILE_SUCCESS, blePending or Failure
 ******************************************************************************/
uint8_t StreamService_CCCDIndCB(void *context, uint16_t connectionHandle,
        uint16_t cccdHdl, uint8_t type, uint16_t value)
{
    uint8_t status = SNP_UNKNOWN_ATTRIBUTE;

    /* Update connection handle (assumes one connection) */
    connHandle = connectionHandle;

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(cccdHdl,
                                                  StreamServiceCharHandles,
                                                  STREAM_NUM_ATTR_SUPPORTED);

    switch (PROFILE_ID_CHAR(charID))
    {
    case STREAM_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_CCCD:
            /* Set global cccd flag which will be used to to gate indications
             or notifications when SetParameter() is called */
            cccdFlag = value;
            status = SNP_SUCCESS;
            break;
        default:
            break;
        }
        break;

    default:
        break;
    }

    /* If a characteristic value changed then callback function to notify
     * application of change
     */
    if (streamServiceCallback && streamServiceCallback->cccdUpdateCB)
    {
        streamServiceCallback->cccdUpdateCB(STREAM_CHAR0, value);
    }

    return status;
}
