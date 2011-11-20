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
SDName: Boss_Krikthir
SD%Complete: 90%
SDComment:
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SAY_AGGRO                       = -1601000,
    SAY_KILL_1                      = -1601001,
    SAY_KILL_2                      = -1601002,
    SAY_KILL_3                      = -1601003,
    SAY_PREFIGHT_1                  = -1601007,
    SAY_PREFIGHT_2                  = -1601008,
    SAY_PREFIGHT_3                  = -1601009,
    SAY_SWARM_1                     = -1601010,
    SAY_SWARM_2                     = -1601011,
    SAY_DEATH                       = -1601012,
    EMOTE_BOSS_GENERIC_FRENZY       = -1000005,

    SPELL_CURSE_OF_FATIGUE          = 52592,
    SPELL_CURSE_OF_FATIGUE_H        = 59368,
    SPELL_MIND_FLAY                 = 52586,
    SPELL_MIND_FLAY_H               = 59367,
    SPELL_ENRAGE                    = 28747,

    NPC_SWARM                       = 28735
};
/*######
## boss_krikthir
######*/

struct MANGOS_DLL_DECL boss_krikthirAI : public ScriptedAI
{
    boss_krikthirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiCurseTimer;
    uint32 m_uiMindFlayTimer;
    uint32 m_uiSwarmTimer;

    void Reset()
    {
        m_uiCurseTimer = 20000;
        m_uiMindFlayTimer = 10000;
        m_uiSwarmTimer = urand(6000, 10000);

        m_pInstance->SetData(TYPE_KRIKTHIR, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_pInstance->SetData(TYPE_KRIKTHIR, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KRIKTHIR, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_creature->GetHealthPercent() < 20)
            DoCastSpellIfCan(m_creature, SPELL_ENRAGE, CAST_AURA_NOT_PRESENT);

        if (m_uiCurseTimer < uiDiff)
        {    
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CURSE_OF_FATIGUE: SPELL_CURSE_OF_FATIGUE_H) == CAST_OK)
                m_uiCurseTimer = urand(11000, 13000);
        }
        else
            m_uiCurseTimer -= uiDiff;

        if (m_uiMindFlayTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_MIND_FLAY : SPELL_MIND_FLAY_H) == CAST_OK)
                    m_uiMindFlayTimer = urand(14000, 18000);
            }
        }
        else
            m_uiMindFlayTimer -= uiDiff;

        if (m_uiSwarmTimer < uiDiff)
        {
            if (!urand(0, 1))
            {
                switch(urand(0, 1))
                {
                    case 0: DoScriptText(SAY_SWARM_1, m_creature); break;
                    case 1: DoScriptText(SAY_SWARM_2, m_creature); break;
                }
            }

            int i;
            i = 0;
            do 
            {
                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (!pTarget)
                    return;

                float x = pTarget->GetPositionX() + urand(3.0f, 15.0f);
                float y = pTarget->GetPositionY() + urand(3.0f, 15.0f);

                if (Creature* pSwarm = m_creature->SummonCreature(NPC_SWARM, x, y, pTarget->GetPositionZ(), pTarget->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                    pSwarm->SetInCombatWith(pTarget);
                i++;
            }while (m_bIsRegularMode ? i <= 5 : i <= 12);
            m_uiSwarmTimer = urand(3000, 5000);
        }else m_uiSwarmTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_krikthir(Creature* pCreature)
{
    return new boss_krikthirAI(pCreature);
}

void AddSC_boss_krikthir()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_krikthir";
    pNewScript->GetAI = &GetAI_boss_krikthir;
    pNewScript->RegisterSelf();
}
