#include "VersionInfo.h"
#include <sstream>

//inline bool VersionInfo::operator>(const VersionInfo& rhs)
//{
//   if (compare(*this, rhs) > 0)
//      return true;
//
//   return false;
//}

int VersionInfo::Compare(int maj, int min, int rev, int bld)
{
   VersionInfo rhs(maj, min, bld, rev);
   return compare(*this, rhs);
}

void VersionInfo::SetValue(const std::wstring version)
{
   if (version.length() > 0)
   {
      std::wistringstream istr(version);
      istr >> major;
      wchar_t sep = istr.get();

      if (istr.fail() || istr.eof()) return;
      istr >> minor;
      sep = istr.get();

      if (istr.fail() || istr.eof()) return;
      istr >> build;
      sep = istr.get();

      if (istr.fail() || istr.eof()) return;
      istr >> revision;
   }
}

std::wstring VersionInfo::GetValue(wchar_t sep)
{
   std::wostringstream ostr;
   ostr << major << sep << minor << sep << build << sep << revision;
   return ostr.str();
}

void VersionInfo::SetFileVersion(const std::wstring filePath)
{
   // read file version
   std::wstring fileVer;

   // add
   SetValue(fileVer);
}

inline int VersionInfo::compare(const VersionInfo & lhs, const VersionInfo & rhs)
{
   if (lhs.major > rhs.major)
      return 1;
   else if (lhs.major == rhs.major) {
      if (lhs.minor > rhs.minor)
         return 1;
      else if (lhs.minor == rhs.minor) {
         if (lhs.build > rhs.build)
            return 1;
         else if (lhs.build == rhs.build) {
            if (lhs.revision > rhs.revision)
               return 1;
            else if (lhs.revision == rhs.revision)
               return 0;
         }
      }
   }

   return -1;
}
