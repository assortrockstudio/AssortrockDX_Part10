#pragma once
#include <Engine/CScript.h>

class CEditorCameraScript :
    public CScript
{
private:
    float       m_Speed;


private:
    void MoveByPerspective();
    void MoveByOrthographic();

public:
    virtual void tick() override;
    CLONE(CEditorCameraScript);
    CEditorCameraScript();
    ~CEditorCameraScript();
};

