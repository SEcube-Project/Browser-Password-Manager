# Browser Password Manager

## Host specs
- We want to use the pin as master password for unlocking the extension, this implies that at each command sent from the extension, the pin must be sent
- The master password corresponds to the user pin in the FW
- The admin in the FW can reset the user password in case of loss; the admin pin must be managed only by the cybersecurity team of the company. Is it ok to suppose this?
- The API rest must be HTTPS? Yes,no problem for sending pin at each command
- Everytime the logout is automatically done at each tab change + timeout + esplicit user request

## Extension specs 
- Master password at the beginning ( the same wtr the user pin of the FW )
- Logout

### Generate a new password
- Select number of characters
- Enable/disable spec characters, upper/lower case, numbers
- Copy password

### Manage saved password
- Modify the password with a new generated one (random generated at the moment or inserted by user)
- List with all the saved pass (icon + hostname + (username + password (hidden by default but visible if 'eye button' is clicked)))
- Add new password
- Delete

### Import/Export
- Exported file encrypted with password

### Enable/Disable automatic fill
- Autocomplete if possible? Only if specified by user
- Always list the corresponding password in the extension tab (like BitWarden)


## Host Endpoint for Extension (REST API)
- addPassword(hostname, username, password)
- addPassword(hostname, password) ????
- deletePassword(hostname, username)
- deletePassword(id)
- generateRandomPassword(n_char, special_char, ....)
- getPassword(hostname)
- getAllPassword() // return only the hostname + username if present. Passwords are not returned.
- exportEncryptedFile(file, pass)
- importEncryptedFile(file, pass)

Update is done by deleting and re-adding. Master password sent at every REST call.

## FW endpoint for Host application
- exportEncryptedPassword(pass) : byte[]
- importEncryptedPassword(pass) : byte[]
- addPassword(hostname, username, password)
- addPassword(hostname, password) ????
- deletePassword(hostname, username)
- deletePassword(id)
- generateRandomPassword(n_char, special_char, ....)
- getPassword(hostname)
- getAllPassword() // return only the hostname + username if present. Passwords are not returned.

+ Flash management

### REST APIs details

- Retrieval of all the connected devices
```
GET /api/v0/devices
```
```json
{
  "count": 1,
  "devices": [
    {
      "index": 0,
      "path": "/media/gabriele97/9016-4EF8/",
      "serial": "SEcube12340000000000000000000000"
    }
  ]
}
```

- Retrieval of all stored passwords
```
GET /api/v0/device/<device_id>/passwords?pin=<device_pin>
```
```json
{
  "count": 6,
  "passwords": [
    {
      "id": 1,
      "hostname": "test.com",
      "username": "test",
      "password": "1234"
    },
    {
      "id": 2,
      "hostname": "test.com",
      "username": "test",
      "password": "1234"
    },
    {
      "id": 3,
      "hostname": "test.com",
      "username": "test",
      "password": "1234"
    },
    {
      "id": 4,
      "hostname": "test.com",
      "username": "test",
      "password": "1234"
    },
    {
      "id": 5,
      "hostname": "facebook.com",
      "username": "2",
      "password": "marco"
    },
    {
      "id": 6,
      "hostname": "facebook.com",
      "username": "zucca",
      "password": "marco"
    }
  ]
}
```

- Retrieval of all stored passwords with **hostname filter**
```
GET /api/v0/device/<device_id>/passwords?pin=<device_pin>&hostname=<filter>
GET /api/v0/device/<device_id>/passwords?pin=<device_pin>&hostname=fac
```
```json
{
  "count": 2,
  "passwords": [
    {
      "id": 5,
      "hostname": "facebook.com",
      "username": "2",
      "password": "marco"
    },
    {
      "id": 6,
      "hostname": "facebook.com",
      "username": "zucca",
      "password": "marco"
    }
  ]
}
```

- Insertion of a new password
```
POST /api/v0/device/<device_id>/passwords?pin=<device_pin>
```

*BODY REQUEST:*
```json
{
  "hostname": "facebook.com",
  "username": "zucca",
  "password": "marco"
}
```

- Details of a single password 
```
GET /api/v0/device/<device_id>/password/<password_id>?pin=<device_pin>
```

```json
{
  "id": 3,
  "hostname": "test.com",
  "username": "test",
  "password": "1234"
}
```

- Update of a single password 
```
PUT /api/v0/device/<device_id>/password/<password_id>?pin=<device_pin>
```

*BODY REQUEST:*
```json
{
  "id": 3,
  "hostname": "test.com",
  "username": "test",
  "password": "1234"
}
```

- Removal of a single password 
```
DELETE /api/v0/device/<device_id>/password/<password_id>?pin=<device_pin>
```

```json
{
  "success": true
}
```

- Password Generation

```
GET /api/v0/device/<device_id>/generate?pin=<device_pin>&upper=[0,1]&special=[0,1]&numbers=[0,1]&length=<password_length>
```

```json
{
  "generated": "m2hWwQBdyODd?tnWgPq@fFO8McmbB:;g6ViOC9oosSZ5AamhnTghjVmd$aBRnvOx"
}
```


# Work distribution
Matteo: FW
Gabriele: Host
Giovanni: Chrome ext
