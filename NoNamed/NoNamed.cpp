#include <iostream>

#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include <CorePch.cpp>

#include <thread> // Cpp11 Universal Thread Creation

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread2(int num)
{
	cout << "Hello Thread 2 " << num << endl;
}

int main()
{
	vector<std::thread> v;

	std::thread t;
	int32 count = t.hardware_concurrency(); // Number of threads ~= Number of CPU Cores
	cout << "core count is : " << count << endl; // id is 0 when theres no job

	for(int32 i = 0; i< 10; i++)
	{
		v.push_back(std::thread(HelloThread2, i));  // give thread t a job

		std::thread::id id = v[i].get_id(); // id is sth when theres some job working on
		cout << "Id of v" << i << " is : " << id << endl;	}

	for(int32 i = 0; i< 10; i++)
	{
		if(v[i].joinable()) // Check if thread is able to join, true if joinable
		{
			v[i].join();
		}
	}

	//t.detach(); // Detach Thread from its parent, becomes an independant thread (aka LINUX Daemon)
}