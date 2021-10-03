#ifndef HAND_BASE_FACTORY_H
#define HAND_BASE_FACTORY_H

#include <map>


template<class ReturnType>
class IFactory
{
public:
    virtual ~IFactory() = default;
    virtual ReturnType* Create() = 0;
};


template<class CbOwner, class ReturnType>
class CallbackFactory : public IFactory<ReturnType>
{
    typedef ReturnType* (CbOwner::*TCallback)();

public:
    CallbackFactory(CbOwner* obj, TCallback func) : m_Object(obj), m_Method(func) {}

    ReturnType* Create() final
    {
        if (m_Object && m_Method)
            return (m_Object->*m_Method)();
        return nullptr;
    }

private:
    CbOwner* m_Object;
    TCallback m_Method;
};


template<class CreateType, class BaseType>
class TypeFactory : public IFactory<BaseType>
{
public:
    BaseType* Create() final
    {
        return new CreateType();
    }
};


template<class ReturnType>
class FactorySet
{
public:
    ~FactorySet()
    {
        for (auto p : m_Factories)
            delete p.second;
    }

    void Register(IFactory<ReturnType>* f, const std::string& target) { m_Factories[target] = f; }

    ReturnType* Create(const std::string& target)
    {
        auto it = m_Factories.find(target);
        if (it != m_Factories.end())
            return (*it).second->Create();
        return nullptr;
    }

private:
    std::map<std::string, IFactory<ReturnType>*> m_Factories;
};

#endif // HAND_BASE_FACTORY_H
