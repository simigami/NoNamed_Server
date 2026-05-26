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

#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for(int i = 0 ; i< 1000; i++)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
		for(int i = 0 ; i< 1000; i++)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "JOB DONE!" << endl;

	mutex m1, m2;

	std::lock(m1, m2);

	lock_guard<mutex> g1(m1, std::adopt_lock);
}