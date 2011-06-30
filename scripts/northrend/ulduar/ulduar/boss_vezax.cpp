/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_vezax
SD%Complete:
SDComment: mark of the faceless needs core support. Searing flames needs fixing, cannot be interrupted
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO       = -1603350,
    SAY_SLAY1       = -1603351,
    SAY_SLAY2       = -1603352,
    SAY_SURGE       = -1603353,
    SAY_DEATH       = -1603354,
    SAY_BERSERK     = -1603355,
    SAY_HARD        = -1603356,
    EMOTE_VAPORS    = -1603357,
    EMOTE_SURGE     = -1603358,
    EMOTE_ANIMUS    = -1603359,

    SPELL_AURA_OF_DESPAIR       = 62692,
    SPELL_SHADOW_CRASH          = 62660,
    SPELL_MARK_OF_FACELESS      = 63276,
    SPELL_MARK_SIMPHON          = 63278,
    SPELL_SEARING_FLAMES        = 62661,
    SPELL_SURGE_OF_DARKNESS     = 62662,
    SPELL_BERSERK               = 26662, 
    SPELL_SARONITE_BARRIER      = 63364,
    SPELL_SUMMON_ANIMUS         = 63145,
    SPELL_SUMMON_VAPORS         = 63081,

    NPC_SARONITE_VAPOR          = 33488,
    SPELL_SARONITE_VAPORS       = 63323, 

    NPC_SARONITE_ANIMUS         = 33524,
    SPELL_PROFOUND_DARKNESS     = 63420, 
    SPELL_ANIMUS_FORMATION      = 63319,

    ACHIEV_MORNING_SARONITE     = 3181,
    ACHIEV_MORNING_SARONITE_H   = 3188,
};

uint8 m_uiSaroniteVaporCount;

struct MANGOS_DLL_DECL boss_vezaxAI : public ScriptedAI
{
    boss_vezaxAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEnrageTimer;
    uint32 m_uiCrashTimer;
    uint32 m_uiMarkTimer;
    uint32 m_uiFlamesTimer;
    uint32 m_uiSurgeTimer;
    uint32 m_uiSaroniteVaporTimer;
    uint32 m_uiSummonAnimusTimer;
    uint64 m_uiAnimusGUID;

    std::list<Creature*> lVapors;

    bool m_bIsHardMode;
    bool m_bActiveHardMode;
    bool m_bIsAnimusAlive;

    void Reset()
    {
        m_uiEnrageTimer         = 600000; //10 minutes
        m_uiFlamesTimer         = urand(8000, 10000);
        m_uiSaroniteVaporTimer  = 30000;
        m_bIsHardMode           = false;
        m_bActiveHardMode       = false;
        m_bIsAnimusAlive        = false;

        m_uiSurgeTimer          = 60000;
        m_uiMarkTimer           = urand(10000, 35000);
        m_uiCrashTimer          = 10000;
        m_uiAnimusGUID          = 0;

        lVapors.clear();

        m_uiSaroniteVaporCount  = 0;
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance) 
            m_pInstance->SetData(TYPE_VEZAX, IN_PROGRESS);

