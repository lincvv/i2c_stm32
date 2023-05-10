#include "i2c_cmsis.h"


void I2C2_Start() {

	//while ((I2C1->SR2 & I2C_SR2_BUSY));			// 1: Communication ongoing on the bus

    I2C2->CR1 |= I2C_CR1_START;						// 1: Repeated start generation

	while ( !(I2C2->SR1 & I2C_SR1_SB)  ||			// 0: No Start condition
			!(I2C2->SR2 & I2C_SR2_MSL) ||			// 0: Slave Mode
			!(I2C2->SR2 & I2C_SR2_BUSY)				// 0: No communication on the bus
	);
}

void I2C2_Stop() {

	I2C2->CR1 |= I2C_CR1_STOP;						// 1: Stop generation after the current byte transfer or after the current Start condition is sent.

	while((I2C2->SR1 & I2C_SR1_STOPF));				// 1: Stop condition detected
}

I2C_Status_Type I2C2_SendAddress(uint8_t address, I2C_Direction_Type direction) {

	uint32_t timeout = 1000000;

	address <<= 1;

	if (direction == I2C_TRANSMITTER) {
		address &= ~(1 << 0);						// Reset the address bit0 for write
        I2C2->DR = address;
		while ((!(I2C2->SR1 & I2C_SR1_ADDR) ||		// 0: No end of address transmission
				!(I2C2->SR2 & I2C_SR2_MSL)  ||		// 0: Slave Mode
				!(I2C2->SR2 & I2C_SR2_BUSY) ||		// 0: No communication on the bus
				!(I2C2->SR2 & I2C_SR2_TRA)) &&		// 0: Data bytes received
				--timeout
		);
	} else if (direction == I2C_RECEIVER) {
		address |= (1 << 0);						// Set the address bit0 for read
        I2C2->DR = address;
		while ((!(I2C2->SR1 & I2C_SR1_ADDR) ||		// 0: No end of address transmission
				!(I2C2->SR2 & I2C_SR2_MSL)  ||		// 0: Slave Mode
				!(I2C2->SR2 & I2C_SR2_BUSY))&&		// 0: No communication on the bus
				--timeout
		);
	}

	if (timeout <= 0) {
		return I2C_ERROR;
	}

	return I2C_OK;
}

void I2C2_SendData(uint8_t data) {

    I2C2->DR = data;

	while ( !(I2C2->SR1 & I2C_SR1_BTF)  ||			// 0: Data byte transfer not done
			!(I2C2->SR1 & I2C_SR1_TXE)  ||			// 0: Data register not empty
			!(I2C2->SR2 & I2C_SR2_MSL)  ||			// 0: Slave Mode
			!(I2C2->SR2 & I2C_SR2_BUSY) ||			// 0: No communication on the bus
			!(I2C2->SR2 & I2C_SR2_TRA)				// 0: Data bytes received
	);
}

uint8_t I2C2_ReceiveData(I2C_Acknowledge_Type acknowledge) {

	if (acknowledge == I2C_ACK) {
        I2C2->CR1 |= I2C_CR1_ACK;					// 1: Acknowledge returned after a byte is received (matched address or data)
	} else if (acknowledge == I2C_NACK) {
        I2C2->CR1 &= ~I2C_CR1_ACK;					// 0: No acknowledge returned
	}

	while ( !(I2C2->SR1 & I2C_SR1_RXNE) ||			// 0: Data register empty
			!(I2C2->SR2 & I2C_SR2_MSL)  ||			// 0: Slave Mode
			!(I2C2->SR2 & I2C_SR2_BUSY)				// 0: No communication on the bus
	);

	return (uint8_t)I2C2->DR;
}
