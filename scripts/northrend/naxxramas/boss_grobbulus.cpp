/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Grobbulus
SD%Complete: 80
SDComment: Timer need more care; Spells of Adds (Posion Cloud) need Mangos Fixes, and further handling
SDCategory: Naxxramas
EndScriptData */

/*Poison Cloud 26590
Slime Spray 28157
Fallout slime 28218
Mutating Injection 28169
Enrages 26527*/

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    EMOTE_SPRAY_SLIME               = -1533021,
    EMOTE_INJECTION                 = -1533158,

    SPELL_SLIME_STREAM              = 28137,
    SPELL_MUTATING_INJECTION        = 28169,
    SPELL_POISON_CLOUD              = 28240,
    SPELL_SLIME_SPRAY               = 28157,
    SPELL_SLIME_SPRAY_H             = 54364,
    SPELL_BERSERK                   = 26662,

    NPC_FALLOUT_SLIME               = 16290,
    MOB_GROBBOLUS_CLOUD             = 16363,

    SPELL_POISON_CLOUD_DAMAGE       = 59116,
};

struct MANGOS_DLL_DECL boss_grobbulusAI : public ScriptedAI
{
    boss_grobbulusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiInjectionTimer;
    uint32 m_uiPoisonCloudTimer;
    uint32 m_uiSlimeSprayTimer;
    uint32 m_uiBerserkTimeSecs;
    uint32 m_uiBerserkTimer;
    uint32 m_uiSlimeStreamTimer;

