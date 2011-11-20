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
SDName: Boss_Jedoga
SD%Complete: 95%
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SAY_AGGRO                           = -1619017,
    SAY_CALL_SACRIFICE_1                = -1619018,
    SAY_CALL_SACRIFICE_2                = -1619019,
    SAY_SACRIFICE_1                     = -1619020,
    SAY_SACRIFICE_2                     = -1619021,
    SAY_SLAY_1                          = -1619022,
    SAY_SLAY_2                          = -1619023,
    SAY_SLAY_3                          = -1619024,
    SAY_DEATH                           = -1619025,
    SAY_PREACHING_1                     = -1619026,
    SAY_PREACHING_2                     = -1619027,
    SAY_PREACHING_3                     = -1619028,
    SAY_PREACHING_4                     = -1619029,
    SAY_PREACHING_5                     = -1619030,

    SAY_VOLUNTEER_1                     = -1619031,         //said by the volunteer image
    SAY_VOLUNTEER_2                     = -1619032,

    NPC_VOLUNTEER                       = 30385,
    NPC_VISUAL_TRIGGER                  = 38667,

    FAC_FRIENDLY                        = 35,
    FAC_HOSTILE                         = 16, 

    SPELL_SPHERE_VISUAL                 = 56075,
    SPELL_SACRIFICE_VISUAL              = 56133,
    SPELL_DARK_BEAM                     = 46016,
    SPELL_GIFT_OF_THE_HERALD            = 56219,
    
    SPELL_LIGHTING_BALL                 = 56891,
    SPELL_LIGHTING_BALL_H               = 60032,

    SPELL_THUNDERSHOCK                  = 56926,
    SPELL_THUNDERSHOCK_H                = 60029,

    SPELL_CYCLONE_STRIKE                = 56855,
    SPELL_CYCLONE_STRIKE_H              = 60030,
};

enum JedogaPhase
{
    PHASE_TWILIGHT_INITIATE = 1,
    PHASE_GROUND            = 2,
    PHASE_VOLUNTEER         = 3,
};

const float volunteerPos[7][4] =
{
    {362.002197f, -729.438904f, -16.179300f, 1.125480f},
    {372.852570f, -730.883850f, -16.179300f, 1.688610f},
    {379.525360f, -726.276672f, -16.179300f, 2.160635f},
    {385.052338f, -718.627014f, -16.179300f, 2.408821f},
    {391.472870f, -710.442200f, -16.082842f, 3.012007f},
    {394.789246f, -701.645203f, -16.179674f, 3.290038f},
    {393.005707f, -694.984816f, -16.179674f, 3.748711f}
};

#define MAX_VOLUNTEER    7

#define CORD_CENTER_X     372.330994f
#define CORD_CENTER_Y    -705.278015f
#define CORD_CENTER_Z     -16.179701f

#define CORD_ABOVE_Z       -0.624178f

#define START_X               372.33f
#define START_Y              -705.28f
#define START_Z               -8.904f
#define START_O             5.427970f

/*######
## boss_jedoga
######*/

struct MANGOS_DLL_DECL boss_jedogaAI : public ScriptedAI
{
    boss_jedogaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ahnkahet* m_pInstance;
    bool m_bIsRegularMode;

    GUIDVector m_vVolunteerGuid;

    ObjectGuid pChosenVolunteerGuid;

    uint32 volunteerDeathTimer;
    uint32 volunteerReachedTimer;
    uint32 volunteerPhaseTimer;
    uint32 lightingBallTimer;
    uint32 thundershockTimer;
    uint32 cycloneStrikeTimer;

    uint8 victimCounter;
    uint8 m_uiPhase;

