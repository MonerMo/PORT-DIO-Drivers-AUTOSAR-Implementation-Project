 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Moner Mohammed AKA Orabi :)
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"

/* Check first if the DET module is enabled */

#if (PORT_DEV_ERROR_DETECT == STD_ON)

/* Include the DET module and check for AUTOSAR version */
#include "Det.h"

#if ((PORT_AR_RELEASE_MAJOR_VERSION != DET_AR_MAJOR_VERSION)\
  || (PORT_AR_RELEASE_MINOR_VERSION != DET_AR_MINOR_VERSION)\
  || (PORT_AR_RELEASE_PATCH_VERSION != DET_AR_PATCH_VERSION))
    #error "The AR version of Det.h does not match the expected version."
#endif

#endif

/********************************************************************/

/* Steps Description:-
 * 1.Declare STATIC variable holding the module status if initialized or not.
 * 2.Declare CONST pointer that holds the structure of array of structure (Port_ConfigType).
 * 3.Check for NULL pointer passed as arguments , If null report to DET module.
 * 4.If the pointer is not NULL then assign it to Port_Channels pointer.
 *
 *
 *
 *
 */

/*(Step 1)*/STATIC uint8 Port_Status = PORT_NOT_INITIALIZED ;
/*(Step 2)*/const  Port_ConfigType* Channels_Ptr = NULL_PTR ;

