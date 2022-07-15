#include "Player.h"
void Player::doMove(std::string in, std::string out)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::string commandLine = path + " " + in + " " + out;
    // Start the child process. 
    if (!CreateProcess(path.c_str(),   // No module name (use command line)
        &commandLine[0],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Wait until child process exits.
    timeForMove.restart();
    WaitForSingleObject(pi.hProcess, INFINITE);
    timeLeft -= timeForMove.getElapsedTime();
    if (timeLeft.asSeconds() < 0)
    {
        throw(std::string("Time over"));
    }
    /* while (WaitForSingleObject(pi.hProcess, 1))
     {
         PROCESS_MEMORY_COUNTERS pmc;
         if (GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc)))
         {
             printf("\tWorkingSetSize: 0x%08X - %u\n", pmc.WorkingSetSize,
                 pmc.WorkingSetSize / 1024);
             printf("\tQuotaPeakPagedPoolUsage: 0x%08X - %u\n",
                 pmc.QuotaPeakPagedPoolUsage, pmc.QuotaPeakPagedPoolUsage / 1024);
             printf("\tQuotaPagedPoolUsage: 0x%08X - %u\n", pmc.QuotaPagedPoolUsage,
                 pmc.QuotaPagedPoolUsage / 1024);
             printf("\tQuotaPeakNonPagedPoolUsage: 0x%08X - %u\n",
                 pmc.QuotaPeakNonPagedPoolUsage, pmc.QuotaPeakNonPagedPoolUsage / 1024);
             printf("\tQuotaNonPagedPoolUsage:0x%08X-%u\n", pmc.QuotaNonPagedPoolUsage, pmc.QuotaNonPagedPoolUsage / 1024);
             printf("\tPagefileUsage: 0x%08X - %u\n", pmc.PagefileUsage, pmc.PagefileUsage / 1024);
             printf("\tPeakPagefileUsage: 0x%08X - %u\n", pmc.PeakPagefileUsage, pmc.PeakPagefileUsage / 1024);
             printf("\tcb: 0x%08X - %u\n", pmc.cb, pmc.cb / 1024);
         }
     }
     */
     // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

std::string generateChess960()
{
	std::string FENBlack = "--------";
	std::string FENWhite = "--------";
	std::string middle = "/pppppppp/8/8/8/8/PPPPPPPP/";
	std::string info = " w --";
	std::mt19937 mt(time(nullptr));
	std::uniform_int_distribution <int> bishop(0, 3);
	unsigned int firstBishop = bishop(mt) * 2;
	unsigned int secondBishop = bishop(mt) * 2 + 1;
	FENBlack[firstBishop] = 'b';
	FENBlack[secondBishop] = 'b';
	FENWhite[firstBishop] = 'B';
	FENWhite[secondBishop] = 'B';
	std::uniform_int_distribution <int> queen(0, 5);
	unsigned int posQueen = queen(mt);
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= posQueen)
		{
			++posQueen;
		}
	}
	FENBlack[posQueen] = 'q';
	FENWhite[posQueen] = 'Q';
	std::uniform_int_distribution <int> knight1(0, 3);
	std::uniform_int_distribution <int> knight2(0, 2);
	unsigned int firstKnight = knight1(mt);
	unsigned int secondKnight = knight2(mt);
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= firstKnight)
		{
			++firstKnight;
		}
	}
	FENBlack[firstKnight] = 'n';
	FENWhite[firstKnight] = 'N';
	for (int pos = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] != '-' && pos <= secondKnight)
		{
			++secondKnight;
		}
	}
	FENBlack[secondKnight] = 'n';
	FENWhite[secondKnight] = 'N';
	for (int pos = 0, freeSq = 0; pos < 8; ++pos)
	{
		if (FENBlack[pos] == '-')
		{
			if (freeSq != 1)
			{
				FENBlack[pos] = 'r';
				FENWhite[pos] = 'R';
			}
			else
			{
				FENBlack[pos] = 'k';
				FENWhite[pos] = 'K';
			}
			++freeSq;
		}
	}

	return FENBlack + middle + FENWhite + info;
}