/*
	liblightmodbus - a lightweight, multiplatform Modbus library
	Copyright (C) 2017 Jacek Wieczorek <mrjjot@gmail.com>

	This file is part of liblightmodbus.

	Liblightmodbus is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Liblightmodbus is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIGHTMODBUS_BASE_H
#define LIGHTMODBUS_BASE_H

/* This is main header file that is ought to be included as library */

//Include proper header files
#include <inttypes.h>
#include "libconf.h"

//Error codes
typedef enum modbusError
{
	MODBUS_ERROR_OK = 0, //Everything went ok
	MODBUS_ERROR_EXCEPTION = 1, //Slave thrown an exception and it's stored in status->exception
	MODBUS_ERROR_PARSE = 2, //Slave did not parse frame (eg. bad function code)
	MODBUS_ERROR_CRC = 4, //Invalid crc error
	MODBUS_ERROR_ALLOC = 8, //Memory allocation problems (eg. system ran out of RAM)
	MODBUS_ERROR_OTHER = 16, //Other reason function was exited (eg. bad function parameter)
	MODBUS_ERROR_FRAME = 32, //Frame contained incorrect data, and exception could not be thrown (eg. bytes count != reg count * 2 in slave's response)
	MODBUS_OK = MODBUS_ERROR_OK
} ModbusError;

//Exception codes
typedef enum modbusExceptionCode
{
	MODBUS_EXCEP_ILLEGAL_FUNC = 1,
	MODBUS_EXCEP_ILLEGAL_ADDR = 2,
	MODBUS_EXCEP_ILLEGAL_VAL = 3,
	MODBUS_EXCEP_SLAVE_FAIL = 4,
	MODBUS_EXCEP_ACK = 5,
	MODBUS_EXCEP_NACK = 7
} ModbusExceptionCode;

//Modbus data types
typedef enum modbusDataType
{
	MODBUS_HOLDING_REGISTER = 1,
	MODBUS_INPUT_REGISTER = 2,
	MODBUS_COIL = 4,
	MODBUS_DISCRETE_INPUT = 8
} ModbusDataType;

#define BITSTOBYTES( n ) ( n != 0 ? ( 1 + ( ( n - 1 ) >> 3 ) ) : 0 )

//Function prototypes
#ifdef LIGHTMODBUS_BIG_ENDIAN
#define modbusSwapEndian( data ) ( data )
#else
#define modbusSwapEndian( data ) ( ( data << 8 ) | ( data >> 8 ) )
#endif

extern uint8_t modbusMaskRead( const uint8_t *mask, uint16_t maskLength, uint16_t bit );
extern uint8_t modbusMaskWrite( uint8_t *mask, uint16_t maskLength, uint16_t bit, uint8_t value );
extern uint16_t modbusCRC( const uint8_t *data, uint16_t length );

#endif
