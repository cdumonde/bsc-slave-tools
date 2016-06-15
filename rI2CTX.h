#ifndef rI2CTX_H
#define rI2CTX_H

#include <stddef.h>
#include <stdint.h>

#define RPOD_I2C_BUFFER_SIZE 2000 
#define RPOD_I2C_CONTROL_CHAR 0xD5
#define RPOD_I2C_FRAME_START 0xD0
#define RPOD_I2C_PARAMETER_START 0xD3
#define RPOD_I2C_FRAME_END 0xD8

extern uint8_t rI2CTX_buffer[RPOD_I2C_BUFFER_SIZE];
extern uint16_t rI2CTX_bufferPos;
extern uint16_t rI2CTX_frameLength;

void rI2CTX_beginFrame();
void rI2CTX_CalculateChecksum(uint16_t lastByte);
uint16_t rI2CTX_endFrame();

enum rI2C_paramTypes {
	rI2C_INT8 = 0x11,
	rI2C_UINT8 = 0x12,
	rI2C_INT16 = 0x21,
	rI2C_UINT16 = 0x22,
	rI2C_INT32 = 0x41,
	rI2C_UINT32 = 0x42,
	rI2C_INT64 = 0x81,
	rI2C_UINT64 = 0x82,
	rI2C_FLOAT = 0x43,
	rI2C_DOUBLE = 0x83
};

void rI2CTX_addParameter(enum rI2C_paramTypes prefix, uint8_t index, size_t data_len, uint64_t data);

#define rI2CTX_addParameter_i8(_index, _data) \
	rI2CTX_addParameter(rI2C_INT8, _index, sizeof(int8_t), (uint64_t) _data)
#define rI2CTX_addParameter_u8(_index, _data) \
	rI2CTX_addParameter(rI2C_UINT8, _index, sizeof(uint8_t), (uint64_t) _data)
#define rI2CTX_addParameter_i16(_index, _data) \
	rI2CTX_addParameter(rI2C_INT16, _index, sizeof(int16_t), (uint64_t) _data)
#define rI2CTX_addParameter_u16(_index, _data) \
	rI2CTX_addParameter(rI2C_UINT16, _index, sizeof(uint16_t), (uint64_t) _data)
#define rI2CTX_addParameter_i32(_index, _data) \
	rI2CTX_addParameter(rI2C_INT32, _index, sizeof(int32_t), (uint64_t) _data)
#define rI2CTX_addParameter_u32(_index, _data) \
	rI2CTX_addParameter(rI2C_UINT32, _index, sizeof(uint32_t), (uint64_t) _data)
#define rI2CTX_addParameter_i64(_index, _data) \
	rI2CTX_addParameter(rI2C_INT64, _index, sizeof(int64_t), (uint64_t) _data)
#define rI2CTX_addParameter_u64(_index, _data) \
	rI2CTX_addParameter(rI2C_UINT64, _index, sizeof(uint64_t), (uint64_t) _data)
#define rI2CTX_addParameter_f(_index, _data) \
	rI2CTX_addParameter(rI2C_FLOAT, _index, sizeof(float), (uint64_t) _data)
#define rI2CTX_addParameter_d(_index, _data) \
	rI2CTX_addParameter(rI2C_DOUBLE, _index, sizeof(double), (uint64_t) _data)

#endif