    void Reset()
    {
        m_uiPhase = PHASE_TWILIGHT_INITIATE;
        SetCombatMovement(false);
        DepawnVolunteers();
        victimCounter = 0;
        volunteerDeathTimer = 600000;
        volunteerPhaseTimer = 20000;
        volunteerReachedTimer = 60000;
        lightingBallTimer = 4000;
        thundershockTimer = 6000;
        cycloneStrikeTimer = 8000;

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_JEDOGA,NOT_STARTED);
            m_pInstance->SetAchiev(TYPE_JEDOGA, true);
        }
        
        m_creature->NearTeleportTo(START_X,START_Y,START_Z,START_O);
        m_creature->GetMotionMaster()->MoveIdle();
        m_creature->CastSpell(m_creature,SPELL_SPHERE_VISUAL,true);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_JEDOGA,DONE);
        DoScriptText(SAY_DEATH, m_creature);
        DepawnVolunteers();
    }

    void SpawnVolunteers()
    {
        for (int i = 0; i < MAX_VOLUNTEER; i++)
        {
            if (Creature* pVolunteer = m_creature->SummonCreature(NPC_VOLUNTEER,
                volunteerPos[i][0], volunteerPos[i][1], volunteerPos[i][2],volunteerPos[i][3], TEMPSUMMON_DEAD_DESPAWN, 5000))
            {
                pVolunteer->setFaction(FAC_FRIENDLY);
                pVolunteer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                pVolunteer->DeleteThreatList();
                pVolunteer->CastSpell(pVolunteer, SPELL_SPHERE_VISUAL, true);
                pVolunteer->HandleEmoteCommand(EMOTE_STATE_KNEEL);
                m_vVolunteerGuid.push_back(pVolunteer->GetObjectGuid());
            } 
        }
    }

    void DepawnVolunteers()
    {
        if (!m_vVolunteerGuid.empty())
        {
            for(GUIDVector::iterator itr = m_vVolunteerGuid.begin(); itr != m_vVolunteerGuid.end(); ++itr)
            {
                if (Creature* pVolunteer = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pVolunteer->isAlive())
                        pVolunteer->ForcedDespawn();
                }
            }
            m_vVolunteerGuid.clear();
        }
    }

    void MoveVolunteer()
    {
        if (!m_vVolunteerGuid.empty())
        {
            uint32 random = urand(0, m_vVolunteerGuid.size() -1);
            pChosenVolunteerGuid = m_vVolunteerGuid[random];
            m_vVolunteerGuid.erase(m_vVolunteerGuid. begin() + random);
            if (Creature* pVolunteer = m_creature->GetMap()->GetCreature(pChosenVolunteerGuid))
            {
                if (pVolunteer->isAlive())
                {
                    if (Creature* pVisualTrigger = m_creature->SummonCreature(NPC_VISUAL_TRIGGER, CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 20000))
                    {
                        pVisualTrigger->GetMotionMaster()->Clear();
                        pVisualTrigger->GetMotionMaster()->MoveIdle();
                        pVisualTrigger->SetVisibility(VISIBILITY_ON);
                        pVisualTrigger->CastSpell(pVisualTrigger, SPELL_SACRIFICE_VISUAL, true);
                    }
                    pVolunteer->GetMotionMaster()->Clear();
                    pVolunteer->GetMotionMaster()->MovePoint(0, CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_Z);
                    pVolunteer->setFaction(FAC_HOSTILE);
                    pVolunteer->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
                    pVolunteer->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                    switch(urand(0, 1))
                    {
                        case 0: DoScriptText(SAY_CALL_SACRIFICE_1, pVolunteer); break;
                        case 1: DoScriptText(SAY_CALL_SACRIFICE_2, pVolunteer); break;
                    }
                }
                else
                {
                    MoveVolunteer();
                }
            }
        }
    }

    bool allStartMobsDead()
    {
        if (m_pInstance)
        {
            for(GUIDList::iterator itr = m_pInstance->m_lTwilightInitiate.begin(); itr != m_pInstance->m_lTwilightInitiate.end(); ++itr)
            {
                if (Creature* pTwilightInitiate = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pTwilightInitiate->isAlive())
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        switch(m_uiPhase)
        {
            case PHASE_TWILIGHT_INITIATE:
            {
                if(allStartMobsDead())
                {
                    m_creature->RemoveAurasDueToSpell(SPELL_SPHERE_VISUAL);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetInCombatWithZone();
                    DoScriptText(SAY_AGGRO, m_creature);
                    SpawnVolunteers();
                    if(m_pInstance)
                        m_pInstance->SetData(TYPE_JEDOGA,IN_PROGRESS);
                    m_uiPhase = PHASE_GROUND;
                    SetCombatMovement(true);
                }
                break;
            }
            case PHASE_GROUND:
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                if (volunteerPhaseTimer    < uiDiff)
                {
                    m_uiPhase = PHASE_VOLUNTEER;
                    MoveVolunteer();
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_creature->NearTeleportTo(CORD_CENTER_X, CORD_CENTER_Y, CORD_ABOVE_Z, START_O);
                    
                    volunteerPhaseTimer = 32000;
                    volunteerDeathTimer = 16000;
                    volunteerReachedTimer = 14500;
                }
                else
                    volunteerPhaseTimer -= uiDiff;

                if (lightingBallTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTING_BALL : SPELL_LIGHTING_BALL_H) == CAST_OK)
                        lightingBallTimer = urand(4000, 6000);
                }
                else
                    lightingBallTimer -= uiDiff;
        
                if (thundershockTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_THUNDERSHOCK : SPELL_THUNDERSHOCK_H) == CAST_OK)
                        thundershockTimer = urand(12000, 18000);
                }
                else
                    thundershockTimer -= uiDiff;

                if (cycloneStrikeTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_CYCLONE_STRIKE : SPELL_CYCLONE_STRIKE_H)  == CAST_OK)
                        cycloneStrikeTimer = urand(14000, 20000);
                }
                else
                    cycloneStrikeTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_VOLUNTEER:
            {
                if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                    return;

                if (volunteerReachedTimer < uiDiff)
                {
                    if (Creature* pChosenVolunteer = m_creature->GetMap()->GetCreature(pChosenVolunteerGuid))
                    {
                        if (pChosenVolunteer->isAlive())
                        {
                            m_creature->CastSpell(pChosenVolunteer, SPELL_DARK_BEAM, true);
                            pChosenVolunteer->GetMotionMaster()->MoveIdle();
                            pChosenVolunteer->RemoveAllAuras();
                            pChosenVolunteer->setFaction(FAC_FRIENDLY);
                            pChosenVolunteer->SetHealth(m_creature->GetMaxHealth());
                            switch(urand(0, 1))
                            {
                                case 0: DoScriptText(SAY_SACRIFICE_1, pChosenVolunteer); break;
                                case 1: DoScriptText(SAY_SACRIFICE_2, pChosenVolunteer); break;
                            }
                        }
                        volunteerReachedTimer = 60000;
                    }
                }else volunteerReachedTimer -= uiDiff;

                if (volunteerDeathTimer < uiDiff)
                {
                    if (Creature* pChosenVolunteer = m_creature->GetMap()->GetCreature(pChosenVolunteerGuid))
                    {
                        if (pChosenVolunteer->isAlive())
                        {
                            pChosenVolunteer->DealDamage(pChosenVolunteer, pChosenVolunteer->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            m_creature->CastSpell(m_creature, SPELL_GIFT_OF_THE_HERALD, true);
                        }
                    }
                    if (m_vVolunteerGuid.empty() || victimCounter >= 6)
                    {
                        DepawnVolunteers();
                        SpawnVolunteers();
                        victimCounter = 0;
                    }

                    m_creature->NearTeleportTo(CORD_CENTER_X, CORD_CENTER_Y, CORD_CENTER_Z, START_O);
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        m_creature->GetMotionMaster()->MoveChase(pTarget);
                        m_creature->Attack(pTarget, true);
                    }
                    
                    victimCounter++;
                    volunteerDeathTimer = 60000;
                    m_uiPhase = PHASE_GROUND;
                    
                }
                else
                    volunteerDeathTimer -= uiDiff;

                break;
            }
        }
    }
};

CreatureAI* GetAI_boss_jedoga(Creature* pCreature)
{
    return new boss_jedogaAI(pCreature);
}

struct MANGOS_DLL_DECL mob_jedoga_volunteerAI : public ScriptedAI
{
    mob_jedoga_volunteerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    instance_ahnkahet *m_pInstance;                          // the instance

    void Reset() { }

    void JustDied(Unit* pKiller)
    {
        if((pKiller->GetTypeId() == TYPEID_PLAYER) || (pKiller->GetOwner()) && (pKiller->GetOwner()->GetTypeId() == TYPEID_PLAYER))
            if (m_pInstance)
                m_pInstance->SetAchiev(TYPE_JEDOGA, false);
    }

    void UpdateAI(const uint32 uiDiff) 
    {
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_jedoga_volunteer(Creature* pCreature)
{
    return new mob_jedoga_volunteerAI(pCreature);
}

void AddSC_boss_jedoga()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_jedoga";
    newscript->GetAI = &GetAI_boss_jedoga;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_jedoga_volunteer";
    newscript->GetAI = &GetAI_mob_jedoga_volunteer;
    newscript->RegisterSelf();
}
