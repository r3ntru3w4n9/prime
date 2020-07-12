/***********************************************************************

  FileName    [mmapstream.h]

  Author      [Kan-Hsuan, Lai]

  This file difines the file reader based on mmap.

***********************************************************************/

#pragma once

#include <string>

class mstream
{
public:
    mstream(const char* filepath);

    mstream& operator>>(std::string &dest);
    mstream& operator>>(unsigned &dest);
    mstream& operator>>(int &dest);

    bool is_open() const;

private:
    char* _content;
    unsigned int _current;
    unsigned int _max_size;
    bool _is_opened;
};