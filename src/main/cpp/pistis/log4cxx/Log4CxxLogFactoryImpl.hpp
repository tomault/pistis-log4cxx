#ifndef __PISTIS__LOG4CXX__LOG4CXXLOGFACTORYIMPL_HPP__
#define __PISTIS__LOG4CXX__LOG4CXXLOGFACTORYIMPL_HPP__

#include <pistis/logging/LogFactoryImpl.hpp>
#include <pistis/logging/LogMessagePool.hpp>
#include <pistis/log4cxx/Log4CxxLog.hpp>
#include <pistis/log4cxx/Log4CxxLogMessageReceiver.hpp>
#include <pistis/log4cxx/Log4CxxLogLevelTranslator.hpp>
#include <log4cxx/helpers/properties.h>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <stddef.h>

namespace pistis {
  namespace log4cxx {

    class Log4CxxLogFactoryImpl : public logging::LogFactoryImpl {
    public:
      Log4CxxLogFactoryImpl(size_t initialMessageSize,
			    size_t maxMessageSize,
			    size_t maxReturnedMessageSize,
			    size_t initialPoolSize,
			    size_t maxPoolSize);
      virtual ~Log4CxxLogFactoryImpl();

      virtual logging::Log* getLog(const std::string& destination) override;
      virtual void setLogLevel(const std::string& destination,
			       logging::LogLevel logLevel) override;

      static Log4CxxLogFactoryImpl* create();

    protected:
      static std::string findConfigurationFile_();
      static void initDefaultConfiguration_();

    private:
      /** @brief Common log message factory */
      logging::LogMessagePool pool_;

      /** @brief Destination -> log map */
      std::unordered_map<std::string, Log4CxxLog*> logs_;

      /** @brief Receivers created by this factory */
      std::vector<Log4CxxLogMessageReceiver*> receivers_;

      /** @brief Translates log4cxx log levels into pistis log levels */
      Log4CxxLogLevelTranslator pistisLogLevel_;

      /** @brief Controls access to critical regions */
      std::mutex sync_;

      /** @brief The default properties for a log */
      static ::log4cxx::helpers::Properties defaultLoggingProperties_;
    };

  }
}


#endif
