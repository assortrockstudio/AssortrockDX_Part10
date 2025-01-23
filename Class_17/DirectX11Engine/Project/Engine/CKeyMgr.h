#pragma once
#include "singleton.h"

enum class KEY
{
    W, S, A, D,
    Z, X, C, V,

    R, T, Y, U, I, O, P,

    _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, 

    NUM0, NUM1, NUM2, NUM3, NUM4, 
    NUM5, NUM6, NUM7, NUM8, NUM9,

    LEFT,
    RIGHT,
    UP,
    DOWN,

    LBTN,
    RBTN,

    ENTER,
    ESC,
    SPACE,
    LSHILFT,
    ALT,
    CTRL,



    KEY_END,
};


enum class KEY_STATE
{
    TAP,
    PRESSED,
    RELEASED,
    NONE,
};

struct tKeyInfo
{
    KEY_STATE   State;
    bool        PrevPressed;
};

class CKeyMgr :
    public CSingleton<CKeyMgr>
{
    SINGLE(CKeyMgr)
private:
    vector<tKeyInfo>    m_vecKey;
    Vec2                m_CurMousePos;
    Vec2                m_PrevMousePos;
    Vec2                m_DragDir;


public:
    void init();
    void tick();

public:
    KEY_STATE GetKeyState(KEY _Key) { return m_vecKey[(UINT)_Key].State; }

    Vec2 GetMousePos() { return m_CurMousePos; }
    Vec2 GetPrevMousePos() { return m_PrevMousePos; }
    Vec2 GetMouseDrag() { return m_DragDir; }

};

