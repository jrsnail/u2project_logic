﻿#include "U2Worker.h"


U2EG_NAMESPACE_USING


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void Worker::join() 
{ 
	if (m_Thread.joinable())
		m_Thread.join();
}
//-----------------------------------------------------------------------
String Worker::threadId2String(std::thread::id& tid)
{
	StringStream stream;
    stream << tid;
	return stream.str();
}