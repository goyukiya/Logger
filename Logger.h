#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * Logger object definition.
 * 
 * Simple logging class. 
 */
class Logger
{
	/** output filestream. */
	std::ofstream logF;
	/** logging mode. */
	unsigned int loggingMode;
	/** deque of messages. */
	std::deque<std::string> messageDeque;
	/** thread for file writting. */
	std::thread writerThread;
	/** lock for shared ressources. */
	std::mutex mtx;
	/** thread synchronization variable. */
	std::condition_variable cv;
	/** logger status. */
	bool isRunning;

protected:
	/** write message from the deque to the file. */
	void writeToFile();

public:
	/** constructor 
	 * \param filepath the path to the log file.
	 */
	Logger(std::string filepath);
	/* destructor. */
	~Logger();
	/** close the logger. */
	void close();
	/** log a message
	 * \param msg the message to write in the log.
	 */
	void log(std::string msg);
	/** set the logging mode.
	 * \param mode the logging mode. 1 for file+console, 0 for console only.
	 */
	void setLogging(unsigned int mode);

};

//eof