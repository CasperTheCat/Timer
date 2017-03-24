//////////////////////////////////////////////////////////////////////////
// Timer project
//

//////////////////////////////////////////////////////////////////////////
// DISABLE CRT SECURE WARNING
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#include <thread>
#include <cstdint>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>

//////////////////////////////////////////////////////////////////////////
// Global bool
//
volatile bool bTerminate;

//////////////////////////////////////////////////////////////////////////
// Timer Function
//
void vmTimer(uint64_t durationSeconds, bool repeat = false)
{
	auto start = std::chrono::system_clock::now();
	while (true) {
		if (bTerminate) 
		{
			std::cout << "Terminating thread" << std::endl;
			return;
		}
		
		// GetDuration
		std::chrono::duration<double> duration = std::chrono::system_clock::now() - start;
		if (duration.count() > durationSeconds)
		{
			// Time to dance
			std::cout << "TIMER EXPIRED " << duration.count() << " seconds" << std::endl;
			MessageBox(nullptr, "EVENT TIMER EXPIRED" , "TiMER" ,MB_ICONSTOP);

			// 
			if (!repeat) return;
			start = std::chrono::system_clock::now();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
}

int main(int argc, char **argv)
{
	bTerminate = false;
	// Start a timer, this program never ends
	// So, we can use threading

	std::cout << "Use \"init [duration] [repeat]\"" << std::endl;
	std::string command;
	char* part;
	std::vector<std::thread> threads;
	// Take input from cin
	while (true)
	{
		getline(std::cin, command);
		part = strtok(&command[0], " ");
		if (part && strcmp(part, "init") == 0)
		{
			part = strtok(nullptr, " ");
			auto duration = std::stoul(part);
			part = strtok(nullptr, " ");
			auto repeat = (strcmp(part, "true") == 0);

			std::cout << "Creating a time with " << duration << " delay that does " << (repeat ? "repeat" : "not repeat") << std::endl;
			threads.push_back(std::thread(vmTimer, duration, repeat));
		}
		else if (part && strcmp(part, "exit") == 0) break;
		else
		{
			std::cout << "Invalid command" << std::endl;
		}
	}

	// Kill all timers
	bTerminate = true;
	
	for (uint32_t i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}

	return 0;
}

