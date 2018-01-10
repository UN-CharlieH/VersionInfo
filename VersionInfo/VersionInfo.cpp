
#include <windows.h>
#include "VersionInfo.h"
#include <sstream>
#include <iomanip>
#include <iostream>

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

std::wstring VersionInfo::GetFixedVerResValue(wchar_t* pBuffer, int type)
{
   WORD v1 = 0, v2 = 0, v3 = 0, v4 = 0;

   wchar_t* pVerBuf;
   unsigned int uLen;
   VS_FIXEDFILEINFO* fileVerInfo;
   if (VerQueryValueW(pBuffer, L"\\", (LPVOID*)&pVerBuf, &uLen)) {
      if (pVerBuf) {
         fileVerInfo = (VS_FIXEDFILEINFO*)pVerBuf;
         if (type == 0) {
            v1 = fileVerInfo->dwFileVersionMS >> 16;
            v2 = fileVerInfo->dwFileVersionMS & 0x0000FFFF;
            v3 = fileVerInfo->dwFileVersionLS >> 16;
            v4 = fileVerInfo->dwFileVersionLS & 0x0000FFFF;
         }
         else if (type == 1) {
            v1 = fileVerInfo->dwProductVersionMS >> 16;
            v2 = fileVerInfo->dwProductVersionMS & 0x0000FFFF;
            v3 = fileVerInfo->dwProductVersionLS >> 16;
            v4 = fileVerInfo->dwProductVersionLS & 0x0000FFFF;
         }
      }
   }

   std::wostringstream ostr;
   ostr << v1 << L"." << v2 << L"." << v3 << L"." << v4;
   return ostr.str();
}

std::wstring TestForStrTable(wchar_t* pBuffer)
{
   std::wstring fileVer;

   wchar_t* pVerBuf;
   unsigned int uLen;
   unsigned int cbTranslate = 0;
   struct LANGANDCODEPAGE {
      WORD wLanguage;
      WORD wCodePage;
   } *lpTranslate;

   // Read the list of languages and code pages.
   if (VerQueryValue(pBuffer, TEXT("\\VarFileInfo\\Translation"), (LPVOID*)&lpTranslate, &cbTranslate))
   {
      // Read the file description for each language and code page.
      for (int i = 0; i < (cbTranslate / sizeof(struct LANGANDCODEPAGE)); i++)
      {
         std::wostringstream ostr;
         ostr << L"\\StringFileInfo\\" << std::setw(4) << std::setfill(L'0') << std::hex << lpTranslate->wLanguage <<
            std::setw(4) << std::setfill(L'0') << std::hex << lpTranslate->wCodePage << L"\\FileVersion";

         // Retrieve file description for language and code page "i". 
         if (VerQueryValue(pBuffer, ostr.str().c_str(), (LPVOID*)&pVerBuf, &uLen)) {
            if (pVerBuf)
               fileVer = pVerBuf;
         }
      }
   }

   return fileVer;
}

std::wstring VersionInfo::GetResItem(const std::wstring& filePath, int type)
{
   int dwRet = 0;
   // read file version
   std::wstring fileVer;
   DWORD dwBufLen = GetFileVersionInfoSizeW(filePath.c_str(), NULL);
   wchar_t* pBuffer = new wchar_t[dwBufLen / sizeof(wchar_t) + 1];
   if (pBuffer)
   {
      pBuffer[dwBufLen / sizeof(wchar_t)] = 0;
      if (GetFileVersionInfoW(filePath.c_str(), 0, dwBufLen, pBuffer))
      {
         if (type ==0 || type==1)
            fileVer = GetFixedVerResValue(pBuffer, type);
         else
            fileVer = TestForStrTable(pBuffer);
      }
      else
         dwRet = GetLastError(); // Failure conditions

      delete[] pBuffer;
   }

   return fileVer;
}

int VersionInfo::SetFileVersion(const std::wstring& filePath)
{
   // read file version
   std::wstring fileVer = GetResItem(filePath, 0);

   // add if valid
   if (fileVer.length() > 0)
      SetValue(fileVer);
   else
      return -1;

   return 0;
}

int VersionInfo::SetProductVersion(const std::wstring & filePath)
{
   // read file version
   std::wstring fileVer = GetResItem(filePath, 1);

   // add if valid
   if (fileVer.length() > 0)
      SetValue(fileVer);
   else
      return -1;

   return 0;
}

int VersionInfo::compare(const VersionInfo & lhs, const VersionInfo & rhs)
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
