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
	/** Output filestream. */
	std::ofstream logF;
	/** Logging mode. */
	unsigned int loggingMode;
	/** Deque of messages. */
	std::deque<std::string> messageDeque;
	/** Thread for file writting. */
	std::thread writerThread;
	/** Lock for shared ressources. */
	std::mutex mtx;
	/** Thread synchronization variable. */
	std::condition_variable cv;
	/** Logger status. */
	bool isRunning;

protected:
	/** Write message from the deque to the file. */
	void writeToFile();

public:
	/** Constructor. */
	Logger();
	/** Constructor. */
	Logger(std::string filepath);
	/** Open a file for writing the log.
	 * \param filepath the path to the log file.
	 */
	void open(std::string filepath);
	/** Destructor. */
	~Logger();
	/** Close the logger. */
	void close();
	/** Log a message
	 * \param msg the message to write in the log.
	 */
	void log(std::string msg);
	/** Set the logging mode.
	 * \param mode the logging mode. 1 for file+console, 0 for console only.
	 */
	void setLogging(unsigned int mode);

};

//eof