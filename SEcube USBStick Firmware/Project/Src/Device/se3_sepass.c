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
#include "se3_pass.h"
#include "se3_dispatcher_core.h" // required for login_struct

/*	sekey_get_implementation_info: This function would be the core of the SEkey behaviour,
 * 	it has to be implemented. The provided code is just a stub
 */

se3_flash_it pass_iterator = { .addr = NULL }; /**< Global variable required by get_all_password() */

uint16_t add_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t pass_id = 0; // id of the key to be stored on the device
	uint16_t host_len = 0; // length of the host provided by the caller (this may be encrypted)
	uint8_t *host = NULL;
	uint16_t user_len = 0; // length of the user provided by the caller (this may be encrypted)
	uint8_t *user = NULL;
	uint16_t pass_len = 0; // length of the password provided by the caller (this may be encrypted)
	uint8_t *pass = NULL;
    bool equal = false;
	se3_flash_it it = { .addr = NULL };

	se3_flash_pass password;

	// preliminary check
	if((req_size-2) < 8){ // minimum size check
		return SE3_ERR_PARAMS;
	}

	// parse request
	memcpy(&pass_id, req, 4); 			// key id
	memcpy(&host_len, req+4, 2); 		// host length
	memcpy(&user_len, req+4+2, 2); 		// username length
	memcpy(&pass_len, req+4+2+2, 2); 	// pass length
	host = (uint8_t*)malloc(host_len); 	// allocate space for the host content
	user = (uint8_t*)malloc(user_len); 	// allocate space for the user content
	pass = (uint8_t*)malloc(pass_len); 	// allocate space for the pass content
	if(host == NULL || pass == NULL || user == NULL){
		return SE3_ERR_MEMORY;
	} else {
		memset(host, 0, host_len);
		memset(user, 0, user_len);
		memset(pass, 0, pass_len);
	}

	if((req_size-2) != (4+2+2+2+host_len+pass_len+user_len)){ // 4B for key ID, 2B for host len, 2B for user len, 2B for pass len
		if(host != NULL){ free(host);	}
		if(user != NULL){ free(user);	}
		if(pass != NULL){ free(pass);	}
		return SE3_ERR_PARAMS;
	}

	memcpy(host, req+4+2+2+2, 		 			host_len);
	memcpy(user, req+4+2+2+2+host_len, 			user_len);
	memcpy(pass, req+4+2+2+2+host_len+user_len, pass_len);

	// now everything is ready to store the pass in the SEcube
	password.id = pass_id;
	password.host_size = host_len;
	password.pass_size = pass_len;
	password.user_size = user_len;
	password.host = host;
	password.user = user;
	password.pass = pass;

	// Insert the password
	se3_flash_it_init(&it);
	if (!se3_pass_find(password.id, &it)) { // search in the flash memory if a password with the same ID is already present
		it.addr = NULL;
	}
	if (NULL != it.addr) { // enter if there's another key with same ID
		equal = se3_pass_id_equal(&it, &password);  // do not replace if equal
		if (equal) { // if not equal delete current key
			if (!se3_flash_it_delete(&it)) {
				if(host != NULL){
					free(host);
				}
				if(pass != NULL){
					free(pass);
				}
				if(user != NULL){
					free(user);
				}
				return SE3_ERR_HW;
			}
		}
	}
	it.addr = NULL;
	if (!equal) { // if not equal create new key
		if (!se3_pass_new(&it, &password)) {
			if(host != NULL){
				free(host);
			}
			if(pass != NULL){
				free(pass);
			}
			if(user != NULL){
				free(user);
			}
			return SE3_ERR_MEMORY;
		}
	}
	if(host != NULL){
		free(host);
	}
	if(pass != NULL){
		free(pass);
	}
	if(user != NULL){
		free(user);
	}
	*resp_size = 2;
	memcpy(resp, "OK", 2);
	return SE3_OK;
}

uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t key_id = 0, kid = 0;
	bool error_ = false;
	se3_flash_it it = { .addr = NULL };
	*resp_size = 0;
	if((req_size - 2) != 4){
		return SE3_ERR_PARAMS;
	}
	memcpy(&kid, req, 4); // retrieve the key id from the input buffer
	se3_flash_it_init(&it);
	while (se3_flash_it_next(&it)){
		if (it.type == SE3_TYPE_PASS){
			SE3_GET32(it.addr, SE3_FLASH_PASS_OFF_ID, key_id);
			if(key_id == kid){
				if (!se3_flash_it_delete(&it)) {
					error_ = true;
				}
			}
		}
	}
	if(error_){
		return SE3_ERR_RESOURCE;
	} else {
		memcpy(resp, "OK", 2);
		*resp_size = 2;
		return SE3_OK;
	}
	return SE3_OK;
}

int16_t isStringContained(uint8_t* a, uint16_t len_text, uint8_t* b, uint16_t len_search){
	for (uint16_t i = 0; i < len_text && i < len_search; i++){
		if(a[i] != b[i]){
			return -1;
		}
	}
	return 0;
}

