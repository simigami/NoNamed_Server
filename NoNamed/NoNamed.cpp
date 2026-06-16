#include "pch.h"
#include "CorePch.h"
#include "NoNamed.h"
#include "Memory.h"
#include "ThreadManager.h"

// class Knight : public RefCountable
// {
// public:
//     Knight()
//     {
//         cout << "Knight Created" << endl;
//     }
//     
//     Knight(int32 hp)
//     {
//         _hp = hp;
//         cout << "Knight Created With hp " << _hp << endl;
//     }
//     
//     ~Knight()
//     {
//         cout << "Knight Destroyed" << endl;
//     }
//     
//     int32 _hp = 100;
//     
// private:
//     Knight* _target = nullptr;
// };
// using KnightRef = TSharedPtr<Knight>;

class Player
{
public:
    Player() {}
    virtual ~Player() {}
};

class Knight : public Player
{
public:
    Knight()
    {
        cout << "Knight()" << endl;
    }

    Knight(int32 hp) : _hp(hp)
    {
        cout << "Knight(hp)" << endl;
    }

    ~Knight()
    {
        cout << "~Knight()" << endl;
    }

    int32 _hp = 100;
    int32 _mp = 10;
};

int main()
{
    // KnightRef k1 = new Knight();
    // k1->ReleaseRef();
    //
    // KnightRef k2 = new Knight();
    // k2->ReleaseRef();
    
    // shared_ptr has 2 elems, [Knight Pointer1] [Ref Counting Class 1]
    // shared_ptr<Knight> spr1(new Knight());
    Knight* knight = xnew<Knight>(100);
    xdelete(knight);

    // // [Knight Pointer Point at spr1] [Ref Counting Class 2]
    // shared_ptr<Knight> spr2(spr1);
    //
    // // [Knight Pointer Pointer | Ref Counting Class 3]
    // shared_ptr<Knight> spr3 = make_shared<Knight>();
    
    Vector<Knight> v(1024);
    Map<int32, Knight> m;
    m[100] = Knight(100);
}