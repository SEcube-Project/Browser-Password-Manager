/**
  ******************************************************************************
  * File Name          : se3_pass.c
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

#include "se3_pass.h"

bool se3_pass_find(uint32_t id, se3_flash_it* it)
{
    uint32_t password_id = 0;
	se3_flash_it_init(it);
	while (se3_flash_it_next(it)) {
		if (it->type == SE3_TYPE_PASS) {
            SE3_GET32(it->addr, SE3_FLASH_PASS_OFF_ID, password_id);
			if (password_id == id) {
				return true;
			}
		}
	}
	return false;
}

bool is_str_eq(uint8_t* orig, uint16_t lorig, uint8_t* val, uint16_t lval)
{
	if(lorig != lval)
		return false;
	else if(orig == NULL && val == NULL)
		return true;
	else if(orig == NULL)
		return false;

	for (uint16_t i = 0; i < lorig && i < lval; i++){
		if (orig[i] != val[i])
			return false;
	}
	return true;
}

bool se3_pass_equal(se3_flash_pass* password, se3_flash_it* it)
{
    se3_flash_pass tmp;
	se3_flash_it_init(it);
	while (se3_flash_it_next(it)) {
		if (it->type == SE3_TYPE_PASS) {
			se3_pass_read(it, &tmp);
        	if (password->host == NULL && tmp.host != NULL) {

    			if(tmp.host != NULL) {free(tmp.host);}
    			if(tmp.user != NULL) {free(tmp.user);}
    			if(tmp.pass != NULL) {free(tmp.pass);}
        		return false;
        	} else if ((password->host == NULL && tmp.host == NULL) || tmp.id == password->id || is_str_eq(tmp.host, tmp.host_size, password->host, password->host_size)) {

				if(tmp.host != NULL) {free(tmp.host);}
				if(tmp.user != NULL) {free(tmp.user);}
				if(tmp.pass != NULL) {free(tmp.pass);}
				return true;
			}

			if(tmp.host != NULL) {free(tmp.host);}
			if(tmp.user != NULL) {free(tmp.user);}
			if(tmp.pass != NULL) {free(tmp.pass);}
		}
	}

	return false;
}

bool se3_pass_write(se3_flash_it* it, se3_flash_pass* password)
{
	bool success = false;
	do {
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_ID, (uint8_t*)&(password->id), 4)) { // id is uint32_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_HOST_LEN, (uint8_t*)&(password->host_size), 2)) { // datalen is uint16_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_USER_LEN, (uint8_t*)&(password->user_size), 2)) { // datalen is uint16_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_PASS_LEN, (uint8_t*)&(password->pass_size), 2)) { // datalen is uint16_t
			break;
		}
		if (password->host_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA, password->host, password->host_size)) {
				break;
			}
		}
		if (password->user_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA + password->host_size, password->user, password->user_size)) {
				break;
			}
		}
		if (password->pass_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA + password->host_size + password->user_size, password->pass, password->pass_size)) {
				break;
			}
		}
		success = true;
	} while (0);

	if (!success) {
        SE3_TRACE(("[se3_pass_write] cannot write to flash block\n"));
	}
	return success;
}


bool se3_pass_new(se3_flash_it* it, se3_flash_pass* password)
{
	uint16_t size = (SE3_FLASH_PASS_SIZE_HEADER + password->host_size + password->pass_size + password->user_size);
    if (size > SE3_FLASH_NODE_DATA_MAX) {
        return false;
    }
	if (!se3_flash_it_new(it, SE3_TYPE_PASS, size)) {
		SE3_TRACE(("[se3_pass_new] pass_new cannot allocate flash block\n"));
		return false;
	}
	return se3_pass_write(it, password);
}


void se3_pass_read(se3_flash_it* it, se3_flash_pass* password)
{
    SE3_GET32(it->addr, SE3_FLASH_PASS_OFF_ID, password->id);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_HOST_LEN, password->host_size);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_PASS_LEN, password->pass_size);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_USER_LEN, password->user_size);

	password->host = (uint8_t*)malloc(password->host_size); 	// allocate space for the host content
	password->user = (uint8_t*)malloc(password->user_size); 	// allocate space for the user content
	password->pass = (uint8_t*)malloc(password->pass_size); 	// allocate space for the pass content

	if (password->host) {
		memcpy(password->host, it->addr + SE3_FLASH_PASS_OFF_DATA, password->host_size);
	}
	if (password->user) {
		memcpy(password->user, it->addr + SE3_FLASH_PASS_OFF_DATA + password->host_size, password->user_size);
	}
	if (password->pass) {
		memcpy(password->pass, it->addr + SE3_FLASH_PASS_OFF_DATA + password->host_size + password->user_size, password->pass_size);
	}
}


void se3_pass_fingerprint(se3_flash_pass* password, const uint8_t* salt, uint8_t* fingerprint)
{
	PBKDF2HmacSha256(password->pass, password->pass_size, salt, SE3_PASS_SALT_SIZE, 1, fingerprint, SE3_PASS_FINGERPRINT_SIZE);
}
