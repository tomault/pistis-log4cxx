#include "Log4CxxLogFactoryImpl.hpp"
#include <pistis/filesystem/Path.hpp>
#include <pistis/logging/LogFactory.hpp>
#include <log4cxx/xml/domconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <iostream>
#include <unistd.h>

using namespace pistis::logging;
using namespace pistis::log4cxx;
namespace path = pistis::filesystem::path;
namespace log4cxx_lib = ::log4cxx;

namespace {
  std::string pathToExecutable() {
#ifdef __linux__
    char buffer[1024];
    size_t n= readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    buffer[n]= 0;
    return path::absolutePath(std::string(buffer));
#else
    return std::string();
#endif
  }
}

log4cxx_lib::helpers::Properties
  Log4CxxLogFactoryImpl::defaultLoggingProperties_;

Log4CxxLogFactoryImpl::Log4CxxLogFactoryImpl(size_t initialMessageSize,
					     size_t maxMessageSize,
					     size_t maxReturnedMessageSize,
					     size_t initialPoolSize,
					     size_t maxPoolSize):
  pool_(initialMessageSize, maxMessageSize, maxReturnedMessageSize,
	initialPoolSize, maxPoolSize),
  logs_(), receivers_() {
}

Log4CxxLogFactoryImpl::~Log4CxxLogFactoryImpl() {
  for (auto i= logs_.begin(); i != logs_.end(); ++i) {
    delete i->second;
  }
  for (auto i= receivers_.begin(); i != receivers_.end(); ++i) {
    delete *i;
  }
}

Log* Log4CxxLogFactoryImpl::getLog(const std::string& destination) {
  std::unique_lock<std::mutex> lock(sync_);
  auto i= logs_.find(destination);
  if (i != logs_.end()) {
    return i->second;
  } else {
    ::log4cxx::LoggerPtr logger= ::log4cxx::Logger::getLogger(destination);
    if (!logger) {
      std::cerr << "WARNING:  Could not get logger for destination \""
		<< destination
		<< "\".  Returning null Log instance.  Expect a segfault."
		<< std::endl;
      return nullptr;
    }

    Log4CxxLogMessageReceiver* receiver=
      new Log4CxxLogMessageReceiver(&pool_, logger);
    Log4CxxLog* log= new Log4CxxLog(&pool_, receiver, destination,
				    pistisLogLevel_[logger->getLevel()]);
    receivers_.push_back(receiver);
    logs_.insert(std::make_pair(destination, log));
    return log;
  }
}

void Log4CxxLogFactoryImpl::setLogLevel(const std::string& destination,
					logging::LogLevel logLevel) {
  std::unique_lock<std::mutex> lock(sync_);
  auto i= logs_.find(destination);
  if (i != logs_.end()) {
    i->second->setLogLevel(logLevel);
  }
}

Log4CxxLogFactoryImpl* Log4CxxLogFactoryImpl::create() {
  std::string configFile= findConfigurationFile_();
  if (!configFile.empty()) {
    ::log4cxx::xml::DOMConfigurator::configure(configFile);
  } else {
    // std::cerr << "WARNING: Unable to find log4cxx configuration file.  "
    // 	      << "Using default configuration instead."
    // 	      << std::endl;
    initDefaultConfiguration_();
    ::log4cxx::PropertyConfigurator::configure(defaultLoggingProperties_);
  }
  return new Log4CxxLogFactoryImpl(256, 4*1024*1024, 16*1024, 20, 100);
}

std::string Log4CxxLogFactoryImpl::findConfigurationFile_() {
  try {
    std::string applicationPath(pathToExecutable());
    std::string applicationDir(path::directoryName(applicationPath));
    std::string applicationName(path::baseName(applicationPath));
    std::string p= path::join(applicationDir, applicationName + "-log4cxx.xml");
    const char* logConfigDirVar= getenv("PISTIS_LOG_CONFIG_DIR");
    if (path::isFile(p)) {
      return p;
    }
    
    p= path::join(applicationDir, "log4cxx.xml");
    if (path::isFile(p)) {
      return p;
    }

    if (logConfigDirVar) {
      std::string logConfigDir(logConfigDirVar);
      p= path::join(logConfigDir, applicationName + "-log4cxx.xml");
      if (path::isFile(p)) {
	return p;
      }
      p= path::join(logConfigDir, "log4cxx.xml");
      if (path::isFile(p)) {
	return p;
      }
    }
    return std::string();
  } catch(const std::exception& e) {
    std::cerr << "WARNING: Exception thrown (" << e.what()
	      << ") while searching for logging system configuration file.  "
	      << "Using default configuration."
	      << std::endl;
  } catch(...) {
    std::cerr << "WARNING: Unknown exception thrown while searching for "
	      << "logging system configuration file.  Using default "
	      << "configuration."
	      << std::endl;
  }
  return std::string();
}

void Log4CxxLogFactoryImpl::initDefaultConfiguration_() {
  defaultLoggingProperties_.put("log4j.rootLogger", "TRACE,A1");
  defaultLoggingProperties_.put("log4j.appender.A1",
				"org.apache.log4j.ConsoleAppender");
  defaultLoggingProperties_.put("log4j.appender.A1.layout",
				"org.apache.log4j.PatternLayout");
  defaultLoggingProperties_.put("log4j.appender.A1.layout.ConversionPattern",
				"%d{yyyy/MM/dd HH:mm:ss.SSS} {%t} %c %p %m");
}

namespace pistis {
  namespace logging {
    LogFactoryImpl* createLogFactoryImpl() {
      return Log4CxxLogFactoryImpl::create();
    }
  }
}
