/*
 * manager.c
 *
 *  Created on: 8 мая 2017 г.
 *      Author: Volk
 */


#include <string.h>
#include <ti/display/Display.h>
#include <ti/sap/snp.h>
#include <ti/sap/snp_rpc.h>
#include <ti/sap/snp_rpc_synchro.h>
#include <ti/sap/sap.h>
#include "manager_service.h"

/*******************************************************************************
 *                                   MACROS
 ******************************************************************************/
#define MAN_NUM_ATTR_SUPPORTED 1

/*******************************************************************************
 *                                  GLOBAL TYPEDEFS
 ******************************************************************************/
/* MAN_Service Service UUID */
static uint8_t MANServiceUUID[SNP_128BIT_UUID_SIZE] =
{ TI_BASE_UUID_128(MAN_SERVICE_UUID) };

/* MAN0 UUID */
static uint8_t ms_MAN0UUID[SNP_128BIT_UUID_SIZE] =
{ TI_BASE_UUID_128(MAN_UUID) };

/*******************************************************************************
 *                             VARIABLES
 ******************************************************************************/
static BLEProfileCallbacks_t *manServiceCallbacks;

/* Used for log messages */
extern Display_Handle displayOut;

/*******************************************************************************
 *                              Profile Attributes - TYPEDEFS
 ******************************************************************************/
static SAP_Service_t    MANService;
static SAP_CharHandle_t MANServiceCharHandles[MAN_NUM_ATTR_SUPPORTED];

/* Profile Service attribute */
static UUIDType_t MANServiceDecl =
{ SNP_128BIT_UUID_SIZE, MANServiceUUID };

/* Characteristic "MAN" Value variable */
static uint8_t MANVal[MAN_LEN] = {0};


/*******************************************************************************
 *                              Profile Attributes - TABLE
 ******************************************************************************/
static SAP_Char_t MANAttrTable[MAN_NUM_ATTR_SUPPORTED] =
{
    /* MAN0 Characteristic */
    {
        { SNP_128BIT_UUID_SIZE, ms_MAN0UUID },        /* UUID */
        SNP_GATT_PROP_READ | SNP_GATT_PROP_WRITE | SNP_GATT_PROP_WRITE_NORSP,
                                                      /* Properties */
        SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE, /* Permissions */
    },
};

/*******************************************************************************
 *                                  LOCAL FUNCTIONS
 ******************************************************************************/
static void Manager_processSNPEventCB(uint16_t event, snpEventParam_t *param);
static uint8_t ManagerService_ReadAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData);
static uint8_t ManagerService_WriteAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t len, uint8_t *pData);

/*******************************************************************************
 *                                 PUBLIC FUNCTIONS
 ******************************************************************************/

/*******************************************************************************
 * @fn      MANService_addService
 *
 * @brief   Initializes the LedService service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 ******************************************************************************/
uint8_t ManagerService_addService(void)
{
    /* Register to receive Connection Established Events */
    SAP_registerEventCB(Manager_processSNPEventCB,
            SNP_CONN_EST_EVT | SNP_CONN_TERM_EVT);

    /* Build Service to register with NP */
    MANService.serviceUUID = MANServiceDecl;
    MANService.serviceType = SNP_PRIMARY_SERVICE;
    MANService.charTableLen = MAN_NUM_ATTR_SUPPORTED;
    MANService.charTable = MANAttrTable;
    MANService.context = NULL;
    MANService.charReadCallback = ManagerService_ReadAttrCB;
    MANService.charWriteCallback = ManagerService_WriteAttrCB;
    MANService.cccdIndCallback = NULL;
    MANService.charAttrHandles = MANServiceCharHandles;

    Display_print1(displayOut,
            0,0,"MANService: Registered service, %d attributes",
                    MAN_NUM_ATTR_SUPPORTED);

    /* Service is set up, register with GATT server on the SNP. */
    return SAP_registerService(&MANService);
}

