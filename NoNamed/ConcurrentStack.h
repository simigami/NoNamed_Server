#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {};

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_cv.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if(!_stack.empty())
		{
			value = std::move(_stack.top());
			_stack.pop();
			return true;
		}

		return false;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_cv.wait(lock, [this] { return !_stack.empty(); });

		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _cv;
};

template<typename T>
class LockFreeStack
{
	struct Node;
	
	struct CountNodePtr
	{
		int32 externalCount = 1;
		Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value))
		{
		}

		shared_ptr<T> data;
		atomic<int32> internalCount = 0;
		CountNodePtr next;
	};

public:
	LockFreeStack() { };

	LockFreeStack(const LockFreeStack&) = delete;
	LockFreeStack& operator=(const LockFreeStack&) = delete;

	void Push(const T& value)
	{
		// Local
		CountNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;
		
		// Possible Race Condition
		node.ptr->next = _head.load();
		while (_head.compare_exchange_weak(node.ptr->next, node) == false)
		{
		}
		
		this_thread::sleep_for(1ms);
	}

	shared_ptr<T> TryPop()
	{
		CountNodePtr oldHead = _head;
		// Declare oldHead is going to use
		while (true)
		{
			// Get Reference RIghts
			IncreaseHeadCount(oldHead);
			
			// All oldHead below is externalCount == 2, so free to reference
			Node* ptr = oldHead.ptr;
			if (ptr == nullptr)
			{
				return shared_ptr<T>();
			}
			else
			{
				// Possible Race Condition
				// Get Ownership Rights
				if (_head.compare_exchange_strong(oldHead, ptr->next))
				{
					// All oldHead below is externalCount == 2, and head is sync!!!
					shared_ptr<T> res;
					res.swap(ptr->data);
					
					// Ok, thread-safe pop completed => TODO Need Deletion Rights
					
					const int32 countIncrease = oldHead.externalCount - 2;
					if (ptr->internalCount.fetch_add(countIncrease) == -countIncrease)
					{
						delete(ptr);
					}
					
					return res;
				}
				// oldHead that has Reference Rights but no Ownership should delete ptr
				else if (ptr->internalCount.fetch_sub(1) == 1)
				{
					delete(ptr);
				}
			}
			// Note that fetch function will return prev value, and add or sub after
		}
	}
	
	void IncreaseHeadCount(CountNodePtr& oldCounter)
	{
		while (true)
		{
			CountNodePtr newCounter = oldCounter;
			// Possible Race Condition when increasing Count
			++newCounter.externalCount;
			
			// Only the oldHead that has newCounter.externalCount == 2 will break, and other will in loop
			if (_head.compare_exchange_weak(oldCounter, newCounter))
			{
				// Safe here, so sync externalCount in oldHead
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}

private:
	// shared ptr
	atomic<CountNodePtr> _head;
};