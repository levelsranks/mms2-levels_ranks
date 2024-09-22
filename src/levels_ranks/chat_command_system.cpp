/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source {project}
 * Written by {name of author} ({fullname}).
 * ======================================================

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <levels_ranks/chat_command_system.hpp>

#include <tier1/utlrbtree.h>

LevelsRanks::ChatCommandSystem::ChatCommandSystem()
 :  Logger(GetName(), NULL, 0, LV_DEFAULT, LEVELS_RANKS_CHAT_COMMAND_SYSTEM_LOGGINING_COLOR), 
    m_mapCallbacks(DefLessFunc(const CUtlSymbolLarge))
{
}

const char *LevelsRanks::ChatCommandSystem::GetName()
{
	return "LevelsRanks - Chat Command System";
}

bool LevelsRanks::ChatCommandSystem::Register(const char *pszName, const Callback_t &fnCallback)
{
	m_mapCallbacks.Insert(m_aSymbolTable.AddString(pszName), fnCallback);

	return true;
}

bool LevelsRanks::ChatCommandSystem::Unregister(const char *pszName)
{
	return m_mapCallbacks.Remove(FindSymbol(pszName));
}

void LevelsRanks::ChatCommandSystem::UnregisterAll()
{
	m_mapCallbacks.Purge();
}

char LevelsRanks::ChatCommandSystem::GetPublicTrigger()
{
	return '!';
}

char LevelsRanks::ChatCommandSystem::GetSilentTrigger()
{
	return '/';
}

bool LevelsRanks::ChatCommandSystem::Handle(CPlayerSlot aSlot, bool bIsSilent, const CUtlVector<CUtlString> &vecArgs)
{
	if(aSlot == -1)
	{
		Message("Type the chat command from root console?\n");

		return false;
	}

	if(!vecArgs.Count())
	{
		if(IsChannelEnabled(LS_DETAILED))
		{
			Detailed("Chat command arguments is empty\n");
		}

		return false;
	}

	const char *pszName = vecArgs[0];

	auto iFoundIndex = m_mapCallbacks.Find(FindSymbol(pszName));

	if(iFoundIndex == m_mapCallbacks.InvalidIndex())
	{
		if(IsChannelEnabled(LS_DETAILED))
		{
			DetailedFormat("Can't be found \"%s\" command\n", pszName);
		}

		return false;
	}

	if(IsChannelEnabled(LS_DETAILED))
	{
		DetailedFormat(u8"Handling \"%s\" command…\n", pszName);
	}

	m_mapCallbacks.Element(iFoundIndex)(aSlot, bIsSilent, vecArgs);

	return true;
}

CUtlSymbolLarge LevelsRanks::ChatCommandSystem::GetSymbol(const char *pszText)
{
	return m_aSymbolTable.AddString(pszText);
}

CUtlSymbolLarge LevelsRanks::ChatCommandSystem::FindSymbol(const char *pszText) const
{
	return m_aSymbolTable.Find(pszText);
}
