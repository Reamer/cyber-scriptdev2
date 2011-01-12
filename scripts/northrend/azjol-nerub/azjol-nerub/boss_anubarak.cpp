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

#define MIDDLE_CORD_X                   552.927734f
#define MIDDLE_CORD_Y                   248.950851f
#define MIDDLE_CORD_Z                   223.912796f

#define ELITE_SPAWN_1_X                 547.412841f
#define ELITE_SPAWN_1_Y                 320.102448f
#define ELITE_SPAWN_1_Z                 236.062057f

#define ELITE_SPAWN_2_X                 554.539185f
#define ELITE_SPAWN_2_Y                 319.792603f
#define ELITE_SPAWN_2_Z                 235.927032f

#define ACHIEV_SPEEDKILL_H              1860

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

        bool m_bIsInTimeForAchiev;
        uint32 SpeedKillTimer;

    bool phase66;
    bool phase66Over;
    bool phase33;
    bool phase33Over;
    bool phase15;
    bool phase15Over;

    uint32 BurrowTimer;
    uint32 VisComeBackTimer;
    uint32 BurComeBackTimer;
    uint32 IsBackTimer;
    uint32 SummonCreatureTimer;
    uint32 LeechingSwarmTimer;
    uint32 CloseDoorTimer;
    uint32 ImpaleTimer;
    uint32 PoundTimer;
    uint32 CarrionSwarmTimer;
    uint32 ImpaleTriggerTimer;

    Unit* pTriggerTarget;

    int i;

    Creature* Elite[5];

    void Reset()
    {
        phase66         = false;
        phase66Over     = false;
        phase33         = false;
        phase33Over     = false;
        phase15         = false;
        phase15Over     = false;

        BurrowTimer             = 9999999;
        VisComeBackTimer        = 9999999;
        BurComeBackTimer        = 9999999;
        IsBackTimer             = 9999999;
        SummonCreatureTimer     = 9999999;
        LeechingSwarmTimer      = 4000;
        CloseDoorTimer          = 4000;
        ImpaleTimer             = 5000;
        ImpaleTriggerTimer      = 9999999;
        PoundTimer              = 12000;
        CarrionSwarmTimer       = 13000;

        m_bIsInTimeForAchiev = true;
        SpeedKillTimer = 240000;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);

        pTriggerTarget = NULL;

        i = 0;

        m_pInstance->SetData(TYPE_ANUBARAK, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
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
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
        DoScriptText(SAY_DEATH, m_creature);
        m_pInstance->SetData(TYPE_ANUBARAK, DONE);

            if (m_bIsInTimeForAchiev && !m_bIsRegularMode)
            {
                if (ACHIEV_SPEEDKILL_H)
                {
                    Map* pMap = m_creature->GetMap();
                    if (pMap && pMap->IsDungeon())
                    {
                        Map::PlayerList const &players = pMap->GetPlayers();
                        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                            itr->getSource()->RewardPlayerAndGroupAtEvent(ACHIEV_SPEEDKILL_H, m_creature);
                    }
                }
            }
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

        // Small hack to prevent a precocious close of the battlefield
        if (CloseDoorTimer < uiDiff)
        {
            m_pInstance->SetData(TYPE_ANUBARAK, IN_PROGRESS);
            CloseDoorTimer = 9999999;
        }else CloseDoorTimer -= uiDiff;

        if (SpeedKillTimer < uiDiff)
        {
            m_bIsInTimeForAchiev = false;
        }
        else
            SpeedKillTimer -= uiDiff;

        if (phase66 || phase33 || phase15)
        {
            // TODO: Impale
            if (ImpaleTimer < uiDiff)
            {
                if (Unit* pImpaleVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    if (pImpaleVictim->GetTypeId() == TYPEID_PLAYER)
                    {
                        m_creature->SummonCreature(NPC_IMPALE_TRIGGER, pImpaleVictim->GetPositionX(), pImpaleVictim->GetPositionY(), pImpaleVictim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 4000);
                        ImpaleTimer = 8000;
                    }
            }else ImpaleTimer -= uiDiff;
        }
        else 
        {
            if (PoundTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_POUND : SPELL_POUND_H);
                PoundTimer = urand(15000, 18000);
            }else PoundTimer -= uiDiff;
            
            if (LeechingSwarmTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LEECHING_SWARM : SPELL_LEECHING_SWARM_H);  
                LeechingSwarmTimer = 15000;  
            }else LeechingSwarmTimer -= uiDiff;

            if (CarrionSwarmTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_CARRION_SWARM); 
                CarrionSwarmTimer = urand(23000, 31000);
            }else CarrionSwarmTimer -= uiDiff;
        }

        if (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.66 && !phase66Over)
        {
            phase66 = true;
            phase66Over = true;
            DoCastSpellIfCan(m_creature, SPELL_BURROW);
            BurrowTimer = 1700;
            SummonCreatureTimer = 2000;
        }
        else if (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.33 && !phase33Over)
        {
            phase33 = true;
            phase33Over = true;
            DoCastSpellIfCan(m_creature, SPELL_BURROW);
            BurrowTimer = 1700;
            SummonCreatureTimer = 2000;
        }
        else if (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.15 && !phase15Over)
        {
            phase15 = true;
            phase15Over = true;
            DoCastSpellIfCan(m_creature, SPELL_BURROW);
            BurrowTimer = 1700;
            SummonCreatureTimer = 2000;
        }

        if (BurrowTimer < uiDiff)
        {
            m_creature->SetVisibility(VISIBILITY_OFF);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->GetMotionMaster()->MoveIdle();
            if (phase66)
                BurComeBackTimer = 7000;
            else if (phase33)
                BurComeBackTimer = 15000;
            else if (phase15)
                BurComeBackTimer = 25000;

            BurrowTimer = 9999999;
        }else BurrowTimer -= uiDiff;

        if (BurComeBackTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BURROW);
            VisComeBackTimer = 8000;

            BurComeBackTimer = 9999999;
        }else BurComeBackTimer -= uiDiff;

        if (VisComeBackTimer < uiDiff)
        {
            m_creature->SetVisibility(VISIBILITY_ON);
            IsBackTimer = 3000;

            VisComeBackTimer = 9999999;
        }else VisComeBackTimer -= uiDiff;

        if (IsBackTimer < uiDiff)
        {
            phase66 = false;
            phase33 = false;
            phase15 = false;
            i = 0;

            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            IsBackTimer = 9999999;
        }else IsBackTimer -= uiDiff;

        if (SummonCreatureTimer < uiDiff)
        {
            if (phase66 || phase33 || phase15)
            {
                switch(i)
                {
                    case 0:
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        if (phase15)
                            m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                        SummonCreatureTimer = 2000;
                        break;
                    case 1:
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        SummonCreatureTimer = phase33 ? 10000 : 6000;
                        if (phase15)
                        {
                            i += 3;
                            SummonCreatureTimer = 10000;
                        }
                        break;
                    case 2:
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        if (phase66)
                        {
                            SummonCreatureTimer = 9999999;
                            break;
                        }
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                        SummonCreatureTimer = 1000;
                        break;
                    case 3:
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                        SummonCreatureTimer = 7000;
                        break;
                    case 4:
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        
                        SummonCreatureTimer = 9999999;
                        break;
                    case 5:
                        m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        
                        SummonCreatureTimer = 2500;
                        break;
                    case 6:
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ADD1, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_1_X, ELITE_SPAWN_1_Y, ELITE_SPAWN_1_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 120000);
                        
                        SummonCreatureTimer = 2000;
                        break;
                    case 7:
                        m_creature->SummonCreature(NPC_ELITE_ADD, ELITE_SPAWN_2_X, ELITE_SPAWN_2_Y, ELITE_SPAWN_2_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                        SummonCreatureTimer = 3000;
                        break;
                    case 8:
                        m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);

                        SummonCreatureTimer = 8000;
                        break;
                    case 9:
                        m_creature->SummonCreature(NPC_ADD2, MIDDLE_CORD_X + urand(0.0f, 10.0f), MIDDLE_CORD_Y + urand(0.0f, 10.0f), MIDDLE_CORD_Z, m_creature->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                        
                        SummonCreatureTimer = 9999999;
                        break;
                    default:
                        break;
                }
                i++;
            }
        }else SummonCreatureTimer -= uiDiff;
 
        if (!phase66 && !phase33 && !phase15)
            DoMeleeAttackIfReady();
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
