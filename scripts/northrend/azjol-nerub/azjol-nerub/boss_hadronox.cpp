/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss_Hadronox
SD%Complete: 60%
SDComment: Just spells implementet; cosmetic mistakes  
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SPELL_LEECH_POISON              = 53030,
    SPELL_LEECH_POISON_H            = 59417,
    SPELL_ARMOR_PENETRATION         = 53418,
    SPELL_ACID_CLOUD                = 53400,
    SPELL_ACID_CLOUD_H              = 59419,
    SPELL_WEB_GRAB                  = 53406, //57731, 53406
    SPELL_WEB_GRAB_H                = 59420, //59420, 59421
};

/*######
## boss_hadronox
######*/

struct MANGOS_DLL_DECL boss_hadronoxAI : public ScriptedAI
{
    boss_hadronoxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiLeechPoisonTimer;
    uint32 m_uiArmorPenetrationTimer;
    uint32 m_uiAcidCloudTimer;
    uint32 m_uiWebGrabTimer;

    void Reset()
    {
        m_uiLeechPoisonTimer        = 2000;
        m_uiArmorPenetrationTimer   = 4000;
        m_uiAcidCloudTimer          = 6000;
        m_uiWebGrabTimer            = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiLeechPoisonTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LEECH_POISON : SPELL_LEECH_POISON_H) == CAST_OK)
                m_uiLeechPoisonTimer = urand(12000, 13000);
        }
        else
            m_uiLeechPoisonTimer -= uiDiff;

        if (m_uiArmorPenetrationTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ARMOR_PENETRATION) == CAST_OK)
                m_uiArmorPenetrationTimer = urand(7000, 11000);
        }
        else
            m_uiArmorPenetrationTimer -= uiDiff;

        if (m_uiAcidCloudTimer < uiDiff)
        {
            if (Unit* pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pVictim, m_bIsRegularMode ? SPELL_ACID_CLOUD : SPELL_ACID_CLOUD_H) == CAST_OK)
                    m_uiAcidCloudTimer = urand(8000, 10000);
            }
        }
        else
            m_uiAcidCloudTimer -= uiDiff;

        if (m_uiWebGrabTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WEB_GRAB : SPELL_WEB_GRAB_H) == CAST_OK)
                m_uiWebGrabTimer = urand(16000, 20000);
        }
        else
            m_uiWebGrabTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_hadronox(Creature* pCreature)
{
    return new boss_hadronoxAI(pCreature);
}

void AddSC_boss_hadronox()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_hadronox";
    pNewScript->GetAI = &GetAI_boss_hadronox;
    pNewScript->RegisterSelf();
}
