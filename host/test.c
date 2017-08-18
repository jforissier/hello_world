/***
*
* FILENAME :
*
*        TA_test.c
*
* DESCRIPTION :
*
*        Sundry test functions
*
* NOTES :
*
*        Copyright Ursus Schneider 2017.  All rights reserved.
*
* AUTHOR :
*
*        Ursus Schneider        START DATE :   15 August 2017
*
* CHANGES :
*
* DATE    		 WHO     				DETAIL
* 15.08.2017    Ursus Schneider 	   Initial release
*
***/

#include <tee_client_api.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "test.h"
#include "crypto.h"
#include "../ta/TA.h"

// check your return code
void check_rc (TEEC_Result rc, const char *errmsg, uint32_t *orig) {

   if (rc != TEEC_SUCCESS) {
      fprintf(stderr, "%s: 0x%08x", errmsg, rc);
      if (orig)
      fprintf(stderr, " (orig=%d)", (int)*orig);
      fprintf(stderr, "\n");

      exit(1);
   }
}

// check if you can encode and then decode something in the CA
void local_encrypt_and_decrypt (void) {

   // just some testing
   char test_in [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   int test_in_len = strlen (test_in);
   char test_encrypted [512] = "";
   int test_encrypted_len;
   char test_decrypted [512] = "";
   int test_decrypted_len;

   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< local_encrypt_and_decrypt >>>>>>>>>>>>>>>>>>>>>>>>>\n");
   printf ("\n");
   printf ("First encrypt using the public key, then decrypt the using the private key\n");
   printf ("\n");
   encrypt_using_public_key (test_in,        test_in_len,        test_encrypted, &test_encrypted_len);
   decrypt_using_private_key  (test_encrypted, test_encrypted_len, test_decrypted, &test_decrypted_len);
   printf ("\n");
   printf ("Origional string (26 chars): %s\n", test_in);
   printf ("Origional string len:        %i\n", test_in_len);
   printf ("Encryted value:              %s\n", test_encrypted);
   printf ("Encryted len:                %i\n", test_encrypted_len);
   printf ("Decrypted value:             %s\n", test_decrypted);
   printf ("Decrypted len:               %i\n", test_decrypted_len);
   printf ("\n");
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< end of test >>>>>>>>>>>>>>>>>>>>>>>>\n");

}
void local_encrypt_and_decrypt_in_secure_world (void){

   TEEC_Result rc;
   TEEC_Context ctx;
   TEEC_Session sess;
   TEEC_Operation op;
   TEEC_UUID uuid = TEST_TA_UUID;
   uint32_t err_origin;

   // header
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< local_encrypt_and_decrypt_in_secure_world >>>>>>>>>>>>>>>>>>>>>>>>>\n");

   /* Initialize a context connecting us to the TEE */
   rc = TEEC_InitializeContext(NULL, &ctx);
   check_rc(rc, "TEEC_InitializeContext", NULL);

   // open a session to the TA
   rc = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
   check_rc(rc, "TEEC_OpenSession", &err_origin);

   // assign param
   op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);

   // all messages will display in the SW
   rc = TEEC_InvokeCommand(&sess, TEST_EN_DE_CRYPT_COMMAND, &op, &err_origin);

   // clean up once you have finished
   printf("Cleaning up after yourself\n");
   TEEC_CloseSession(&sess);
   TEEC_FinalizeContext(&ctx);
}

