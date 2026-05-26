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

class SpinLock
{
public:
	void lock()
	{
		// CAS
		bool expected = false;
		bool desired = true;

		while(!_locked.compare_exchange_strong(expected, desired))
		{
			expected = false;
		}

		_locked.store(true);
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	atomic<bool> _locked =false;
};

class SleepLock
{
public:
	void lock()
	{
		// CAS
		bool expected = false;
		bool desired = true;

		while(!_locked.compare_exchange_strong(expected, desired))
		{
			expected = false;

			// Sleep instead of looping (feat. Cpp11 sleep_for)
			this_thread::sleep_for(std::chrono::milliseconds(1));

			// Goes to Kernel mode -> Self-Context-Switch
			this_thread::yield(); 
		}

		_locked.store(true);
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	atomic<bool> _locked =false;
};

SpinLock spinLock;
SleepLock sleepLock;
mutex m;
int32 sum = 0;

void Add()
{
	for(int i = 0 ; i< 100'000; i++)
	{
		lock_guard<SleepLock> g(sleepLock);
		sum++;
	}
}

void Sub()
{
		for(int i = 0 ; i< 100'000; i++)
	{
		lock_guard<SleepLock> g(sleepLock);
		sum--;
	}
}

int main()
{
	std::thread t1(Add);
	std::thread t2(Sub);

	t1.join();
	t2.join();

	cout << "JOB DONE! sum : " << sum << endl;
}