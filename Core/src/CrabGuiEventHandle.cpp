
#include "CrabGuiDefine.h"
#include "CrabGuiEventHandle.h"

#include <string.h>


namespace CrabGui
{

	EventDesc::EventDesc()
	{
		memset(this, 0, sizeof(EventDesc));
	}


	EventHandle::EventHandle()
	{
	}

	EventHandle::~EventHandle()
	{
	}


	// 响应事件
	Bool EventHandle::eventHandle(const EventDesc* pEvent)
	{
		return False;
	}

}
