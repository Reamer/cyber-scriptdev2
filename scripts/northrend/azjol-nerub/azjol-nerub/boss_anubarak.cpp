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
SDName: Boss_Anubarak
SD%Complete: 80%
SDComment: Some cosmetic mistakes. TODO: spikes.
SDCategory: Azjol'Nerub
EndScriptData */

#include "precompiled.h"
#include "azjol-nerub.h"

enum
{
    SAY_INTRO                       = -1601014,
    SAY_AGGRO                       = -1601015,
    SAY_KILL_1                      = -1601016,
    SAY_KILL_2                      = -1601017,
    SAY_KILL_3                      = -1601018,
    SAY_SUBMERGE_1                  = -1601019,
    SAY_SUBMERGE_2                  = -1601020,
    SAY_LOCUST_1                    = -1601021,
    SAY_LOCUST_2                    = -1601022,
    SAY_LOCUST_3                    = -1601023,
    SAY_DEATH                       = -1601024,

    SPELL_EARTH_EXPLOSION           = 42373,
    SPELL_CARRION_SWARM             = 53520,
    SPELL_IMPALE                    = 53454,
    SPELL_IMPALE_H                  = 59446,
    SPELL_LEECHING_SWARM            = 53467,
    SPELL_LEECHING_SWARM_H          = 59430,
    SPELL_POUND                     = 53472,
    SPELL_POUND_H                   = 59433,
    SPELL_POUND_DMG                 = 53509,
    SPELL_POUND_DMG_H               = 59432,
    SPELL_BURROW                    = 26381,

    NPC_ADD1                        = 28736,
    NPC_ADD2                        = 29349,
    NPC_ELITE_ADD                   = 28732,

    NPC_IMPALE_TRIGGER              = 105000,

    FAC_HOSTILE                     = 16,

    ACHIEV_SPEEDKILL_H              = 1860,

};

enum Phasen{
    PHASE_NORMAL    = 0,
    PHASE_ADDS      = 1,
};

static const float aPercentOfLife[4] = {66.6f, 33.3f, 15.0f, 0.0f};

#define MIDDLE_CORD_X                   552.927734f
#define MIDDLE_CORD_Y                   248.950851f
#define MIDDLE_CORD_Z                   223.912796f

#define ELITE_SPAWN_1_X                 547.412841f
#define ELITE_SPAWN_1_Y                 320.102448f
#define ELITE_SPAWN_1_Z                 236.062057f

#define ELITE_SPAWN_2_X                 554.539185f
#define ELITE_SPAWN_2_Y                 319.792603f
#define ELITE_SPAWN_2_Z                 235.927032f

/*######
## boss_anubarak
######*/

struct MANGOS_DLL_DECL boss_anubarakAI : public ScriptedAI
{
    boss_anubarakAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_azjol_nerub*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_azjol_nerub* m_pInstance;
    bool m_bIsRegularMode;
    uint8 m_uiPhase;

    uint32 m_uiSummonCreatureTimer;
    uint32 m_uiLeechingSwarmTimer;
    uint32 m_uiImpaleTimer;
    uint32 m_uiPoundTimer;
    uint32 m_uiCarrionSwarmTimer;
    
    uint32 m_uiWaveStepCount;
    uint32 countInvisiblePhases;
    uint32 m_uiBurrowStepCount;
    uint32 m_uiBurronStepTimer;

    GUIDList m_lBettleGUIDList;

