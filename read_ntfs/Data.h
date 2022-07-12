#pragma once
#include "Attribute.h"
class Data :
    public Attribute
{
public:
    int content_offset; // from data attribute
    int content_size;
};

