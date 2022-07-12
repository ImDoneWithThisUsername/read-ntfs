#pragma once
#include<iostream>
using namespace std;
class Header
{
public:
	int offset = hex_to_dec("0x00");
	int first_altribute_offset = hex_to_dec("0x14");
	int flag_offset = hex_to_dec("0x16");//2 bit
	Header() {};
};

