#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include "CoreGlobal.h"
#include "RefCounting.h"
#include "ThreadManager.h"

class Wraith : public RefCountable
{
public:
    int32 _hp = 150;
    int32 _posX = 0;
    int32 _posY = 0;
};

using WraithRef = TSharedPtr<Wraith>; 

class Missile : public RefCountable
{
public:
    void SetTarget(WraithRef target)
    {
        _target = target;
    }
    
    bool Update()
    {
        if (_target == nullptr) return true;
        
        int posX = _target->_posX;
        int posY = _target->_posY;
        
        if (_target->_hp == 0)
        {
            _target = nullptr;
            return true;
        }
        
        // TODO : Follow
    }
    
private:
    WraithRef _target = nullptr;
};

using MissileRef = TSharedPtr<Missile>; 

int main()
{
    WraithRef wraith(new Wraith());
    wraith->ReleaseRef();
    
    MissileRef missile(new Missile());
    missile->ReleaseRef();
    
    missile->SetTarget(wraith);
    wraith->_hp = 0;
    wraith = nullptr;
    
    while (true)
    {
        if (missile)
        {
            missile->Update();
        }
    }
}