/** @file Log4CxxLogFactoryImplTests.cpp
 *
 *  Unit tests for pistis::log4cxx::Log4CxxLogFactoryImpl and indirectly,
 *  the entire logging system.
 */
#include <pistis/logging/LogFactory.hpp>
#include <gtest/gtest.h>

using namespace pistis::logging;

TEST(Log4CxxLogFactoryImplTests, EndToEndLoggingTest) {
  // Eventually, we'll configure the logging system to write to a file
  // and read back the result.  For now, just print to stdout.
  Log* log= LogFactory::getLog("some.destination");
  ASSERT_TRUE(log);
  log->trace() << "This is the trace level" << std::endl;
  log->debug() << "This is the debug level" << std::endl;
  log->info() << "This is the info level" << std::endl;
  log->warn() << "This is the warn level" << std::endl;
  log->error() << "This is the error level" << std::endl;
}
