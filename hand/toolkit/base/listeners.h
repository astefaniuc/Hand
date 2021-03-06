#ifndef HAND_BASE_LISTENERS_H
#define HAND_BASE_LISTENERS_H

#include <vector>


template<class CallerType>
class ICallback
{
public:
    virtual ~ICallback() = default;
    virtual void Execute(CallerType* caller) = 0;
    virtual void* GetObject() = 0;
};


template<class CbOwner, class CallerType>
class CCallback : public ICallback<CallerType>
{
    typedef void (CbOwner::*TCallback)(CallerType*);

public:
    CCallback(CbOwner* obj, TCallback func)
        : m_Object(obj), m_Function(func) {}

    void Execute(CallerType* caller) final
    {
        if (m_Object && m_Function)
            (m_Object->*m_Function)(caller);
    }

    void* GetObject() final { return m_Object; }

private:
    CbOwner* m_Object;
    TCallback m_Function;
};


template<class CallerType>
class Listeners
{
public:
    typedef ICallback<CallerType> Callback;

    ~Listeners()
    {
        for (auto cb : m_Register)
            delete cb;
        m_Register.clear();
    }

    void Notify(CallerType* caller)
    {
        for (Callback* listener : m_Register)
            listener->Execute(caller);
    }

    template <class CbOwner>
    void Add(CbOwner* listener, void (CbOwner::*method)(CallerType*))
    {
        m_Register.push_back(new CCallback<CbOwner, CallerType>(listener, method));
    }
    void Remove(void* listener)
    {
        for (unsigned i = 0; i < m_Register.size(); ++i)
        {
            if (m_Register[i]->GetObject() == listener)
            {
                delete m_Register[i];
                m_Register.erase(m_Register.begin() + i);
                return;
            }
        }
    }

private:
    std::vector<Callback*> m_Register;
};

#endif // HAND_BASE_LISTENERS_H
