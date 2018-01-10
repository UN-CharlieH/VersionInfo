#include "stdafx.h"
#include "versioninfo.h"
#include <sstream>
#include <vector>
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VersionInfo_Tests
{		
	TEST_CLASS(VersionInfo_Test1)
	{
       std::map<std::wstring, std::vector<int>> GetTestList() {
          std::map<std::wstring, std::vector<int>> testlist;

          testlist[L"1.2.3.4"] = std::vector<int>({ 1,2,3,4 });
          testlist[L"1,2,3,4"] = std::vector<int>({ 1,2,3,4 });
          testlist[L"1234"] = std::vector<int>({ 1234,0,0,0 });
          testlist[L"1.2.3.4_Debug"] = std::vector<int>({ 1,2,3,4 });
          testlist[L"1.2.34_Debug"] = std::vector<int>({ 1,2,34,0 });
          testlist[L"1.2.3_Debug.4"] = std::vector<int>({ 1,2,3,0 });
          return testlist;
       }

       std::wstring MakeMessage(const std::wstring& input, const std::wstring& output) {
          std::wostringstream ostr;
          ostr << L"Input: " << input << L" Output: " << output;
          return ostr.str();
       }

    public:
		
		TEST_METHOD(ConstructorParse)
		{
           std::map<std::wstring, std::vector<int>> testlist = GetTestList();

           for (std::map<std::wstring, std::vector<int>>::iterator idx = testlist.begin(); idx != testlist.end(); idx++)
           {
              VersionInfo versTest(idx->first);
              if (versTest.major != idx->second[0] ||
                 versTest.minor != idx->second[1] ||
                 versTest.build != idx->second[2] ||
                 versTest.revision != idx->second[3])
              {
                 Assert::Fail(MakeMessage(idx->first, versTest.GetValue()).c_str());
              }
           }
		}

        TEST_METHOD(CompareVersion)
        {
           VersionInfo v1(1, 2, 3, 4);
           VersionInfo v2(2, 3, 4, 5);
           VersionInfo v2_dup(2, 3, 4, 5);
           VersionInfo v3(1, 3, 4, 5);
           VersionInfo v4(1, 2, 4, 5);
           VersionInfo v5(1, 2, 3, 5);

           VersionInfo v7, v8;
           v7.SetValue(L"1.2.1928.0");
           v8.SetValue(L"1.3.2015_v2");
           if (v7 >= v8)
              Assert::Fail(MakeMessage(v7.GetValue(), v8.GetValue()).c_str());

           if (v1 > v2)
              Assert::Fail(MakeMessage(v1.GetValue(), v2.GetValue()).c_str());

           if (v2 != v2_dup)
              Assert::Fail(MakeMessage(v2.GetValue(), v2_dup.GetValue()).c_str());
        }

        TEST_METHOD(SetValue)
        {
           std::map<std::wstring, std::vector<int>> testlist = GetTestList();

           for (std::map<std::wstring, std::vector<int>>::iterator idx = testlist.begin(); idx != testlist.end(); idx++)
           {
              VersionInfo versTest;
              versTest.SetValue(idx->first);
              if (versTest.major != idx->second[0] ||
                 versTest.minor != idx->second[1] ||
                 versTest.build != idx->second[2] ||
                 versTest.revision != idx->second[3])
              {
                 std::wostringstream ostr;
                 ostr << L"Input: " << idx->first << L" Output: " << versTest.GetValue();
                 Assert::Fail(ostr.str().c_str());
              }
           }
        }

        TEST_METHOD(SetFileVersion)
        {
           VersionInfo vers;
           vers.SetFileVersion(L"winhttp.dll");

           // tracks with OS version
           Assert::IsTrue(vers.major >= 6);
           Assert::IsTrue(vers.minor >= 0);
           Assert::IsTrue(vers.build > 0);
           Assert::IsTrue(vers.revision >= 0);

           vers.SetFileVersion(L"wininet.dll");
           Assert::IsTrue(vers.major >= 6);
           Assert::IsTrue(vers.minor >= 0);
           Assert::IsTrue(vers.build > 0);
           Assert::IsTrue(vers.revision >= 0);

        }

        TEST_METHOD(SetProductVersion)
        {
           VersionInfo vers;
           vers.SetProductVersion(L"winhttp.dll");

           Assert::IsTrue(vers.major >= 6);
           Assert::IsTrue(vers.minor >= 0);
           Assert::IsTrue(vers.build >= 0);
           Assert::IsTrue(vers.revision >= 0);

           vers.SetProductVersion(L"wininet.dll");
           Assert::IsTrue(vers.major >= 6);
           Assert::IsTrue(vers.minor >= 0);
           Assert::IsTrue(vers.build > 0);
           Assert::IsTrue(vers.revision >= 0);

        }
    };
}
