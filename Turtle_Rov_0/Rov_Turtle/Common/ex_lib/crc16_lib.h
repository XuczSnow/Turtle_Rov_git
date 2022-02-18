/**
  ******************************************************************************
  * @file           : CRC16_Lib.h
  * @brief          : Header for CRC16_Lib.c file.
  *                   This file contains the common defines of the application.
	* @auther					: Xucz
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	
#ifndef CRC16_LIB_H
#define CRC16_LIB_H

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_XMODEM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);
unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen);
#endif