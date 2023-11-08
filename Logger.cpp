#include "Logger.h"

#include <exception>

// ---------------------------------------------------------------------------------
//			Protected methods
// ---------------------------------------------------------------------------------

void Logger::writeToFile()
{
    while (isRunning)
    {
        std::unique_lock<std::mutex> lck(mtx);
        // wait for queued message
        cv.wait(lck, [&]{ return !messageDeque.empty(); });
        // write the message at the front
        std::string msg = messageDeque.front();
        messageDeque.pop_front();
        logF << msg << std::endl;
    }
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

Logger::Logger(): loggingMode(0), isRunning(false)
{}

void Logger::open(std::string filepath)
{
    logF.open(filepath);
    // check if failed
    if (!logF.is_open())
        throw std::exception("Logger:FailedToOpenFile");
    // logging to file
    loggingMode= 1;
    isRunning = true;
    // start thread
    std::thread thr(&Logger::writeToFile, this);
    writerThread = std::move(thr);
    // background task
    writerThread.detach();     
}

Logger::~Logger()
{
    // stop the thread if running
    if (isRunning)
        close();
    // close the filestream if open
    if (logF.is_open())
        logF.close();
}

void Logger::close()
{
    isRunning = false;
}

void Logger::log(std::string msg)
{
    if (isRunning && loggingMode)
    {
        // push the message in the deque with ressource locking
        std::unique_lock<std::mutex> lck(mtx);
        messageDeque.push_back(msg);
        lck.unlock();
        // notify the thread
        cv.notify_one();
    }
    std::cout << msg << std::endl;
}

void Logger::setLogging(unsigned int mode)
{
    loggingMode = (mode > 0) ? 1 : 0;
}

// eof