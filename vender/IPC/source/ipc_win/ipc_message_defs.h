#ifndef GAMEHALLMAIN_IPC_IPC_MESSAGE_DEFS_H_
#define GAMEHALLMAIN_IPC_IPC_MESSAGE_DEFS_H_ 1

#define BEGIN_IPC_MSG_MAP(message)                         \
{                                                          \

#define IPC_MSG_HANDLER(MSG_ID, func)                      \
  if (message.GetType() == MSG_ID) {                       \
    func(const_cast<ipc::Message*>(&message));             \
    return true;                                           \
  }

#define END_IPC_MSG_MAP() \
}


// 主进程发送到IE核Render进程 IPC 消息ID定义
#define IPC_B2R_SetProcessData                          20001
#define IPC_B2R_UpdateParent                            20002
#define IPC_B2R_UpdateBounds                            20003
#define IPC_B2R_Navigate                                20004
#define IPC_B2R_RouteNewWindow                          20005
#define IPC_B2R_CloseTab                                20006
#define IPC_B2R_TabActivated                            20007
#define IPC_B2R_TabDeactivated                          20008
#define IPC_B2R_RefreshTab                              20009
#define IPC_B2R_MuteTab                                 20010
#define IPC_B2R_FocusTab                                20011
#define IPC_B2R_EnableContextMenu                       20012
#define IPC_B2R_RenderJSCheckClose                      20013
#define IPC_B2R_UpdateFlashFakeServerPort               20014
#define IPC_B2R_SetRenderZoom                           20015
#define IPC_B2R_UpdateFlashFakeData                     20016
#define IPC_B2R_UpdateFlashPatchInfo                    20017
#define IPC_B2R_CloseForSwitchRenderMode                20018

// IE核Render进程发送到主进程 IPC 消息ID定义
#define IPC_R2B_WebViewReady                            30001
#define IPC_R2B_WebViewDestroy                          30002
#define IPC_R2B_NavigateComplete                        30003
#define IPC_R2B_DocumentComplete                        30004
#define IPC_R2B_TitleChange                             30005
#define IPC_R2B_SetCookie                               30006
#define IPC_R2B_Sync_GetCookie                          30007
#define IPC_R2B_NewWindow                               30008
#define IPC_R2B_BindTabFinished                         30009
#define IPC_R2B_CloseByScript                           30010
#define IPC_R2B_ToggleFullscreen                        30011
#define IPC_R2B_SetMultiCookie                          30012
#define IPC_R2B_BeforeNavigate                          30013
#define IPC_R2B_NavigateError                           30014
#define IPC_R2B_ActivateTab                             30015
#define IPC_R2B_Sync_SwitchRenderMode                   30016
#define IPC_R2B_UpdateIEServerWindow                    30017
#define IPC_R2B_UpdateFavicon                           30018
#define IPC_R2B_SetIEServerWindow                       30019
#define IPC_R2B_JSShowLogin                             31001
#define IPC_R2B_JSSetLoginInfo                          31002
#define IPC_R2B_JSShowUnionLogin                        31003
#define IPC_R2B_JSNotifyLoginSucceeded                  31004
#define IPC_R2B_JSSetLoginType                          31005


#endif  // GAMEHALLMAIN_IPC_IPC_MESSAGE_DEFS_H_

