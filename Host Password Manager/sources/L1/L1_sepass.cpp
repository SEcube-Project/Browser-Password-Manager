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

bool L1::L1SEpass_DeletePassword(uint32_t pass_id){
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

bool L1::L1SEpass_AddPassword(uint32_t pass_id, uint16_t host_len, uint16_t user_len, uint16_t pass_len, std::shared_ptr<uint8_t[]> host_data, std::shared_ptr<uint8_t[]> user_data, std::shared_ptr<uint8_t[]> pass_data){
	uint16_t data_len = 0;
	uint16_t resp_len = 0;
	uint16_t op = L1Commands::OptionsPasswordManager::SE3_SEPASS_OP_ADD;
	uint16_t offset = L1Request::Offset::DATA;
	this->base.FillSessionBuffer((unsigned char*)&op, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_id, offset, 4);
	offset += 4;
	this->base.FillSessionBuffer((unsigned char*)&host_len, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&user_len, offset, 2);
	offset += 2;
	this->base.FillSessionBuffer((unsigned char*)&pass_len, offset, 2);
	offset += 2;
	if(host_data != nullptr && user_data != nullptr && pass_data != nullptr){ // this is in case the host wants to explicitly send the key content to the SEcube
		this->base.FillSessionBuffer((unsigned char*)host_data.get(), offset, host_len);
		offset += host_len;
		this->base.FillSessionBuffer((unsigned char*)user_data.get(), offset, user_len);
		offset += user_len;
		this->base.FillSessionBuffer((unsigned char*)pass_data.get(), offset, pass_len);
		offset += pass_len;
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

void L1::L1SEGetAllPasswords(std::vector<se3Pass>& passList)
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
	data_len = offset - L1Request::Offset::DATA;
	try{
		TXRXData(L1Commands::Codes::SEPASS, data_len, 0, &resp_len);
	} catch(L1Exception& e){
		passList.clear();
		throw passListExc;
	}
	// copy response to local buffer
	memset(buffer.get(), 0, L1Response::Size::MAX_DATA);
	memcpy(buffer.get(), (this->base.GetSessionBuffer()+L1Request::Offset::DATA), resp_len);
	if(resp_len == 0){ // if the response is empty, the SEcube has returned all the IDs in its flash memory
		return;
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
			return; // when the SEcube reaches the end of the flash (all keys returned) it sends 0, so we have our condition to terminate
		}
		se3Pass pass;
		pass.id = passid;
		pass.hostSize = host_len;
		pass.userSize = user_len;
		pass.passSize = pass_len;
		passList.push_back(pass); // copy ID in list
	}
}

bool L1::L1SEpass_Info(string& id, string& name, uint8_t mode)
{

	return true;
}

bool L1::L1SEpass_GetKeyEnc(uint32_t key_export_id, uint32_t key_wrapping_key, std::shared_ptr<uint8_t[]>& key_export_data, uint16_t& key_export_len){

	return true;
}

