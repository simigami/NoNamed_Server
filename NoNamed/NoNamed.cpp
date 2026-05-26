#include <iostream>

#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include <CorePch.cpp>

#include <chrono> // Time Comparison
#include <thread> // Cpp11 Universal Thread Creation
#include <atomic> // Cpp 11 Universal Atomic Transaction

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread2(int num)
{
	cout << "Hello Thread 2 " << num << endl;
}

atomic<int32> sum = 0;

void add()
{
	for(int32 i =0; i < 100'0000; i++)
	{
		//sum++; // this is 3 instruction in assembly
		sum.fetch_add(1); // you can also arithmetic value with this method

		// move eax, dword ptr
		// inc eax (+= 1)
		// move dword ptr, eax
	}
}

void sub()
{
	for(int32 i =0; i < 100'0000; i++)
	{
		sum--;
	}
}

int main()
{
	vector<std::thread> v;

	auto start = chrono::high_resolution_clock::now();

	add();
	sub();

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> ms_double = end - start;

	cout << "sum is : " << sum << endl; // sum is absolutely 0
	cout << "time is : " << ms_double.count()  << " ms..." << endl;

	start = chrono::high_resolution_clock::now();

	std::thread t1(add);
	std::thread t2(sub);

	t1.join();
	t2.join();

	end = chrono::high_resolution_clock::now();
	ms_double = end - start;

	cout << "sum is : " << sum << endl; // sum is usually not 0 (?)
	cout << "time is : " << ms_double.count()  << " ms..." << endl;
}