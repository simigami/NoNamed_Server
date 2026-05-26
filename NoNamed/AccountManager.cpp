#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

#include <mutex>

void AccountManager::ProcessLogin()
{
	// User Lock inside User Class
	User* User = UserManager::Instance()->GetUser(1000);

	// Acount Lock
	lock_guard<mutex> guard(_mutex);
}