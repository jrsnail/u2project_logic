#include "U2CancelableRequest.h"

#include "U2MsgLoop.h"


U2EG_NAMESPACE_USING


//---------------------------------------------------------------------
//---------------------------------------------------------------------
CancelableRequestProvider::CancelableRequestProvider()
    : next_handle_(1)
{
}
//---------------------------------------------------------------------
CancelableRequestProvider::~CancelableRequestProvider()
{
    // There may be requests whose result callback has not been run yet. We need
    // to cancel them otherwise they may try and call us back after we've been
    // deleted, or do other bad things. This can occur on shutdown (or browser
    // context destruction) when a request is scheduled, completed (but not
    // dispatched), then the BrowserContext is deleted.
    U2_LOCK_MUTEX(m_mtxPendingRequests);
    while (!m_PendingRequests.empty())
        CancelRequest(m_PendingRequests.begin());
}
//---------------------------------------------------------------------
CancelableRequestProvider::Handle CancelableRequestProvider::AddRequest(
    CancelableRequestBase* request,
    CancelableRequestConsumerBase* consumer)
{
    Handle handle;
    {
        U2_LOCK_MUTEX(m_mtxPendingRequests);

        handle = next_handle_;
        m_PendingRequests[next_handle_] = request;
        ++next_handle_;
        DCHECK(next_handle_) << "next_handle_ may have wrapped around to invalid state.";
    }

    consumer->OnRequestAdded(this, handle);

    request->Init(this, handle, consumer);
    return handle;
}
//---------------------------------------------------------------------
void CancelableRequestProvider::CancelRequest(Handle handle)
{
    U2_LOCK_MUTEX(m_mtxPendingRequests);
    
    CancelableRequestMap::iterator item = m_PendingRequests.find(handle);
    if (item == m_PendingRequests.end())
    {
        NOTREACHED() << "Trying to cancel an unknown request";
        return;
    }

    item->second->consumer()->OnRequestRemoved(this, item->first);
    item->second->set_canceled();
    m_PendingRequests.erase(item);
}
//---------------------------------------------------------------------
void CancelableRequestProvider::RequestCompleted(Handle handle)
{
    CancelableRequestConsumerBase* consumer = nullptr;
    {
        U2_LOCK_MUTEX(m_mtxPendingRequests);

        CancelableRequestMap::iterator i = m_PendingRequests.find(handle);
        if (i == m_PendingRequests.end())
        {
            NOTREACHED() << "Trying to cancel an unknown request";
            return;
        }
        consumer = i->second->consumer();

        // This message should only get sent if the class is not cancelled, or
        // else the consumer might be gone).
        assert(!i->second->canceled());

        m_PendingRequests.erase(i);
    }

    // Notify the consumer that the request is gone
    consumer->OnRequestRemoved(this, handle);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
CancelableRequestBase::CancelableRequestBase()
    : provider_(nullptr)
    , consumer_(nullptr)
    , handle_(0)
{
    m_pCallbackThread = MsgLoop::current();
}
//---------------------------------------------------------------------
CancelableRequestBase::~CancelableRequestBase()
{
}
//---------------------------------------------------------------------
void CancelableRequestBase::Init(CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle,
    CancelableRequestConsumerBase* consumer)
{
    assert(handle_ == 0 && provider_ == nullptr && consumer_ == nullptr);
    provider_ = provider;
    consumer_ = consumer;
    handle_ = handle;
}