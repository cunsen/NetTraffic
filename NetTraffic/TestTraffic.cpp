#include "stdafx.h"
#include "Vlpp.h"
#include "MFNetTraffic.h"

using namespace vl;
using namespace vl::unittest;
using namespace vl::console;
using namespace vl::filesystem;
using namespace vl::stream;

WString GetTestOutPutPath();

FileStream g_fileStream(GetTestOutPutPath() + L"TrafficInsterfaces.txt", FileStream::WriteOnly);
BomEncoder g_encoder(BomEncoder::Utf16);
EncoderStream g_encoderStream(g_fileStream, g_encoder);
StreamWriter g_writer(g_encoderStream);

WString GetExePath()
{
	wchar_t buffer[65536];
	GetModuleFileName(NULL, buffer, sizeof(buffer) / sizeof(*buffer));
	vint pos = -1;
	vint index = 0;
	while (buffer[index])
	{
		if (buffer[index] == L'\\' || buffer[index] == L'/')
		{
			pos = index;
		}
		index++;
	}
	return WString(buffer, pos + 1);
}

WString GetTestOutPutPath()
{
	return GetExePath() + L"../../Output/";
}

TEST_CASE(TestCrtOutpath)
{
	FilePath fpath = GetTestOutPutPath();
	filesystem::Folder folder(fpath);
	if (!folder.Exists())
	{
		TEST_ASSERT(folder.Create(false));
	}
}

TEST_CASE(TestTrafficInsterfaces)
{
	MFNetTraffic traffic;
	TEST_ASSERT(traffic.GetInterfaces());
	for (vint i = 0; i < traffic.Interfaces.Count(); i++)
	{
		g_writer.WriteLine(traffic.Interfaces.Get(i));
	}
	for (vint i = 0; i < traffic.Bandwidths.Count(); i++)
	{
		g_writer.WriteLine(itow(traffic.Bandwidths.Get(i)));
	}
}
