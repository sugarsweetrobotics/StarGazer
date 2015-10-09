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
    "conf.default.markType", "HLD1L",
    "conf.default.landmarkHeightFix", "false",
    "conf.default.landmarkHeight", "1.5",
    // Widget
    "conf.__widget__.debug", "text",
    "conf.__widget__.port", "text",
    "conf.__widget__.markType", "radio",
    "conf.__widget__.landmarkHeightFix", "radio",
    "conf.__widget__.landmarkHeight", "slider.0.1",
    // Constraints
    "conf.__constraints__.markType", "(HLD1S, HLD1L, HLD2S, HLD2L, HLD3S, HLD3L)",
    "conf.__constraints__.landmarkHeightFix", "(true,false)",
    "conf.__constraints__.landmarkHeight", "1.0<=x<=4.5",
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
  bindParameter("markType", m_markType, "HLD1L");
  bindParameter("landmarkHeightFix", m_landmarkHeightFix, "false");
  bindParameter("landmarkHeight", m_landmarkHeight, "1.5");
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
  m_hSG = 0;
  SG_RESULT result;
  if ((result = stargazer_init(m_port.c_str(), &m_hSG)) != SG_OK) {
    std::cout << "[RTC::StarGazer] Initialize (" << m_port << ") failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  if ((result = stargazer_calcStop(m_hSG)) != SG_OK) {
    std::cout << "[RTC::StarGazer] calcStop failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  char version[255];
  if ((result = stargazer_getVersion(m_hSG, version, 255)) != SG_OK) {
    std::cout << "[RTC::StarGazer] getVersion failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }
  std::cout << "[RTC::StarGazer] Version: " << version << "\n";
    
  SG_MARKTYPE markType;
  if ((result = stargazer_getMarkType(m_hSG, &markType)) != SG_OK) {
    std::cout << "[RTC::StarGazer] getMarkType failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }
    
  char* markTypeStrs[] = {"HLD1S",
			  "HLD1L",
			  "HLD2S",
			  "HLD2L",
			  "HLD3S",
			  "HLD3L",
			  "UNKNOWN"};
    
  if (markType >= 6 || markType < 0) { markType = SG_UNKNOWN; }
  std::cout << "[RTC::StarGazer] MarkType: " << markTypeStrs[markType] << "\n";
  
  SG_MARKTYPE requiredMarkType = SG_UNKNOWN;
  for (uint32_t i = 0;i < 7;i++) {
    if (m_markType == markTypeStrs[i]) {
      requiredMarkType = (SG_MARKTYPE)i;
    }
  }

  if (requiredMarkType == SG_UNKNOWN) {
    std::cout << "[RTC::StarGazer] Unknown MarkType request. (markType=" << m_markType << "\n";
    return RTC::RTC_ERROR;
  }

  if ((result = stargazer_setMarkType(m_hSG, requiredMarkType)) != SG_OK) {
    std::cout << "[RTC::StarGazer] setMarkType failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  bool heightFix = coil::toBool(m_landmarkHeightFix, "true", "false", "false");
  if ((result = stargazer_setLandmarkHeightFix(m_hSG, heightFix ? 1 : 0)) != SG_OK) {
    std::cout << "[RTC::StarGazer] setLandmarkHeightFix failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << "[RTC::StarGazer] setLandmarkHeight(" << m_landmarkHeight << ")\n";
  if ((result = stargazer_setLandmarkHeight(m_hSG, m_landmarkHeight)) != SG_OK) {
    std::cout << "[RTC::StarGazer] setLandmarkHeight failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  if ((result = stargazer_setEnd(m_hSG)) != SG_OK) {
    std::cout << "[RTC::StarGazer] setEnd failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }

  if ((result = stargazer_calcStart(m_hSG)) != SG_OK) {
    std::cout << "[RTC::StarGazer] calcStart failed (return = " << result << ")" << std::endl;
    return RTC::RTC_ERROR;
  }
  std::cout << "[RTC::StarGazer] calcStart." << std::endl;
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


