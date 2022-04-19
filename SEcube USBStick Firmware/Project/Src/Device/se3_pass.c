/**
  ******************************************************************************
  * File Name          : se3_passs.c
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

#include "se3_pass.h"


bool se3_pass_find(uint32_t id, se3_flash_it* it)
{
    uint32_t key_id = 0;
	se3_flash_it_init(it);
	while (se3_flash_it_next(it)) {
		if (it->type == SE3_TYPE_PASS) {
            SE3_GET32(it->addr, SE3_FLASH_PASS_OFF_ID, key_id);
			if (key_id == id) {
				return true;
			}
		}
	}
	return false;
}


bool se3_pass_id_equal(se3_flash_it* it, se3_flash_pass* key)
{
	uint32_t u32tmp = 0;
	if (key->host == NULL || key->pass == NULL){
		return false;
	}
	SE3_GET32(it->addr, SE3_FLASH_PASS_OFF_ID, u32tmp);
	if (u32tmp != key->id){	return false; }
	return true;
}

bool se3_pass_write(se3_flash_it* it, se3_flash_pass* key)
{
	bool success = false;
	do {
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_ID, (uint8_t*)&(key->id), 4)) { // id is uint32_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_HOST_LEN, (uint8_t*)&(key->host_size), 2)) { // datalen is uint16_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_USER_LEN, (uint8_t*)&(key->user_size), 2)) { // datalen is uint16_t
			break;
		}
		if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_PASS_LEN, (uint8_t*)&(key->pass_size), 2)) { // datalen is uint16_t
			break;
		}
		if (key->host_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA, key->host, key->host_size)) {
				break;
			}
		}
		if (key->user_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA + key->host_size, key->user, key->user_size)) {
				break;
			}
		}
		if (key->pass_size) {
			if (!se3_flash_it_write(it, SE3_FLASH_PASS_OFF_DATA + key->host_size + key->user_size, key->pass, key->pass_size)) {
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

bool se3_pass_new(se3_flash_it* it, se3_flash_pass* key)
{
	uint16_t size = (SE3_FLASH_PASS_SIZE_HEADER + key->host_size + key->pass_size + key->user_size);
    if (size > SE3_FLASH_NODE_DATA_MAX) {
        return false;
    }
	if (!se3_flash_it_new(it, SE3_TYPE_PASS, size)) {
		SE3_TRACE(("E pass_new cannot allocate flash block\n"));
		return false;
	}
	return se3_pass_write(it, key);
}

void se3_pass_read(se3_flash_it* it, se3_flash_pass* key)
{
    SE3_GET32(it->addr, SE3_FLASH_PASS_OFF_ID, key->id);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_HOST_LEN, key->host_size);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_PASS_LEN, key->pass_size);
    SE3_GET16(it->addr, SE3_FLASH_PASS_OFF_USER_LEN, key->user_size);
	if (key->host) {
		memcpy(key->host, it->addr + SE3_FLASH_PASS_OFF_DATA, key->host_size);
	}
	if (key->user) {
		memcpy(key->user, it->addr + SE3_FLASH_PASS_OFF_DATA + key->host_size, key->user_size);
	}
	if (key->pass) {
		memcpy(key->pass, it->addr + SE3_FLASH_PASS_OFF_DATA + key->host_size + key->user_size, key->pass_size);
	}
}

void se3_pass_fingerprint(se3_flash_pass* key, const uint8_t* salt, uint8_t* fingerprint)
{
	PBKDF2HmacSha256(key->pass, key->pass_size, salt, SE3_KEY_SALT_SIZE, 1, fingerprint, SE3_KEY_FINGERPRINT_SIZE);
}