        DoCast(m_creature, SPELL_AURA_OF_DESPAIR);

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VEZAX, FAIL);

        m_creature->RemoveAurasDueToSpell(SPELL_SARONITE_BARRIER);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance) 
        {
            m_pInstance->SetData(TYPE_VEZAX, DONE);

            if(m_bIsHardMode)
            {
                m_pInstance->SetData(TYPE_VEZAX_HARD, DONE);

                // hacky way to complete achievements; use only if you have this function
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_MORNING_SARONITE : ACHIEV_MORNING_SARONITE_H);
				// hack used when the hard mode loot is within the Animus corpse
				// PLEASE REMOVE FOR REVISION
                if(Creature* pAnimus = m_pInstance->instance->GetCreature(m_uiAnimusGUID))
                    pAnimus->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0,1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void PrepareHardMode()
    {
        m_bActiveHardMode = true;
        m_uiSummonAnimusTimer = 7000;

        GetCreatureListWithEntryInGrid(lVapors, m_creature, NPC_SARONITE_VAPOR, 200.0f);

        if (!lVapors.empty())
        {
			// vapors need more speed
            for(std::list<Creature*>::iterator iter = lVapors.begin(); iter != lVapors.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                {
                    (*iter)->RemoveAllAuras();
                    (*iter)->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    (*iter)->SetSpeedRate(MOVE_RUN, 3.0f);
                    (*iter)->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                    (*iter)->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ());
                }
            }
        }
    }

    void ActivateHardMode()
    {
        m_creature->CastStop();
        DoCast(m_creature, SPELL_SARONITE_BARRIER);
        //DoCast(m_creature, SPELL_SUMMON_ANIMUS); // make it by summon creature because of better handling the auras
        DoScriptText(EMOTE_ANIMUS, m_creature);
        DoScriptText(SAY_HARD, m_creature);
        
        if(Creature* pAnimus = m_creature->SummonCreature(NPC_SARONITE_ANIMUS, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), m_creature->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 900000))
        {
            pAnimus->SetInCombatWithZone();
            m_uiAnimusGUID = pAnimus->GetGUID();
        }

        if (!lVapors.empty())
        {
            for(std::list<Creature*>::iterator iter = lVapors.begin(); iter != lVapors.end(); ++iter)
            {
                if ((*iter) && (*iter)->isAlive())
                    (*iter)->ForcedDespawn();
            }
        }

        m_bIsHardMode = true;
        m_bIsAnimusAlive = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // prepare hard mode
        if(m_uiSaroniteVaporCount == 6 && !m_bIsHardMode && !m_bActiveHardMode)
            PrepareHardMode();

        // summon animus
        if(m_uiSummonAnimusTimer < uiDiff && !m_bIsHardMode && m_bActiveHardMode)
            ActivateHardMode();
        else m_uiSummonAnimusTimer -= uiDiff;

        // saronite vapor
        if(m_uiSaroniteVaporTimer < uiDiff && !m_bIsHardMode)
        {
            m_creature->CastStop();
            DoCast(m_creature, SPELL_SUMMON_VAPORS);
            DoScriptText(EMOTE_VAPORS, m_creature);
            m_uiSaroniteVaporCount += 1;
            m_uiSaroniteVaporTimer = 30000;
        }
        else m_uiSaroniteVaporTimer -= uiDiff;

        // searing flames
        if(m_uiFlamesTimer < uiDiff && !m_bIsAnimusAlive)
        {
            DoCast(m_creature, SPELL_SEARING_FLAMES);
            m_uiFlamesTimer = urand(5000, 10000);
        }
        else m_uiFlamesTimer -= uiDiff; 

        // surge of darkness
        if(m_uiSurgeTimer < uiDiff)
        {
            m_creature->CastStop();
            DoScriptText(SAY_SURGE, m_creature);
            DoScriptText(EMOTE_SURGE, m_creature);
            DoCast(m_creature, SPELL_SURGE_OF_DARKNESS);
            m_uiSurgeTimer = 60000;
        }
        else m_uiSurgeTimer -= uiDiff; 

        // mark of faceless
        if(m_uiMarkTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    DoCastSpellIfCan(pTarget, SPELL_MARK_OF_FACELESS);
                    m_uiMarkTimer = urand(25000, 30000);
                }
            }
            
        }
        else m_uiMarkTimer -= uiDiff;


        // shadow crash
        if(m_uiCrashTimer < uiDiff)
        {
            if (Player* pTarget = GetPlayerAtMinimumRange(15.0f))
                DoCast(pTarget, SPELL_SHADOW_CRASH);
            else 
                DoCast(m_creature->getVictim(), SPELL_SHADOW_CRASH);
            m_uiCrashTimer = 10000;
        }
        else m_uiCrashTimer -= uiDiff;

        // enrage 10 min
        if(m_uiEnrageTimer < uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            m_uiEnrageTimer = 30000;
        }
        else m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

// Saronite animus
struct MANGOS_DLL_DECL mob_saronite_animusAI : public ScriptedAI
{
    mob_saronite_animusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiProfoundDarknessTimer;

    void Reset()
    {
        m_uiProfoundDarknessTimer = 3000;
        DoCast(m_creature, SPELL_ANIMUS_FORMATION);
        m_creature->SetRespawnDelay(DAY);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance)
        {
            if (Creature* pVezax = m_pInstance->GetSingleCreatureFromStorage(NPC_VEZAX))
            {
                if (pVezax->isAlive())
                {
                    pVezax->RemoveAurasDueToSpell(SPELL_SARONITE_BARRIER);
                    ((boss_vezaxAI*)pVezax->AI())->m_bIsAnimusAlive = false;
                }
            }
        }
        // used for hard mode loot
        // REMOVE THIS FOR REVISION
        //m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_VEZAX) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiProfoundDarknessTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_PROFOUND_DARKNESS);
            m_uiProfoundDarknessTimer = urand(2000, 3000);
        }
        else m_uiProfoundDarknessTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_saronite_vaporAI : public ScriptedAI
{
    mob_saronite_vaporAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiDieTimer;

    void Reset()
    {
        m_uiDieTimer = 600000;
        m_creature->SetRespawnDelay(DAY);
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void JustDied(Unit *killer)
    {
        m_uiSaroniteVaporCount -= 1;
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        // Mana regen pool
        if(uiDamage >= m_creature->GetHealth())
        {
            if (Creature* pVezax = m_pInstance->GetSingleCreatureFromStorage(NPC_VEZAX))
            {
                m_creature->CastSpell(m_creature, SPELL_SARONITE_VAPORS, true, 0, 0, pVezax->GetObjectGuid());
            }
            uiDamage = 0;
            m_uiDieTimer = 500;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_VEZAX) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (m_uiDieTimer < diff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else m_uiDieTimer -= diff;
    }
};

CreatureAI* GetAI_boss_vezax(Creature* pCreature)
{
    return new boss_vezaxAI(pCreature);
}

CreatureAI* GetAI_mob_saronite_animus(Creature* pCreature)
{
    return new mob_saronite_animusAI(pCreature);
}

CreatureAI* GetAI_mob_saronite_vapor(Creature* pCreature)
{
    return new mob_saronite_vaporAI(pCreature);
}

void AddSC_boss_vezax()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_vezax";
    newscript->GetAI = &GetAI_boss_vezax;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_saronite_animus";
    newscript->GetAI = &GetAI_mob_saronite_animus;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_saronite_vapor";
    newscript->GetAI = &GetAI_mob_saronite_vapor;
    newscript->RegisterSelf();
}