void Port_Init(Port_ConfigType* ConfigPtr){

    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        if(NULL_PTR == ConfigPtr){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
        }else{
            /*nothing to do here :p */
        }

    #endif

    /*The ConfigPtr is not null then assign ConfigPtr pointer to Port_Channels. */
    Channels_Ptr = ConfigPtr ;
    /* Set the Port_Status to PORT_INITIALIZED */
    Port_Status = PORT_INITIALIZED ;

    /* Now go through each pin in the Port_ConfigType Array in the Port_PBcfg.c */
    Port_PinType ArrIndex ;
    for(ArrIndex = 0 ; ArrIndex < PORT_CONFIGURED_CHANNELS ; ArrIndex++){

        /* We need to declare a pointer that holds the base address of the configured port. */
        volatile uint32* PortBaseAddress_Ptr = NULL_PTR ;

        /* Also we need Error variable to check for example : pin number not in range , invalid port number and invalid mode. */
        boolean error = FALSE ;

        /* Now check for the port number validity */
        switch(Channels_Ptr->Channels[ArrIndex].Port_Num){
            case PORT_A:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTA_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_B:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTB_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_C:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTC_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_D:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTD_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_E:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTE_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_F:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTF_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            default:
                /* This case means that we have an invalid input for Port */
                //error = TRUE ;
                break;
        }

        /* Now we check if the port_num is correct or not , if yes enable clock if not do nothing. */
        if(TRUE == error){
            /* Do nothing because we have invalid port_num input :P */
        }else{
            /* Here activate the clock for the port using SYSCTL_RCGCGPIO_REG. */
            SYSCTL_RCGCGPIO_REG |= (1<<Channels_Ptr->Channels[ArrIndex].Port_Num) ;

            /* Wait for the flag to be fired in PRGPIO register (should be fired within 3 CLK cycles. */
            while(!(SYSCTL_PRGPIO_REG & (1<<Channels_Ptr->Channels[ArrIndex].Port_Num) ) );
        }


        /* Now we check for the validity of pin number */
        /*Cases :
         * PORT A,B,C,D have 0-7 range of pins
         * PORT E have 0-5 range of pins
         * PORT F have 0-4 range of pins
         */
        if(TRUE == error){
            /* Do nothing because we have invalid port_num input :P */
        }else{
            if((Channels_Ptr->Channels[ArrIndex].Port_Num >= PORT_A) && (Channels_Ptr->Channels[ArrIndex].Port_Num <= PORT_D)){
                /* Here we should check if the pin is in the range of 0-7 */
                if((Channels_Ptr->Channels[ArrIndex].Ch_Num >= PORT_PIN_0)
                 &&(Channels_Ptr->Channels[ArrIndex].Ch_Num <= PORT_PIN_7)){
                    /* nothing to do here pin num is correct :-) */
                }else{
                    /* pin number is incorrect here */
                    //error = TRUE ;
                }
            }

            if(Channels_Ptr->Channels[ArrIndex].Port_Num == PORT_E){
                /* Here we should check if the pin is in the range of 0-7 */
                if((Channels_Ptr->Channels[ArrIndex].Ch_Num >= PORT_PIN_0)
                 &&(Channels_Ptr->Channels[ArrIndex].Ch_Num <= PORT_PIN_5)){
                    /* nothing to do here pin num is correct :-) */
                }else{
                    /* pin number is incorrect here */
                    //error = TRUE ;
                }
            }

            if(Channels_Ptr->Channels[ArrIndex].Port_Num == PORT_F){
                /* Here we should check if the pin is in the range of 0-7 */
                if((Channels_Ptr->Channels[ArrIndex].Ch_Num >= PORT_PIN_0)
                 &&(Channels_Ptr->Channels[ArrIndex].Ch_Num <= PORT_PIN_4)){
                    /* nothing to do here pin num is correct :-) */
                }else{
                    /* pin number is incorrect here */
                    //error = TRUE ;
                }
            }

        }




        /* Now we need to unlock the port and enable the commit to the corresponding pin
         * but only special pins that needs to be unlocked and we are going to mention them below,
         * Special Cases :
         * PD7 & PF0 for NMI pins .
         * PC[0:3] for JTAG/SWD pins.
         */
        if(TRUE == error){

        }else if( ( (Channels_Ptr->Channels[ArrIndex].Port_Num == 3 ) && (Channels_Ptr->Channels[ArrIndex].Ch_Num == 7 ) )
              || ( (Channels_Ptr->Channels[ArrIndex].Port_Num == 5) && (Channels_Ptr->Channels[ArrIndex].Ch_Num  == 0) ) ){

            /* Here we are checking if the pin is either PD7 or PF0 in order to unlock the
             * corresponding LOCK register GPIOLOCK with value 0x4C4F434B and Set the corresponding bit
             * in the GPIOCR commit register.
             */

            /* Unlocking */
            *(volatile uint32 *)((volatile uint8*)PortBaseAddress_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B ;

            /* Setting Corresponding bit to enable commit */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_COMMIT_REG_OFFSET) ,
                    Channels_Ptr->Channels[ArrIndex].Ch_Num) ;

        }else if( (Channels_Ptr->Channels[ArrIndex].Port_Num == PORT_C) && (Channels_Ptr->Channels[ArrIndex].Ch_Num <= 3) ){

            /* Here we checked if the passed configuration JTAG pins PC[0:3] */
            /* We can just leave and don't modify it so I'm going to use continue keyword */
            continue ;

        }else{
            /* No need to do anything here all other pins are hard wired to 0x01
             * ensuring that it is always possible to commit new values
             * to the AFSEL , PUD , PDR , DEN registers.
             */
        }


        /* Now hold mode configuration */

        /* First we will hold the DIO pin mode
         * 1.Disable Analog function for the specified pin. GPIOAMSEL
         * 2.Clear Alternative functionality for the specified pin. GPIOAFSEL
         * 3.Clear the pin corresponding 4 PMCx bits in the GPIOPCTL
         * 4.Enable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
         */
        if(TRUE == error){

            /* you have an error from the upper procedures */

        }else if(Channels_Ptr->Channels[ArrIndex].Ch_Mode == PORT_PIN_MODE_DIO){

            /* Step 1 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* Step 2 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* the register value anded with zeros at 4 * Ch_Num  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) &=
                    ~(0x0000000F << (Channels_Ptr->Channels[ArrIndex].Ch_Num * 4)) ;

            /* Step 4 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[ArrIndex].Ch_Num);
        }else if(Channels_Ptr->Channels[ArrIndex].Ch_Mode == PORT_PIN_MODE_ADC){

            /* ADC pin mode
             * 1.Enable Analog function for the specified pin. GPIOAMSEL
             * 2.Clear Alternative functionality for the specified pin. GPIOAFSEL
             * 3.Clear the pin corresponding 4 PMCx bits in the GPIOPCTL
             * 4.Disable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
             */

            /* Step 1 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* Step 2 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* the register value anded with zeros at 4 * Ch_Num  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) &=
                    ~(0x0000000F << (Channels_Ptr->Channels[ArrIndex].Ch_Num * 4)) ;

            /* Step 4 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[ArrIndex].Ch_Num);
        }else if(((Channels_Ptr->Channels[ArrIndex].Ch_Mode >= PORT_PIN_MODE_ALT_1) &&
                (Channels_Ptr->Channels[ArrIndex].Ch_Mode <= PORT_PIN_MODE_ALT_9)) ||
                ((Channels_Ptr->Channels[ArrIndex].Ch_Mode >= PORT_PIN_MODE_ALT_14) &&
                (Channels_Ptr->Channels[ArrIndex].Ch_Mode <= PORT_PIN_MODE_ALT_15))){

            /* Here we are dealing with alternative functions for pins */
            /* Alternative pin mode
             * 1.Disable Analog function for the specified pin. GPIOAMSEL
             * 2.Set Alternative functionality for the specified pin. GPIOAFSEL
             * 3.Set the pin corresponding 4 PMCx bits in the GPIOPCTL with the alternative functionality value.
             * 4.Enable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
             */
            /* Step 1 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* Step 2 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[ArrIndex].Ch_Num);

            /* the register value OR'ed with alternative functionality value at shifted position
             * to the left with value equal to (4 * Ch_Num)  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) |=
                    (Channels_Ptr->Channels[ArrIndex].Ch_Mode & 0x0000000F <<
                    (Channels_Ptr->Channels[ArrIndex].Ch_Num * 4)) ;

            /* Step 4 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[ArrIndex].Ch_Num);

        }else{
            //error = TRUE ;
        }


        /* Now we want to handle the Direction
         *
         * OUTPUT Cases :
         * Check for Initial value and set the initial value.
         *
         *
         * INPUT Cases :
         * Pull Up Resistor or Pull Down Resistor or neither.
         *
         *
         */
         if(TRUE == error){

         }else if(Channels_Ptr->Channels[ArrIndex].Ch_Direction == PORT_PIN_OUT){

             /* Set the corresponding bit in GPIODIR register in order to make the direction output */
             SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                     Channels_Ptr->Channels[ArrIndex].Ch_Num) ;

             /* Now check for the initial value */
             if(Channels_Ptr->Channels[ArrIndex].Ch_Initial_value == PORT_PIN_LEVEL_LOW){
                 /* CLEAR the corresponding bit in the data register GPIODATA */

                 CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DATA_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num) ;

             }else{
                 /* else case is of course PORT_PIN_LEVEL_HIGH */

                 SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DATA_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num) ;
             }
         }else if(Channels_Ptr->Channels[ArrIndex].Ch_Direction == PORT_PIN_IN){
             /* CLEAR the corresponding bit in GPIODIR register in order to make the direction INPUT */
             CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                     Channels_Ptr->Channels[ArrIndex].Ch_Num) ;

             /* Check the Internal Resistor cases OFF - Pull Up - Pull Down*/
             if(Channels_Ptr->Channels[ArrIndex].Resistor == PULL_UP){
                 /* Pull Up Resistor Enable */
                 SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_PULL_UP_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num);

             }else if(Channels_Ptr->Channels[ArrIndex].Resistor == PULL_DOWN){
                 /* Pull Down Resistor Enable */
                 SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_PULL_DOWN_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num);

             }else if(Channels_Ptr->Channels[ArrIndex].Resistor == OFF){
                 /* Disable Pulled Resistors */
                 CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_PULL_DOWN_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num);

                 CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_PULL_UP_REG_OFFSET) ,
                         Channels_Ptr->Channels[ArrIndex].Ch_Num);

             }else{
                 //error = TRUE ;
             }

         }else{
             //error = TRUE ;
         }

    }
}
/********************************************************************/
#if ( PORT_SET_PIN_DIRECTION_API == STD_ON )