uint16_t get_all_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	// Filter mode
	uint8_t filter_mode = NO_FILTER;
	uint16_t filter_field_len = 0;
	uint8_t* filter_field = NULL;
	memcpy(&filter_mode, req, 2);
	if(filter_mode != NO_FILTER){
		memcpy(&filter_field_len, req+2, 2);
		filter_field = (uint8_t*)malloc(filter_field_len);
		memcpy(filter_field, req+4, filter_field_len);
	}

	uint32_t pass_id = 0;
	uint16_t offset = 0, host_len = 0, user_len = 0, pass_len = 0;
	*resp_size = 0;
	do {
		/* 7536 is the limit of data we can write in requests and responses to/from the SEcube */
		if(offset >= 7532){ // max is 7536 but we must keep 4 bytes available for ID=0 sent in case we reach the end of the flash (see below at the end of the function)
			if(filter_field != NULL){
				free(filter_field);
			}
			return SE3_OK; // still in the middle of the flash...not all IDs returned (but return here because we can't go past 7536 bytes, next time we will start from next flash sector)
		}
		if (pass_iterator.addr != NULL && pass_iterator.type == SE3_TYPE_PASS) {
			SE3_GET32(pass_iterator.addr, SE3_FLASH_PASS_OFF_ID, pass_id); // get key ID and copy it to the response buffer
			SE3_GET16(pass_iterator.addr, SE3_FLASH_PASS_OFF_HOST_LEN, host_len); // get key length
			SE3_GET16(pass_iterator.addr, SE3_FLASH_PASS_OFF_USER_LEN, user_len); // get key length
			SE3_GET16(pass_iterator.addr, SE3_FLASH_PASS_OFF_PASS_LEN, pass_len); // get key length

			memcpy(resp + offset, &pass_id, 4);
			offset += 4;
			memcpy(resp + offset, &host_len, 2);
			offset += 2;
			memcpy(resp + offset, &user_len, 2);
			offset += 2;
			memcpy(resp + offset, &pass_len, 2);
			offset += 2;

			memcpy(resp + offset, pass_iterator.addr + SE3_FLASH_PASS_OFF_DATA, host_len);
			offset += host_len;
			memcpy(resp + offset, pass_iterator.addr + SE3_FLASH_PASS_OFF_DATA + host_len, user_len);
			offset += user_len;
			memcpy(resp + offset, pass_iterator.addr + SE3_FLASH_PASS_OFF_DATA + host_len + user_len, pass_len);
			offset += pass_len;

			// Filter the unwanted elements
			if(filter_mode == HOST_FILTER){
				if(isStringContained(resp + offset - pass_len - user_len - host_len, host_len, filter_field, filter_field_len) < 0){
					offset -= (10 + host_len + user_len + pass_len);
				}
			} else if (filter_mode == USER_FILTER){
				if(isStringContained(resp + offset - pass_len - user_len, user_len, filter_field, filter_field_len) < 0){
					offset -= (10 + host_len + user_len + pass_len);
				}
			}

		}
	} while (se3_flash_it_next(&pass_iterator));
	/* reset the iterator to the beginning of the flash (required for next call of load_key_ids).
	 * notice that this is done only if we reach the end of the flash (we scanned all IDs)...if we still
	 * haven't scanned all IDs, we won't execute the next line because we will return with the instruction
	 * few lines above. */
	se3_flash_it_init(&pass_iterator);
	memset(resp + offset, 0, 10); // put all zeroes as the last id (id = 0 is not valid so the host side will understand that we reached the end of the flash)
	*resp_size = offset + 10;

	if(filter_field != NULL){
		free(filter_field);
	}
	return SE3_OK;
}

uint8_t lowercase_chars[26] = "abcdefghijklmnopqrstuvwxyz";
uint8_t uppercase_chars[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
uint8_t numbers_chars[10] = "1234567890";
uint8_t special_chars[13] = "-_.:;,?&%$!@#";

uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint16_t pass_len = 0;
	uint8_t special_char = 0;
	uint8_t numbers = 0;
	uint8_t uppercase = 0;

	memcpy(&pass_len, req, 2);
	memcpy(&uppercase, req + 2, 1);
	memcpy(&numbers, req + 3, 1);
	memcpy(&special_char, req + 4, 1);

	uint8_t* all_usable = (uint8_t*)malloc(numbers*10 + special_char*13 + uppercase*26 + 26);
	uint8_t all_usable_count = 26;

	memcpy(all_usable, lowercase_chars, 26);
	if(uppercase == 1) {
		memcpy(all_usable + all_usable_count, uppercase_chars, 26);
		all_usable_count+=26;
	}
	if(numbers == 1) {
		memcpy(all_usable + all_usable_count, numbers_chars, 10);
		all_usable_count+=10;
	}
	if(special_char == 1) {
		memcpy(all_usable + all_usable_count, special_chars, 13);
		all_usable_count+=13;
	}

	uint8_t *key_data = NULL;
	key_data = (uint8_t*)malloc(pass_len); // allocate space for the key content
	if(key_data == NULL){
		if(all_usable != NULL){ free(all_usable); }
		return SE3_ERR_MEMORY;
	} else {
		memset(key_data, 0, pass_len);
	}

	if(se3_rand(pass_len, key_data) != pass_len){
		if(key_data != NULL){ free(key_data);	}
		if(all_usable != NULL){ free(all_usable); }
		return SE3_ERR_HW;
	}

	for (int i = 0; i < pass_len; i++){
		resp[i] = all_usable[((uint8_t)key_data[i]) % all_usable_count];
	}

	*resp_size = pass_len;

	if(key_data != NULL){ free(key_data);	}
	if(all_usable != NULL){ free(all_usable); }

	return SE3_OK;
}

uint16_t export_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}

uint16_t import_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

	return SE3_OK;
}
