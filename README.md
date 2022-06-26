# Browser Password Manager

Passwords are a key part of everyday life and to be secure they must be complex. But this complexity is in conflict with the ability to remember them. Another problem arises if we think about a secure place where to store our passwords. Is a server safe enough? Better a text file? What about a database?
The best solution to this critical problem is for sure a password manager. A physical password manager is even better! This is the motivation that led to the creation of this project.

The main focus is security, followed by user experience (exploiting UX and UI). The project is aimed at the development of a password manager that can be used by any user, regardless of his or her experience with the software. The development is done from scratch, starting from the firmware, going through the host middleware and finally to the extension

## Implementation overview

The goal is to develop a secure Password Manager, where passwords are stored in a secure and removable USB device, that is based on SECube. Different challenges need to be solved in order to achieve this goal: from the SECube's firmware to the development of a Chromium Extension. 

The main problem is the communication between the Chromium Browser and the SECube: due to the high level of restrictions imposed by any modern web browser, indeed they act as a big and complex sandboxes for good reasons, it's nearly impossible to have a direct connection between the Extension and any physical device connected to the Host PC. 

Because of this, a third actor needs to be introduced: a middleware that is capable of being both interfaced with any (Chromium-based) browser and able to communicate with the SECube device. Thus, the middleware is a third software that is intended to be installed and run on the host PC continuously (like a service on Windows or a daemon on Linux, or executed by the user when he needs it, as he prefers).



Due to all these actors, developing a secure Password Manager becomes very complicated. It is very important, in cases like this, to be pretty organized and try to be systematic in developing all the needed software, without trying to reinvent the wheel. 

The three components have been developed completely independent from others one, trying to move all the possible logic and sensitive data to the lowest level (i.e. the Host Middleware and the SECube): this is due to the fact that these two actors are the most secure while the Extension is the most exposed one to possible threats. 

The communication between the Extension and the Middleware happens by means of HTTPs: it is a secure version of HTTP based on TLS, allowing to have a complete communication between the two parties. The communication between the Middleware and the SECube happens by means of a USB connection, and the communication is encrypted. 



