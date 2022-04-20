/**
  ******************************************************************************
  * File Name          : se3_sekey.h
  * Description        : Firmware functionalities to support the KMS
  ******************************************************************************
  *
  * Copyright(c) 2016-present Blu5 Group <https://www.blu5group.com>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 3 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, see <https://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef SE3_SEPASS_H
#define SE3_SEPASS_H

#include "se3_keys.h"
#include <stdint.h>
#include <stdbool.h>

#include "se3_security_core.h"

enum{
	SE3_SEPASS_OP_ADD = 1, /**< Add password */
	SE3_SEPASS_OP_DELETE = 2, /**< Delete password */
	SE3_SEPASS_OP_GET_BY_ID = 3, /**< Get all password items */
	SE3_SEPASS_OP_GETALL = 4, /**< Get all password items */
	SE3_SEPASS_OP_GENERATE_RANDOM = 5, /**< Generate Random Password */
	SE3_SEPASS_OP_EXPORT = 6, /**< Export Passwords */
	SE3_SEPASS_OP_IMPORT = 7, /**< Import Passwords */
};

enum{
	NO_FILTER = 0,
	HOST_FILTER = 1,
	USER_FILTER = 2
};


/** \brief Retrieve the ID and length of the keys stored on the SEcube.
 * This function iterates over the entire flash memory reading the ID and length of each stored key. These data are written
 * into the response buffer. Since there is a maximum amount of bytes that can be sent in the communication back to the host
 * PC, this function may be called multiple times in a row. Each time it is called, it will start searching for keys from the
 * last one that was found. When all the keys have been scanned, the function will insert a set of 0s into the response buffer
 * so that the host PC will know that the key ID and length retrieval process has ended.
 * Notice that some keys, depending on the request coming from the host, may be skipped. This function, in fact, is used both
 * by manually administered keys and by the SEkey KMS, therefore a flag determines if the keys that need to be returned are the
 * one for manual management or automatic management (KMS). */
uint16_t add_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Send to the host the value of a key as ciphertext.
 * This function is required by the SEkey administrator in order to distribute the keys to the users.
 * This function will not work if the active login on the SEcube is not in administrator mode. The value of the key passed
 * in the request buffer will be written as ciphertext in the response buffer, provided that a single key with that ID is
 * stored in the flash memory. In case of multiple keys with the same ID, an error is returned. In order to encrypt the key
 * to be returned, another key present in the flash memory has to be specified by the host in the request buffer. */
uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Write the name and the ID of the SEkey user associated to the SEcube into the flash memory.
 * This action is performed only by the SEkey administrator therefore it is not allowed when the active login
 * is in user mode. */
uint16_t get_all_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t get_password_by_id(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Delete all the keys stored in the flash memory of the SEcube.
 * The host can specify a list of IDs, inside the request buffer, not to be deleted in order to
 * preserve them inside the flash memory. This is useful for SEkey. */
uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Checks if the SEcube is ready to be used with SEkey. */
uint16_t export_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Simply delete a key from the flash memory. The ID of the key to be deleted is passed in the request buffer. */
uint16_t import_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);


#endif
