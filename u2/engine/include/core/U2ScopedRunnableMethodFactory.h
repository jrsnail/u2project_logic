#ifndef __U2ScopedRunnableMethodFactory_H__
#define __U2ScopedRunnableMethodFactory_H__

#include "U2Prerequisites.h"
#include "U2STLRedefined.h"
#include "U2HeaderPrefix.h"


U2EG_NAMESPACE_BEGIN


template<class T>
class _U2Export ScopedRunnableMethodFactory
{
protected:
    template <class Method, class Params>
    class RunnableMethod : public Task
    {
    public:
        RunnableMethod(const std::weak_ptr<T>& obj, Method meth, const Params& params)
            : obj_(obj),
            meth_(meth),
            params_(params)
        {}

        virtual void run()
        {
            if (!obj_.expired())
                dispatchToMethod(obj_.get(), meth_, params_);
        }

    private:
        template <class ObjT, class Method>
        inline void dispatchToMethod(ObjT* obj, Method method, const Tuple0& arg, Tuple0*) 
        {
            (obj->*method)();
        }

    private:
        std::weak_ptr<T> obj_;
        Method meth_;
        Params params_;
    };

public:
    explicit ScopedRunnableMethodFactory(T* object) 
        : m_wpObj(object)
    {}

    template <class Method>
    inline Task* createRunnableMethod(Method method) 
    {
        return U2_NEW RunnableMethod<Method, std::tuple<> >(
            m_wpObj, method, std::make_tuple<>()
            );
    }


    template <class Method, class A>
    inline Task* createRunnableMethod(Method method, const A& a)
    {
        return new RunnableMethod<Method, std::tuple<A> >(
            m_wpObj, method, std::make_tuple<A>(a)
            );
    }



protected:
    std::weak_ptr<T> m_wpObj;
};




/** @} */
/** @} */

U2EG_NAMESPACE_END

#include "U2HeaderSuffix.h"

#endif // __U2ScopedRunnableMethodFactory_H__
