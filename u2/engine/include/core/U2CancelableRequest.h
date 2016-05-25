#ifndef __U2CancelableRequest_H__
#define __U2CancelableRequest_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2Object.h"
#include "U2AtomicScalar.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


class CancelableRequestBase;
class CancelableRequestConsumerBase;
class MsgLoop;


class _U2Export CancelableRequestProvider : public Object, public GeneralAllocatedObject
{
    friend class CancelableRequestBase;

public:
    // Identifies a specific request from this provider.
    typedef u2int32 Handle;

public:
    CancelableRequestProvider();
    virtual ~CancelableRequestProvider();

    // Called by the enduser of the request to cancel it. This MUST be called on
    // the same thread that originally issued the request (which is also the same
    // thread that would have received the callback if it was not canceled).
    // handle must be for a valid pending (not yet complete or cancelled) request.
    void CancelRequest(Handle handle);

protected:
    // Adds a new request and initializes it. This is called by a derived class
    // to add a new request. The request's Init() will be called (which is why
    // the consumer is required. The handle to the new request is returned.
    Handle AddRequest(CancelableRequestBase* request,
        CancelableRequestConsumerBase* consumer);

    // Called by the CancelableRequest when the request has executed. It will
    // be removed from the list of pending requests (as opposed to canceling,
    // which will also set some state on the request).
    void RequestCompleted(Handle handle);

private:
    DISALLOW_COPY_AND_ASSIGN(CancelableRequestProvider);

private:
    U2_MUTEX(m_mtxPendingRequests);

    typedef map<Handle, std::shared_ptr<CancelableRequestBase> >::type CancelableRequestMap;
    // Lists all outstanding requests. Protected by the |lock_|.
    CancelableRequestMap m_PendingRequests;

    // The next handle value we will return. Protected by the |lock_|.
    u2int32 next_handle_;
};




// Classes wishing to make requests on a provider should have an instance of
// this class. Callers will need to pass a pointer to this consumer object
// when they make the request. It will automatically track any pending
// requests, and will automatically cancel them on destruction to prevent the
// accidental calling of freed memory.
//
// It is recommended to just have this class as a member variable since there
// is nothing to be gained by inheriting from it other than polluting your
// namespace.
//
// THIS CLASS IS NOT THREADSAFE (unlike the provider). You must make requests
// and get callbacks all from the same thread.

// Base class used to notify of new requests.
class CancelableRequestConsumerBase
{
    friend class CancelableRequestBase;
    friend class CancelableRequestProvider;

protected:

    virtual ~CancelableRequestConsumerBase() { }

    // Adds a new request to the list of requests that are being tracked. This
    // is called by the provider when a new request is created.
    virtual void OnRequestAdded(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle) = 0;

    // Removes the given request from the list of pending requests. Called
    // by the CancelableRequest immediately after the callback has executed for a
    // given request, and by the provider when a request is canceled.
    virtual void OnRequestRemoved(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle) = 0;

    // Sent to provider before executing a callback.
    virtual void WillExecute(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle) = 0;

    // Sent after executing a callback.
    virtual void DidExecute(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle) = 0;
};



// Template for clients to use. It allows them to associate random "client
// data" with a specific request. The default value for this type is 0.
// The type T should be small and easily copyable (like a pointer
// or an integer).
template<class T>
class CancelableRequestConsumerTSimple : public CancelableRequestConsumerBase
{
public:
    CancelableRequestConsumerTSimple();

    // Cancel any outstanding requests so that we do not get called back after we
    // are destroyed. As these requests are removed, the providers will call us
    // back on OnRequestRemoved, which will then update the list. To iterate
    // successfully while the list is changing out from under us, we make a copy.
    virtual ~CancelableRequestConsumerTSimple();

    // Associates some random data with a specified request. The request MUST be
    // outstanding, or it will assert. This is intended to be called immediately
    // after a request is issued.
    void SetClientData(CancelableRequestProvider* p,
        CancelableRequestProvider::Handle h,
        T client_data);

    // Retrieves previously associated data for a specified request. The request
    // MUST be outstanding, or it will assert. This is intended to be called
    // during processing of a callback to retrieve extra data.
    T GetClientData(CancelableRequestProvider* p,
        CancelableRequestProvider::Handle h);

    // Returns the data associated with the current request being processed. This
    // is only valid during the time a callback is being processed.
    T GetClientDataForCurrentRequest();

    // Returns true if there are any pending requests.
    bool HasPendingRequests() const;

    // Returns the number of pending requests.
    size_t PendingRequestCount() const;

