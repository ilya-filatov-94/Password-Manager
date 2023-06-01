# Password Manager - Desktop application for Windows

## Main objectives of the project
1. Password manager for Windows, written using the Qt framework.
2. Encrypting database with the AES (aka Rijndael) encryption algorithm using a 256 bit key.
3. Using local database saved on USB flash drive.
4. Two factor authentication (2FA) using SMTP server with TLS connection.
5. Generating passwords.
6. Checking the master password in the database of passwords that have been compromised or "pwned" in a data breach.
7. Automatic verification of the strength of the passwords.
8. The integrated search function allows to search by name of resouce the complete database.
9. The function of copy logins and passwords to the clipboard without entering into the Windows clipboard history (storage for a limited amount of time).
10. Checking for an Internet connection for Windows for work smtp-server.
11. Changing background for tabs using dragging and dropping. 


## DEMO
[video description](https://github.com/ilya-filatov-94/PasswordManager/blob/main/video_description_of_app.mp4)
![DEMO of application](./video_description_of_app_gif.gif "DEMO of application")


## Technologies used
Qt5 framework, multithreading, SQLITE, [library for SMTP server](https://github.com/bluetiger9/SmtpClient-for-Qt), [database of  check passwords](https://haveibeenpwned.com), [the encryption library Qt-Secret](https://github.com/QuasarApp/Qt-Secret)

