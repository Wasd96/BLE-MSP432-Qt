/*
 * manager.h
 *
 *  Created on: 8 мая 2017 г.
 *      Author: Volk
 */

#ifndef PROFILE_MANAGER_SERVICE_H_
#define PROFILE_MANAGER_SERVICE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *                                INCLUDES
 ******************************************************************************/
#include <stdint.h>
#include "profile_util.h"


/*******************************************************************************
 *                                CONSTANTS
 ******************************************************************************/

/* Characteristic Types - These must be listed in order that they appear
   in service */
#define MAN_CHAR0               0x00

#define MAN_ID              PROFILE_ID_CREATE(MAN_CHAR0,PROFILE_VALUE)

/* LED Service UUID */
#define MAN_SERVICE_UUID        0x1200

/* LED Characteristic UUIDs */
#define MAN_UUID            0x1201

#define MAN_LEN             5

#define MAN_LEN_MIN         1


/*******************************************************************************
 *                                  FUNCTIONS
 ******************************************************************************/
/*
 * регистрация сервиса в сервере GATT
 */
extern uint8_t ManagerService_addService(void);

/*
 * регистрация вызовов хост-МК
 */
extern uint8_t ManagerService_registerAppCBs(BLEProfileCallbacks_t *appCallbacks);

/*
 * Установка параметра
 *
 *    len   - length of data to write
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern uint8_t ManagerService_setParameter(uint16_t len,
        void *value);

/*
 * Получение параметра
 *
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern uint8_t ManagerService_getParameter(void *value);

#ifdef __cplusplus
}
#endif


#endif /* PROFILE_MANAGER_SERVICE_H_ */
