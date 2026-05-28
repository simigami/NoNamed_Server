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
#include <future>

class Knight
{
public:
	Knight() { Hp = 100; };
	int32 GetHp() { return Hp; };

private:
	int32 Hp;
};

int64 Calculate()
{
	int64 sum = 0;
	for(int i = 0 ; i <= 1'000'000; ++i)
	{
		sum += i;
	}

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message : ");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// Sync Calc
	int64 sum = Calculate();
	cout << "SYNC JOB DONE ! sum : " << sum << endl;


	// Async Calc
	{
		// 1. Deferred future = Execute with lazy-evaluation (Single-Thread)
		// 2. Async future = Execute with new thread (Multi-Thread)
		// 3. Both future = Execute more efficient way (Any)
		Knight k;
		std::future<int32> future = std::async(std::launch::async, &Knight::GetHp, k);

		// TODO

		cout << "ASYNC JOB DONE ! hp : " << future.get() << endl;
	}

	// Promise
	{
		// Promise a future to return sth
		std::promise<string> promise;
		std::future<string> future = promise.get_future(); // 1:1 Sync with Promise

		// future resides in main thread, while promise is moved to t1
		thread t1(PromiseWorker, std::move(promise));

		t1.join();

		cout << "ASYNC JOB DONE ! Message : " << future.get() << endl;
	}

	// Packaged Task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t2(TaskWorker, std::move(task));

		t2.join();

		cout << "ASYNC JOB DONE ! Sum : " << future.get() << endl;
	}
}