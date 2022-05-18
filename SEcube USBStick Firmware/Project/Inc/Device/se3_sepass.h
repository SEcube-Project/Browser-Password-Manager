/**
  ******************************************************************************
  * File Name          : se3_sepass.h
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

#include <stdint.h>
#include <stdbool.h>

#include "se3_security_core.h"

enum{
	SE3_SEPASS_OP_ADD = 1, 				/**< Add password */
	SE3_SEPASS_OP_MODIFY = 2, 				/**< Add password */
	SE3_SEPASS_OP_DELETE = 3, 			/**< Delete password */
	SE3_SEPASS_OP_GET_BY_ID = 4, 		/**< Get password by id */
	SE3_SEPASS_OP_GETALL = 5, 			/**< Get all password items */
	SE3_SEPASS_OP_GENERATE_RANDOM = 6, 	/**< Generate Random Password */
	SE3_SEPASS_OP_EXPORT = 7, 			/**< Export Passwords - TODO */
	SE3_SEPASS_OP_IMPORT = 8, 			/**< Import Passwords - TODO */
};

enum{
	NO_FILTER = 0,
	HOST_FILTER = 1,
	USER_FILTER = 2
};

/** \brief Store a password inside the flash memory of the SEcube.
 *
 * The request needs a new id, so the request take in order 4B of id, 2B of hostname length, 2B of username lenght, 2B of password lenght and
 * in order the hostname, username and password of the size specifed before. If another key with the same id is already stored in the SEcube,
 * the old key is replaced by the new one.
 */
uint16_t add_new_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Modify a password inside the flash memory of the SEcube.
 *
 * The request needs a new id, so the request take in order 4B of id, 2B of hostname length, 2B of username lenght, 2B of password lenght and
 * in order the hostname, username and password of the size specifed before. If another key with the same id is already stored in the SEcube,
 * the old key is replaced by the new one.
 */
uint16_t modify_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Simply delete a password from the flash memory. The ID of the password to be deleted is passed in the request buffer. */
uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Retrieve all the passwords stored on the SEcube.
 * This function iterates over the entire flash memory reading the id, hostname length, username length, password length, hostname
 * value, username value and password value of each stored key. These data are written into the response buffer. Since there
 * is a maximum amount of bytes that can be sent in the communication back to the host PC, this function may be called multiple
 * times in a row. Each time it is called, it will start searching for password from the last one that was found. When all the
 * passwords have been scanned, the function will insert a set of 0s into the response buffer so that the host PC will know that
 * the password ID and length retrieval process has ended. */
uint16_t get_all_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Simply retrieve a password from the flash memory. The ID of the password to be searched is passed in the request buffer. */
uint16_t get_password_by_id(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/** \brief Generate a random password of the desired length. It is possible to select if include uppercase characters, numbers and
 * special characters with parameters to the input buffer: 4B pass length, 2B uppercase enable, 2B numbers enable, 2B special enable */
uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/**
 * TODO
 */
uint16_t export_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

/**
 * TODO
 */
uint16_t import_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

#endif
