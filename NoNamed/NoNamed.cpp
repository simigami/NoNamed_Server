#include <iostream>

#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include <CorePch.cpp>

#include <chrono> // Time Comparison
#include <thread> // Cpp11 Universal Thread Creation
#include <atomic> // Cpp 11 Universal Atomic Transaction
#include <vector>
#include <iomanip>
#include <mutex>

vector<int32> vec;
mutex m;
constexpr int32 SIZE = 100'00;

template <typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()
	{
		_mutex->unlock();
	}

private:
	T* _mutex;
};

void Push()
{
	for(int i = 0; i < SIZE; ++i)
	{
		LockGuard<std::mutex> lockguard(m);
		vec.push_back(i);
	}
}

int main()
{
	std::cout << std::fixed;
    std::cout << std::setprecision(6);

	vector<std::thread> vt;

	vec.reserve(SIZE * 2);

	auto start =  chrono::high_resolution_clock::now();

	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	auto end = chrono::high_resolution_clock::now();
	auto ms_double = end - start;

	double ratio = static_cast<double>(vec.size()) / (SIZE * 2);

	cout << "time is : " << ms_double.count()  << " ms..." << endl;
	cout << "size of vec is : " << vec.size() << " , expected : " << SIZE * 2 << " , ratio : " << ratio;
}