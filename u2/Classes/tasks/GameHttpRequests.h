#ifndef __GameHttpRequests_H__
#define __GameHttpRequests_H__


#include "U2Core.h"
#include <json/json.h>


U2EG_NAMESPACE_USING


class TimeHReq : public HttpRequest
{
public:
    TimeHReq(const u2::String& type, const u2::String& name);
    virtual ~TimeHReq();

    virtual const u2::String& getHttpResponse() override;

    virtual void serialize() override;

private:
};


class RegisterHReq : public HttpRequest
{
public:
    RegisterHReq(const u2::String& type, const u2::String& name);
    virtual ~RegisterHReq();

    virtual const u2::String& getHttpResponse() override;

    virtual void serialize() override;

private:
};


class PlayHReq : public HttpRequest
{
public:
    PlayHReq(const u2::String& type, const u2::String& name);
    virtual ~PlayHReq();

    virtual const u2::String& getHttpResponse() override;

    virtual void serialize() override;

private:
};


#endif /* defined(__GameHttpRequests_H__) */
