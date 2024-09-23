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

#include <levels_ranks_plugin.hpp>

LevelsRanksPlugin::CPlayer::CPlayer()
 :  m_pLanguage(nullptr), 
    m_aYourArgumentPhrase({nullptr, nullptr})
{
}

void LevelsRanksPlugin::CPlayer::Init()
{
	m_pLanguage = nullptr;
	m_aYourArgumentPhrase = {nullptr, nullptr};
}

void LevelsRanksPlugin::CPlayer::Destroy()
{
	Init();
}

const ILevelsRanks::ILanguage *LevelsRanksPlugin::CPlayer::GetLanguage() const
{
	return m_pLanguage;
}

void LevelsRanksPlugin::CPlayer::SetLanguage(const ILanguage *pData)
{
	m_pLanguage = pData;
}

bool LevelsRanksPlugin::CPlayer::AddLanguageListener(const LanguageHandleCallback_t *pfnCallback)
{
	int iFound = m_vecLanguageCallbacks.Find(pfnCallback);

	bool bIsExists = m_vecLanguageCallbacks.IsValidIndex(iFound);

	if(bIsExists)
	{
		m_vecLanguageCallbacks.AddToTail(pfnCallback);
	}

	return bIsExists;
}

bool LevelsRanksPlugin::CPlayer::RemoveLanguageListener(const LanguageHandleCallback_t *pfnCallback)
{
	return m_vecLanguageCallbacks.FindAndRemove(pfnCallback);
}

void LevelsRanksPlugin::CPlayer::OnLanguageReceived(CPlayerSlot aSlot, CLanguage *pData)
{
	SetLanguage(pData);

	for(const auto &it : m_vecLanguageCallbacks)
	{
		(*it)(aSlot, pData);
	}
}

void LevelsRanksPlugin::CPlayer::TranslatePhrases(const Translations *pTranslations, const CLanguage &aServerLanguage, CUtlVector<CUtlString> &vecMessages)
{
	const struct
	{
		const char *pszName;
		TranslatedPhrase *pTranslated;
	} aPhrases[] =
	{
		{
			"Your argument",
			&m_aYourArgumentPhrase,
		}
	};

	const Translations::CPhrase::CContent *paContent;

	Translations::CPhrase::CFormat aFormat;

	int iFound {};

	const auto *pLanguage = GetLanguage();

	const char *pszServerContryCode = aServerLanguage.GetCountryCode(), 
	           *pszContryCode = pLanguage ? pLanguage->GetCountryCode() : pszServerContryCode;

	for(const auto &aPhrase : aPhrases)
	{
		const char *pszPhraseName = aPhrase.pszName;

		if(pTranslations->FindPhrase(pszPhraseName, iFound))
		{
			const auto &aTranslationsPhrase = pTranslations->GetPhrase(iFound);

			if(!aTranslationsPhrase.Find(pszContryCode, paContent) && !aTranslationsPhrase.Find(pszServerContryCode, paContent))
			{
				CUtlString sMessage;

				sMessage.Format("Not found \"%s\" country code for \"%s\" phrase\n", pszContryCode, pszPhraseName);
				vecMessages.AddToTail(sMessage);

				continue;
			}

			aPhrase.pTranslated->m_pFormat = &aTranslationsPhrase.GetFormat();
		}
		else
		{
			CUtlString sMessage;

			sMessage.Format("Not found \"%s\" phrase\n", pszPhraseName);
			vecMessages.AddToTail(sMessage);

			continue;
		}

		if(!paContent->IsEmpty())
		{
			aPhrase.pTranslated->m_pContent = paContent;
		}
	}
}

const LevelsRanksPlugin::CPlayer::TranslatedPhrase &LevelsRanksPlugin::CPlayer::GetYourArgumentPhrase() const
{
	return m_aYourArgumentPhrase;
}

const ILevelsRanks::ILanguage *LevelsRanksPlugin::GetServerLanguage() const
{
	return &m_aServerLanguage;
}
