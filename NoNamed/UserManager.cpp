#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// Account Lock in Instance
	Account* account = AccountManager::Instance()->GetAccount(1000);

	// User Lock
	lock_guard<mutex> guard(_mutex);
}
