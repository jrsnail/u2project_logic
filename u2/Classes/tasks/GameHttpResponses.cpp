#include "GameHttpResponses.h"


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TimeHRsp::TimeHRsp(const String& type, const String& name)
    : HttpResponse(type, name)
    , m_ulServerBaseTime(0L)
{
}
//-----------------------------------------------------------------------
TimeHRsp::~TimeHRsp()
{
}
//-----------------------------------------------------------------------
void TimeHRsp::deserialize()
{
    InStreamQueue<DataFilterInStream> in;
    in.push<VariableMemInStream>("aaa", &getData());
    in.push<DataFilterInStream>("bbb");
    m_ulServerBaseTime = in->readUint64();
}
//-----------------------------------------------------------------------
void TimeHRsp::run()
{
    deserialize();
}