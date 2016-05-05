#include "Log4CxxLogMessageReceiver.hpp"

using namespace pistis::logging;
using namespace pistis::log4cxx;

const Log4CxxLogLevelTranslator Log4CxxLogMessageReceiver::_LOG4CXX_LEVEL;

Log4CxxLogMessageReceiver::Log4CxxLogMessageReceiver(
    LogMessageFactory* factory,
    const ::log4cxx::LoggerPtr& logger
):
    _factory(factory), _logger(logger) {
  // Intentionally left blank
}

void Log4CxxLogMessageReceiver::receive(LogMessage* msg) {
  if (msg) {
    if (!msg->empty()) {
      _write(msg->logLevel(), msg->begin(), msg->end());
    }
    _factory->release(msg);
  }
}

void Log4CxxLogMessageReceiver::setLogLevel(LogLevel logLevel) {
  _logger->setLevel(_LOG4CXX_LEVEL[logLevel]);
}

void Log4CxxLogMessageReceiver::_write(LogLevel level, const char* begin,
				       const char* end) {
  _logger->log(_LOG4CXX_LEVEL[level], std::string(begin, end));
}
