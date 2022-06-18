/**
  ******************************************************************************
  * File Name          : se3_sepass.c
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
 *  \file se3_sepass.c
 *  \date 22/04/2022
 *  \version SEcube SDK 1.5.1
 *  \brief Firmware functionalities to support the Browser Password Management.
 */

#include "se3_sepass.h"
#include "se3_pass.h"
#include "se3_rand.h"

se3_flash_it pass_iterator = { .addr = NULL }; /**< Global variable required by get_all_password() */

/* Support structure for random password generation with custom configuration
 * like uppercase, numbers and special characters
 * */
const uint8_t characters_num = 26;
const uint8_t lowercase_chars[26] = "abcdefghijklmnopqrstuvwxyz";
const uint8_t uppercase_chars[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const uint8_t numbers_num = 10;
const uint8_t numbers_chars[10] = "1234567890";
const uint8_t special_num = 13;
const uint8_t special_chars[13] = "-_.:;,?&%$!@#";
typedef struct {
	uint8_t* set;
	uint8_t len;
} CharacterSet;


/**
 * Private function used to de-allocate all fields of the passed se3_flash_pass structure
 */
void deallocate_user_struct(se3_flash_pass *password){
	if(password->host) { free(password->host); }
	if(password->user) { free(password->user); }
	if(password->pass) { free(password->pass); }
}

/**
 * Private function used to check if a string is contained into another
 */
int16_t is_string_contained(uint8_t* a, uint16_t len_text, uint8_t* b, uint16_t len_search){
	for (uint16_t i = 0; i < len_text && i < len_search; i++){
		if(a[i] != b[i]){
			return -1;
		}
	}
	return 0;
}


uint16_t modify_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t pass_id = 0; 	// id of the key to be stored on the device
	uint16_t host_len = 0; 	// length of the host provided by the caller (this may be encrypted)
	uint8_t *host = NULL;
	uint16_t user_len = 0; 	// length of the user provided by the caller (this may be encrypted)
	uint8_t *user = NULL;
	uint16_t pass_len = 0; 	// length of the password provided by the caller (this may be encrypted)
	uint8_t *pass = NULL;
	se3_flash_it it = { .addr = NULL };

	se3_flash_pass password;

	// preliminary check
	if((req_size-2) < 10){ // minimum size check
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
	if(host == NULL || pass == NULL || user == NULL || pass_id == 0){
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
	// save it into a support structure
	password.id = pass_id;
	password.host_size = host_len;
	password.pass_size = pass_len;
	password.user_size = user_len;
	password.host = host;
	password.user = user;
	password.pass = pass;

	// Search in the flash memory if a password with the same ID is already present
	if (se3_pass_find(password.id, &it)) {
		// Try to delete the record
		if (!se3_flash_it_delete(&it)) {
			if(host != NULL){free(host);}
			if(pass != NULL){free(pass);}
			if(user != NULL){free(user);}
			return SE3_ERR_HW;
		}
	} else {
		// If not found
		if(host != NULL){ free(host);	}
		if(user != NULL){ free(user);	}
		if(pass != NULL){ free(pass);	}
		return SE3_ERR_PARAMS;
	}

	// Create the item
	se3_flash_it_init(&it);
	if (!se3_pass_new(&it, &password)) {
		if(host != NULL){free(host);}
		if(pass != NULL){free(pass);}
		if(user != NULL){free(user);}
		return SE3_ERR_MEMORY;
	}

	if(host != NULL){free(host);}
	if(pass != NULL){free(pass);}
	if(user != NULL){free(user);}

	*resp_size = 2;
	memcpy(resp, "OK", 2);
	return SE3_OK;
}


uint16_t add_new_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t pass_id = 0; 	// id of the key to be stored on the device
	uint16_t host_len = 0; 	// length of the host provided by the caller (this may be encrypted)
	uint8_t *host = NULL;
	uint16_t user_len = 0; 	// length of the user provided by the caller (this may be encrypted)
	uint8_t *user = NULL;
	uint16_t pass_len = 0; 	// length of the password provided by the caller (this may be encrypted)
	uint8_t *pass = NULL;
	se3_flash_it it = { .addr = NULL };

	se3_flash_pass password;

	// preliminary check
	if((req_size-2) < 10){ // minimum size check
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
	if(host == NULL || pass == NULL || user == NULL || pass_id == 0){
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
	// save it into a support structure
	password.id = pass_id;
	password.host_size = host_len;
	password.pass_size = pass_len;
	password.user_size = user_len;
	password.host = host;
	password.user = user;
	password.pass = pass;

	// Search in the flash memory if a password with the same ID or same hostname
	if (se3_pass_equal(&password, &it)) {
		// Try to delete the record
		if(host != NULL){ free(host);	}
		if(user != NULL){ free(user);	}
		if(pass != NULL){ free(pass);	}
		return SE3_ERR_PARAMS;
	}

	// Create the item
	se3_flash_it_init(&it);
	if (!se3_pass_new(&it, &password)) {
		if(host != NULL){free(host);}
		if(pass != NULL){free(pass);}
		if(user != NULL){free(user);}
		return SE3_ERR_MEMORY;
	}

	if(host != NULL){free(host);}
	if(pass != NULL){free(pass);}
	if(user != NULL){free(user);}

	*resp_size = 2;
	memcpy(resp, "OK", 2);
	return SE3_OK;
}

uint16_t delete_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t key_id = 0, kid = 0;
	bool error_ = false;
	bool found = false;
	se3_flash_it it = { .addr = NULL };
	*resp_size = 0;

	// preliminary check
	if((req_size - 2) != 4){	// minimum size check, 4B for key
		return SE3_ERR_PARAMS;
	}
	memcpy(&kid, req, 4); 		// retrieve the key id from the input buffer

	if(kid == 0){
		return SE3_ERR_PARAMS;
	}

	se3_flash_it_init(&it);
	while (se3_flash_it_next(&it)){
		if (it.type == SE3_TYPE_PASS){
			SE3_GET32(it.addr, SE3_FLASH_PASS_OFF_ID, key_id);
			if(key_id == kid){
				found = true;
				// Delete the record if has same id
				if (!se3_flash_it_delete(&it)) {
					error_ = true;
				}
			}
		}
	}
	if(error_ || !found){
		return SE3_ERR_RESOURCE;
	} else {
		memcpy(resp, "OK", 2);
		*resp_size = 2;
		return SE3_OK;
	}
	return SE3_OK;
}


uint16_t get_password_by_id(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint32_t key_id = 0, kid = 0;
	se3_flash_it it = { .addr = NULL };
	se3_flash_pass password;
	*resp_size = 0;

	// preliminary check
	if((req_size - 2) != 4){	// minimum size check, 4B for key
		return SE3_ERR_PARAMS;
	}

	memcpy(&kid, req, 4); // retrieve the key id from the input buffer

	if(kid == 0){
		return SE3_ERR_PARAMS;
	}

	se3_flash_it_init(&it);
	while (se3_flash_it_next(&it)){
		if (it.type == SE3_TYPE_PASS){
			SE3_GET32(it.addr, SE3_FLASH_PASS_OFF_ID, key_id);
			if(key_id == kid){
				// Read password record
				se3_pass_read(&it, &password);

				// Save the found password into the response
				memcpy(resp, &password.id, 4);
				memcpy(resp + 4, &password.host_size, 2);
				memcpy(resp + 6, &password.user_size, 2);
				memcpy(resp + 8, &password.pass_size, 2);
				memcpy(resp + 10, password.host, password.host_size);
				memcpy(resp + 10 + password.host_size, password.user, password.user_size);
				memcpy(resp + 10 + password.host_size + password.user_size, password.pass, password.pass_size);

				*resp_size = 10 + password.host_size + password.user_size + password.pass_size;

				deallocate_user_struct(&password);
				break;
			}
		}
	}

	memset(resp + (*resp_size), 0, 10); // put all zeroes as the last id (id = 0 is not valid so the host side will understand that we reached the end of the flash)
	*resp_size = (*resp_size) + 10;

	return SE3_OK;
}


uint16_t get_all_passwords(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){

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

	uint16_t offset = 0;
	se3_flash_pass password;
	*resp_size = 0;
	do {
		/* 7536 is the limit of data we can write in requests and responses to/from the SEcube */
		if(offset >= 7532){ // max is 7536 but we must keep 4 bytes available for ID=0 sent in case we reach the end of the flash (see below at the end of the function)
			if(filter_field != NULL){ free(filter_field); }
			return SE3_OK; // still in the middle of the flash...not all IDs returned (but return here because we can't go past 7536 bytes, next time we will start from next flash sector)
		}
		if (pass_iterator.addr != NULL && pass_iterator.type == SE3_TYPE_PASS) {
			se3_pass_read(&pass_iterator, &password);

			memcpy(resp + offset, &password.id, 4);
			offset += 4;
			memcpy(resp + offset, &password.host_size, 2);
			offset += 2;
			memcpy(resp + offset, &password.user_size, 2);
			offset += 2;
			memcpy(resp + offset, &password.pass_size, 2);
			offset += 2;

			memcpy(resp + offset, password.host, password.host_size);
			offset += password.host_size;
			memcpy(resp + offset, password.user, password.user_size);
			offset += password.user_size;
			memcpy(resp + offset, password.pass, password.pass_size);
			offset += password.pass_size;


			// Filter the unwanted elements
			if(filter_mode == HOST_FILTER){
				if(is_string_contained(password.host, password.host_size, filter_field, filter_field_len) < 0){
					offset -= (10 + password.host_size + password.user_size + password.pass_size);
				}
			} else if (filter_mode == USER_FILTER){
				if(is_string_contained(password.user, password.user_size, filter_field, filter_field_len) < 0){
					offset -= (10 + password.host_size + password.user_size + password.pass_size);
				}
			}

			deallocate_user_struct(&password);
		}
	} while (se3_flash_it_next(&pass_iterator));
	/* reset the iterator to the beginning of the flash
	 * notice that this is done only if we reach the end of the flash (we scanned all IDs)...if we still
	 * haven't scanned all IDs, we won't execute the next line because we will return with the instruction
	 * few lines above. */
	se3_flash_it_init(&pass_iterator);
	memset(resp + offset, 0, 10); // put all zeroes as the last id (id = 0 is not valid so the host side will understand that we reached the end of the flash)
	*resp_size = offset + 10;

	if(filter_field != NULL){ free(filter_field); }

	return SE3_OK;
}

uint16_t generate_random_password(uint16_t req_size, const uint8_t* req, uint16_t* resp_size, uint8_t* resp){
	uint16_t pass_len = 0;
	uint8_t special_char = 0;
	uint8_t numbers = 0;
	uint8_t uppercase = 0;

	memcpy(&pass_len, req, 2);
	memcpy(&uppercase, req + 2, 1);
	memcpy(&numbers, req + 3, 1);
	memcpy(&special_char, req + 4, 1);

	if(pass_len > 1024){
		return SE3_ERR_MEMORY;
	}

	uint8_t set_number = 1 + numbers + special_char + uppercase;
	CharacterSet* all_usable = malloc(set_number*sizeof *all_usable);

	// Construct support array for the selected mode and size
	CharacterSet base_set;
	base_set.set = (uint8_t*)&lowercase_chars;
	base_set.len = characters_num;
	all_usable[0] = base_set;

	uint8_t idx = 1;
	if(uppercase == 1) {
		CharacterSet upper_set;
		upper_set.set = (uint8_t*)&uppercase_chars;
		upper_set.len = characters_num;
		all_usable[idx++] = upper_set;
	}
	if(numbers == 1) {
		CharacterSet number_set;
		number_set.set = (uint8_t*)&numbers_chars;
		number_set.len = numbers_num;
		all_usable[idx++] = number_set;
	}
	if(special_char == 1) {
		CharacterSet special_set;
		special_set.set = (uint8_t*)&special_chars;
		special_set.len = special_num;
		all_usable[idx++] = special_set;
	}


	// Allocate space for the key content
	uint8_t *key_data = NULL;
	key_data = (uint8_t*)malloc(pass_len*2);
	if(key_data == NULL){
		if(all_usable != NULL){ free(all_usable); }
		return SE3_ERR_MEMORY;
	} else {
		memset(key_data, 0, pass_len*2);
	}

	// Generate the random key
	if(se3_rand(2*pass_len, key_data) != 2*pass_len){
		if(key_data != NULL){ free(key_data);	}
		if(all_usable != NULL){ free(all_usable); }
		return SE3_ERR_HW;
	}

	// Map the random values into the selected characters
	CharacterSet tmp;
	for (int i = 0; i < pass_len; i++){
		tmp = all_usable[((uint8_t)key_data[i*2]) % set_number];
		resp[i] = tmp.set[((uint8_t)key_data[i*2+1]) % tmp.len];
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
