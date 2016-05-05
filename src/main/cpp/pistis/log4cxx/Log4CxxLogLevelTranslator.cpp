#include "Log4CxxLogLevelTranslator.hpp"

using namespace pistis::logging;
using namespace pistis::log4cxx;

std::once_flag Log4CxxLogLevelTranslator::_initOnce;
::log4cxx::LevelPtr* Log4CxxLogLevelTranslator::_log4CxxLevel= nullptr;
std::map<int, LogLevel>* Log4CxxLogLevelTranslator::_pistisLevel= nullptr;

Log4CxxLogLevelTranslator::Log4CxxLogLevelTranslator() {
  std::call_once(_initOnce, &Log4CxxLogLevelTranslator::_init);
}

Log4CxxLogLevelTranslator::~Log4CxxLogLevelTranslator() {
}

void Log4CxxLogLevelTranslator::_init() {
  _log4CxxLevel= new ::log4cxx::LevelPtr[(uint32_t)LogLevel::ERROR + 1];
  _log4CxxLevel[0]= ::log4cxx::Level::getOff();
  _log4CxxLevel[(uint32_t)LogLevel::TRACE]= ::log4cxx::Level::getTrace();
  _log4CxxLevel[(uint32_t)LogLevel::DEBUG]= ::log4cxx::Level::getDebug();
  _log4CxxLevel[(uint32_t)LogLevel::INFO]= ::log4cxx::Level::getInfo();
  _log4CxxLevel[(uint32_t)LogLevel::WARN]= ::log4cxx::Level::getWarn();
  _log4CxxLevel[(uint32_t)LogLevel::ERROR]= ::log4cxx::Level::getError();

  _pistisLevel= new std::map<int, LogLevel>;
  (*_pistisLevel)[::log4cxx::Level::getTrace()->toInt()]= LogLevel::TRACE;
  (*_pistisLevel)[::log4cxx::Level::getDebug()->toInt()]= LogLevel::DEBUG;
  (*_pistisLevel)[::log4cxx::Level::getInfo()->toInt()]= LogLevel::INFO;
  (*_pistisLevel)[::log4cxx::Level::getWarn()->toInt()]= LogLevel::WARN;
  (*_pistisLevel)[::log4cxx::Level::getError()->toInt()]= LogLevel::ERROR;
}

