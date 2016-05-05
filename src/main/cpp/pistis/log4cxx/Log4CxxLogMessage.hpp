#ifndef __PISTIS__LOG4CXX__LOG4CXXLOGMESSAGE_HPP__
#define __PISTIS__LOG4CXX__LOG4CXXLOGMESSAGE_HPP__

#include <pistis/logging/LogMessage.hpp>
#include <log4cxx/logger.h>

namepsace pistis {
  namespace log4cxx {
    
    class Log4CxxLogMessage : public logging::LogMessage {

    private:
      log4cxx::LoggerPtr _logger; ///< Where this message should be written to
    };

  }
}
#endif
