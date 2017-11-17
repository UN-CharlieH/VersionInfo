#pragma once

#include <string>

// major.minor[.build[.revision]]
// Missing version components default to zero.
// TODO: define behavior for negative numbers.
class VersionInfo {
public:
   VersionInfo() :
      major(0), minor(0), build(0), revision(0) { };
   VersionInfo(const std::wstring& version) :
      major(0), minor(0), build(0), revision(0) { SetValue(version);  };
   VersionInfo(int maj, int min, int bld = 0, int rev = 0) :
      major(maj), minor(min), build(bld), revision(rev) { };
   ~VersionInfo() { };

   inline bool operator>(const VersionInfo& rhs) { return compare(*this, rhs) > 0 ? true : false; }
   inline bool operator>=(const VersionInfo& rhs) { return !(*this < rhs); }
   inline bool operator<(const VersionInfo& rhs) { return compare(*this, rhs) < 0 ? true : false; }
   inline bool operator<=(const VersionInfo& rhs) { return !(*this > rhs); }
   inline bool operator==(const VersionInfo& rhs) { return compare(*this, rhs) == 0 ? true : false; }
   inline bool operator!=(const VersionInfo& rhs) { return !(*this == rhs); }

   // >0 for passed version smaller, <0 for passed version greater
   int Compare(int maj, int min, int rev, int bld);

   // Add a version string to the current object
   void SetValue(const std::wstring version);

   // Get a version string formatted using the passed separator, or default '.'
   std::wstring GetValue(wchar_t sep = L'.');

   // Read the passed file version
   int SetFileVersion(const std::wstring& filePath);

   int major;
   int minor;
   int build;
   int revision;

private:
   int compare(const VersionInfo& lhs, const VersionInfo& rhs);
};
