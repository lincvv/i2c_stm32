/*
 * I2C2.c
 *
 *  Created on: 15 ���. 2018 �.
 *      Author: vlinc
 */
#include "i2c_cmsis.h"
#include "printf.h"
#include "i2c.h"

extern I2C_HandleTypeDef hi2c2;

/**
 *
 * @brief Read data from device i2c.
 * @note the default use is I2C1(CMSIS)

 * @param slave_addr Address of the slave device
 * @param addr Register address
 * @param size The size of the received data
 * @param data Pointer to data buffer
 *
 */
void read_i2c1(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *data)
{
    // I2C Send
    I2C1_Start();
    I2C1_SendAddress(slave_addr, I2C_TRANSMITTER);
    I2C1_SendData(addr);

    // I2C Receive
    uint8_t count = 0;
    I2C1_Start();
    I2C1_SendAddress(slave_addr, I2C_RECEIVER);

    for (count = 0; count < size - 1; ++count ) {
        data[count] = I2C1_ReceiveData(I2C_ACK);
    }

    I2C1_Stop();
    data[count] = I2C1_ReceiveData(I2C_NACK);
}

/**
 *
 * @brief Write data to device i2c.
 * @note the default use is I2C1(CMSIS)

 * @param slave_addr Address of the slave device
 * @param addr Register address
 * @param size The size of the transmitted data
 * @param data Pointer to data buffer
 *
 */

void write_i2c1(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *data)
{
    // I2C Send
    I2C1_Start();
    I2C1_SendAddress(slave_addr, I2C_TRANSMITTER);
    I2C1_SendData(addr);

    for (int8_t i = 0; i < size; ++i) {
        I2C1_SendData(data[i]);
    }

    I2C1_Stop();
}

/**
 *
 * @brief Read data from device i2c.
 * @note used by HAL. The I2C number can be selected using extern I2C_HandleTypeDef

 * @param slave_addr Address of the slave device
 * @param addr Register address
 * @param size The size of the received data
 * @param puch_data Pointer to data buffer
 *
 */

uint8_t hal_read_i2c(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *puch_data)
{
	HAL_StatusTypeDef statusWrite, statusRead = HAL_OK;

	uint8_t read_i2c_data[4];

	read_i2c_data[0]=addr;

	statusWrite = HAL_I2C_Master_Transmit_IT(hi2c, (slave_addr - 1), (uint8_t*)&read_i2c_data, 1);

	if (statusWrite == HAL_ERROR) {
		printf("ERROR_Write"); // Requires printf implementation

	}
	else if(statusWrite == HAL_OK) {
		printf("OK_Write \n"); // Requires printf implementation
	}
    HAL_Delay(10);

	statusRead = HAL_I2C_Master_Receive(hi2c, slave_addr, read_i2c_data, size, 1000);

	if (statusRead != HAL_ERROR) {
		*puch_data = read_i2c_data[0];

	} else {
		printf("ERROR_Read"); // Requires printf implementation
	}

	return statusRead;

}

/**
 *
 * @brief Write data to device i2c.
 * @note used by HAL. The I2C number can be selected using extern I2C_HandleTypeDef

 * @param slave_addr Address of the slave device
 * @param addr Register address
 * @param size The size of the transmitted data
 * @param data data buffer
 *
 */

uint8_t hal_write_i2c(uint8_t slave_addr, uint8_t addr, uint8_t data)
{
	HAL_StatusTypeDef statusError;
	uint8_t write_i2c_data[2];
	write_i2c_data[0]=addr;
	write_i2c_data[1]=data;

	statusError = HAL_I2C_Master_Transmit_IT(hi2c, slave_addr, write_i2c_data, 2);
	if(statusError == HAL_ERROR){
		printf("Error! \n"); // Requires printf implementation
	}
	else if(statusError == HAL_OK){
		printf("OK_Write \n"); // Requires printf implementation
	}

	return statusError;
}
