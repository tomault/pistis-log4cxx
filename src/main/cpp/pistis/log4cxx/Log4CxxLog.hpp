#ifndef __PISTIS__LOG4CXX__LOG4CXXLOG_HPP__
#define __PISTIS__LOG4CXX__LOG4CXXLOG_HPP__

#include <pistis/logging/Log.hpp>
#include <pistis/logging/LogLevel.hpp>
#include <pistis/logging/LogMessageFactory.hpp>
#include <pistis/log4cxx/Log4CxxLogMessageReceiver.hpp>
#include <string>

namespace pistis {
  namespace log4cxx {
    
    class Log4CxxLog : public logging::Log {
    public:
      Log4CxxLog(logging::LogMessageFactory* factory,
		 Log4CxxLogMessageReceiver* receiver,
		 const std::string& destination,
		 logging::LogLevel logLevel);

      logging::LogMessageFactory* factory() const {
	return getFactory_();
      }
      
      Log4CxxLogMessageReceiver* receiver() const {
	return static_cast<Log4CxxLogMessageReceiver*>(getReceiver_());
      }
      
      void setLogLevel(logging::LogLevel logLevel);
    };

  }
}    
#endif
