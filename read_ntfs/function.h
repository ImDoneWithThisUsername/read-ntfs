#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bitset>

using namespace std;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);

//convert from hexcimal to decimal
int hex_to_dec(string hex_num);

//convert from decimal to heximal
string dec_to_hex(int dec_num);

//read byte_len bytes from byte_offset in sector
string read_byte(BYTE sector[512], int byte_offset, int byte_len);

//convert from little eldian to big eldian
string to_big_eldian(string s);

//read ascii charater
string read_char(BYTE sector[512], int byte_offset, int byte_len);

//convert from little eldian to dec
int litte_eldian_to_dec(string byte_string);

//read mft name
string read_name(BYTE sector[512], int file_name_offset, int file_name_length);

//convert from hexcimal to binary
string hex_to_binary(string hex_num);

//check if file name is a text file
bool is_text_file(string file_name);

//read hex byte and convert to decimal
int read_little_eldian_to_dec(BYTE sector[512], int offset, int length);