#pragma once
#ifndef idC4123BF1_C7A2_4DF3_AEA77895CE6309D9
#define idC4123BF1_C7A2_4DF3_AEA77895CE6309D9

#include <windows.h>
#include <ole2.h>
#include <oleidl.h>

class DropTarget : public IDropTarget {
public:
    IFACEMETHODIMP QueryInterface(REFIID iid, void **ppv);
    IFACEMETHODIMP_(ULONG) STDMETHODCALLTYPE AddRef(void);
    IFACEMETHODIMP_(ULONG) STDMETHODCALLTYPE Release(void);
    IFACEMETHODIMP DragOver(DWORD dwState, POINTL pt,
                            DWORD *pdwEffect);
    IFACEMETHODIMP DragEnter(IDataObject *pDataObject, DWORD dwState, POINTL pt,
                             DWORD *pdwEffect);
    IFACEMETHODIMP DragLeave(void);
    IFACEMETHODIMP Drop(IDataObject *pDataObject, DWORD dwState, POINTL pt,
                        DWORD *pdwEffect);

    STDMETHODIMP_(BOOL) DragDropRegister(HWND hwnd, DWORD dwState=MK_LBUTTON);
    
    virtual DWORD OnDragEnter(POINTL point, DWORD effect, DWORD state,
                              IDataObject *data);
    virtual DWORD OnDragOver(POINTL point, DWORD effect, DWORD state);
    virtual void OnDragLeave(void) {}
    virtual DWORD OnDrop(POINTL point, DWORD effect, DWORD state,
                       IDataObject *data) = 0;
    
protected:
    HWND m_hwnd;
    DWORD m_state;
private:
    volatile ULONG m_ref;
};

#endif
