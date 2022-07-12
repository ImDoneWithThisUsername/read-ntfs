#pragma once
#include "StandardInfomation.h"
#include "FileName.h"
#include "Data.h"
#include "Header.h"
#include "function.h"
#include "ObjID.h"

class MFT
{
public:
	Header header;
	StandardInfomation standard_infomation;
	FileName file_name;
	ObjID object_id;
	Data data;
	BYTE sector1[512], sector2[512];
	bool isDir = false;
	bool isSystem = false;
	bool isMFT = true;
	bool isResident = true;
	bool isArchieve = false;
	int offset;
	struct LCN_VCN
	{
		vector<vector<int>> lcn_vcn_list;
	};

	MFT(BYTE iSector1[512], BYTE iSector2[512], int iOffset)
	{
		offset = iOffset;
		for (int i = 0; i < 512; i++)
		{
			sector1[i] = iSector1[i];
			sector2[i] = iSector2[i];
		}
		//read header flag
		string byte_value = read_byte(sector1, header.flag_offset, 2);
		if (byte_value == "0300")
			this->isDir = true;

		//read offset to the first attribute byte value (standard infomation)
		standard_infomation.offset = read_little_eldian_to_dec(sector1, header.first_altribute_offset, 2);

		//read length of standard infomartion
		standard_infomation.length = read_little_eldian_to_dec(sector1, standard_infomation.offset + 4, 4);

		//read flag in standard infomation
		//read attribute offset (count from start of standard infomation)
		int attribute_offset = read_little_eldian_to_dec(sector1, standard_infomation.offset + 20, 2);
		//read attribute offset (count from start of mft)
		attribute_offset = attribute_offset + standard_infomation.offset;

		//read flag value
		byte_value = read_byte(sector1, attribute_offset + hex_to_dec("20"), 4);
		standard_infomation.flags = to_big_eldian(byte_value);
		string binary_value = hex_to_binary(standard_infomation.flags);
		//if bit[2] == 1 -> system file
		if (binary_value[31 - 2] == '1') 
			this->isSystem = true;
		if (binary_value[31 - 5] == '1')
			this->isArchieve = true;
		for (int i = 0; i < 32; i++)
		{
			if (binary_value[i] == '1')
				break;
			if (i == 31 && binary_value[i] == '0')
				isMFT = false;
		}
		//if all bit == 0 -> not an mft file

		//offset to file name(standard infomation offset + standard infomation length)
		file_name.offset = standard_infomation.offset + standard_infomation.length;
		//read file name length
		file_name.length = read_little_eldian_to_dec(sector1, file_name.offset + 4, 4);

		//read offset of file name attribute
		file_name.file_data_offset = read_little_eldian_to_dec(sector1, file_name.offset + 20, 2);

		//read name length
		byte_value = read_byte(sector1, file_name.file_data_offset + file_name.offset + 64, 1);
		file_name.length_of_name = litte_eldian_to_dec(byte_value);
		//pass 1 byte namespace
		file_name.name_of_file = read_name(sector1, file_name.file_data_offset + file_name.offset + 66,
			file_name.length_of_name);

		//obj id offset
		object_id.offset = file_name.offset + file_name.length;
		//obj id length
		object_id.length = read_little_eldian_to_dec(sector1, object_id.offset + 4, 4);
		//data attribute offset
		data.offset = object_id.offset + object_id.length;
		//data attribute length
		data.length = read_little_eldian_to_dec(sector1, data.offset + 4, 4);
		//resident or non-resident byte
		byte_value = read_byte(sector1, data.offset + hex_to_dec("08"), 1);
		if (byte_value == "01")
		{
			this->isResident = false;
			data.content_size = 0;
			byte_value = read_byte(sector1, data.offset + hex_to_dec("0x40"), 1);
			//lcn number
			int number_of_byte_lcn = byte_value[0] - '0';
			//byte cluster
			int number_of_byte_cluster = byte_value[1] - '0';
		}
		else
		{
			//read data content size
			data.content_size = read_little_eldian_to_dec(sector1, data.offset + hex_to_dec("10"), 4);
			//read data content offset
			data.content_offset = read_little_eldian_to_dec(sector1, data.offset + hex_to_dec("14"), 2);
		}

	};

	void print_data()
	{
		if (this->isResident && is_text_file(this->file_name.name_of_file))
		{
			string content = "";
			if (data.content_size == 0)
				return;
			if (data.content_offset + data.offset + data.content_size < 512)
			{
				content = read_char(sector1, data.content_offset + data.offset, data.content_size);
			}
			else if(data.length > 0)
			{
				int continue_data_length = data.offset + data.length - 512;
				content = read_char(sector1, data.content_offset + data.offset, 
					data.content_size - continue_data_length);
				content += read_char(sector2, 0, continue_data_length);
			}
			cout << file_name.name_of_file << endl;
			cout << content << endl;
		}
	};

	void out_put()
	{
		string flag_mean = "";
		flag_mean += isResident ?  "Resident" : "Non-resident";
		if (isDir) flag_mean += ", Directory ";
		if (isSystem) flag_mean += ", System";
		if (isArchieve) flag_mean += ", Archieve";
		cout << "MFT entry offset:\t\t " << dec_to_hex(this->offset) << endl;
		cout << "File typy:\t\t\t " << flag_mean << endl;
		cout << "Header offset:\t\t\t " << this->header.offset << endl;
		cout << "First altribute offset:\t\t " << this->header.first_altribute_offset << endl;
		cout << "Flag offset:\t\t\t " << this->header.flag_offset << endl;
		cout << "Standard infomation offset:\t " << this->standard_infomation.offset << endl;
		cout << "Standard infomation length:\t " << this->standard_infomation.length << endl;
		cout << "Standard infomation flag:\t " << this->standard_infomation.flags << endl;
		cout << "File name offset:\t\t " << this->file_name.offset << endl;
		cout << "File name length:\t\t " << this->file_name.length << endl;
		cout << "File name data offset:\t\t " << this->file_name.file_data_offset << endl;
		cout << "File name data length:\t\t " << this->file_name.length_of_name << endl;
		cout << "Name of this file:\t\t " << this->file_name.name_of_file << endl;
		cout << "Object ID offset:\t\t " << this->object_id.offset << endl;
		cout << "Object ID length:\t\t " << this->object_id.length << endl;
		cout << "Data offset:\t\t\t " << this->data.offset << endl;
		cout << "Data length:\t\t\t " << this->data.length << endl;
		cout << "Data content offset:\t\t " << this->data.content_offset << endl;
		cout << "Data content length:\t\t " << this->data.content_size << endl;
		this->print_data();
	}
};

