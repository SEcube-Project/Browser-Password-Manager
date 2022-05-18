/**
  ******************************************************************************
  * File Name          : L1_sekey.cpp
  * Description        : L1 APIs required to implement SEkey.
  ******************************************************************************
  *
  * Copyright � 2016-present Blu5 Group <https://www.blu5group.com>
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
 * @file	L1_sekey.cpp
 * @date	July, 2017
 * @brief	L1 APIs required to implement SEkey. These must not be used explicitly.
 * @version SEcube Open Source SDK 1.5.1
 */

#include "L1.h"

using namespace std;

bool L1::L1SEDeletePassword(uint32_t pass_id){
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_DELETE;
	uint16_t offset = L1Request::Offset::DATA;
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_id, offset, 4);
	offset += 4;
	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		return false;
	}
	if(resp_len != 2 || strncmp((const char*)(this->base.GetSessionBuffer()+L1Response::Offset::DATA), "OK", 2) != 0){
		return false;
	} else {
		return true;
	}
}

bool L1::L1SEGenerateRandomPassword(uint16_t pass_len, uint8_t enable_upper_case, uint8_t enable_special_chars, uint8_t enable_numbers_chars, std::shared_ptr<uint8_t[]> generated_pass){
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_GENERATE_RANDOM;
	uint16_t offset = L1Request::Offset::DATA;
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_len, offset, 2);
	offset += 2;

	this->base.FillSessionBuffer((unsigned char*)&enable_upper_case, offset, 1);
	offset += 1;

	this->base.FillSessionBuffer((unsigned char*)&enable_numbers_chars, offset, 1);
	offset += 1;

	this->base.FillSessionBuffer((unsigned char*)&enable_special_chars, offset, 1);
	offset += 1;

	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		// TODO
	}

	unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(L1Response::Size::MAX_DATA);
	memset(buffer.get(), 0, L1Response::Size::MAX_DATA);
	memcpy(buffer.get(), (this->base.GetSessionBuffer()+L1Request::Offset::DATA), resp_len);

	memcpy(generated_pass.get(), buffer.get(), pass_len);
	return true;
}

bool L1::L1SEAddPassword(uint16_t pass_id, std::shared_ptr<uint8_t[]> host_data, uint16_t host_len, std::shared_ptr<uint8_t[]> user_data, uint16_t user_len, std::shared_ptr<uint8_t[]> pass_data, uint16_t pass_len){
	se3Pass pass;
	pass.id = pass_id;
	pass.hostSize = host_len;
	pass.userSize = user_len;
	pass.passSize = pass_len;
	pass.host = host_data.get();
	pass.user = user_data.get();
	pass.pass = pass_data.get();

	return L1::L1SEModifyPassword(pass_id, pass, false);
}

bool L1::L1SEModifyPassword(uint32_t pass_id, se3Pass& password){

	return L1::L1SEModifyPassword(pass_id, password, true);
}

bool L1::L1SEModifyPassword(uint32_t pass_id, se3Pass& password, bool isModify){
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_ADD;
	if (isModify) {
		op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_MODIFY;
	}
	uint16_t offset = L1Request::Offset::DATA;
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_id, offset, 4);
	offset += 4;
	this->base.FillSessionBuffer((unsigned char*)&password.hostSize, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&password.userSize, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&password.passSize, offset, 2);
	offset += 2;
	if(password.host != nullptr && password.user != nullptr && password.pass != nullptr){ // this is in case the host wants to explicitly send the key content to the SEcube
		this->base.FillSessionBuffer((unsigned char*)password.host, offset, password.hostSize);
		offset += password.hostSize;
		this->base.FillSessionBuffer((unsigned char*)password.user, offset, password.userSize);
		offset += password.userSize;
		this->base.FillSessionBuffer((unsigned char*)password.pass, offset, password.passSize);
		offset += password.passSize;
	}
	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		return false;
	}
	if(resp_len != 2){
		return false;
	}
	char okbuf[] = "OK";
	if(memcmp((const void*)(this->base.GetSessionBuffer()+L1Response::Offset::DATA), (const void*)okbuf, 2) != 0){
		return false;
	} else {
		return true;
	}
}

