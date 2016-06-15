#include "rI2CTX.h"

#include <assert.h>
#include <string.h>

uint8_t rI2CTX_buffer[RPOD_I2C_BUFFER_SIZE];
uint16_t rI2CTX_bufferPos;
uint8_t checksum;
uint16_t rI2CTX_frameLength;

void rI2CTX_beginFrame()
{
	rI2CTX_buffer[0] = RPOD_I2C_CONTROL_CHAR;
	rI2CTX_buffer[1] = RPOD_I2C_FRAME_START;
	rI2CTX_bufferPos = 4;
	//2,3 will be filled in with the length in endFrame()
}

void rI2CTX_calculateChecksum(uint16_t lastByte)
{
	checksum = 0;
	uint16_t i;
	for (i = 0; i < lastByte;i++)
		checksum ^= rI2CTX_buffer[i];
}

uint16_t rI2CTX_endFrame()
{
	rI2CTX_frameLength = rI2CTX_bufferPos;

	uint8_t length1 = (rI2CTX_frameLength & 0xFF00) >> 8;
	uint8_t length2 = rI2CTX_frameLength & 0xFF;

	if (length1 != RPOD_I2C_CONTROL_CHAR && length2 != RPOD_I2C_CONTROL_CHAR) {
		rI2CTX_buffer[2] = length1;
		rI2CTX_buffer[3] = length2;
	} else if (length1 == RPOD_I2C_CONTROL_CHAR && length2 == RPOD_I2C_CONTROL_CHAR) {
		rI2CTX_buffer[2] = length1;
		rI2CTX_buffer[3] = length1;
		rI2CTX_buffer[4] = length2;
		rI2CTX_buffer[5] = length2;
		memcpy(rI2CTX_buffer + 4, rI2CTX_buffer + 6, rI2CTX_bufferPos - 4); //DOUBLE CHECK THIS
		rI2CTX_bufferPos += 2;
	} else {
		if (length1 == RPOD_I2C_CONTROL_CHAR){
			rI2CTX_buffer[2] = length1;
			rI2CTX_buffer[3] = length1;
			rI2CTX_buffer[4] = length2;
		} else {
			rI2CTX_buffer[2] = length1;
			rI2CTX_buffer[3] = length2;
			rI2CTX_buffer[4] = length2;
		}
		memcpy(rI2CTX_buffer + 4, rI2CTX_buffer + 5, rI2CTX_bufferPos - 4);  //DOUBLE CHECK THIS
		rI2CTX_bufferPos += 1;
	}

	rI2CTX_calculateChecksum(rI2CTX_bufferPos);

	rI2CTX_buffer[rI2CTX_bufferPos] = RPOD_I2C_CONTROL_CHAR;
	rI2CTX_buffer[rI2CTX_bufferPos + 1] = RPOD_I2C_FRAME_END;
	rI2CTX_buffer[rI2CTX_bufferPos + 2] = checksum;
	rI2CTX_buffer[rI2CTX_bufferPos + 3] = 0x00;
	rI2CTX_bufferPos += 4;

	return rI2CTX_bufferPos;
}

void rI2CTX_addParameter(enum rI2C_paramTypes prefix, uint8_t index, size_t data_len, uint64_t data)
{
	uint16_t i;
	uint64_t rawData;

	assert(data_len <= sizeof(uint64_t));

	rI2CTX_buffer[rI2CTX_bufferPos] = RPOD_I2C_CONTROL_CHAR;
	rI2CTX_buffer[rI2CTX_bufferPos + 1] = RPOD_I2C_PARAMETER_START;
	rI2CTX_buffer[rI2CTX_bufferPos + 2] = prefix;
	rI2CTX_buffer[rI2CTX_bufferPos + 3] = index;
	rI2CTX_bufferPos += 4;

	memcpy(&rawData, &data, data_len);

	for (i = 0; i < data_len; i++) {
		uint8_t byte = (rawData >> ((data_len - i - 1) * 8)) & 0xff;
		if (byte == RPOD_I2C_CONTROL_CHAR) {
			rI2CTX_buffer[rI2CTX_bufferPos] = RPOD_I2C_CONTROL_CHAR;
			rI2CTX_buffer[rI2CTX_bufferPos + 1] = RPOD_I2C_CONTROL_CHAR;
			rI2CTX_bufferPos += 2;
		} else {
			rI2CTX_buffer[rI2CTX_bufferPos] = byte;
			rI2CTX_bufferPos += 1;
		}
	}
}
