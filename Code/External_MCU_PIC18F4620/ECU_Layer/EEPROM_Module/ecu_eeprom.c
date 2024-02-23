/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  ecu_eeprom.c
 *       Module:  -
 *       Author: Mohamed Osama
 *
 *       Created on February 19, 2024, 9:18 PM  
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include"ecu_eeprom.h"

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static mssp_i2c_t i2c_obj = {
    .i2c_clock = 100000,
    .i2c_cfg.i2c_SMBus_control = I2C_SMBUS_DISABLE,
    .i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE,
    .i2c_cfg.i2c_mode = MSSP_I2C_MASTER_MODE,
    .i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK
};
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
void EEPROM_INIT(void){
	MSSP_I2C_Init(&i2c_obj);
}

void EEPROM_WriteByte(uint8 addr , uint8 loc ,uint8 data){
    uint8 ack=0;
	MSSP_I2C_Master_Send_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,addr,&ack);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,loc,&ack);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,data,&ack);
	MSSP_I2C_Master_Send_Stop(&i2c_obj);
}

uint8 EEPROM_ReadByte(uint8 addr , uint8 loc){
    uint8 ack=0;
	uint8 data=0;
	MSSP_I2C_Master_Send_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,addr,&ack);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,loc,&ack);
	MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,(addr|0x01),&ack);
	MSSP_I2C_Master_Read_Blocking(&i2c_obj,I2C_MASTER_SEND_NACK,&data);
	MSSP_I2C_Master_Send_Stop(&i2c_obj);
	return data;
}
void EEPROM_WriteMultipleByte(uint8 addr , uint8 loc ,uint8 *data, uint8 data_size){
    uint8 ack=0;
	uint8 cnt=0;
	MSSP_I2C_Master_Send_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,addr,&ack);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,loc,&ack);
	while (cnt < data_size){
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,data[cnt],&ack);
	cnt++;	
	}
	MSSP_I2C_Master_Send_Stop(&i2c_obj);
}
void EEPROM_ReadMultipleByte(uint8 addr , uint8 loc ,uint8 *data, uint8 data_size){
    uint8 ack=0;
	uint8 cnt=0;
	MSSP_I2C_Master_Send_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,addr,&ack);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,loc,&ack);
	MSSP_I2C_Master_Send_Repeated_Start(&i2c_obj);
	MSSP_I2C_Master_Write_Blocking(&i2c_obj,(addr|0x01),&ack);
	while (cnt < data_size)
    {
		if (cnt == data_size-1)
		{
            MSSP_I2C_Master_Read_Blocking(&i2c_obj,I2C_MASTER_SEND_NACK,&data[cnt]);
            break;
		}
		else
        {
            MSSP_I2C_Master_Read_Blocking(&i2c_obj,I2C_MASTER_SEND_ACK,&data[cnt]);
            cnt++;
        }
    }
     MSSP_I2C_Master_Send_Stop(&i2c_obj);
}

/**********************************************************************************************************************
 *  END OF FILE: ecu_eeprom.c
 *********************************************************************************************************************/