void Port_SetPinDirection(Port_PinType Pin , Port_PinDirectionType Direction){

    /* Expected DET errors for this function.
     * PORT_E_UNINIT - Port module not initialized.
     * PORT_E_PARAM_PIN - Incorrect Pin id passed.
     * PORT_E_DIRECTION_UNCHANGEABLE - Pin Not configured as direction changeable.
     */

    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        /* PORT_E_UNINIT - Port module not initialized.  */
        if(Port_Status == PORT_NOT_INITIALIZED) {
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
        }else{
            /* nothing to do */
        }

        /* PORT_E_PARAM_PIN - Incorrect Pin id passed. */
        if(Pin >= PORT_CONFIGURED_CHANNELS){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
        }else{
            /* nothing to do */
        }

        /* PORT_E_DIRECTION_UNCHANGEABLE - Pin Not configured as direction changeable. */
        if(Channels_Ptr->Channels[Pin].Ch_Direction_Changeable == STD_OFF){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
        }else{
            /* nothing to do */
        }

    #endif

        /* We need to declare a pointer that holds the base address of the configured port. */
        volatile uint32* PortBaseAddress_Ptr = NULL_PTR ;

        /* Now check for the port base address */
        switch(Channels_Ptr->Channels[Pin].Port_Num){
            case PORT_A:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTA_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_B:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTB_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_C:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTC_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_D:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTD_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_E:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTE_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_F:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTF_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            default:
                break;
        }

        /* Now for direction adjustments
         * 1.Check if the pins are for JTAG do nothing
         * 2.If not change the direction
         */

        /* Checking for JTAG pins */
        if( (Channels_Ptr->Channels[Pin].Port_Num == PORT_C) && (Channels_Ptr->Channels[Pin].Ch_Num <= 3) )
        {
            /* the pins are for JTAG do nothing */

        }else if(Direction == PORT_PIN_OUT){

            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                    Channels_Ptr->Channels[Pin].Ch_Num);
        }else if(Direction == PORT_PIN_IN){
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                    Channels_Ptr->Channels[Pin].Ch_Num);
        }else{
            /* Do Nothing */
        }

}
#endif

