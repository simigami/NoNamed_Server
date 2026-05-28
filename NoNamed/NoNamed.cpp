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
HANDLE handle;

void Producer()
{
	while(true)
	{
		unique_lock<mutex> lock(m);
		q.push(100);

		::SetEvent(handle); // 4. Gives Event -> set handle as signaled

		this_thread::sleep_for(100ms); // 1. What happens if 100ms turns to 10000000ms? 

		// 7. if sleep for 10000000ms,  WaitForSingleObject will stop the t2
	}
}

void Consumer()
{
	while(true) // 2. Consumer will run loop even when theres no elements inside a queue
	{
		::WaitForSingleObject(handle, INFINITE); // 5. Kernel will decide whether to go, or stop the t2

		// 6. Kernel will change handle as non-signaled if go

		unique_lock<mutex> lock(m);
		if(!q.empty())
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	handle = ::CreateEvent(NULL, FALSE, FALSE, NULL); // 3. Make a event to stop t2 when queue has no elements

	thread t1(Producer); 
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle); // Close Event

	cout << "JOB DONE ! " << endl;
}