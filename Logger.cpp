#include "Logger.h"

#include <exception>

void Logger::writeToFile()
{
	while (isRunning)
	{
		std::unique_lock<std::mutex> lck(mtx);
        // wait for queued message
		cv.wait(lck, [&] { return !messageDeque.empty(); });
        // write the message at the front
		std::string msg = messageDeque.front();
		messageDeque.pop_front();
		logF << msg << std::endl;
	}
}

Logger::Logger(std::string filepath): logF(filepath), loggingMode(1), isRunning(false)
{
    if (logF.is_open())
    {
        isRunning = true;
        // start thread
        std::thread thr(&Logger::writeToFile, this);
        writerThread = std::move(thr);
        writerThread.detach(); // background task
    }
    else throw std::exception("Logger:FailedToOpenFile");
}

Logger::~Logger()
{
    // stop the thread if running
    if (isRunning) close();
    // close the filestream if open
    if (logF.is_open()) logF.close();
}

void Logger::close()
{
    isRunning = false;
}

void Logger::log(std::string msg)
{
    if (loggingMode)
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
    loggingMode = mode;
}

// eof