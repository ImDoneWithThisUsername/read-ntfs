#include "function.h"
#include "Struct_MFT.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        //printf("Success!\n");
    }
}

int hex_to_dec(string hex_num)
{
    stringstream ss;
    int dec;
    ss << hex_num;
    ss >> hex >> dec;
    return dec;
}

string dec_to_hex(int dec_num)
{
    stringstream ss;
    ss << hex << dec_num;
    return ss.str();
}

string read_byte(BYTE sector[512], int byte_offset, int byte_len)
{
    stringstream res;
    res << hex;
    for (int i = byte_offset; i < byte_offset + byte_len; ++i)
    {
        res << setw(2) << setfill('0') << (int)sector[i];
    }
    return res.str();
}

string to_big_eldian(string s)
{
    string res = "";
    for (int i = s.length() - 1; i >= 0; i -= 2)
    {
        res += s[i - 1];
        res += s[i];
    }
    return res;
}

string read_char(BYTE sector[512], int byte_offset, int byte_len)
{
    string res;
    for (int i = byte_offset; i < byte_offset + byte_len; ++i)
    {
        res += sector[i];
    }
    return res;
}

int litte_eldian_to_dec(string byte_string)
{
    return hex_to_dec(to_big_eldian(byte_string));
}

string read_name(BYTE sector[512], int file_name_offset, int file_name_length)
{
    // get unicode name
    string name = "";
    for (int i = file_name_offset; i < file_name_offset + file_name_length * 2; i++)
    {
        name += sector[i];
    }
    return name;
}

string hex_to_binary(string hex_num)
{
    stringstream ss;
    ss << hex << hex_num;
    unsigned n;
    ss >> n;
    bitset<32> b(n);
    return b.to_string();
}

bool is_text_file(string file_name)
{
    if (file_name.length() <= 4)
        return false;
    string s = "";
    for (int i = file_name.length() - 4; i < file_name.length(); i++)
    {
        s += file_name[i];
    }
    return true;
}

int read_little_eldian_to_dec(BYTE sector[512], int offset, int length)
{
    string byte_value = read_byte(sector, offset, 2);
    return litte_eldian_to_dec(byte_value);
}