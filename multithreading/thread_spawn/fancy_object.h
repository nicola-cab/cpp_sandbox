/*
 * Example of move semantic in action
 * */
#pragma once
#include <iostream>
#include <ostream>
class A
{
public:
    static int a;
    A()
    {
        std::cout << "Ctor " << std::endl;
    }
    A( const A& )
    {
        std::cout << "Copy Ctor" << std::endl;
    }
    A(A&&)
    {
        std::cout << "Move Ctor" << std::endl;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const A& a)
    {
        os<<a.a;
        return os;
    }
    
};

int A::a = 10;