    void Reset()
    {
        Despawnall();
        m_uiInjectionTimer = 12*IN_MILLISECONDS;
        m_uiPoisonCloudTimer = urand (20*IN_MILLISECONDS, 25*IN_MILLISECONDS);
        m_uiSlimeSprayTimer = urand(20*IN_MILLISECONDS, 30*IN_MILLISECONDS);
        m_uiBerserkTimeSecs = m_bIsRegularMode ? 12*MINUTE : 9*MINUTE;
        m_uiBerserkTimer = m_uiBerserkTimeSecs*IN_MILLISECONDS;
        m_uiSlimeStreamTimer = 5*IN_MILLISECONDS;           // The first few secs it is ok to be out of range
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, IN_PROGRESS);
    }

    void JustDied(Unit* pKiller)
    {
        Despawnall();
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_GROBBULUS, FAIL);
    }

    // This custom selecting function, because we only want to select players without mutagen aura
    bool DoCastMutagenInjection()
    {
        if (m_creature->IsNonMeleeSpellCasted(true))
            return false;

        std::vector<Unit*> suitableTargets;
        ThreatList const& threatList = m_creature->getThreatManager().getThreatList();
        ThreatList::const_iterator itr = threatList.begin();

        for (itr; itr != threatList.end(); ++itr)
        {
            if (Unit* pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER && !pTarget->HasAura(SPELL_MUTATING_INJECTION))
                    suitableTargets.push_back(pTarget);
            }
        }

        if (suitableTargets.empty())
            return false;

        Unit* pTarget = suitableTargets[urand(0, suitableTargets.size() - 1)];
        if (DoCastSpellIfCan(pTarget, SPELL_MUTATING_INJECTION) == CAST_OK)
        {
            DoScriptText(EMOTE_INJECTION, m_creature, pTarget);
            return true;
        }
        else
            return false;
    }
    
    void Despawnall()
    {
        std::list<Creature*> m_pCloud;
        GetCreatureListWithEntryInGrid(m_pCloud, m_creature, MOB_GROBBOLUS_CLOUD, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pCloud.empty())
        for(std::list<Creature*>::iterator itr = m_pCloud.begin(); itr != m_pCloud.end(); ++itr)
        {
            (*itr)->ForcedDespawn();
        }

        std::list<Creature*> m_pSpray;
        GetCreatureListWithEntryInGrid(m_pSpray, m_creature, NPC_FALLOUT_SLIME, DEFAULT_VISIBILITY_INSTANCE);

        if (!m_pSpray.empty())
        for(std::list<Creature*>::iterator iter = m_pSpray.begin(); iter != m_pSpray.end(); ++iter)
        {
            (*iter)->ForcedDespawn();
        } 
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_SLIME_SPRAY)
        {
            SpawnFalloutSlime(target);
        }
        else if(spell->Id == SPELL_SLIME_SPRAY_H)
        {
            SpawnFalloutSlime(target);
            SpawnFalloutSlime(target);
        }
    }
    void SpawnFalloutSlime(Unit *target)
    {
        if (Creature* pTemp = m_creature->SummonCreature(NPC_FALLOUT_SLIME, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 1000))
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
            {
                pTemp->AddThreat(pTarget,0.0f);
                pTemp->AI()->AttackStart(pTarget);
            }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Slime Stream
        if (!m_uiSlimeStreamTimer)
        {
            if (!m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
            {
                if (DoCastSpellIfCan(m_creature, SPELL_SLIME_STREAM) == CAST_OK)
                    // Give some time, to re-reach grobbulus
                    m_uiSlimeStreamTimer = 3*IN_MILLISECONDS;
            }
        }
        else
        {
            if (m_uiSlimeStreamTimer < uiDiff)
                m_uiSlimeStreamTimer = 0;
            else
                m_uiSlimeStreamTimer -= uiDiff;
        }

        // Berserk
        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                    m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        // SlimeSpray
        if (m_uiSlimeSprayTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SLIME_SPRAY : SPELL_SLIME_SPRAY_H) == CAST_OK)
            {
                m_uiSlimeSprayTimer = urand(30*IN_MILLISECONDS, 60*IN_MILLISECONDS);
                DoScriptText(EMOTE_SPRAY_SLIME, m_creature);
            }
        }
        else
            m_uiSlimeSprayTimer -= uiDiff;

        // Mutagen Injection
        if (m_uiInjectionTimer < uiDiff)
        {
            if (DoCastMutagenInjection())
            {
                // Timer dependend on time of encounter  - on enrage time between 5-8s, heroic 2-5s (TODO no reliable source for heroic)
                if (m_bIsRegularMode)
                    m_uiInjectionTimer = urand(10*IN_MILLISECONDS, 13*IN_MILLISECONDS) -  5 * (m_uiBerserkTimeSecs*IN_MILLISECONDS - m_uiBerserkTimer) / m_uiBerserkTimeSecs;
                else
                    m_uiInjectionTimer = urand(10*IN_MILLISECONDS, 13*IN_MILLISECONDS) -  8 * (m_uiBerserkTimeSecs*IN_MILLISECONDS - m_uiBerserkTimer) / m_uiBerserkTimeSecs;
            }
        }
        else
            m_uiInjectionTimer -= uiDiff;

        // Poison Cloud
        if (m_uiPoisonCloudTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_POISON_CLOUD) == CAST_OK)
                m_uiPoisonCloudTimer = 15*IN_MILLISECONDS;
        }
        else
            m_uiPoisonCloudTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_grobbulus_poison_cloudAI : public Scripted_NoMovementAI
{
    npc_grobbulus_poison_cloudAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        Reset();
    }

    uint32 Cloud_Timer;

    void Reset()
    {
        Cloud_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Cloud_Timer < diff)
        {
            DoCast(m_creature, SPELL_POISON_CLOUD_DAMAGE);
            Cloud_Timer = 10000;
        }else Cloud_Timer -= diff;
    }
};

CreatureAI* GetAI_boss_grobbulus(Creature* pCreature)
{
    return new boss_grobbulusAI(pCreature);
}

CreatureAI* GetAI_npc_grobbulus_poison_cloud(Creature* pCreature)
{
    return new npc_grobbulus_poison_cloudAI(pCreature);
}

void AddSC_boss_grobbulus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grobbulus";
    newscript->GetAI = &GetAI_boss_grobbulus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_grobbulus_poison_cloud";
    newscript->GetAI = &GetAI_npc_grobbulus_poison_cloud;
    newscript->RegisterSelf();
}

