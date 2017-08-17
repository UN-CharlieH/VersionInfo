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
	public:
		
		TEST_METHOD(ParseVersion)
		{
           std::map<std::wstring, std::vector<int>> testlist;

           testlist[L"1.2.3.4"] = std::vector<int>({ 1,2,3,4 });
           testlist[L"1,2,3,4"] = std::vector<int>({ 1,2,3,4 });
           testlist[L"1234"] = std::vector<int>({ 1234,0,0,0 });
           VersionInfo versTest;
           for (std::map<std::wstring, std::vector<int>>::iterator idx = testlist.begin(); idx != testlist.end(); idx++)
           {
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

        TEST_METHOD(CompareVersion)
        {
           VersionInfo v1(1, 2, 3, 4);
           VersionInfo v2(2, 3, 4, 5);
           VersionInfo v2_dup(2, 3, 4, 5);
           VersionInfo v3(1, 3, 4, 5);
           VersionInfo v4(1, 2, 4, 5);
           VersionInfo v5(1, 2, 3, 5);

           if (v2 > v1)
           {
              Assert::Fail(L"Comparison failed");
           }
        }

        TEST_METHOD(SetValue)
        {
           Assert:: Fail(L"Comparison failed");
        }

        TEST_METHOD(SetFileVersion)
        {
           Assert::Fail(L"Comparison failed");
        }

    };
}
