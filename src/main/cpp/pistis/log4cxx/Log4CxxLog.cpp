#include "Log4CxxLog.hpp"

using namespace pistis::logging;
using namespace pistis::log4cxx;

Log4CxxLog::Log4CxxLog(LogMessageFactory* factory,
		       Log4CxxLogMessageReceiver* receiver,
		       const std::string& destination, LogLevel logLevel):
    Log(factory, receiver, destination, logLevel) {
  // Intentionally left blank
}

void Log4CxxLog::setLogLevel(LogLevel logLevel) {
  // TODO: This can cause log messages to be lost if a new message is acquired
  //       and written to the receiver in between the two calls.  Fix this
  //       later by tracking the number of LogStreams active in Log, and
  //       adding a call that blocks further threads from acquiring
  //       LogStreams, a call that waits until the number of outstanding
  //       streams hits zero, and a call that unblocks threads from receiving
  //       messages.
  setLogLevel_(logLevel);
  receiver()->setLogLevel(logLevel);
}
