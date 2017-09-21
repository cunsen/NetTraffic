#pragma once
#include "Vlpp.h"

using namespace vl;
using namespace vl::collections;
class MFNetTraffic
{
public:
	MFNetTraffic();
	~MFNetTraffic();
	enum eTrafficType
	{
		TrafficNone,
		TrafficAll,
		TrafficIncoming,
		TrafficOutGoing,
	};


public:
	bool GetInterfaces();

public:

	List<WString> Interfaces;
	List<DWORD> Bandwidths;			//����
	List<DWORD> TotalTraffics;		//�ܵ�����

};