    // Cancels all requests outstanding.
    void CancelAllRequests();

    // Cancels all requests outstanding matching the client data.
    void CancelAllRequestsForClientData(T client_data);

    // Returns the handle for the first request that has the specified client data
    // (in |handle|). Returns true if there is a request for the specified client
    // data, false otherwise.
    bool GetFirstHandleForClientData(T client_data,
        CancelableRequestProvider::Handle* handle);

    // Gets the client data for all pending requests.
    void GetAllClientData(std::vector<T>* data);

protected:
    struct PendingRequest
    {
        PendingRequest(CancelableRequestProvider* p, CancelableRequestProvider::Handle h)
            : provider(p)
            , handle(h) 
        {
        }

        PendingRequest() : provider(NULL), handle(0) {}

        // Comparison operator for stl.
        bool operator<(const PendingRequest& other) const
        {
            if (provider != other.provider)
                return provider < other.provider;
            return handle < other.handle;
        }

        bool is_valid() const { return provider != NULL; }

        CancelableRequestProvider* provider;
        CancelableRequestProvider::Handle handle;
    };

    typedef map<PendingRequest, T>::type PendingRequestList;

    virtual T get_initial_t() const;

    virtual void OnRequestAdded(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle);

    virtual void OnRequestRemoved(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle);

    virtual void WillExecute(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle);

    virtual void DidExecute(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle);

    // Lists all outstanding requests.
    PendingRequestList m_PendingRequests;

