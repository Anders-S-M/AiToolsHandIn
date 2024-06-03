#include <iostream>
#include <array>

void func(int *a)
{
    int size = (sizeof(a)/sizeof(a[0]));

    std::cout << "sizeof(a): " << sizeof(a) << std::endl;
    std::cout << "sizeof(a[0]): " << sizeof(a[0]) << std::endl;
    for(int i = 0; i<size; i++)
    {
        std::cout << "HATO" << std::endl;
    }
}

int main()
{
    int myarray[6] = { 1, 3, 2, 2, 9, 2 };

    func(myarray);

    return 0;
}

