#pragma once

#include <string>

// major.minor[.build[.revision]]
class VersionInfo {
public:
   VersionInfo() :
      major(0), minor(0), build(0), revision(0) { };
   VersionInfo(std::wstring version) { SetValue(version);  };
   VersionInfo(int maj, int min, int bld = 0, int rev = 0) :
      major(maj), minor(min), build(bld), revision(rev) { };
   ~VersionInfo() { };

   bool operator>(VersionInfo& value);

   // >0 for passed version smaller, <0 for passed version greater
   int Compare(int maj, int min, int rev, int bld);

   void SetValue(std::wstring version);

   // Read the OS version
   void SetOSVersion();

   // Read the passed file version
   void SetFileVersion(std::wstring filePath);

   int major;
   int minor;
   int build;
   int revision;
};
