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
	SE3_SEPASS_OP_MODIFY = 3, /**< Delete password */
	SE3_SEPASS_OP_GET_BY_ID = 4, /**< Get all password items */
	SE3_SEPASS_OP_GETALL = 5, /**< Get all password items */
	SE3_SEPASS_OP_GENERATE_RANDOM = 6, /**< Generate Random Password */
	SE3_SEPASS_OP_EXPORT = 7, /**< Export Passwords */
	SE3_SEPASS_OP_IMPORT = 8, /**< Import Passwords */
};

enum{
	NO_FILTER = 0,
	HOST_FILTER = 1,
	USER_FILTER = 2
};

uint16_t add_new_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t get_all_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t modify_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t get_password_by_id(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t export_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

uint16_t import_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp);

#endif
