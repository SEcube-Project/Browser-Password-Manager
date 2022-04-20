/**
  ******************************************************************************
  * File Name          : manual_key_management.cpp
  * Description        : Usage example of manual key management with L1 APIs.
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

/*! \file  manual_keys.cpp
 *  \brief This file is an example about how to manually handle the crypto keys stored inside the SEcube.
 *  \date 16/12/2020
 *  \version SEcube SDK 1.5.1
 *  This example is useful for those who do not want to use the SEkey key management system.
 */

#include "../sources/L1/L1.h"
#include <thread> 		// thread::sleep_for
#include <algorithm> 	// std::find
#include <string>

using namespace std;

/* WARNING: this example will write and delete keys in the internal memory of the SEcube. The internal memory will be scanned for
 * existing keys, those keys won't be affected by any change. */

int fillByteArray(std::string s, shared_ptr<uint8_t[]> val){
	int cnt = 0;
	for (char c : s){
		val[cnt++] = c;
	}
	return cnt;
}

void printArray(uint8_t* string, int len){
	for(int i=0; i <len; i++){
		printf("%c", string[i]);
	}
	printf("\n");
}

// RENAME THIS TO main()
int main(){
	/* we recommend using smart pointers to manage L0 and L1 objects in order
	   to ensure proper memory management by their constructors and destructors. */
	unique_ptr<L0> l0 = make_unique<L0>();
	unique_ptr<L1> l1 = make_unique<L1>();

	cout << "This example shows how to manually use cryptographic keys with the SEcube." << endl;
	this_thread::sleep_for(chrono::milliseconds(1000)); // these delays are simply here to slow down the process making it more comprehensible
	cout << "Looking for SEcube devices...\n" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000));

	int numdevices = l0->GetNumberDevices(); // this API checks how many SEcube devices are connected to the PC
	if(numdevices == 0){
		cout << "No SEcube devices found! Quit." << endl;
		return 0;
	}
	vector<pair<string, string>> devices;
	int ret = l0->GetDeviceList(devices); // this API fills the vector with pairs including details about the devices (path and serial number)
	if(ret){
		cout << "Error while searching for SEcube devices! Quit." << endl;
		return -1;
	}
	cout << "Number of SEcube devices found: " << numdevices << endl;
	cout << "List of SEcube devices (path, serial number):" << endl;
	int index = 0;
	uint8_t empty_serial_number[L0Communication::Size::SERIAL];
	memset(empty_serial_number, 0, L0Communication::Size::SERIAL);
	for(pair<string, string> p : devices){
		if(p.second.empty() || memcmp(p.second.data(), empty_serial_number, L0Communication::Size::SERIAL)==0){
			cout << index << ") " << p.first << " - serial number not available (please initialize this SEcube)" << endl;
		} else {
			cout << index << ") " << p.first << " - " << p.second << endl;
		}
		index++;
	}
	int sel = 0;
	if (numdevices > 1){
		cout << "\nEnter the number corresponding to the SEcube device that you want to use..." << endl;
		/* warning: if cin does not wait for input in debug mode with eclipse, open the launch configuration and select
		 * the "use external console for inferior" checkbox under the debugger tab (see https://stackoverflow.com/questions/44283534/c-debug-mode-in-eclipse-causes-program-to-not-wait-for-cin)*/
		if(!(cin >> sel)){
			cout << "Input error...quit." << endl;
			return -1;
		}
	} else {
		sel = 0;
	}

	if((sel >= 0) && (sel < numdevices)){
		array<uint8_t, L0Communication::Size::SERIAL> sn;
		sn.fill(0);
		if(devices.at(sel).second.length() > L0Communication::Size::SERIAL){
			cout << "Unexpected error...quit." << endl;
			return -1;
		} else {
			memcpy(sn.data(), devices.at(sel).second.data(), devices.at(sel).second.length());
		}
		l1->L1SelectSEcube(sn); // select secube with correct serial number
		cout << "\nDevice " << devices.at(sel).first << " - " << devices.at(sel).second << " selected." << endl;

		array<uint8_t, 32> pin = {'t','e','s','t'}; // customize this PIN according to the PIN that you set on your SEcube device
		l1->L1Login(pin, SE3_ACCESS_USER, true); // login to the SEcube

		shared_ptr<uint8_t[]> hostVal = make_unique<uint8_t[]>(100);
		shared_ptr<uint8_t[]> passVal = make_unique<uint8_t[]>(100);
		shared_ptr<uint8_t[]> userVal = make_unique<uint8_t[]>(100);





		// ############## PASS CREATION ##############
		// Pass 1
		uint32_t id1 = 0;
		uint32_t id2 = 0;
		uint32_t id3 = 0;
		uint16_t hostSize = fillByteArray("youtube.com", hostVal);
		uint16_t userSize = fillByteArray("mat@gmail.com", userVal);
		uint16_t passSize = fillByteArray("pass10sZero", passVal);
		if(l1->L1SEAddPassword(hostSize, userSize, passSize, hostVal, userVal, passVal, id1)){
			printf("Added new pass with id %d!\n", id1);
		} else {
			printf("Unable to add!\n");
		}

		// Pass 2
		hostSize = fillByteArray("gmail.com", hostVal);
		userSize = fillByteArray("mat@gmail.com", userVal);
		passSize = fillByteArray("qwertyuiop", passVal);
		if(l1->L1SEAddPassword(hostSize, userSize, passSize, hostVal, userVal, passVal, id2)){
			printf("Added new pass with id %d!\n", id2);
		} else {
			printf("Unable to add!\n");
		}

		// Pass 3
		hostSize = fillByteArray("youporn.com", hostVal);
		userSize = fillByteArray("xxx@gmail.com", userVal);
		passSize = fillByteArray("xxXxxa33dss", passVal);
		if(l1->L1SEAddPassword(hostSize, userSize, passSize, hostVal, userVal, passVal, id3)){
			printf("Added new pass with id %d!\n", id3);
		} else {
			printf("Unable to add!\n");
		}


		// ############## SEARCH BY ID ##############
		se3Pass searchedById;
		if(l1->L1SEGetPasswordById(id2, searchedById)){
			printf("\n\nFound pass with id %d\n", id2);
			printf("Element Id:\t\t%d\n", searchedById.id);
			printf("Element Hostname:\t");
			printArray(searchedById.host, searchedById.hostSize);
			printf("Element Username:\t");
			printArray(searchedById.user, searchedById.userSize);
			printf("Element Password:\t");
			printArray(searchedById.pass, searchedById.passSize);
			printf("\n");
		} else {
			printf("Unable to find pass by id\n");
		}


		// ############## LIST ALL ##############
		std::vector<se3Pass> passList;
		l1->L1SEGetAllPasswords(passList);
		printf("\n\nComplete pass list\n");
		for(se3Pass elem : passList){
			printf("Element Id:\t\t%d\n", elem.id);
			printf("Element Hostname:\t");
			printArray(elem.host, elem.hostSize);
			printf("Element Username:\t");
			printArray(elem.user, elem.userSize);
			printf("Element Password:\t");
			printArray(elem.pass, elem.passSize);
			printf("\n");
		}

		// ############## FILTER BY HOSTNAME ##############
		printf("\n\nPasswords with youtube.com as hostname\n");
		passList.clear();
		shared_ptr<uint8_t[]> hostFilter = make_unique<uint8_t[]>(100);
		userSize = fillByteArray("youtube.com", hostFilter);
		l1->L1SEGetAllPasswordsByHostName(passList, hostFilter, userSize);
		for(se3Pass elem : passList){
			printf("Element Id:\t\t%d\n", elem.id);
			printf("Element Hostname:\t");
			printArray(elem.host, elem.hostSize);
			printf("Element Username:\t");
			printArray(elem.user, elem.userSize);
			printf("Element Password:\t");
			printArray(elem.pass, elem.passSize);
			printf("\n");
		}


		// ############## DELETE ALL PASSWORD ##############
		l1->L1SEGetAllPasswords(passList);
		for(se3Pass elem : passList){
			if(l1->L1SEDeletePassword(elem.id)){
				printf("Deleted pass %d: \n", elem.id);
			} else {
				printf("Unable to delete pass %d: \n", elem.id);
			}
		}


		// ############## GENERATE RANDOM PASS ##############
		shared_ptr<uint8_t[]> pass = make_unique<uint8_t[]>(100);
		if(l1->L1SEGenerateRandomPassword(100, true, true, true, pass)){
			printf("\n\nNew generated password: ");
			printArray(pass.get(),  100);
		} else {
			printf("\n\nUnable to generated password");
		}

	}
	return 0;
}