/********************************************************************/
void Port_RefreshPortDirection(void){

    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        /* PORT_E_UNINIT - Port module not initialized.  */
        if(Port_Status == PORT_NOT_INITIALIZED){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
        }else{
            /* nothing to do */
        }

    #endif

    /* Now we are going to loop through the configured pins
     * 1. Check if the pin is JTAG pin then continue and skip it.
     * 2. If it is not JTAG pin check if it is Direction Changeable if no refresh it
     * if yes just don't do anything.
     */




        Port_PinType ArrIndex ;
        for(ArrIndex = 0 ; ArrIndex < PORT_CONFIGURED_CHANNELS ; ArrIndex++){

            /* We need to declare a pointer that holds the base address of the configured port. */
            volatile uint32* PortBaseAddress_Ptr = NULL_PTR ;

            /* Now check for the port base address */
            switch(Channels_Ptr->Channels[ArrIndex].Port_Num){
                case PORT_A:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTA_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                case PORT_B:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTB_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                case PORT_C:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTC_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                case PORT_D:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTD_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                case PORT_E:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTE_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                case PORT_F:
                    PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTF_BASE_ADDRESS; /* Casting to volatile uint32* */
                    break;
                default:
                    break;
            }

            /* Now check for JTAG pins */
            /* Checking for JTAG pins */
            if( (Channels_Ptr->Channels[ArrIndex].Port_Num == PORT_C) && (Channels_Ptr->Channels[ArrIndex].Ch_Num <= 3) )
            {
                /* the pins are for JTAG just skip it */
                continue ;
            }else if(Channels_Ptr->Channels[ArrIndex].Ch_Direction_Changeable == STD_OFF){
                /* Now you know that the current pin is not direction changeable
                 * So , just check if it is output or input and refresh the direction.
                 */

                if(Channels_Ptr->Channels[ArrIndex].Ch_Direction == PORT_PIN_IN){

                    /* Direction Input */
                    /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                            Channels_Ptr->Channels[ArrIndex].Ch_Num);

                }else if(Channels_Ptr->Channels[ArrIndex].Ch_Direction == PORT_PIN_OUT){

                    /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortBaseAddress_Ptr + PORT_DIR_REG_OFFSET) ,
                            Channels_Ptr->Channels[ArrIndex].Ch_Num);
                }else{
                    /* the else case just for complete syntax :p */
                }
            }else{
                /* Pin direction changeable */
                /* nothing to do here */
            }
        }

}
/********************************************************************/
#if ( PORT_VERSION_INFO_API == STD_ON )

void Port_GetVersionInfo(Std_VersionInfoType* versioninfo){

    /* Expected Errors :
     * 1. PORT_E_PARAM_POINTER , API Called with a NULL Pointer Parameter
     * 2. PORT_E_UNINIT , API Service called prior to module initialization.
     */

    #if (PORT_DEV_ERROR_DETECT == STD_ON)

        /* API called with NULL pointer parameter */
        if(versioninfo == NULL_PTR){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        }else{
            /* nothing to do */
        }

        /* PORT_E_UNINIT - Port module not initialized.  */
        if(Port_Status == PORT_NOT_INITIALIZED){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
        }else{
            /* nothing to do */
        }

    #endif

        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
}

