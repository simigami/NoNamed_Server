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

#include "Windows.h"

mutex m;
queue<int32> q;

condition_variable cv;

void Producer()
{
	while(true)
	{
		// 1. Get a Lock
		// 2. Modify condition_variable value
		// 3. Release a Lock
		// 4. Send Another thread by condition_variable
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // A. if theres a WAIT thread. READY one thread.
	}
}

void Consumer()
{
	while(true)
	{
		unique_lock<mutex> lock(m);

		// 1. Get a Lock
		// 2. Check Condition by Lambda Function
		// 3. If Condition Verified, go
		// 4. If not Verified, Release a Lock and WAIT
		cv.wait(lock, [](){ return q.empty() == false; });

		int32 data = q.front();
		q.pop();
		cout << data << endl;
	}
}

int main()
{
	thread t1(Producer); 
	thread t2(Consumer);

	t1.join();
	t2.join();

	cout << "JOB DONE ! " << endl;
}