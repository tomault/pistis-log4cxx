#ifndef __PISTIS__LOG4CXX__LOG4CXXLOGMESSAGERECEIVER_HPP__
#define __PISTIS__LOG4CXX__LOG4CXXLOGMESSAGERECEIVER_HPP__

#include <pistis/logging/LogMessageFactory.hpp>
#include <pistis/logging/LogMessageReceiver.hpp>
#include <pistis/log4cxx/Log4CxxLogLevelTranslator.hpp>
#include <log4cxx/logger.h>
#include <mutex>

namespace pistis {
  namespace log4cxx {
    
    class Log4CxxLogMessageReceiver
      : public logging::LogMessageReceiver {
    public:
      Log4CxxLogMessageReceiver(logging::LogMessageFactory* factory,
				const ::log4cxx::LoggerPtr& logger);

      virtual void receive(logging::LogMessage* msg);
      void setLogLevel(logging::LogLevel logLevel);

    protected:
      virtual void _write(logging::LogLevel level, const char* msg,
			  const char* end);

    private:
      logging::LogMessageFactory* _factory; ///< Where messages will be returned
      ::log4cxx::LoggerPtr _logger; ///< Where received messages will be written

      static const Log4CxxLogLevelTranslator _LOG4CXX_LEVEL;
    };

  }
}
#endif
