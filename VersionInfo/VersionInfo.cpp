
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

int VersionInfo::SetFileVersion(const std::wstring& filePath)
{
   int dwRet = 0;
   // read file version
   std::wstring fileVer;
   DWORD dwBufLen = GetFileVersionInfoSizeW(filePath.c_str(), NULL);
   wchar_t* pBuffer = new wchar_t[dwBufLen/sizeof(wchar_t) + 1];
   if (pBuffer)
   {
      pBuffer[dwBufLen / sizeof(wchar_t)] = 0;
      if (GetFileVersionInfoW(filePath.c_str(), 0, dwBufLen, pBuffer))
      {
         fileVer = TestForStrTable(pBuffer);
      }
      else
         dwRet = GetLastError(); // Failure conditions

      delete[] pBuffer;
   }

   // add
   if (fileVer.length() > 0)
      SetValue(fileVer);

   return dwRet;
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
