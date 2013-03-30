#include <DropTarget.hpp>

HRESULT DropTarget::QueryInterface(REFIID iid, void **ppv)
{
    if (iid == IID_IUnknown)
        *ppv = static_cast<IUnknown*>(this);
    else if (iid == IID_IDropTarget)
        *ppv = static_cast<IDropTarget*>(this);
    else {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

ULONG DropTarget::AddRef(void)
{
    return InterlockedIncrement(&m_ref);
}

ULONG DropTarget::Release(void)
{
    ULONG ref = InterlockedDecrement(&m_ref);
    if (ref == 0)
        delete this;
    return ref;
}

BOOL DropTarget::DragDropRegister(HWND hwnd, DWORD dwState)
{
    m_hwnd = hwnd;
    m_state = dwState;
    return SUCCEEDED(RegisterDragDrop(hwnd, this));
}

HRESULT DropTarget::DragEnter(IDataObject *pDataObject, DWORD dwState,
                              POINTL pt, DWORD *pdwEffect)
{
    if (pdwEffect == NULL)
        return E_INVALIDARG; // Thank you for trying to crash me
    
    if (dwState != m_state) {
        *pdwEffect = DROPEFFECT_NONE;
        return S_OK;
    }
    
    *pdwEffect = OnDragEnter(pt, *pdwEffect, dwState, pDataObject);
    return S_OK;
}

HRESULT DropTarget::DragOver(DWORD dwState, POINTL pt, DWORD *pdwEffect)
{
    if (pdwEffect == NULL)
        return E_INVALIDARG; // Thank you for trying to crash me
    
    if (dwState != m_state) {
        *pdwEffect = DROPEFFECT_NONE;
        return S_OK;
    }
    
    *pdwEffect = OnDragOver(pt, *pdwEffect, dwState);
    return S_OK;
}

HRESULT DropTarget::DragLeave(void)
{
    OnDragLeave();
    return S_OK;
}

HRESULT DropTarget::Drop(IDataObject *pDataObject, DWORD dwState, POINTL pt,
                         DWORD *pdwEffect)
{
    if (pdwEffect == NULL)
        return E_INVALIDARG; // Thank you for trying to crash me
    
    *pdwEffect = OnDrop(pt, *pdwEffect, dwState, pDataObject);
    return S_OK;
}

DWORD DropTarget::OnDragEnter(POINTL point, DWORD effect, DWORD state,
                              IDataObject *data)
{
    return DROPEFFECT_COPY;
}

DWORD DropTarget::OnDragOver(POINTL point, DWORD effect, DWORD state)
{
    return effect;
}
