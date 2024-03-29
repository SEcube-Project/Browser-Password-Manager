/**
  ******************************************************************************
  * File Name          : se3_pass.h
  * Description        : Low-level password management
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

/** \brief Flash password structure
 *
 *  Disposition of the fields within the flash node:
 *  0:3     												id
 *  4:5     												host_size
 *  6:7     												user_size
 *  8:9     												pass_size
 *  10:(10+host_size-1) 									hostname
 *  (10+host_size):(10+host_size+user_size-1) 				username
 *  (10+host_size+pass_size):(10+host_size+user_size-1) 	password
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

/** Flash password fields */
enum {
    SE3_FLASH_PASS_OFF_ID = 0,
    SE3_FLASH_PASS_OFF_HOST_LEN = 4,
    SE3_FLASH_PASS_OFF_USER_LEN = 6,
    SE3_FLASH_PASS_OFF_PASS_LEN = 8,
    SE3_FLASH_PASS_OFF_DATA = 10,
    SE3_FLASH_PASS_SIZE_HEADER = SE3_FLASH_PASS_OFF_DATA
};


/** \brief Find a password
 *
 *  Find a password in the flash memory
 *  \param id identifier of the password
 *  \param it a flash iterator that will be set to the password's position
 *  \return true on success
 */
bool se3_pass_find(uint32_t id, se3_flash_it* it);

/** \brief Add a new password
 *  
 *  Create a new node with the necessary amount of space for the passsword,
 *  then write the passsword.
 *  \remark if a flash operation fails, the hwerror flag (se3c0.hwerror) is set.
 *  \param it a flash iterator which will receive the position of the new node
 *  \param password a flash password structure containing the password information
 *      The data and name fields must point to a valid memory region,
 *      unless their size (data_size, name_size) is zero.
 *  \return true on success, else false
 */
bool se3_pass_new(se3_flash_it* it, se3_flash_pass* password);


/** \brief Read a password
 *  
 *  Read a password from a flash node
 *  \param it a flash iterator pointing to the password
 *  \param password a flash password structure which will receive the password's information.
 *      The data and name fields will be filled only if not NULL.
 */
void se3_pass_read(se3_flash_it* it, se3_flash_pass* password);

/** \brief Chech if two password record has the same id or hostname
 *  
 *  Chech if two password record has the same id or hostname
 *  \param it a flash iterator pointing to a flash node of password type
 *  \param password a flash password structure containing the password information
 *  \return true if the id or username is equal
 */
bool se3_pass_equal(se3_flash_pass* password, se3_flash_it* it);

/** \brief Check if the two strings are equals
 *
 *  Write password data to a flash node
 *  \remark if a flash operation fails, the hwerror flag (se3c0.hwerror) is set.
 *  \param orig string one
 *  \param lorig length of the first string
 *  \param val string two
 *  \param lval length of the second string
 *  \return true if equal, else false
 */
bool is_str_eq(uint8_t* orig, uint16_t lorig, uint8_t* val, uint16_t lval);

/** \brief Write password data
 *
 *  Write password data to a flash node
 *  \remark if a flash operation fails, the hwerror flag (se3c0.hwerror) is set.
 *  \param it a flash iterator pointing to a newly created flash node of password type
 *  \param password a flash password structure containing the password information
 *      The data and name fields must point to a valid memory region,
 *      unless their size (data_size, name_size) is zero.
 *  \return true on success, else false
 */
bool se3_pass_write(se3_flash_it* it, se3_flash_pass* password);

/** \brief Produce salted password fingerprint
 *  
 *  \param password a flash password structure containing the password information
 *  \param salt a 32-byte salt
 *  \param fingerprint output 32-byte fingerprint of the password data
 */
void se3_pass_fingerprint(se3_flash_pass* password, const uint8_t* salt, uint8_t* fingerprint);

#endif