#endif
/********************************************************************/
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode){

    /* Expected Errors
     * 1.PORT_E_UNINIT , API service called prior to module initialization.
     * 2.PORT_E_PARAM_PIN , Incorrect Port Pin ID Passed.
     * 3.PORT_E_PARAM_INVALID_MODE , Port Pin Mode passed not valid
     * 4.PORT_E_MODE_UNCHANGEABLE , API called when the mode is unchangeable
     */

    #if (PORT_DEV_ERROR_DETECT == STD_ON)
        /* PORT_E_UNINIT - Port module not initialized.  */
        if(Port_Status == PORT_NOT_INITIALIZED)
        {
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
        }else{
            /* nothing to do */
        }

        /* PORT_E_PARAM_PIN , Incorrect Port Pin ID Passed. */
        if(Pin >= PORT_CONFIGURED_CHANNELS){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_PIN);

        }else{
            /* nothing to do */
        }

        /* PORT_E_PARAM_INVALID_MODE , Port Pin Mode passed not valid */
        if(Mode > PORT_PIN_MODE_DIO){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_INVALID_MODE);
        }else{
            /* nothing to do */
        }

        /* PORT_E_MODE_UNCHANGEABLE , API called when the mode is unchangeable */
        if(Channels_Ptr->Channels[Pin].Ch_Mode_Changeable == STD_OFF){
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_MODE_UNCHANGEABLE);
        }else{
            /* nothing to do */
        }
    #endif

        /* We need to declare a pointer that holds the base address of the configured port. */
        volatile uint32* PortBaseAddress_Ptr = NULL_PTR ;

        /* Now check for the port base address */
        switch(Channels_Ptr->Channels[Pin].Port_Num){
            case PORT_A:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTA_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_B:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTB_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_C:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTC_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_D:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTD_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_E:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTE_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            case PORT_F:
                PortBaseAddress_Ptr = (volatile uint32* )GPIO_PORTF_BASE_ADDRESS; /* Casting to volatile uint32* */
                break;
            default:
                break;
        }

        /* Now hold mode configuration */

        /* First we will hold the DIO pin mode
         * 1.Disable Analog function for the specified pin. GPIOAMSEL
         * 2.Clear Alternative functionality for the specified pin. GPIOAFSEL
         * 3.Clear the pin corresponding 4 PMCx bits in the GPIOPCTL
         * 4.Enable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
         */
        if(Mode == PORT_PIN_MODE_DIO){

            /* Step 1 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[Pin].Ch_Num);

            /* Step 2 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[Pin].Ch_Num);

            /* the register value anded with zeros at 4 * Ch_Num  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) &=
                    ~(0x0000000F << (Channels_Ptr->Channels[Pin].Ch_Num * 4)) ;

            /* Step 4 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[Pin].Ch_Num);
        }else if(Mode == PORT_PIN_MODE_ADC){

            /* ADC pin mode
             * 1.Enable Analog function for the specified pin. GPIOAMSEL
             * 2.Clear Alternative functionality for the specified pin. GPIOAFSEL
             * 3.Clear the pin corresponding 4 PMCx bits in the GPIOPCTL
             * 4.Disable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
             */

            /* Step 1 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[Pin].Ch_Num);

            /* Step 2 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[Pin].Ch_Num);

            /* the register value anded with zeros at 4 * Ch_Num  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) &=
                    ~(0x0000000F << (Channels_Ptr->Channels[Pin].Ch_Num * 4)) ;

            /* Step 4 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[Pin].Ch_Num);
        }else if(((Mode >= PORT_PIN_MODE_ALT_1) && (Mode <= PORT_PIN_MODE_ALT_9)) ||
                ((Mode >= PORT_PIN_MODE_ALT_14) && (Mode <= PORT_PIN_MODE_ALT_15))){

            /* Here we are dealing with alternative functions for pins */
            /* Alternative pin mode
             * 1.Disable Analog function for the specified pin. GPIOAMSEL
             * 2.Set Alternative functionality for the specified pin. GPIOAFSEL
             * 3.Set the pin corresponding 4 PMCx bits in the GPIOPCTL with the alternative functionality value.
             * 4.Enable the corresponding I/O pins by writing one to the corresponding position in GPIODEN.
             */
            /* Step 1 */
            CLEAR_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) ,
                     Channels_Ptr->Channels[Pin].Ch_Num);

            /* Step 2 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_ALT_FUNC_REG_OFFSET) ,
                      Channels_Ptr->Channels[Pin].Ch_Num);

            /* the register value OR'ed with alternative functionality value at shifted position
             * to the left with value equal to (4 * Ch_Num)  (Step 3) */
            *(volatile uint32*)((volatile uint8 *)PortBaseAddress_Ptr + PORT_CTL_REG_OFFSET) |=
                    (Mode & 0x0000000F <<
                    (Channels_Ptr->Channels[Pin].Ch_Num * 4)) ;

            /* Step 4 */
            SET_BIT(*(volatile uint32*)((volatile uint8*)PortBaseAddress_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) ,
                    Channels_Ptr->Channels[Pin].Ch_Num);

        }else{
            /* do nothing */
        }

}


