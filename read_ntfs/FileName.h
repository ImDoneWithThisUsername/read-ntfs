#pragma once
#include "Attribute.h"
class FileName :
    public Attribute
{
public:
    int file_data_offset;//attribute data offset of file name, from start
                         //of file name
    int length_of_name;
    string name_of_file;
};

