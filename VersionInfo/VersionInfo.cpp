#include "VersionInfo.h"

bool VersionInfo::operator>(VersionInfo & value)
{
   return false;
}

int VersionInfo::Compare(int maj, int min, int rev, int bld)
{
   return 0;
}

void VersionInfo::SetValue(std::wstring version)
{
   major = 0;
   minor = 1;
   revision = 2;
   build = 3;
}

void VersionInfo::SetOSVersion()
{
}

void VersionInfo::SetFileVersion(std::wstring filePath)
{
}
