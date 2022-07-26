// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

struct CBase  
{
    virtual void Execute() = 0;

    std::atomic<bool>           m_bIsEnabled;   //включенность
    std::atomic<std::uint32_t>  m_nIterations;   //число циклов до включения
};

struct ItemInfo
{
    CBase*                      m_pItem;
	std::atomic<bool>           m_bIsEnabled;   //включенность
	std::atomic<std::uint32_t>  m_nIterations;  //число циклов до включения
};



class A : public CBase
{
    //Разный вес классов
private:
    float asd;
};

class B : public CBase
{
private:
	float asd;
};

class C : public CBase
{
   
};

class D : public CBase
{

};


int main()
{
    std::vector<CBase*> objects;// 300-1000 объектов
    for (auto pObj : objects)
    {
        if (pObj->m_bIsEnabled && pObj->m_nIterations % 3 == 0)
        {
            pObj->Execute();
        }
    }


    //Далее создаем вектор из ItemInfo
    //сравниваем работу напрямую с CBase* и через ItemInfo
    //при работе через ItemInfo сначала собираем валидные m_pItem потом их вызываем



    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
