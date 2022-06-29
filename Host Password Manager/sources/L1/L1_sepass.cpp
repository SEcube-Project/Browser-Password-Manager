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
 * @file	L1_sepass.cpp
 * @date	July, 2022
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

	// Fill the command buffer
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_id, offset, 4);
	offset += 4;
	data_len = offset - L1Request::Offset::DATA;

	// Send the request
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

	// Fill the command buffer
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
	// Send the request
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		return false;
	}

	// Parse response
	unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(L1Response::Size::MAX_DATA);
	memset(buffer.get(), 0, L1Response::Size::MAX_DATA);
	memcpy(buffer.get(), (this->base.GetSessionBuffer()+L1Request::Offset::DATA), resp_len);

	memcpy(generated_pass.get(), buffer.get(), pass_len);
	return true;
}

bool L1::L1SEAddPassword(uint16_t pass_id, uint8_t *host_data, uint16_t host_len, uint8_t *user_data, uint16_t user_len, uint8_t *pass_data, uint16_t pass_len){
	se3Pass pass;
	pass.id = pass_id;
	pass.hostSize = host_len;
	pass.userSize = user_len;
	pass.passSize = pass_len;

	pass.host = std::string((const char*)host_data, host_len);
	pass.user = std::string((const char*)user_data, user_len);
	pass.pass = std::string((const char*)pass_data, pass_len);

	// Call a wrapped methos
	return L1::L1SEModifyPassword(pass_id, pass, false);
}

bool L1::L1SEModifyPassword(uint32_t pass_id, se3Pass& password){

	return L1::L1SEModifyPassword(pass_id, password, true);
}

bool L1::L1SEModifyPassword(uint32_t pass_id, se3Pass& password, bool isModify){
	uint16_t data_len = 0;
	uint16_t resp_len = 0;

	// Manage add and modify with a single method to reduce code redundancy
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_ADD;
	if (isModify) {
		op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_MODIFY;
	}

	// Fill the command buffer
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
	if(!password.host.empty() && !password.user.empty() && !password.pass.empty()) { // this is in case the host wants to explicitly send the key content to the SEcube
		this->base.FillSessionBuffer((unsigned char*)password.host.c_str(), offset, password.hostSize);
		offset += password.hostSize;
		this->base.FillSessionBuffer((unsigned char*)password.user.c_str(), offset, password.userSize);
		offset += password.userSize;
		this->base.FillSessionBuffer((unsigned char*)password.pass.c_str(), offset, password.passSize);
		offset += password.passSize;
	}
	data_len = offset - L1Request::Offset::DATA;

	// Send request
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		return false;
	}
	if(resp_len != 2){
		return false;
	}

	// Read response
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

	// Fill the command buffer
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

	offset = 0;
	memcpy(&passid, buffer.get(), 4);
	memcpy(&host_len, buffer.get() + (offset += 4), 2);
	memcpy(&user_len, buffer.get() + (offset += 2), 2);
	memcpy(&pass_len, buffer.get() + (offset += 2), 2);
	if(passid == 0){
		return false; // when the SEcube reaches the end of the flash (all keys returned) it sends 0, so we have our condition to terminate
	}

	// Parse response
	pass.id = passid;
	pass.hostSize = host_len;
	pass.userSize = user_len;
	pass.passSize = pass_len;

	pass.host = std::string((const char*)buffer.get() + (offset += 2), host_len);
	pass.user = std::string((const char*)buffer.get() + (offset += host_len), user_len);
	pass.pass = std::string((const char*)buffer.get() + (offset += user_len), pass_len);

	return true;
}

bool L1::L1SEGetAllPasswordsByUserName(std::vector<uint8_t> username, std::vector<se3Pass>& passList)
{
	// Call the get password wrapper
	return L1SEGetAllPasswords(USER_FILTER, &username, passList);
}

bool L1::L1SEGetAllPasswordsByHostName(std::vector<uint8_t> hostname, std::vector<se3Pass>& passList)
{
	// Call the get password wrapper
	return L1SEGetAllPasswords(HOST_FILTER, &hostname, passList);
}

bool L1::L1SEGetAllPasswords(std::vector<se3Pass>& passList)
{
	// Call the get password wrapper
	return L1SEGetAllPasswords(NO_FILTER, nullptr, passList);
}

bool L1::L1SEGetAllPasswords(uint8_t filterType, std::vector<uint8_t> *filter, std::vector<se3Pass>& passList)
{
	L1PasswordListException passListExc;
	passList.clear();
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_GETALL;
	uint16_t offset = L1Request::Offset::DATA;
	unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(L1Response::Size::MAX_DATA);

	// Fill the session request buffer
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	if(filterType != NO_FILTER){
		this->base.FillSessionBuffer((unsigned char*)&filterType, offset, 2);
		offset += 2;

		uint16_t filterSize = filter->size();
		this->base.FillSessionBuffer((unsigned char*)&filterSize, offset, 2);
		offset += 2;

		this->base.FillSessionBuffer((unsigned char*)filter->data(), offset, filter->size());
		offset += filter->size();
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

		pass.host = std::string((const char*)buffer.get() + offset, host_len);
		offset += host_len;

		pass.user = std::string((const char*)buffer.get() + offset, user_len);
		offset += user_len;

		pass.pass = std::string((const char*)buffer.get() + offset, pass_len);
		offset += pass_len;

		passList.push_back(pass); // copy password record into list
	}

	return true;
}