    void Reset()
    {
        m_uiPhase               = PHASE_NORMAL;
        countInvisiblePhases    = 0;
        m_uiBurrowStepCount     = 0;
        m_uiBurronStepTimer     = 0;
        m_uiWaveStepCount       = 0;
        m_uiSummonCreatureTimer = 2000;
        m_uiLeechingSwarmTimer  = 4000;
        m_uiImpaleTimer         = 5000;
        m_uiPoundTimer          = 12000;
        m_uiCarrionSwarmTimer   = 13000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
        
        RemoveAllBettles();

    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
        }
    }

    void RemoveAllBettles()
    {
        if (!m_lBettleGUIDList.empty())
        {
            for(GUIDList::iterator itr = m_lBettleGUIDList.begin(); itr != m_lBettleGUIDList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                        pTemp->ForcedDespawn();
        }
        m_lBettleGUIDList.clear();
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

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, FAIL);
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
        DoScriptText(SAY_DEATH, m_creature);
        RemoveAllBettles();
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUBARAK, DONE);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_POUND)
        {
            if (pTarget)
                pTarget->CastSpell(pTarget, SPELL_POUND_DMG, true);
        }

        if (pSpell->Id == SPELL_POUND_H)
        {
            if (pTarget)
                pTarget->CastSpell(pTarget, SPELL_POUND_DMG_H, true); 
        }     
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_uiPhase)
        {
            case PHASE_NORMAL:
            {
                if (m_creature->GetHealthPercent() < aPercentOfLife[countInvisiblePhases])
                {
                    ++countInvisiblePhases;
                    m_uiPhase = PHASE_ADDS;
                    DoCastSpellIfCan(m_creature, SPELL_BURROW);
                    m_uiSummonCreatureTimer = 2000;
                    m_uiBurronStepTimer = 2000;
                    m_uiBurrowStepCount = 0;
                    m_uiWaveStepCount = 0;
                }

                if (m_uiPoundTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POUND : SPELL_POUND_H) == CAST_OK)
                        m_uiPoundTimer = urand(15000, 18000);
                }
                else
                    m_uiPoundTimer -= uiDiff;
            
                if (m_uiLeechingSwarmTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LEECHING_SWARM : SPELL_LEECHING_SWARM_H) == CAST_OK)
                        m_uiLeechingSwarmTimer = 15000;  
                }
                else
                    m_uiLeechingSwarmTimer -= uiDiff;

                if (m_uiCarrionSwarmTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_CARRION_SWARM) == CAST_OK)
                        m_uiCarrionSwarmTimer = urand(23000, 31000);
                }
                else
                    m_uiCarrionSwarmTimer -= uiDiff;
                
                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_ADDS:
            {
                if (m_uiImpaleTimer < uiDiff)
                {
                    if (Unit* pImpaleVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, 0.0f, SELECT_FLAG_PLAYER))
                    {
                        m_creature->SummonCreature(NPC_IMPALE_TRIGGER, pImpaleVictim->GetPositionX(), pImpaleVictim->GetPositionY(), pImpaleVictim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        m_uiImpaleTimer = 8000;
                    }
                }
                else
                    m_uiImpaleTimer -= uiDiff;

                if (m_uiBurronStepTimer < uiDiff)
                {
                    switch (m_uiBurrowStepCount)
                    {
                        case 0:
                        {
                            m_creature->SetVisibility(VISIBILITY_OFF);
                            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            m_creature->GetMotionMaster()->MoveIdle();
                            switch (countInvisiblePhases)
                            {
                                case 0: m_uiBurronStepTimer = 7000; break;
                                case 1: m_uiBurronStepTimer = 15000; break;
                                case 2: m_uiBurronStepTimer = 25000; break;
                                default: m_uiBurronStepTimer = 2000;
                            }
                            break;
                        }
                        case 1:
                        {
                            DoCastSpellIfCan(m_creature, SPELL_BURROW);
                            m_uiBurronStepTimer = 8000;
                            break;
                        }
                        case 2:
                        {
                            m_uiPhase = PHASE_NORMAL;
                            m_creature->SetVisibility(VISIBILITY_ON);
                            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            m_uiBurronStepTimer = 2000;
                            break;
                        }
                        default:
                        {
                            m_uiBurrowStepCount = 1;
                            m_uiBurronStepTimer = 2000;
                        }
                    }
                     ++m_uiBurrowStepCount;
                }
                else
                    m_uiBurronStepTimer -= uiDiff;

                if (m_uiSummonCreatureTimer < uiDiff)
                {
                    switch(m_uiWaveStepCount)
                    {
                        case 0:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (countInvisiblePhases == 3)
                                if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                    m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());

                            m_uiSummonCreatureTimer = 2000;
                            break;
                        }
                        case 1:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            switch (countInvisiblePhases)
                            {
                                case 1:
                                    m_uiSummonCreatureTimer = 6000;
                                    break;
                                case 2:
                                    m_uiSummonCreatureTimer = 10000;
                                    break;
                                case 3:
                                    m_uiSummonCreatureTimer = 10000;
                                    m_uiWaveStepCount += 3;
                                    break;
                            }
                            break;
                        }
                        case 2:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (countInvisiblePhases == 1)
                            {
                                m_uiWaveStepCount = 10; // END
                                break;
                            }
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());

                            m_uiSummonCreatureTimer = 1000;
                            break;
                        }
                        case 3:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            m_uiSummonCreatureTimer = 7000;
                            break;
                        }
                        case 4:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                        
                            m_uiWaveStepCount = 10; // END
                            break;
                        }
                        case 5:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                        
                            m_uiSummonCreatureTimer = 2500;
                            break;
                        }
                        case 6:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 120000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                        
                            m_uiSummonCreatureTimer = 2000;
                            break;
                        }
                        case 7:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                            m_uiSummonCreatureTimer = 3000;
                            break;
                        }
                        case 8:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());

                            m_uiSummonCreatureTimer = 8000;
                            break;
                        }
                        case 9:
                        {
                            if (Creature* pTemp = m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_DEAD_DESPAWN, 30000))
                                m_lBettleGUIDList.push_back(pTemp->GetObjectGuid());
                        
                            m_uiSummonCreatureTimer = 9999999;
                            break;
                        }
                        default:
                            break;
                    }
                    m_uiWaveStepCount++;
                    
                }
                else
                    m_uiSummonCreatureTimer -= uiDiff;
            }
        }
    }
};