// test encrypting in the secure world
void encrypt_in_secure_world (void) {

   char test_in [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   int test_in_len = strlen (test_in);

   char test_decrypted [256] = "";
   int test_decrypted_len;

   TEEC_Result rc;
   TEEC_Context ctx;
   TEEC_Session sess;
   TEEC_Operation op;
   TEEC_SharedMemory field_in;
   TEEC_SharedMemory field_back;
   TEEC_UUID uuid = TEST_TA_UUID;
   uint32_t err_origin;

   // header
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< test_encrypt_secure_world >>>>>>>>>>>>>>>>>>>>>>>>>\n");

   /* Initialize a context connecting us to the TEE */
   rc = TEEC_InitializeContext(NULL, &ctx);
   check_rc(rc, "TEEC_InitializeContext", NULL);

   // open a session to the TA
   rc = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
   check_rc(rc, "TEEC_OpenSession", &err_origin);

   // ok, create the needed shared memory blocks we will be using later
   field_in.buffer = NULL;
   field_in.size = 256;
   field_in.flags = TEEC_MEM_INPUT;
   rc = TEEC_AllocateSharedMemory(&ctx, &field_in);
   check_rc(rc, "TEEC_AllocateSharedMemory for field_in", NULL);

   // field back
   field_back.buffer = NULL;
   field_back.size = 256;
   field_back.flags = TEEC_MEM_OUTPUT;
   rc = TEEC_AllocateSharedMemory(&ctx, &field_back);
   check_rc(rc, "TEEC_AllocateSharedMemory for field_back", NULL);

   /* Clear the TEEC_Operation struct */
   memset(&op, 0, sizeof(op));

   // assign param
   op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_WHOLE, TEEC_NONE, TEEC_NONE);
   op.params[0].memref.parent = &field_in;
   op.params[1].memref.parent = &field_back;

   // prepare for encrypt
   printf ("\n");
   printf ("Encrypt in secure world test\n");
   printf ("\n");
   memcpy(field_in.buffer, test_in, test_in_len);
   op.params[0].memref.size = test_in_len;
   rc = TEEC_InvokeCommand(&sess, TEST_ENCRYPT_IN_TA_COMMAND, &op, &err_origin);
   check_rc(rc, "TEEC_InvokeCommand", &err_origin);
   decrypt_using_private_key ((char *)field_back.buffer, op.params[1].memref.size, test_decrypted, &test_decrypted_len);
   printf ("Origional string (26 chars):  %s\n", test_in);
   printf ("Origional string len:         %i\n", test_in_len);
   printf ("SW Encryted value:            %s\n", (char *) field_back.buffer);
   printf ("SW Encryted len:              %i\n", (int) op.params[1].memref.size);
   printf ("NW Decrypted value:           %s\n", test_decrypted);
   printf ("NW Decrypted len:             %i\n", test_decrypted_len);
   printf ("\n");
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< end of test >>>>>>>>>>>>>>>>>>>>>>>>\n");

   // clean up once you have finished
   printf("Cleaning up after yourself\n");
   TEEC_CloseSession(&sess);
   TEEC_FinalizeContext(&ctx);
}

// send something encrypted to the secure world for decryption
void decrypt_in_secure_world (void) {

   char test_in [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
   int test_in_len = strlen (test_in);

   char test_encrypted [256] = "";
   int test_encrypted_len;

   TEEC_Result rc;
   TEEC_Context ctx;
   TEEC_Session sess;
   TEEC_Operation op;
   TEEC_SharedMemory field_in;
   TEEC_SharedMemory field_back;
   TEEC_UUID uuid = TEST_TA_UUID;
   uint32_t err_origin;

   // header
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< test_decrypt_secure_world >>>>>>>>>>>>>>>>>>>>>>>>>\n");

   /* Initialize a context connecting us to the TEE */
   rc = TEEC_InitializeContext(NULL, &ctx);
   check_rc(rc, "TEEC_InitializeContext", NULL);

   // open a session to the TA
   rc = TEEC_OpenSession(&ctx, &sess, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
   check_rc(rc, "TEEC_OpenSession", &err_origin);

   // prepare for encrypt
   printf ("\n");
   printf ("Decrypt in secure world test\n");
   printf ("\n");
   encrypt_using_public_key (test_in, test_in_len, test_encrypted, &test_encrypted_len);

   // ok, create the needed shared memory blocks we will be using later
   field_in.buffer = NULL;
   field_in.size = test_encrypted_len;
   field_in.flags = TEEC_MEM_INPUT;
   rc = TEEC_AllocateSharedMemory(&ctx, &field_in);
   check_rc(rc, "TEEC_AllocateSharedMemory for field_in", NULL);

   memcpy(field_in.buffer, test_encrypted, test_encrypted_len);

   // field back
   field_back.buffer = NULL;
   field_back.size = 256;
   field_back.flags = TEEC_MEM_OUTPUT;
   rc = TEEC_AllocateSharedMemory(&ctx, &field_back);
   check_rc(rc, "TEEC_AllocateSharedMemory for field_back", NULL);

   /* Clear the TEEC_Operation struct */
   memset(&op, 0, sizeof(op));

   // assign param
   op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_WHOLE, TEEC_MEMREF_WHOLE, TEEC_NONE, TEEC_NONE);
   op.params[0].memref.parent = &field_in;
   op.params[1].memref.parent = &field_back;

   // decrypt in TA
   rc = TEEC_InvokeCommand(&sess, TEST_DECRYPT_IN_TA_COMMAND, &op, &err_origin);
   printf ("Origional string (26 chars): %s\n", test_in);
   printf ("Origional string len:        %i\n", test_in_len);
   printf ("NW Encryted value:           %s\n", test_encrypted);
   printf ("NW Encryted len:             %i\n", test_encrypted_len);
   printf ("SW Decrypted value:          %s\n", (char *) field_back.buffer);
   printf ("SW Decrypted field len:      %i\n", (int) op.params[1].memref.size);
   printf ("\n");
   printf("<<<<<<<<<<<<<<<<<<<<<<<<<<< end of test >>>>>>>>>>>>>>>>>>>>>>>>\n");

   // clean up once you have finished
   printf("Cleaning up after yourself\n");
   TEEC_CloseSession(&sess);
   TEEC_FinalizeContext(&ctx);
}
