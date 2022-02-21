# Fortigate Authenticate
Command line tool to authenticate fortigate, Written in C++

## Dependency
If you are compiling from source, then you need `libcurl`.      
Install using
```sh
sudo apt-get install libcurl4-openssl-dev
```

## Source Installation
```sh
git clone https://github.com/ms-jagadeeshan/fortigate-authenticate.git
cd fortigate-authenticate/src
g++ fauth.cpp util.cpp -o ../bin/fauth -lcurl
```
This will clone and compile the program.
Binary program is stored as bin/fauth

## Usage
```sh
# ./fauth username:password
$ ./fauth ma30c203:1234
# or to securely enter password, don't pass any argument, program will ask for username and password
$ ./fauth
> Enter username:
> Enter password: 
```
