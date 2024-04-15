 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Moner Mohammed AKA Orabi :)
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Moner Mohammed's ID = 1001 :) */
#define PORT_VENDOR_ID    (1001U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Dio Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((PORT_AR_RELEASE_MAJOR_VERSION != STD_TYPES_AR_RELEASE_MAJOR_VERSION)\
  || (PORT_AR_RELEASE_MINOR_VERSION != STD_TYPES_AR_RELEASE_MINOR_VERSION)\
  || (PORT_AR_RELEASE_PATCH_VERSION != STD_TYPES_AR_RELEASE_PATCH_VERSION))
    #error "The AR version of Std_Types.h does not match the expected version."
#endif

/* Including Port_Cfg.h for Pre-compile configurations */
#include "Port_Cfg.h"

/* AUTOSAR checking between Port_Cfg.h and Port.h Files in PORT Module */
#if ((PORT_AR_RELEASE_MAJOR_VERSION != PORT_CFG_AR_RELEASE_MAJOR_VERSION)\
  || (PORT_AR_RELEASE_MINOR_VERSION != PORT_CFG_AR_RELEASE_MINOR_VERSION)\
  || (PORT_AR_RELEASE_PATCH_VERSION != PORT_CFG_AR_RELEASE_PATCH_VERSION))
    #error "The AR version of Std_Types.h does not match the expected version."
#endif

/* Software version checking between Port_Cfg.h and Port.h Files in PORT Module */
#if ((PORT_SW_MAJOR_VERSION != PORT_CFG_SW_MAJOR_VERSION)\
  || (PORT_SW_MINOR_VERSION != PORT_CFG_SW_MINOR_VERSION)\
  || (PORT_SW_PATCH_VERSION != PORT_CFG_SW_PATCH_VERSION))
    #error "The AR version of Std_Types.h does not match the expected version."
#endif

/* Non AUTOSAR Files */
#include "Common_Macros.h"

/*******************************************************************************
 *                           API Service Id Macros                             *
 *******************************************************************************/

/* Service ID for PORT initialization */
#define PORT_INIT_SID                       (uint8)0x00

/* Service ID for Setting Pin direction in a specific port */
#define PORT_SET_PIN_DIRECTION_SID          (uint8)0x01

/* Service ID for Refreshing a Pin in specific port */
#define PORT_REFRESH_PORT_DIRECTION_SID     (uint8)0x02

/* Service ID for Getting version info for the module */
#define PORT_GET_VERSION_INFO_SID           (uint8)0x03

/* Service ID for Setting a Pin Mode in a specific port */
#define PORT_SET_PIN_MODE_SID               (uint8)0x04


/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/* DET code for Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN                    (uint8)0x0A

/* DET code for Port Pin not configured as direction changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B

/* DET code for Port_Init service called with wrong parameter. */
#define PORT_E_PARAM_CONFIG                 (uint8)0x0C

/* DET code for API Port_SetPinMode Service called with an un-valid mode for pin */
#define PORT_E_PARAM_INVALID_MODE           (uint8)0x0D

/* DET code for API Port_SetPinMode service called when mode is unchangeable. */
#define PORT_E_MODE_UNCHANGEABLE            (uint8)0x0E

/* DET code for API service called without module initialization */
#define PORT_E_UNINIT                       (uint8)0x0F

/* DET code for APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER                (uint8)0x10

/*******************************************************************************
 *                              Module Definitions                             *
 *******************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Description: uint8 to hold the number of the PIN */
typedef uint8 Port_PinType;

/* Description: uint8 to hold PIN mode type */
typedef uint8 Port_PinModeType;


/* Description: Enum to hold PIN direction */
typedef enum
{
    PORT_PIN_IN,PORT_PIN_OUT
}Port_PinDirectionType;


/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* not in Requirements */
typedef enum{
    PORT_PIN_LEVEL_LOW,PORT_PIN_LEVEL_HIGH
}Port_PinInitialValue;

/* Description: Structure to configure each individual PIN:
 *
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *	3. Initial Value of the pin ( this port pin configuration is mandatory when the port pin is used for DIO. )
 *	4. the direction of pin --> INPUT or OUTPUT
 *  5. Pin Mode.
 *  6. the internal resistor --> Disable, Pull up or Pull down.
 *  7. Boolean Flag to tell either this pin direction is changeable or not.
 *  8. Boolean Flag to tell either this pin mode is changeable or not.
 *
 */

typedef struct 
{

    uint8                   Port_Num;
    Port_PinType            Ch_Num;
    Port_PinInitialValue    Ch_Initial_value;
    Port_PinDirectionType   Ch_Direction;
    Port_PinModeType        Ch_Mode;
    Port_InternalResistor   Resistor;
    boolean                 Ch_Direction_Changeable;
    boolean                 Ch_Mode_Changeable;


}Port_ConfigChannel;

typedef struct Port_ConfigChannel
{
    Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNELS] ;
} Port_ConfigType;


/*******************************************************************************
 *           Defines for PORT NAME , PORT PIN NAME , PORT PIN MODE             *
 *******************************************************************************/

/**************** 1. PORT NAME ***************************/
#define PORT_A          (uint8)0
#define PORT_B          (uint8)1
#define PORT_C          (uint8)2
#define PORT_D          (uint8)3
#define PORT_E          (uint8)4
#define PORT_F          (uint8)5

/**************** 2. PORT PIN NAME ***************************/
#define PORT_PIN_0      (uint8)0
#define PORT_PIN_1      (uint8)1
#define PORT_PIN_2      (uint8)2
#define PORT_PIN_3      (uint8)3
#define PORT_PIN_4      (uint8)4
#define PORT_PIN_5      (uint8)5
#define PORT_PIN_6      (uint8)6
#define PORT_PIN_7      (uint8)7

/**************** 3. PORT PIN MODE ***************************/

#define PORT_PIN_MODE_ADC       (uint8)0
#define PORT_PIN_MODE_ALT_1     (uint8)1
#define PORT_PIN_MODE_ALT_2     (uint8)2
#define PORT_PIN_MODE_ALT_3     (uint8)3
#define PORT_PIN_MODE_ALT_4     (uint8)4
#define PORT_PIN_MODE_ALT_5     (uint8)5
#define PORT_PIN_MODE_ALT_6     (uint8)6
#define PORT_PIN_MODE_ALT_7     (uint8)7
#define PORT_PIN_MODE_ALT_8     (uint8)8
#define PORT_PIN_MODE_ALT_9     (uint8)9
#define PORT_PIN_MODE_ALT_14    (uint8)14
#define PORT_PIN_MODE_ALT_15    (uint8)15
#define PORT_PIN_MODE_DIO       (uint8)16


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
* Service ID: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr (Pointer to configuration set.)
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
void Port_Init(Port_ConfigType* ConfigPtr);




/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in):
*               -Pin parameter , Holds the Port Pin ID number.
*               -Direction parameter , Holds the Port Pin ID Direction.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: : Sets the port pin direction.
************************************************************************************/
#if ( PORT_SET_PIN_DIRECTION_API == STD_ON )

void Port_SetPinDirection(Port_PinType Pin , Port_PinDirectionType Direction);

#endif



/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection(void);


/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo , Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
#if ( PORT_VERSION_INFO_API == STD_ON )

void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);

#endif


/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in):
*               -Pin , Port Pin ID number.
*               -Mode , New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
************************************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);


/* Configuration Structure  */
extern const Port_ConfigType Port_Configuration ;

#endif /* PORT_H */
