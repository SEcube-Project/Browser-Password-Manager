/**
  ******************************************************************************
  * File Name          : se3_passs.h
  * Description        : Low-level key management
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

#ifndef SE3_PASS_H
#define SE3_PASS_H

#pragma once
#include "se3c1def.h"
#include "se3_flash.h"
#include "pbkdf2.h"
#define SE3_TYPE_PASS 100

/** \brief Flash key structure
 *
 *  Disposition of the fields within the flash node:
 *  0:3     id
 *  4:5     host_size
 *  6:7     pass_size
 *  8:(6+data_size-1) data
 */
typedef struct se3_flash_pass_ {
	uint32_t id;
	uint16_t host_size;
	uint16_t user_size;
	uint16_t pass_size;
	uint8_t* host;
	uint8_t* user;
	uint8_t* pass;
} se3_flash_pass;

/** Flash key fields */
enum {
    SE3_FLASH_PASS_OFF_ID = 0,
    SE3_FLASH_PASS_OFF_HOST_LEN = 4,
    SE3_FLASH_PASS_OFF_USER_LEN = 6,
    SE3_FLASH_PASS_OFF_PASS_LEN = 8,
    SE3_FLASH_PASS_OFF_DATA = 10,
    SE3_FLASH_PASS_SIZE_HEADER = SE3_FLASH_PASS_OFF_DATA
};

/** \brief Find a key
 *
 *  Find a key in the flash memory
 *  \param id identifier of the key
 *  \param it a flash iterator that will be set to the key's position
 *  \return true on success
 */
bool se3_pass_find(uint32_t id, se3_flash_it* it);

/** \brief Add a new key
 *  
 *  Create a new node with the necessary amount of space for the key,
 *  then write the key.
 *  \remark if a flash operation fails, the hwerror flag (se3c0.hwerror) is set.
 *  \param it a flash iterator which will receive the position of the new node
 *  \param key a flash key structure containing the key information
 *      The data and name fields must point to a valid memory region,
 *      unless their size (data_size, name_size) is zero.
 *  \return true on success, else false
 */
bool se3_pass_new(se3_flash_it* it, se3_flash_pass* key);

bool se3_pass_id_equal(se3_flash_it* it, se3_flash_pass* key);

/** \brief Read a key
 *  
 *  Read a key from a flash node
 *  \param it a flash iterator pointing to the key
 *  \param key a flash key structure which will receive the key's information. 
 *      The data and name fields will be filled only if not NULL.
 */
void se3_pass_read(se3_flash_it* it, se3_flash_pass* key);

/** \brief Write key data
 *  
 *  Write key data to a flash node
 *  \remark if a flash operation fails, the hwerror flag (se3c0.hwerror) is set.
 *  \param it a flash iterator pointing to a newly created flash node of key type
 *  \param key a flash key structure containing the key information
 *      The data and name fields must point to a valid memory region,
 *      unless their size (data_size, name_size) is zero.
 *  \return true on success, else false
 */
bool se3_pass_write(se3_flash_it* it, se3_flash_pass* key);

/** \brief Produce salted key fingerprint
 *  
 *  \param key a flash key structure containing the key information
 *  \param salt a 32-byte salt
 *  \param fingerprint output 32-byte fingerprint of the key data
 */
void se3_pass_fingerprint(se3_flash_pass* key, const uint8_t* salt, uint8_t* fingerprint);

#endif
