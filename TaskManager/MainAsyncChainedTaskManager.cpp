#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdlib.h>


class Task;

typedef void(*Func)(Task*);

class Task
{
	public:
	int AlocateNumber;//Dont set it by hand! only for test purposes
	Func currentMethod = nullptr;
};
class TaskManager {
private:
	static const uint8_t NumberOfTasks = 8;
	Task* tasks[NumberOfTasks];
	uint8_t currentTaskNumber;
	public:
TaskManager()
{
	currentTaskNumber = 0;
	for (size_t i = 0; i < NumberOfTasks; i++)
	{
		tasks[i] = nullptr;
	}
}

void UnsafeRegister(Task& task, uint8_t positionIndicator)
{
	tasks[positionIndicator] = &task;
	task.AlocateNumber = positionIndicator;
}

void UnsafeUnregister(Task* task)
{
	tasks[task->AlocateNumber] = nullptr;
}

void GetNext()
{
	++currentTaskNumber;
	if (currentTaskNumber >= NumberOfTasks)
	{
		currentTaskNumber = 0;
	}
}

void Invoke()
{
	if (tasks[currentTaskNumber] != nullptr)
	{
		tasks[currentTaskNumber]->currentMethod(tasks[currentTaskNumber]);
	}
	GetNext();
}

void Register(Task& task)
{
	uint8_t firstFreeIndicator = 0;

	while (firstFreeIndicator < NumberOfTasks)
	{
		if (tasks[firstFreeIndicator] == nullptr)
		{
			UnsafeRegister(task,firstFreeIndicator);
			return;
		}
		++firstFreeIndicator;
	}
}
};