#pragma once

#include <string>
#include <cstdint>

struct Title
{
    std::string name;
    std::string rightsID;
    uint64_t titleKeyUpper;
    uint64_t titleKeyLower;
    uint64_t titleID;
    uint8_t mKey;
    uint64_t size;
    uint32_t releaseDate;
    uint32_t addedDate;
    std::string size_string;
};

enum class SortOrder
    : uint32_t
{
    NAME_ASC = 0,
    NAME_DEC,
    SIZE_ASC,
    SIZE_DEC,
    ADDED_DATE_ASC,
    ADDED_DATE_DEC,
    RELEASE_DATE_ASC,
    RELEASE_DATE_DEC
};

inline const char* getSort(SortOrder so)
{
    switch(so)
    {
        case SortOrder::NAME_ASC: return "Name (asc)";
        case SortOrder::NAME_DEC: return "Name (dec)";
        case SortOrder::SIZE_ASC: return "Size (asc)";
        case SortOrder::SIZE_DEC: return "Size (dec)";
        case SortOrder::RELEASE_DATE_ASC: return "Release Date (asc)";
        case SortOrder::RELEASE_DATE_DEC: return "Release Date (dec)";
        case SortOrder::ADDED_DATE_ASC: return "Added Date (asc)";
        case SortOrder::ADDED_DATE_DEC: return "Added Date (dec)";
    }
    return "Name (asc)";
}

bool loadTitles(void);
bool loadInfo(void);
bool getUpdateList(void);
bool getUpdateInfo(void);
bool sorter(Title lhs, Title rhs);