#include "Framework.h"
#include "BinaryReader.h"

BinaryReader::BinaryReader(string filePath)
{
    file = CreateFileA(filePath.c_str(), GENERIC_READ, 
        FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryReader::BinaryReader(wstring filePath)
{
    file = CreateFile(filePath.c_str(), GENERIC_READ, 
        FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryReader::~BinaryReader()
{
    CloseHandle(file);
}

int BinaryReader::Int()
{
    int tmp;
    ReadFile(file, &tmp, sizeof(int), &size, nullptr);
    return tmp;
}

UINT BinaryReader::UInt()
{
    UINT tmp;
    ReadFile(file, &tmp, sizeof(UINT), &size, nullptr);
    return tmp;
}

float BinaryReader::Float()
{
    float tmp;
    ReadFile(file, &tmp, sizeof(float), &size, nullptr);
    return tmp;
}

string BinaryReader::String()
{
    UINT strSize = UInt();

    char* tmp = new char[strSize + 1];
    ReadFile(file, tmp, sizeof(char) * strSize, &size, nullptr);
    tmp[strSize] = '\0';

    return tmp;
}

wstring BinaryReader::WString()
{
    UINT strSize = UInt();

    WCHAR* tmp = new WCHAR[strSize + 1];
    ReadFile(file, tmp, sizeof(WCHAR) * strSize, &size, nullptr);
    tmp[strSize] = '\0';

    return tmp;
}

void BinaryReader::Byte(void** data, UINT dataSize)
{
    ReadFile(file, &data, dataSize, &size, nullptr);
}
