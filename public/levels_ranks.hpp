/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Levels Ranks 2
 * Written by Wend4r (Vladimir Ezhikov).
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


#ifndef _INCLUDE_METAMOD_SOURCE_LEVELS_RANKS_HPP_
#	define _INCLUDE_METAMOD_SOURCE_LEVELS_RANKS_HPP_

#	pragma once

#	include <stddef.h>

#	include <functional>

#	include <playerslot.h>

#	define LEVELS_RANKS_INTERFACE_NAME "Levels Ranks 2 v1.0.0"

class CGameEntitySystem;
class CBaseGameSystemFactory;
class IGameEventManager2;

/**
 * @brief A Levels Ranks 2 interface.
 * Note: gets with "ismm->MetaFactory(LEVELS_RANKS_INTERFACE_NAME, NULL, NULL);"
**/
class ILevelsRanks
{
public:
	/**
	 * @brief Gets a game entity system.
	 * 
	 * @return              A double pointer to a game entity system.
	 */
	virtual CGameEntitySystem **GetGameEntitySystemPointer() const = 0;

	/**
	 * @brief Gets a first game system.
	 * 
	 * @return              A double pointer to a first game system.
	 */
	virtual CBaseGameSystemFactory **GetFirstGameSystemPointer() const = 0;

	/**
	 * @brief Gets a game event manager.
	 * 
	 * @return              A double pointer to a game event manager.
	 */
	virtual IGameEventManager2 **GetGameEventManagerPointer() const = 0;

public: // Language ones.
	/**
	 * @brief A player data language interface.
	**/
	class ILanguage
	{
	public:
		/**
		 * @brief Gets a name of a language.
		 * 
		 * @return              Returns a language name.
		 */
		virtual const char *GetName() const = 0;

		/**
		 * @brief Gets a country code of a language.
		 * 
		 * @return              Returns a country code of a language.
		 */
		virtual const char *GetCountryCode() const = 0;
	}; // ILanguage

public:
	using LanguageHandleCallback_t = std::function<void (CPlayerSlot, ILanguage *)>;

public: // Player ones.
	/**
	 * @brief A player data interface.
	**/
	class IPlayer
	{
	public:
		/**
		 * @brief Gets a language.
		 * 
		 * @return              Returns a language, otherwise "nullptr" that not been received.
		 */
		virtual const ILanguage *GetLanguage() const = 0;

		/**
		 * @brief Sets a language to player.
		 * 
		 * @param pData         A language to set.
		 */
		virtual void SetLanguage(const ILanguage *pData) = 0;

		/**
		 * @brief Add a language listener.
		 * 
		 * @param pfnCallback   Callback, who will be called when language has received.
		 * 
		 * @return              Returns true if this has listenen.
		 */
		virtual bool AddLanguageListener(const LanguageHandleCallback_t *pfnCallback) = 0;

		/**
		 * @brief Removes a language listener.
		 * 
		 * @param pfnCallback   A callback to remove a listenen.
		 * 
		 * @return              Returns "true" if this has removed, otherwise
		 *                      "false" if not exists.
		 */
		virtual bool RemoveLanguageListener(const LanguageHandleCallback_t *pfnCallback) = 0;
	}; // IPlayer

	/**
	 * @brief Gets a server language.
	 * 
	 * @return              Returns a server language.
	 */
	virtual const ILanguage *GetServerLanguage() const = 0;

	/**
	 * @brief Gets a language by a name.
	 * 
	 * @param psz           A case insensitive language name.
	 * 
	 * @return              Returns a found language, otherwise
	 *                      "nullptr".
	 */
	virtual const ILanguage *GetLanguageByName(const char *psz) const = 0;

	/**
	 * @brief Gets a player data.
	 * 
	 * @param aSlot         A player slot.
	 * 
	 * @return              Returns a player data.
	 */
	virtual IPlayer *GetPlayerData(const CPlayerSlot &aSlot) = 0;
}; // ILevelsRanks

#endif // _INCLUDE_METAMOD_SOURCE_LEVELS_RANKS_HPP_
