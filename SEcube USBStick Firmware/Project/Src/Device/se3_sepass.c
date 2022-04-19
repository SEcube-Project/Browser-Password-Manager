/**
  ******************************************************************************
  * File Name          : se3_sekey.c
  * Description        : Firmware functionalities to support the KMS.
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

/**
 *  \file se3_sekey.c
 *  \date 22/06/2020
 *  \version SEcube SDK 1.5.1
 *  \brief Firmware functionalities to support the KMS.
 */

#include "se3_sepass.h"
#include "se3_dispatcher_core.h" // required for login_struct

/*	sekey_get_implementation_info: This function would be the core of the SEkey behaviour,
 * 	it has to be implemented. The provided code is just a stub
 */

uint16_t add_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t get_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t get_all_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t export_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t import_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}