enum
{
    SPELL_STRIKE    = 52532,
    SPELL_CLEAVE    = 49806,
};
#define ELITE_SPAWN_1_X_END             556.115845f
#define ELITE_SPAWN_1_Y_END             260.768311f
#define ELITE_SPAWN_1_Z_END             223.889069f

#define ELITE_SPAWN_2_X_END             547.456970f
#define ELITE_SPAWN_2_Y_END             260.716064f
#define ELITE_SPAWN_2_Z_END             223.643402f

#define ELITE_WP_1_X                    546.684875f
#define ELITE_WP_1_Y                    278.920990f
#define ELITE_WP_1_Z                    224.348969f

#define ELITE_WP_2_X                    555.046631f
#define ELITE_WP_2_Y                    278.821503f
#define ELITE_WP_2_Z                    224.304581f

struct MANGOS_DLL_DECL npc_elite_anubAI : public ScriptedAI
{
    npc_elite_anubAI(Creature* pCreature) : ScriptedAI(pCreature)
    {Reset();}
    

    uint32 moveWpTimer;
    uint32 moveMiddleTimer;
    uint32 strikeTimer;
    uint32 cleaveTimer;

    bool setSpeed;

    void Reset()
    {
        moveWpTimer = 100;
        moveMiddleTimer = 7000;
        strikeTimer = urand(9000,12000);
        cleaveTimer = urand(4000,7000);
        setSpeed = false;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (!m_creature->IsTemporarySummon())
                return;

            if (m_creature->GetPositionX() < 550.0f)
            {
                if (moveWpTimer < uiDiff)
                {
                    m_creature->GetMotionMaster()->MovePoint(0, ELITE_WP_1_X, ELITE_WP_1_Y, ELITE_WP_1_Z);
                    moveWpTimer = 9999999;
                    return;
                }else moveWpTimer -= uiDiff;
            }
            else
            {
                if (moveWpTimer < uiDiff)
                {
                    m_creature->GetMotionMaster()->MovePoint(0, ELITE_WP_2_X, ELITE_WP_2_Y, ELITE_WP_2_Z);
                    moveWpTimer = 9999999;
                    return;
                }else moveWpTimer -= uiDiff;
            }

            if (m_creature->GetPositionX() < 550.0f)
            {
                if (moveMiddleTimer < uiDiff)
                {
                    m_creature->GetMotionMaster()->MovePoint(0, ELITE_SPAWN_2_X_END, ELITE_SPAWN_2_Y_END, ELITE_SPAWN_2_Z_END);
                    moveMiddleTimer = 9999999;
                    return;
                }else moveMiddleTimer -= uiDiff;
            }
            else
            {
                if (moveMiddleTimer < uiDiff)
                {
                    m_creature->GetMotionMaster()->MovePoint(0, ELITE_SPAWN_1_X_END, ELITE_SPAWN_1_Y_END, ELITE_SPAWN_1_Z_END);
                    moveMiddleTimer = 9999999;
                    return;
                }else moveMiddleTimer -= uiDiff;
            }
            return;

            if (!setSpeed)
            {
                m_creature->SetSpeedRate(MOVE_WALK, 1.7f);
                m_creature->SetSpeedRate(MOVE_RUN, 1.7f);
                setSpeed = true;
            }
        } 
        else
        {
            if (strikeTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_STRIKE);
                strikeTimer = urand(5000,7000);
            }                
            else
                strikeTimer -= uiDiff;

            if (cleaveTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
                cleaveTimer = urand(14000,17000);
            }
            else
                cleaveTimer -= uiDiff;
        }
        DoMeleeAttackIfReady();
    }  
};

CreatureAI* GetAI_boss_anubarak(Creature* pCreature)
{
    return new boss_anubarakAI(pCreature);
}

CreatureAI* GetAI_npc_elite_anub(Creature* pCreature)
{
    return new npc_elite_anubAI(pCreature);
}

void AddSC_boss_anubarak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_anubarak";
    pNewScript->GetAI = &GetAI_boss_anubarak;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_elite_anub";
    pNewScript->GetAI = &GetAI_npc_elite_anub;
    pNewScript->RegisterSelf();
}
