#include <cstdlib>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <sstream>

#include "Poco/AsyncChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FormattingChannel.h"
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/SimpleFileChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/SplitterChannel.h"

#define COUNT 100
#define DELAY_MS 1000
#define DELAY_S 1 * 1000 * DELAY_MS
#define LOGGER Poco::Logger::root()

// Poco::Logger &LOGGER = Poco::Logger::get("root");

// #define DEBUG(msg) poco_debug(LOGGER, msg);
// #define INFO(msg) poco_information(LOGGER, msg);
// #define NOTICE(msg) poco_notice(LOGGER, msg);
// #define WARNING(msg) poco_warning(LOGGER, msg);
// #define ERROR(msg) poco_error(LOGGER, msg);
// #define CRITICAL(msg) poco_critical(LOGGER, msg);
// #define FATAL(msg) poco_fatal(LOGGER, msg);

#define DEBUG(msg) { std::ostringstream oss; oss << msg; poco_debug(LOGGER, oss.str()); }
#define INFO(msg) { std::ostringstream oss; oss << msg; poco_information(LOGGER, oss.str()); }
#define NOTICE(msg) { std::ostringstream oss; oss << msg; poco_notice(LOGGER, oss.str()); }
#define WARNING(msg) { std::ostringstream oss; oss << msg; poco_warning(LOGGER, oss.str()); }
#define ERROR(msg) { std::ostringstream oss; oss << msg; poco_error(LOGGER, oss.str()); }
#define CRITICAL(msg) { std::ostringstream oss; oss << msg; poco_critical(LOGGER, oss.str()); }
#define FATAL(msg) { std::ostringstream oss; oss << msg; poco_fatal(LOGGER, oss.str()); }

using Poco::AsyncChannel;
using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::FormattingChannel;
using Poco::Logger;
using Poco::PatternFormatter;
using Poco::SimpleFileChannel;
using Poco::FileChannel;
using Poco::SplitterChannel;

void log_thread_1()
{
    // Logger &logger1 = Logger::get("TestLogger1");

    for (int i = 0; i < COUNT; ++i)
    {
        // poco_information(logger1, "log_thread_1 information");
        INFO("log_thread_1 information " << i);
        usleep(DELAY_S);
    }
}

void log_thread_2()
{
    // Logger &logger2 = Logger::get("TestLogger2");

    for (int i = 0; i < COUNT; ++i)
    {
        // poco_information(logger2, "log_thread_2 information");
        INFO("log_thread_2 information " << i);
        usleep(DELAY_S);
    }
}

int main(int argc, char **argv)
{
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    // AutoPtr<SimpleFileChannel> pFile(new SimpleFileChannel("test.log"));
    AutoPtr<FileChannel> pFile(new FileChannel("test.log"));
    AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);

    pFile->setProperty("rotation", "1 K");  // Rotate when log file size exceeds 1 MB
    pFile->setProperty("archive", "timestamp");  // Archive old log files with a timestamp
    pFile->setProperty("times", "local");
    pFile->setProperty("compress", "true");
    pFile->setProperty("purgeCount", "3");

    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", "%L%Y-%m-%d %H:%M:%S.%i %P %J %O %u %q: %t");

    AutoPtr<FormattingChannel> pPFC1(new FormattingChannel(pPF, pCons));
    AutoPtr<FormattingChannel> pPFC2(new FormattingChannel(pPF, pFile));

    pSplitter->addChannel(pPFC1);
    pSplitter->addChannel(pPFC2);
    // pSplitter->addChannel(pCons);
    // pSplitter->addChannel(pFile);

    AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pSplitter));

    LOGGER.setChannel(pAsync);
    // LOGGER.setChannel(pSplitter);
    LOGGER.setLevel(Poco::Message::PRIO_DEBUG);

    // These below callers do not print out the file name and line number
    // LOGGER.debug("debug");
    // LOGGER.information("information");
    // LOGGER.notice("notice");
    // LOGGER.warning("warning");
    // LOGGER.error("error");
    // LOGGER.critical("critical");
    // LOGGER.fatal("fatal");

    DEBUG("debug");
    INFO("information");
    NOTICE("notice");
    WARNING("warning");
    ERROR("error");
    CRITICAL("critical");
    FATAL("fatal");

    std::thread thread_1(log_thread_1);
    std::thread thread_2(log_thread_2);

    thread_1.join();
    thread_2.join();

    return 0;
}
