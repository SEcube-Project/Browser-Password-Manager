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

using namespace std;

/* WARNING: this example will write and delete keys in the internal memory of the SEcube. The internal memory will be scanned for
 * existing keys, those keys won't be affected by any change. */

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
	cout << "\nEnter the number corresponding to the SEcube device that you want to use..." << endl;
	/* warning: if cin does not wait for input in debug mode with eclipse, open the launch configuration and select
	 * the "use external console for inferior" checkbox under the debugger tab (see https://stackoverflow.com/questions/44283534/c-debug-mode-in-eclipse-causes-program-to-not-wait-for-cin)*/
	if(!(cin >> sel)){
		cout << "Input error...quit." << endl;
		return -1;
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

		// preliminary step: save list of existing keys
		vector<pair<uint32_t, uint16_t>> preexistingkeys;
		vector<pair<uint32_t, uint16_t>> keys;
		try{
			l1->L1KeyList(preexistingkeys);
		} catch (...) {
			cout << "Unexpected error...quit." << endl;
			l1->L1Logout();
			return -1;
		}

		cout << "\nLet us start listing the keys currently stored inside the SEcube device..." << endl;
		if(preexistingkeys.size() == 0){
			cout << "\nThere are no keys currently stored inside the SEcube device." << endl;
		} else {
			int cnt = 0;
			for(pair<uint32_t, uint16_t> k : preexistingkeys){
				cout << cnt << ") Key ID " << k.first << " - length: " << 8*k.second << " bit" << endl;
				cnt++;
			}
		}

		cout << "\nNow we're gonna create 3 new keys inside the memory of the SEcube." << endl;
		cout << "The keys will have sizes of 128, 192, and 256 bit respectively." << endl;
		cout << "We will provide the key value only for the first one (128 bit), the other two will be generated automatically inside the SEcube with the embedded TRNG." << endl;
		cout << "The identifiers of the keys, which are numbers of 32 bits, will be generated randomly." << endl;

		// flash first key (128 bit) to the device, here we provide explicitly also the value of the key
		shared_ptr<uint8_t[]> hostVal = make_unique<uint8_t[]>(3);
		shared_ptr<uint8_t[]> passVal = make_unique<uint8_t[]>(2);
		shared_ptr<uint8_t[]> userVal = make_unique<uint8_t[]>(2);

		hostVal[0] = 'a';
		hostVal[1] = 'b';
		hostVal[2] = 'c';
		passVal[0] = 'd';
		passVal[1] = 'e';
		userVal[0] = 'f';
		userVal[1] = 'g';

		if(l1->L1SEpass_InsertKey(10, 3, 2, 2, hostVal, userVal, passVal)){
			printf("OK!\n");
		}
	}

	return 0;
}
