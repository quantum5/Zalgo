#include <MainWindow.hpp>
#include <commctrl.h>
#include <objbase.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "ole32.lib")

class COleInitialize {
public:
    COleInitialize() : m_hr(OleInitialize(NULL)) { }
    ~COleInitialize() { if (SUCCEEDED(m_hr)) OleUninitialize(); }
    operator HRESULT() const { return m_hr; }
    HRESULT m_hr;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    COleInitialize oleInit;
    InitCommonControls();

    MainWindow *win = MainWindow::Create(L"U̬̩͔̐̿ṅ̨̤̬̽i̛̘̳̹ͤ̍c̠̳̬͛o̘̳͗̿̈́d̗̅̓͗͠ḛ͍͛ ̬̠̹̉ͭ͛D̨̤̰̀̂ͦì̛͇͂a͍͇͛ͨͤc̡̟͖͗̔ͤͅr͔͇ͮ̓̍͢i̴͇͇̇͋̽t̛̟̟͋i̲̘̿̊c̺͎ͬ͗-̴̤́̔P̋̍҉͎̹ö̲̯͈̋͞l͎͂l̻̖ͩ̂͜u̵͔̳̇̋t̳̪͐̎e͈̓͢ḓ̗̭̓ ̹̫͛Ṭͫe̙̝̦̊̊̑͢x̶͉ͦ̚t̞̔̈́̀ ̡̪̪̙͒͗G̘̜̋e͍̯̻͋ͬͦn̹̩̫̑̈́ẽ͔̳r̠͙͒̀̅ͅa̭ͫ̓́t̘̺̋̏̚o̰̙̦ͪṛͦͣ́ͅ");
    if (win) {
        ShowWindow(win->GetHWND(), nCmdShow);
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}
