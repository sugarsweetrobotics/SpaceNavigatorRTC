// -*- C++ -*-
/*!
 * @file  SpaceNavigator.cpp
 * @brief Space Navigator RT Component
 * @date $Date$
 *
 * $Id$
 */

#include "SpaceNavigator.h"

#include "3DConnexion.h"


HWND GetConsoleHwnd(void){

    #define MY_BUFSIZE 1024 // コンソール ウィンドウのタイトル用のバッファサイズ
    HWND hwndFound;         // 呼び出し側へ返される値
    char pszNewWindowTitle[MY_BUFSIZE];
                           // 作成されるウィンドウのタイトルが入ります
    char pszOldWindowTitle[MY_BUFSIZE]; // 元のウィンドウタイトルが入ります

    // 現在のウィンドウタイトルを取得します

    GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

    // 独自に、ウィンドウの新規タイトルをフォーマットします

    wsprintf(pszNewWindowTitle,"%d/%d",
             GetTickCount(),
             GetCurrentProcessId());

    // 現在のウィンドウタイトルを変更します

    SetConsoleTitle(pszNewWindowTitle);

    // ウィンドウタイトルのアップデートを確実なものにさせます

    Sleep(40);

    // ウィンドウの新規タイトルを探しにいきます

    hwndFound=FindWindow(NULL, pszNewWindowTitle);

    // 元のウィンドウタイトルへ戻します

    SetConsoleTitle(pszOldWindowTitle);

    return(hwndFound);
}


// Module specification
// <rtc-template block="module_spec">
static const char* spacenavigator_spec[] =
  {
    "implementation_id", "SpaceNavigator",
    "type_name",         "SpaceNavigator",
    "description",       "Space Navigator RT Component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Inpu",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SpaceNavigator::SpaceNavigator(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_outOut("out", m_out)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
SpaceNavigator::~SpaceNavigator()
{
}

HWND hWnd2;

RTC::ReturnCode_t SpaceNavigator::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("out", m_outOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>

  hWnd2 = GetConsoleHwnd();
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SpaceNavigator::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



RTC::ReturnCode_t SpaceNavigator::onActivated(RTC::UniqueId ec_id)
{
  m_p3DConnexion = new C3DConnexion(::GetModuleHandleA(NULL), hWnd2);
  m_out.data.length(6);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SpaceNavigator::onDeactivated(RTC::UniqueId ec_id)
{
  delete m_p3DConnexion;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SpaceNavigator::onExecute(RTC::UniqueId ec_id)
{
  m_p3DConnexion->UpdateData();
  memcpy(&(m_out.data[0]), m_p3DConnexion->data, sizeof(double)*6);
  m_outOut.write();
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SpaceNavigator::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SpaceNavigator::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SpaceNavigatorInit(RTC::Manager* manager)
  {
    coil::Properties profile(spacenavigator_spec);
    manager->registerFactory(profile,
                             RTC::Create<SpaceNavigator>,
                             RTC::Delete<SpaceNavigator>);
  }
  
};


