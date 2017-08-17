#pragma once

#include <string>

// major.minor[.build[.revision]]
// missing version components default to zero.
// TODO: define behavior for negative numbers.
class VersionInfo {
public:
   VersionInfo() :
      major(0), minor(0), build(0), revision(0) { };
   VersionInfo(const std::wstring version) { SetValue(version);  };
   VersionInfo(int maj, int min, int bld = 0, int rev = 0) :
      major(maj), minor(min), build(bld), revision(rev) { };
   ~VersionInfo() { };

   inline bool operator>(const VersionInfo& rhs) { return compare(*this, rhs) > 0 ? true : false; }
   inline bool operator<(const VersionInfo& rhs) { return compare(*this, rhs) < 0 ? true : false; }
   inline bool operator==(const VersionInfo& rhs) { return compare(*this, rhs) == 0 ? true : false; }

   // >0 for passed version smaller, <0 for passed version greater
   int Compare(int maj, int min, int rev, int bld);

   void SetValue(const std::wstring version);
   std::wstring GetValue(wchar_t sep = L'.');

   // Read the passed file version
   void SetFileVersion(const std::wstring filePath);

   int major;
   int minor;
   int build;
   int revision;
private:
   inline int compare(const VersionInfo& lhs, const VersionInfo& rhs);
};
