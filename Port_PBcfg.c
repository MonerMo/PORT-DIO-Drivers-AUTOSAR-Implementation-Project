/*
 * Port_PBcfg.c
 *
 *  Created on: Apr 5, 2024
 *      Author: orabi
 */

/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

#include "Port.h"

/* Checking AUTOSAR version between both Port_PBcfg.c and Port.h files */
#if ((PORT_PBCFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_PBCFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Checking Software versions for both Port_PBcfg.c and Port.h files because they are the same module. */
#if ((PORT_PBCFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_PBCFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_PBCFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif


/* Now the configuration array of Structures */
/*
 * Data Type : Port_ConfigType , Which is an array of structures of type Port_ConfigChannel
 * Holds the following :
 *
 *  uint8                   Port_Num;
    Port_PinType            Ch_Num;
    uint8                   Ch_Initial_value;
    boolean                 Ch_Direction_Changeable;
    boolean                 Ch_Mode_Changeable;
    Port_PinDirectionType   Ch_Direction;
    Port_PinModeType        Ch_Mode;
    Port_InternalResistor   Resistor;
 *
 */

//const Port_ConfigType Port_Configuration = {

  //      PORT_F , PORT_PIN_1 , PORT_PIN_LEVEL_HIGH , STD_OFF , STD_OFF , PORT_PIN_OUT , PORT_PIN_MODE_DIO , OFF

//};


