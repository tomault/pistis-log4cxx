#ifndef __PISTIS__LOG4CXX__LOG4CXXLOGLEVELTRANSLATOR_HPP__
#define __PISTIS__LOG4CXX__LOG4CXXLOGLEVELTRANSLATOR_HPP__

#include <pistis/logging/LogLevel.hpp>
#include <log4cxx/level.h>
#include <map>
#include <mutex>

namespace pistis {
  namespace log4cxx {

    class Log4CxxLogLevelTranslator {
    public:
      Log4CxxLogLevelTranslator();
      ~Log4CxxLogLevelTranslator();

      ::log4cxx::LevelPtr operator[](logging::LogLevel logLevel) const {
	return _log4CxxLevel[(uint32_t)logLevel];
      }
      logging::LogLevel operator[](::log4cxx::LevelPtr level) const {
	if (!level) {
	  return logging::LogLevel::TRACE;
	}
	auto i= _pistisLevel->find(level->toInt());
	return (i != _pistisLevel->end()) ? i->second 
                                          : logging::LogLevel::ERROR;
      }

    private:
      static std::once_flag _initOnce;
      static ::log4cxx::LevelPtr* _log4CxxLevel;
      static std::map<int, logging::LogLevel>* _pistisLevel;

      static void _init();
    };
  }
}
#endif

