/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_Gluth
SD%Complete: 80
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_ZOMBIE                = -1533119,

    SPELL_MORTALWOUND           = 25646,
    SPELL_DECIMATE              = 28374,
    SPELL_ENRAGE                = 28371,
    SPELL_ENRAGE_H              = 54427,
    SPELL_BERSERK               = 26662,
    SPELL_ZOMBIE_CHOW_SEARCH    = 28404,            //don't work. Onehit for Player

    NPC_ZOMBIE_CHOW             = 16360
};

const float ADD_SPAWN[5][3] =
{
    {3269.5f, -3161.2f, 297.4f},
    {3277.0f, -3190.1f, 297.4f},
    {3316.0f, -3188.6f, 297.4f},
    {3247.0f, -3139.2f, 297.4f},
    {3258.0f, -3121.1f, 297.4f}
};
struct MANGOS_DLL_DECL boss_gluthAI : public ScriptedAI
{
    boss_gluthAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiMortalWoundTimer;
    uint32 m_uiDecimateTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiSummonTimer;

    uint32 m_uiBerserkTimer;

    uint32 m_uiRangeCheck_Timer;
    std::list<uint64> m_lZombieGUIDList;

    void Reset()
    {
        m_uiMortalWoundTimer = 8000;
        m_uiDecimateTimer    = 110000;
        m_uiEnrageTimer      = 30000;
        m_uiSummonTimer      = 10000;
        m_uiBerserkTimer     = 7*MINUTE*IN_MILLISECONDS;

        m_uiRangeCheck_Timer = 1000;
        m_lZombieGUIDList.clear();
        if (!m_lZombieGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                if (Creature* pTemp = (Creature*)m_creature->GetMap()->GetUnit( *itr))
                    if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        pTemp->ForcedDespawn();
        }
        m_lZombieGUIDList.clear();
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, DONE);

        std::list<Creature*> pZombies;
        GetCreatureListWithEntryInGrid(pZombies, m_creature, NPC_ZOMBIE_CHOW, DEFAULT_VISIBILITY_INSTANCE);

        if (!pZombies.empty())
            for(std::list<Creature*>::iterator itr = pZombies.begin(); itr != pZombies.end(); ++itr)
                (*itr)->ForcedDespawn();
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GLUTH, FAIL);
    }

    void JustSummoned(Creature* summoned)
    {
        //summoned->SetSpeedRate(MOVE_RUN, 0.5f);
        //summoned->SetSpeedRate(MOVE_WALK, 0.5f);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Enrage
        if (m_uiEnrageTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ENRAGE : SPELL_ENRAGE_H);
            m_uiEnrageTimer = 20000;
        }else m_uiEnrageTimer -= uiDiff;

        // Berserk
        if (m_uiBerserkTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK);
            m_uiBerserkTimer = 300000;
        }else m_uiBerserkTimer -= uiDiff;

        // Mortal Wound
        if (m_uiMortalWoundTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_MORTALWOUND);
            m_uiMortalWoundTimer = 10000;
        }else m_uiMortalWoundTimer -= uiDiff;

        //Decimate_Timer
        if (m_uiDecimateTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(),SPELL_DECIMATE); // need core support

            // workaround below
            std::list<HostileReference*> t_list = m_creature->getThreatManager().getThreatList();
            if (t_list.size())
            {
                //begin + 1 , so we don't target the one with the highest threat
                std::list<HostileReference*>::iterator itr = t_list.begin();
                for(; itr!= t_list.end(); ++itr)
                {
                    Unit *target = m_creature->GetMap()->GetUnit( (*itr)->getUnitGuid());
                    if (target && target->isAlive() && target->GetTypeId() == TYPEID_PLAYER &&
                        (target->GetHealth() > target->GetMaxHealth() * 0.05))
                        target->SetHealth(target->GetMaxHealth() * 0.05);
                }
            }
            // Move Zombies
            if (!m_lZombieGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)m_creature->GetMap()->GetUnit( *itr))
                        if (pTemp->isAlive())
                        {
                            if (m_creature->GetHealth() > m_creature->GetMaxHealth() * 0.05) // remove when SPELL_DECIMATE is working
                                pTemp->SetHealth(pTemp->GetMaxHealth() * 0.05);
                            pTemp->AddThreat(m_creature, 1000000000.0f); // force move toward to Gluth
                        }
            }
            m_uiDecimateTimer = 104000;
        }else m_uiDecimateTimer -= uiDiff;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (!m_lZombieGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lZombieGUIDList.begin(); itr != m_lZombieGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)m_creature->GetMap()->GetUnit( *itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            DoScriptText(EMOTE_ZOMBIE, m_creature);
                            m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.05);
                            pTemp->ForcedDespawn();
                        }
            }
            m_uiRangeCheck_Timer = 1000;
        }else m_uiRangeCheck_Timer -= uiDiff;

        // Summon
        if (m_uiSummonTimer < uiDiff)
        {
            SummonZombie();

            if (!m_bIsRegularMode)
            {
                SummonZombie();
            }

            m_uiSummonTimer = 10000;
        }else m_uiSummonTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
    void SummonZombie()
    {
        uint32 ran = rand()%5;
        if (Creature* pZombie = m_creature->SummonCreature(NPC_ZOMBIE_CHOW, ADD_SPAWN[ran][0], ADD_SPAWN[ran][1], ADD_SPAWN[ran][2], 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 80000))
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                pZombie->AddThreat(pTarget);
                m_lZombieGUIDList.push_back(pZombie->GetGUID());
            }
        }
    }

};

CreatureAI* GetAI_boss_gluth(Creature* pCreature)
{
    return new boss_gluthAI(pCreature);
}

void AddSC_boss_gluth()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_gluth";
    NewScript->GetAI = &GetAI_boss_gluth;
    NewScript->RegisterSelf();
}