    // This is valid while processing a request and is used to identify the
    // provider/handle of request.
    PendingRequest m_CurrentRequest;
};
//---------------------------------------------------------------------
template<class T>
CancelableRequestConsumerTSimple<T>::CancelableRequestConsumerTSimple()
{
}
//---------------------------------------------------------------------
template<class T>
CancelableRequestConsumerTSimple<T>::~CancelableRequestConsumerTSimple()
{
    CancelAllRequests();
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::SetClientData(
    CancelableRequestProvider* p,
    CancelableRequestProvider::Handle h,
    T client_data)
{
    PendingRequest request(p, h);
    DCHECK(m_PendingRequests.find(request) != m_PendingRequests.end());
    m_PendingRequests[request] = client_data;
}
//---------------------------------------------------------------------
template<class T>
T CancelableRequestConsumerTSimple<T>::GetClientData(
    CancelableRequestProvider* p,
    CancelableRequestProvider::Handle h)
{
    PendingRequest request(p, h);
    DCHECK(m_PendingRequests.find(request) != m_PendingRequests.end());
    return m_PendingRequests[request];
}
//---------------------------------------------------------------------
template<class T>
T CancelableRequestConsumerTSimple<T>::GetClientDataForCurrentRequest()
{
    DCHECK(m_CurrentRequest.is_valid());
    return GetClientData(m_CurrentRequest.provider, m_CurrentRequest.handle);
}
//---------------------------------------------------------------------
template<class T>
bool CancelableRequestConsumerTSimple<T>::HasPendingRequests() const
{
    return !m_PendingRequests.empty();
}
//---------------------------------------------------------------------
template<class T>
size_t CancelableRequestConsumerTSimple<T>::PendingRequestCount() const
{
    return m_PendingRequests.size();
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::CancelAllRequests()
{
    // TODO(atwilson): This code is not thread safe as it is called from the
    // consumer thread (via the destructor) and accesses pending_requests_
    // without acquiring the provider lock (http://crbug.com/85970).
    PendingRequestList copied_requests(m_PendingRequests);
    for (typename PendingRequestList::iterator i = copied_requests.begin();
    i != copied_requests.end(); ++i)
    {
        i->first.provider->CancelRequest(i->first.handle);
    }
    copied_requests.clear();

    // That should have cleared all the pending items.
    DCHECK(m_PendingRequests.empty());
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::CancelAllRequestsForClientData(T client_data)
{
    PendingRequestList copied_requests(m_PendingRequests);
    for (typename PendingRequestList::const_iterator i = copied_requests.begin();
    i != copied_requests.end(); ++i)
    {
        if (i->second == client_data)
            i->first.provider->CancelRequest(i->first.handle);
    }
    copied_requests.clear();
}
//---------------------------------------------------------------------
template<class T>
bool CancelableRequestConsumerTSimple<T>::GetFirstHandleForClientData(
    T client_data,
    CancelableRequestProvider::Handle* handle)
{
    for (typename PendingRequestList::const_iterator i =
        m_PendingRequests.begin(); i != m_PendingRequests.end(); ++i)
    {
        if (i->second == client_data)
        {
            *handle = i->first.handle;
            return true;
        }
    }
    *handle = 0;
    return false;
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::GetAllClientData(
    std::vector<T>* data)
{
    DCHECK(data);
    for (typename PendingRequestList::iterator i = m_PendingRequests.begin();
    i != m_PendingRequests.end(); ++i)
    {
        data->push_back(i->second);
    }
}
//---------------------------------------------------------------------
template<class T>
T CancelableRequestConsumerTSimple<T>::get_initial_t() const
{
    return 0;
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::OnRequestAdded(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle)
{
    // Make sure we're not mixing/matching providers (since the provider is
    // responsible for providing thread safety until http://crbug.com/85970 is
    // fixed, we can't use the same consumer with multiple providers).
#ifndef NDEBUG
    if (!m_PendingRequests.empty())
        DCHECK(m_PendingRequests.begin()->first.provider == provider);
#endif
    DCHECK(m_PendingRequests.find(PendingRequest(provider, handle)) ==
        m_PendingRequests.end());
    m_PendingRequests[PendingRequest(provider, handle)] = get_initial_t();
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::OnRequestRemoved(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle)
{
    typename PendingRequestList::iterator i =
        m_PendingRequests.find(PendingRequest(provider, handle));
    if (i == m_PendingRequests.end())
    {
        NOTREACHED() << "Got a complete notification for a nonexistent request";
        return;
    }

    m_PendingRequests.erase(i);
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::WillExecute(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle)
{
    m_CurrentRequest = PendingRequest(provider, handle);
}
//---------------------------------------------------------------------
template<class T>
void CancelableRequestConsumerTSimple<T>::DidExecute(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle)
{
    m_CurrentRequest = PendingRequest();
}



// See CancelableRequestConsumerTSimple. The default value for T
// is given in |initial_t|.
template<class T, T initial_t>
class CancelableRequestConsumerT : public CancelableRequestConsumerTSimple<T>
{
public:
    CancelableRequestConsumerT();
    virtual ~CancelableRequestConsumerT();

protected:
    virtual T get_initial_t() const;
};

template<class T, T initial_t>
CancelableRequestConsumerT<T, initial_t>::CancelableRequestConsumerT()
{
}

template<class T, T initial_t>
CancelableRequestConsumerT<T, initial_t>::~CancelableRequestConsumerT()
{
}

template<class T, T initial_t>
T CancelableRequestConsumerT<T, initial_t>::get_initial_t() const
{
    return initial_t;
}

// Some clients may not want to store data. Rather than do some complicated
// thing with virtual functions to allow some consumers to store extra data and
// some not to, we just define a default one that stores some dummy data.
typedef CancelableRequestConsumerT<u2int32, 0> CancelableRequestConsumer;



// The request object that is used by a CancelableRequestProvider to send
// results to a CancelableRequestConsumer. This request handles the returning
// of results from a thread where the request is being executed to the thread
// and callback where the results are used. IT SHOULD BE PASSED AS A
// scoped_refptr TO KEEP IT ALIVE.
//
// It does not handle input parameters to the request. The caller must either
// transfer those separately or derive from this class to add the desired
// parameters.
//
// When the processing is complete on this message, the caller MUST call
// ForwardResult() with the return arguments that will be passed to the
// callback. If the request has been canceled, Return is optional (it will not
// do anything). If you do not have to return to the caller, the cancelable
// request system should not be used! (just use regular fire-and-forget tasks).
//
// Callback parameters are passed by value. In some cases, the request will
// want to return a large amount of data (for example, an image). One good
// approach is to derive from the CancelableRequest and make the data object
// (for example, a std::vector) owned by the CancelableRequest. The pointer
// to this data would be passed for the callback parameter. Since the
// CancelableRequest outlives the callback call, the data will be valid on the
// other thread for the callback, but will still be destroyed properly.

// Non-templatized base class that provides cancellation
class CancelableRequestBase : public Object, public GeneralAllocatedObject
{
    friend class CancelableRequestProvider;

public:
    // Initializes most things to empty, Init() must be called to complete
    // initialization of the object. This will be done by the provider when the
    // request is dispatched.
    //
    // This must be called on the same thread the callback will be executed on, it
    // will save that thread for later.
    //
    // This two-phase init is done so that the constructor can have no parameters,
    // which makes it much more convenient for derived classes, which can be
    // common. The derived classes need only declare the variables they provide in
    // the constructor rather than many lines of internal tracking data that are
    // passed to the base class (us).
    //
    // In addition, not all of the information (for example, the handle) is known
    // at construction time.
    CancelableRequestBase();

    CancelableRequestConsumerBase* consumer() const
    {
        return consumer_;
    }

    CancelableRequestProvider::Handle handle() const
    {
        return handle_;
    }

    // The canceled flag indicates that the request should not be executed.
    // A request can never be uncanceled, so only a setter for true is provided.
    // This can be called multiple times, but only from one thread.
    void set_canceled()
    {
        m_Canceled.set(true);
    }

    bool canceled()
    {
        return m_Canceled.get();
    }

protected:
    virtual ~CancelableRequestBase();

    // Initializes the object with the particulars from the provider. It may only
    // be called once (it is called by the provider, which is a friend).
    void Init(CancelableRequestProvider* provider,
        CancelableRequestProvider::Handle handle,
        CancelableRequestConsumerBase* consumer);

    // Attempts to execute callback on |callback_thread_| if the request has not
    // been canceled yet.
    template<class Function>
    typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
        std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
    >::type DoForward(Function&& f, bool force_async)
    {
        if (force_async || m_pCallbackThread != MsgLoop::current())
        {
            m_pCallbackThread->PostTask(f);
        }
        else
        {
            // We can do synchronous callbacks when we're on the same thread.
            ExecuteCallback(f);
        }
    }

    // Tells the provider that the request is complete, which then tells the
    // consumer.
    void NotifyCompleted() const
    {
        provider_->RequestCompleted(handle());
        consumer_->DidExecute(provider_, handle_);
    }

    // Cover method for CancelableRequestConsumerBase::WillExecute.
    void WillExecute()
    {
        consumer_->WillExecute(provider_, handle_);
    }

private:
    // Executes the |forwarded_call| and notifies the provider and the consumer
    // that this request has been completed. This must be called on the
    // |callback_thread_|.
    template<class Function>
    typename std::enable_if<!std::is_pointer<typename std::remove_reference<Function>::type>::value &&
        std::is_void<decltype(std::declval<typename std::remove_pointer<typename std::remove_reference<Function>::type>::type>()())>::value
    >::type ExecuteCallback(Function&& f)
    {
        assert(m_pCallbackThread == MsgLoop::current());

        if (!m_Canceled.get())
        {
            WillExecute();

            // Execute the callback.
            f();
        }

        // Notify the provider that the request is complete. The provider will
        // notify the consumer for us. Note that it is possible for the callback to
        // cancel this request; we must check canceled again.
        if (!m_Canceled.get())
            NotifyCompleted();
    }

    DISALLOW_COPY_AND_ASSIGN(CancelableRequestBase);

private:
    // The message loop that this request was created on. The callback will
    // happen on the same thread.
    MsgLoop* m_pCallbackThread;

    // The provider for this request. When we execute, we will notify this that
    // request is complete to it can remove us from the requests it tracks.
    CancelableRequestProvider* provider_;

    // Notified after we execute that the request is complete.  This should only
    // be accessed if !canceled_.IsSet(), otherwise the pointer is invalid.
    CancelableRequestConsumerBase* consumer_;

    // The handle to this request inside the provider. This will be initialized
    // to 0 when the request is created, and the provider will set it once the
    // request has been dispatched.
    CancelableRequestProvider::Handle handle_;

    // Set if the caller cancels this request. No callbacks should be made when
    // this is set.
    AtomicScalar<bool> m_Canceled;
};



// CancelableRequest wraps a normal Function for use in the
// CancelableRequest framework.
//
// When using this class, the provider MUST call Init() (on the base class)
// before this is valid.
//
// The two functions ForwardResult(), and ForwardResultAsync() are used to
// invoke the wrapped callback on the correct thread.  They are the same
// except that ForwardResult() will run the callback synchronously if
// we are already on the right thread.
//
// The caller does not need to check for cancel before calling ForwardResult()
// or ForwardResultAsync(). If the request has not been canceled, one of the
// these MUST be called.
//
// If there are any pointers in the parameters, they must live at least as
// long as the request so that it can be forwarded to the other thread.
// For complex objects, this would typically be done by having a derived
// request own the data itself.
template<class Function>
class CancelableRequest : public CancelableRequestBase
{
public:
    explicit CancelableRequest(const Function& callback)
        : CancelableRequestBase()
        , m_Callback(callback)
    {
    }

    void ForwardResult(void)
    {
        if (canceled()) return;
        DoForward(m_Callback, false);
    }

    void ForwardResultAsync()
    {
        if (canceled()) return;
        DoForward(m_Callback, true);
    }

protected:
    virtual ~CancelableRequest() {}

private:
    Function m_Callback;
};



U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2CancelableRequest_H__
