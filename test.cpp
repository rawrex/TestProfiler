#include <iostream>
#include <atomic>
#include <memory>
#include <random>
#include <vector>

struct CBase  
{
    virtual void Execute() = 0;
		
	virtual ~CBase() = default;

    std::atomic<bool> is_enabled;
    std::atomic<std::uint32_t> number_of_iterations;
};

class A : public CBase
{
	// Some heavy data
};

class B : public CBase
{
	// Some heavy data
};

class C : public CBase
{
	// Some heavy data
};

class D : public CBase
{
	// Some heavy data
};

struct ItemInfo
{
    CBase* item;

	std::atomic<bool> is_enabled; 
	std::atomic<std::uint32_t> numebr_of_iterations; 
};

int main()
{
    std::vector<CBase*> objects;

    for (auto pObj : objects)
    {
        if (pObj->m_bIsEnabled && pObj->m_nIterations % 3 == 0)
        {
            pObj->Execute();
        }
    }

}
