#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdlib.h>

class Task;
typedef void(*Action)();

class Task
{
	public:
	int AlocateNumber;//Dont set it by hand! only for test purposes
	Action currentMethod = nullptr;
};


class TaskManager
{
private:
	static const uint8_t NumberOfActions = 8;
	Action actions[NumberOfActions];
	uint8_t currentTask;
public:
	TaskManager()
	{
		currentTask = 0;
		for (size_t i = 0; i < NumberOfActions; i++)
		{
			actions[i] = nullptr;
		}
	}
	void UnsafeRegister(Task& task, uint8_t positionIndicator)
	{
		actions[positionIndicator] = (task.currentMethod);
		task.AlocateNumber = positionIndicator;
	}
	void UnsafeUnregister(Task& task)
	{
		actions[task.AlocateNumber] = nullptr;
		task.AlocateNumber = 255;
	}

	void GetNext()
	{
		++currentTask;
		if (currentTask >= NumberOfActions)
		{
			currentTask = 0;
		}
	}
	void Invoke()
	{
		if (actions[currentTask] != nullptr)
		{
			(actions[currentTask])();
		}
		GetNext();
	}

	void Register(Task& task)
	{
		uint8_t firstFreeIndicator = 0;
		while (firstFreeIndicator < NumberOfActions)
		{
			if (actions[firstFreeIndicator] == nullptr)
			{
				UnsafeRegister(task,firstFreeIndicator);
				return;
			}
			++firstFreeIndicator;
		}
	}
};