/*******************************************************************************
 * @fn      MANService_registerAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  BLE_PROFILE_SUCCESS or BLE_PROFILE_ALREADY_IN_REQ_MODE
 ******************************************************************************/
uint8_t ManagerService_registerAppCBs(BLEProfileCallbacks_t *appCallbacks)
{
    if (manServiceCallbacks == NULL)
    {
        if (appCallbacks->cccdUpdateCB != NULL
                && appCallbacks->charChangeCB != NULL)
        {
            manServiceCallbacks = appCallbacks;
            Display_print0(displayOut, 0, 0,
                    "MANService: Registered callbacks to application.");
        }
        else
        {
            Display_print0(displayOut, 0, 0,
                    "MANService: nullptr given for application callbacks.");
            return BLE_PROFILE_FAILURE;
        }
        return BLE_PROFILE_SUCCESS;
    }

    return BLE_PROFILE_ALREADY_IN_REQ_MODE;
}

/*******************************************************************************
 * @fn      MANService_setParameter
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
uint8_t ManagerService_setParameter(uint16_t len, void *value)
{
    uint8_t ret = BLE_PROFILE_SUCCESS;

    memset(MANVal, 0x00, 5);

    if (len <= MAN_LEN && len >= MAN_LEN_MIN)
    {
        memcpy(MANVal, value, len);
    }
    else
    {
        ret = BLE_PROFILE_INVALID_RANGE;
    }

    return ret;
}

/*******************************************************************************
 * @fn      MANService_getParameter
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
uint8_t ManagerService_getParameter(void *value)
{
    uint8_t ret = BLE_PROFILE_SUCCESS;

    memcpy(value, MANVal, MAN_LEN);

    return ret;
}

/*******************************************************************************
 * @fn      MANService_ReadAttrCB
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
static uint8_t ManagerService_ReadAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t offset, uint16_t size, uint16_t * pLen,
        uint8_t *pData)
{
    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl,
            MANServiceCharHandles,
            MAN_NUM_ATTR_SUPPORTED);
    uint8_t isValid = 0;

    /* Find settings for the characteristic to be read. */
    switch (PROFILE_ID_CHAR(charID))
    {
    case MAN_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            *pLen = strlen((char*)MANVal);
            memcpy(pData, MANVal, MAN_LEN);
            isValid = 1;
            break;

            /* Other considerations for MAN0 can be inserted here */
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
 * @fn      MANService_WriteAttrCB
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
static uint8_t ManagerService_WriteAttrCB(void *context, uint16_t conn,
        uint16_t charHdl, uint16_t len, uint8_t *pData)
{
    uint8_t status = SNP_UNKNOWN_ATTRIBUTE;
    memset(MANVal, 0x00, 5);

    /* Get characteristic from handle */
    uint8_t charID = ProfileUtil_getCharIDFromHdl(charHdl,
            MANServiceCharHandles,
            MAN_NUM_ATTR_SUPPORTED);

    /* Find settings for the characteristic to be read. */
    switch (PROFILE_ID_CHAR(charID))
    {
      case MAN_CHAR0:
        switch (PROFILE_ID_CHARTYPE(charID))
        {
        case PROFILE_VALUE:
            memcpy(MANVal, pData, len);
            status = SNP_SUCCESS;
            break;

        /* Other considerations for MAN1 can be inserted here */
        default:
            break;
        }
        break;

      default:
        break;
    }

    /* If a characteristic value changed then callback function to notify
       application of change */
    if (status != SNP_UNKNOWN_ATTRIBUTE && manServiceCallbacks->charChangeCB != NULL)
    {
        manServiceCallbacks->charChangeCB(MAN_ID);
    }

    return status;
}

/*******************************************************************************
 * @fn      MAN_processSNPEventCB
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
static void Manager_processSNPEventCB(uint16_t event, snpEventParam_t *param)
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
