// -*- C++ -*-
/*!
 * @file  StarGazer.cpp
 * @brief Hagisonic Star Gazer RTC
 * @date $Date$
 *
 * GPL
 *
 * $Id$
 */

#include "StarGazer.h"

// Module specification
// <rtc-template block="module_spec">
static const char* stargazer_spec[] =
  {
    "implementation_id", "StarGazer",
    "type_name",         "StarGazer",
    "description",       "Hagisonic Star Gazer RTC",
    "version",           "1.0.0",
    "vendor",            "ssr",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    "conf.default.port", "/dev/ttyUSB0",
    // Widget
    "conf.__widget__.debug", "text",
    "conf.__widget__.port", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
StarGazer::StarGazer(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_markerInfoOut("markerInfo", m_markerInfo)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
StarGazer::~StarGazer()
{
}



RTC::ReturnCode_t StarGazer::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("markerInfo", m_markerInfoOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  bindParameter("port", m_port, "/dev/ttyUSB0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StarGazer::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t StarGazer::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t StarGazer::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t StarGazer::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t StarGazer::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t StarGazer::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void StarGazerInit(RTC::Manager* manager)
  {
    coil::Properties profile(stargazer_spec);
    manager->registerFactory(profile,
                             RTC::Create<StarGazer>,
                             RTC::Delete<StarGazer>);
  }
  
};