bool L1::L1SEGetPasswordById(uint32_t pass_id, se3Pass& pass)
{
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_GET_BY_ID;
	uint16_t offset = L1Request::Offset::DATA;
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_id, offset, 4);
	offset += 4;

	// Make request for pass id
	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		return false;
	}

	unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(L1Response::Size::MAX_DATA);
	// copy response to local buffer
	memset(buffer.get(), 0, L1Response::Size::MAX_DATA);
	memcpy(buffer.get(), (this->base.GetSessionBuffer()+L1Request::Offset::DATA), resp_len);
	if(resp_len == 0){ // if the response is empty, the SEcube has returned all the IDs in its flash memory
		return false;
	}

	uint32_t passid = 0;
	uint16_t host_len = 0;
	uint16_t user_len = 0;
	uint16_t pass_len = 0;
	memcpy(&passid, buffer.get(), 4);
	memcpy(&host_len, buffer.get()+4, 2);
	memcpy(&user_len, buffer.get()+6, 2);
	memcpy(&pass_len, buffer.get()+8, 2);
	if(passid == 0){
		return false; // when the SEcube reaches the end of the flash (all keys returned) it sends 0, so we have our condition to terminate
	}

	pass.id = passid;
	pass.hostSize = host_len;
	pass.userSize = user_len;
	pass.passSize = pass_len;
	pass.host = (uint8_t*)malloc(host_len*sizeof(uint8_t));
	memcpy(pass.host,  buffer.get()+10, host_len);
	pass.user = (uint8_t*)malloc(user_len*sizeof(uint8_t));
	memcpy(pass.user,  buffer.get()+10 + host_len, user_len);
	pass.pass = (uint8_t*)malloc(pass_len*sizeof(uint8_t));
	memcpy(pass.pass,  buffer.get()+10+host_len+user_len, pass_len);

	return true;
}

bool L1::L1SEGetAllPasswordsByUserName(std::shared_ptr<uint8_t[]> username, uint16_t usernameLen, std::vector<se3Pass>& passList)
{
	return L1SEGetAllPasswords(USER_FILTER, username, usernameLen, passList);
}

bool L1::L1SEGetAllPasswordsByHostName(std::shared_ptr<uint8_t[]> hostname, uint16_t hostnameLen, std::vector<se3Pass>& passList)
{
	return L1SEGetAllPasswords(HOST_FILTER, hostname, hostnameLen, passList);
}

bool L1::L1SEGetAllPasswords(std::vector<se3Pass>& passList)
{
	return L1SEGetAllPasswords(NO_FILTER, NULL, 0, passList);
}

bool L1::L1SEGetAllPasswords(uint8_t filterType, std::shared_ptr<uint8_t[]> filterField, uint16_t filterLen, std::vector<se3Pass>& passList)
{
	L1PasswordListException passListExc;
	passList.clear();
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_GETALL;
	uint16_t offset = L1Request::Offset::DATA;
	unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(L1Response::Size::MAX_DATA);

	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	if(filterType != NO_FILTER){
		this->base.FillSessionBuffer((unsigned char*)&filterType, offset, 2);
		offset += 2;
		this->base.FillSessionBuffer((unsigned char*)&filterLen, offset, 2);
		offset += 2;
		this->base.FillSessionBuffer((unsigned char*)filterField.get(), offset, filterLen);
		offset += filterLen;
	}

	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		passList.clear();
		return false;
	}
	// copy response to local buffer
	memset(buffer.get(), 0, L1Response::Size::MAX_DATA);
	memcpy(buffer.get(), (this->base.GetSessionBuffer()+L1Request::Offset::DATA), resp_len);
	if(resp_len == 0){ // if the response is empty, the SEcube has returned all the IDs in its flash memory
		return false;
	}
	// iterate over buffer reading the IDs and key length
	offset = 0;
	while(offset < resp_len){
		uint32_t passid = 0;
		uint16_t host_len = 0;
		uint16_t user_len = 0;
		uint16_t pass_len = 0;

		memcpy(&passid, buffer.get()+offset, 4);
		offset+=4;
		memcpy(&host_len, buffer.get()+offset, 2);
		offset+=2;
		memcpy(&user_len, buffer.get()+offset, 2);
		offset+=2;
		memcpy(&pass_len, buffer.get()+offset, 2);
		offset+=2;
		if(passid == 0){
			return false; // when the SEcube reaches the end of the flash (all keys returned) it sends 0, so we have our condition to terminate
		}

		se3Pass pass;
		pass.id = passid;
		pass.hostSize = host_len;
		pass.userSize = user_len;
		pass.passSize = pass_len;
		pass.host = (uint8_t*)malloc(host_len*sizeof(uint8_t));
		memcpy(pass.host,  buffer.get()+offset, host_len);
		offset+=host_len;
		pass.user = (uint8_t*)malloc(user_len*sizeof(uint8_t));
		memcpy(pass.user,  buffer.get()+offset, user_len);
		offset+=user_len;
		pass.pass = (uint8_t*)malloc(pass_len*sizeof(uint8_t));
		memcpy(pass.pass,  buffer.get()+offset, pass_len);
		offset+=pass_len;
		passList.push_back(pass); // copy ID in list
	}
	return true;
}
