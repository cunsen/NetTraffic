#include "stdafx.h"
#include "MFNetTraffic.h"
#include <winperf.h>

PERF_OBJECT_TYPE *FirstObject(PERF_DATA_BLOCK *dataBlock)
{
	return (PERF_OBJECT_TYPE *)((BYTE *)dataBlock + dataBlock->HeaderLength);
}

PERF_OBJECT_TYPE *NextObject(PERF_OBJECT_TYPE *act)
{
	return (PERF_OBJECT_TYPE *)((BYTE *)act + act->TotalByteLength);
}

PERF_COUNTER_DEFINITION *FirstCounter(PERF_OBJECT_TYPE *perfObject)
{
	return (PERF_COUNTER_DEFINITION *)((BYTE *)perfObject + perfObject->HeaderLength);
}

PERF_COUNTER_DEFINITION *NextCounter(PERF_COUNTER_DEFINITION *perfCounter)
{
	return (PERF_COUNTER_DEFINITION *)((BYTE *)perfCounter + perfCounter->ByteLength);
}

PERF_INSTANCE_DEFINITION *FirstInstance(PERF_OBJECT_TYPE *pObject)
{
	return (PERF_INSTANCE_DEFINITION *)((BYTE *)pObject + pObject->DefinitionLength);
}

PERF_COUNTER_BLOCK *GetCounterBlock(PERF_INSTANCE_DEFINITION *pInstance)
{
	return (PERF_COUNTER_BLOCK *)((BYTE *)pInstance + pInstance->ByteLength);
}

PERF_INSTANCE_DEFINITION *NextInstance(PERF_INSTANCE_DEFINITION *pInstance)
{
	PERF_COUNTER_BLOCK  *pCtrBlk = GetCounterBlock(pInstance);

	return (PERF_INSTANCE_DEFINITION *)((BYTE *)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}


vint GetObjectLength(PERF_DATA_BLOCK* pblock)
{
	if (pblock)
	{
		return pblock->HeaderLength;
	}
	return 0;
}

MFNetTraffic::MFNetTraffic()
{
}


MFNetTraffic::~MFNetTraffic()
{
}

bool MFNetTraffic::GetInterfaces()
{
	try
	{
		const vuint dafaultBufferSize = 40960L;
		Interfaces.Clear();
		Array<unsigned char> data;
		DWORD type;
		DWORD size = dafaultBufferSize;
		vuint ret;
	
		//RegQueryValueEx的固定调用格式		
	
		do 
		{
			data.Resize(size);
			ret = RegQueryValueEx(HKEY_PERFORMANCE_DATA, L"510", 0, &type, &data[0], &size);
			size += dafaultBufferSize;
		} while (ret == ERROR_MORE_DATA);
		if (ret != ERROR_SUCCESS)
		{
			return false;
		}
	
		PERF_DATA_BLOCK	 *dataBlockPtr = (PERF_DATA_BLOCK *)&data[0];
		PERF_OBJECT_TYPE *objectPtr = FirstObject(dataBlockPtr);
	
		for (size_t i = 0; i < dataBlockPtr->NumObjectTypes; i++)
		{
			if (objectPtr->ObjectNameTitleIndex == 510)
			{
				DWORD processIDOffset = ULONG_MAX;
				PERF_COUNTER_DEFINITION *counterPtr = FirstCounter(objectPtr);
	
				for (int b = 0; b < (int)objectPtr->NumCounters; b++)
				{
					if (counterPtr->CounterNameTitleIndex == 520)
						processIDOffset = counterPtr->CounterOffset;
	
					counterPtr = NextCounter(counterPtr);
				}
	
				if (processIDOffset == ULONG_MAX) {
					return true;
				}
	
				PERF_INSTANCE_DEFINITION *instancePtr = FirstInstance(objectPtr);
	
				for (int b = 0; b < objectPtr->NumInstances; b++)
				{
					wchar_t *namePtr = (wchar_t *)((BYTE *)instancePtr + instancePtr->NameOffset);
					PERF_COUNTER_BLOCK *counterBlockPtr = GetCounterBlock(instancePtr);
					DWORD bandwith = *((DWORD *)((BYTE *)counterBlockPtr + processIDOffset));
					DWORD tottraff = 0;
	
					Interfaces.Add(namePtr);
					Bandwidths.Add(bandwith);
					TotalTraffics.Add(tottraff);  // initial 0, just for creating the list
					instancePtr = NextInstance(instancePtr);
				}	
			}
	
			objectPtr = NextObject(objectPtr);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
