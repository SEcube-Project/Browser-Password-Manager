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
