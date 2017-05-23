/*
 * stream_service.h
 *
 *  Created on: 9 мая 2017 г.
 *      Author: Volk
 */

#ifndef PROFILE_STREAM_SERVICE_H_
#define PROFILE_STREAM_SERVICE_H_

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
#define STREAM_CHAR0               0x00

#define STREAM_ID              PROFILE_ID_CREATE(STREAM_CHAR0,PROFILE_VALUE)

/* LED Service UUID */
#define STREAM_SERVICE_UUID        0x1220

/* LED Characteristic UUIDs */
#define STREAM_UUID            0x1221

#define STREAM_LEN             8


/*******************************************************************************
 *                                  FUNCTIONS
 ******************************************************************************/
/*
 * регистрация сервиса в сервере GATT
 */
extern uint8_t StreamService_addService(void);

/*
 * регистрация вызовов хост-МК
 */
extern uint8_t StreamService_registerAppCBs(BLEProfileCallbacks_t *appCallbacks);

/*
 * Установка параметра
 *
 *    len   - length of data to write
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern uint8_t StreamService_setParameter(uint16_t len,
        void *value);

/*
 * Получение параметра
 *
 *    value - pointer to data to write.  This is dependent on
 *            the parameter ID and may be cast to the appropriate
 *            data type (example: data type of uint16_t will be cast to
 *            uint16_t pointer).
 */
extern uint8_t StreamService_getParameter(void *value);

#ifdef __cplusplus
}
#endif

#endif /* PROFILE_STREAM_SERVICE_H_ */
