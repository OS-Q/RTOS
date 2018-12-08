/*
*********************************************************************************************************
*	                                  
*	模块名称 : GUI界面主函数
*	文件名称 : MainTask.c
*	版    本 : V1.0
*	说    明 : 创建一个简单的GUI界面，使用的是官方实例，方便客户检测触摸。
*	修改记录 :
*		版本号   日期         作者          说明
*		v1.0    2015-08-05  Eric2013  	    首版    
*                                     
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "MainTask.h"


/*
*********************************************************************************************************
*	                       GUI_WIDGET_CREATE_INFO类型数组
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Counting...",     0,      30,  90, 260, 140, FRAMEWIN_CF_MOVEABLE },
  { TEXT_CreateIndirect,     "00",     GUI_ID_TEXT0,    10,  10, 120,  80 },
  { RADIO_CreateIndirect,    "",       GUI_ID_RADIO0,  150,  10, 100,  80, 0, 4 },
  { BUTTON_CreateIndirect,   "OK",     GUI_ID_OK,       10, 100,  60,  18 },
  { BUTTON_CreateIndirect,   "Cancel", GUI_ID_CANCEL,  180, 100,  60,  18 },
};

/*
*********************************************************************************************************
*	                                      数组
*********************************************************************************************************
*/
static const char * _apLabel[] = {
  "GUI_FontFD32",
  "GUI_FontFD48",
  "GUI_FontFD64",
  "GUI_FontFD80",
};

static const GUI_FONT * _apFont[] = {
  &GUI_FontD32,
  &GUI_FontD48,
  &GUI_FontD64,
  &GUI_FontD80
};

static const char * _asExplain[] = {
  "Please use the RADIO buttons to select",
  "the big digit font used for counting."
};

/*
*********************************************************************************************************
*	函 数 名: _SetFont
*	功能说明: 设置显示字体		
*	形    参: hDlg  对话框句柄
*	返 回 值: 无
*********************************************************************************************************
*/
static void _SetFont(WM_HWIN hDlg) 
{
	WM_HWIN hItem;
	int Index;
	hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
	Index = RADIO_GetValue(hItem);
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
	TEXT_SetFont(hItem, _apFont[Index]);
}

/*
*********************************************************************************************************
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口回调函数		
*	形    参: pMsg  回调参数 
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	int i;
	
	switch (pMsg->MsgId) 
	{
		/* 重绘消息 */
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLUE);
			GUI_Clear();
			GUI_SetColor(GUI_WHITE);
			GUI_SetFont(&GUI_Font24_ASCII);
			GUI_DispStringHCenterAt("Counting Sample", 160, 5);
			GUI_SetFont(&GUI_Font8x16);
			for (i = 0; i < GUI_COUNTOF(_asExplain); i++) 
			{
				GUI_DispStringAt(_asExplain[i], 5, 40 + i * 16);
			}
			
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: _cbCallback
*	功能说明: 对话框回调函数	
*	形    参: pMsg  回调参数 
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
	int i;
	int NCode, Id;
	WM_HWIN hDlg, hItem;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
			hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
			for (i = 0; i < GUI_COUNTOF(_apLabel); i++) 
			{
				RADIO_SetText(hItem, _apLabel[i], i);
			}
			_SetFont(hDlg);
			break;
			
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);    
			NCode = pMsg->Data.v;             
			switch (NCode) 
			{
				case WM_NOTIFICATION_VALUE_CHANGED: 
					_SetFont(hDlg);
					break;
				
				/* 两个按钮的消息处理 */
				case WM_NOTIFICATION_RELEASED:     
					if (Id == GUI_ID_OK) 
					{            
						GUI_EndDialog(hDlg, 0);
					}
					if (Id == GUI_ID_CANCEL) 
					{       
						GUI_EndDialog(hDlg, 1);
					}
					break;
			}
			break;
			
		default:
		WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MainTask(void) 
{
	int Value = 0;
	WM_HWIN hDlgFrame;
	
	/* 初始化 */
	GUI_Init();
//    TOUCH_Calibration();
	GUI_CURSOR_Show();
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);  
	WM_SetCreateFlags(WM_CF_MEMDEV);   
	hDlgFrame = 0;
	
	while(1) 
	{
		WM_HWIN hDlg, hText;
		char acText[3] = {0};
		
		GUI_Delay(100);
		/* 如果对话框被关闭就重新的将其再打开 */
		if (!WM_IsWindow(hDlgFrame)) 
		{
			Value = 0;
			hDlgFrame = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
		}
		
		Value = (Value + 1) % 100;
		acText[0] = '0' + Value / 10;
		acText[1] = '0' + Value % 10;
		hDlg = WM_GetClientWindow(hDlgFrame);
		hText = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetText(hText, acText);
	}
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
