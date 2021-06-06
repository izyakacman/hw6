#include "command.h"

#include <chrono>

/**
*	Обработка команды в режиме статических блоков
*/
bool StaticCommandHandler::ProcessCommand(Command* cmd, const std::string& s) 
{
	if (s != EndOfFileString)
	{
		if (s == "{")
		{
			cmd->PrintPool();
			cmd->SetCurrent(ICommandHandlerPtr{ new DinamicCommandHandler(m_count) });
		}
		else
		{
			cmd->PushPool(s);

			if (cmd->GetPoolSize() == m_count)
			{
				cmd->PrintPool();
			}
		}

		return true;
	}
	else
	{
		cmd->PrintPool();
		return false;
	}
}

/**
*	Обработка команды в режиме динамических блоков
*/
bool DinamicCommandHandler::ProcessCommand(Command* cmd, const std::string& s)
{
	if (s != EndOfFileString)
	{
		if (s == "{")
		{
			++m_open_brace_count;
		}
		else
		if (s == "}")
		{
			if (m_open_brace_count == 0)
			{
				cmd->PrintPool();
				cmd->SetCurrent(ICommandHandlerPtr{ new StaticCommandHandler(m_count) });
			}
			else
			{
				--m_open_brace_count;
			}
		}
		else
		{
			cmd->PushPool(s);
		}

		return true;
	}
	else
	{
		return false;
	}
}

Command::Command(int count) : m_count{ count }
{
	m_handler = ICommandHandlerPtr{ new StaticCommandHandler(m_count) };
}

/**
*	Добавление команды в блок команд
*/
void Command::PushPool(const std::string& s)
{
	if (m_pool.size() == 0)
	{
		m_first_cmd_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	m_pool.push_back(s);
}

/**
*	Вывод строки в консоль и в файл
*/
void Command::PrintString(std::fstream& f, const std::string& s)
{
	std::cout << s;
	f << s;
}

/**
*	Вывод блока команд
*/
void Command::PrintPool()
{
	if (m_pool.size())
	{
		std::fstream file_stream("bulk" + std::to_string(m_first_cmd_time) + ".log", std::ios::out);

		PrintString(file_stream, "bulk: ");

		for(int i=0; i < m_pool.size(); ++i)
		{
			PrintString(file_stream, m_pool[i]);

			if (i != m_pool.size() - 1)
			{
				PrintString(file_stream, ", ");
			}
		}
		PrintString(file_stream, "\n");

		m_pool.clear();
	}
}
