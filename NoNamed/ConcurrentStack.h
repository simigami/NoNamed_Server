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

	struct Node
	{
		Node(const T& value) : data(value)
		{
		}

		T data;
		Node* next;
	};

public:
	LockFreeStack() {};

	LockFreeStack(const LockFreeStack&) = delete;
	LockFreeStack& operator=(const LockFreeStack&) = delete;

	void Push(const T& value)
	{
		Node* newNode = new Node(value);
		newNode->next = _head;

		// Possible Race Condition, use CAS to protect
		while(_head.compare_exchange_weak(newNode->next, newNode) == false)
		{
		}
		
		// this_thread::sleep_for(10ms);
	}

	bool TryPop(T& value)
	{
		++_popCount;
		Node* oldHead = _head;
	
		// Possible Race Condition 1, use CAS to protect
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			
		}
		if (!oldHead)
		{
			--_popCount;
			return false;
		}
		
		value = oldHead->data;
		TryDelete(oldHead);
		
		// Possible Race Condition 2
		// delete(oldHead);
		
		return true;
	}

	void WaitPop(T& value)
	{

	}
	
	// 1. 데이터 분리 -> 2. 카운트 체크 -> 3. 나 혼자면 삭제
	void TryDelete(Node* oldHead)
	{
		// Check whether popCnt is 1
		if (_popCount == 1)
		{
			// reference of oldHead is protected due to the CAS in TryPop, So delete oldHead is thread-safe
			delete oldHead;
			
			// Also, try delete in pendingList
			Node* node = _pendingList.exchange(nullptr);
			if (--_popCount == 0)
			{
				// pop Count is normal, No Race Condition, so delete in pending List
				DeleteNodes(node);
			}
			else if (node)
			{
				// Race Condition Occurred, get pending List back
				// Since we do not know length of _pendingList, it should be List Appneding method
				MergeToPendingList(node);
			}
		}
		else
		{
			// Race Condition Occurred, give oldHead to pending List
			MergeToPendingList(oldHead);
		}
		
		// Possible Scenario is that if Pop Threads is LOT more then Push Threads, (--_popCount == 0) will usually not be executed
		// So, many node will just keep appends to PendingList that will result in memory overflow.
	}
	
	
	void MergeToPendingList(Node* first, Node* last)
	{
		// Possible RaceCondition, Protected by CAS
		last->next = _pendingList;
		
		while (_pendingList.compare_exchange_weak(last->next, first))
		{
			
		}
	}
	
	void MergeToPendingList(Node* node)
	{
		Node* last = node;
		while (last->next)
		{
			last = last->next;
		}
		MergeToPendingList(node, last);
	}
	
	void MergeOneNode(Node* node)
	{
		MergeToPendingList(node, node);
	}
	
	void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete(node);
			node = next;
		}
	}

private:
	atomic<Node*> _head;
	
	// for ref counting
	atomic<uint32> _popCount{0};

	// for need to delete nodes
	atomic<Node*> _pendingList;
};