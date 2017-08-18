/***
*
* FILENAME :
*
*        TA.h
*
* DESCRIPTION :
*
*        TA to test en/decrypting
*
* NOTES :
*
*        Copyright Ursus Schneider 2017.  All rights reserved.
*
* AUTHOR :
*
*        Ursus Schneider        START DATE :    15 April 2017
*
* CHANGES :
*
* DATE         WHO               DETAIL
* 26.06.2017    Ursus Schneider  Initial release
*
***/
#ifndef TA_H
#define TA_H

/* This UUID is generated with uuidgen
the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html */
#define TEST_TA_UUID { 0x8aaaf200, 0x2450, 0x11e4, \
		{ 0xab, 0xe2, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

/* The Trusted Application Function ID(s) implemented in this TA */
#define TEST_EN_DE_CRYPT_COMMAND 0
#define TEST_ENCRYPT_IN_TA_COMMAND 1
#define TEST_DECRYPT_IN_TA_COMMAND 2

#endif /* TA_H */
