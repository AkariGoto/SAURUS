
#ifndef DESIGNLAB_ROBOT_GUI_ASURA2GUI_H_
#define DESIGNLAB_ROBOT_GUI_ASURA2GUI_H_

#ifndef __AFXWIN_H__
#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CASURA2GUIApp:
// このクラスの実装については、ASURA2GUI.cpp を参照してください
//



class CASURA2GUIDoc;


class CASURA2GUIApp : public CWinApp
{
public:
    CASURA2GUIApp() noexcept;

    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // 実装
    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()

public:
    CASURA2GUIDoc* pASURA2GUIDoc{ nullptr };

    /// モーションプランスレッドのID
    DWORD motionPlanThreadID{ 0 };

    //AsuraUDPThreadID
    DWORD AsuraUDPThreadID{ 0 };
};

extern CASURA2GUIApp theApp;


#endif  // DESIGNLAB_ROBOT_GUI_ASURA2GUI_H_
