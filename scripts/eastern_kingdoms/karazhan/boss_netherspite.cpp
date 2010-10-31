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
SDName: Boss_Netherspite
SD%Complete: 90%
SDComment: find spell ID for tail swipe added in patch 3.0.2, sometimes root in banish phase dispels if he takes damage (core issue?)
SDCategory: Karazhan
EndScriptData */

#include "precompiled.h"
#include "karazhan.h"

enum
{
    // netherspite spells
    SPELL_NETHERBURN            = 30522,
    SPELL_VOID_ZONE             = 37063,
    SPELL_NETHERBREATH          = 38523,
    SPELL_EMPOWERMENT           = 38549,
    SPELL_NETHER_INFUSION       = 38688,
    SPELL_NETHERSPITE_ROAR      = 38684,
    SPELL_BANISH_VISUAL         = 39833,
    SPELL_ROOT                  = 42716,

    // void zone spells
    SPELL_CONSUMPTION           = 30497,

    // beam buffs
    SPELL_PERSEVERENCE_NS       = 30466,
    SPELL_PERSEVERENCE_PLR      = 30421,
    SPELL_SERENITY_NS           = 30467,
    SPELL_SERENITY_PLR          = 30422,
    SPELL_DOMINANCE_NS          = 30468,
    SPELL_DOMINANCE_PLR         = 30423,

    // beam debuffs
    SPELL_EXHAUSTION_DOM        = 38639,
    SPELL_EXHAUSTION_SER        = 38638,
    SPELL_EXHAUSTION_PER        = 38637,

    // beam spells
    SPELL_BEAM_DOM              = 30402,
    SPELL_BEAM_SER              = 30401,
    SPELL_BEAM_PER              = 30400,
    SPELL_BLUE_PORTAL           = 30491,
    SPELL_GREEN_PORTAL          = 30490,
    SPELL_RED_PORTAL            = 30487,

    // emotes
    EMOTE_PHASE_BEAM            = -1532089,
    EMOTE_PHASE_BANISH          = -1532090,

    // npcs
    NPC_PORTAL_CREATURE         = 17369,
    //NPC_VOID_ZONE               = 16697, // unused

    // max num of types of portals
    MAX_PORTAL_TYPE             = 3,

    // max size of raid group
    MAX_RAID_SIZE_10            = 10
};

struct Spawn_Locations
{
    float x, y, z;

    // should make the code much cleaner
    Spawn_Locations& operator= (const Spawn_Locations &rhs)
    {
        // Only do assignment if RHS is a different object from this.
        if (this != &rhs)
        {
            this->x = rhs.x;
            this->y = rhs.y;
            this->z = rhs.z;
        }

        return *this;
    }
};

static const Spawn_Locations BasicCoordinates[] =
{
    {-11105.508789f, -1600.851685f, 279.950256f},
    {-11195.353516f, -1613.237183f, 278.237258f},
    {-11137.846680f, -1685.607422f, 278.239258f}
};

static Spawn_Locations PortalCoordinates[] =
{
    {-11105.508789f, -1600.851685f, 279.950256f},
    {-11195.353516f, -1613.237183f, 278.237258f},
    {-11137.846680f, -1685.607422f, 278.239258f}
};

enum Phases
{
    NULL_PHASE = 0,
    BEAM_PHASE,
    BANISH_PHASE
};

enum Events
{
    NULL_EVENT = 0,
    EVENT_ENRAGE,
    EVENT_EXHAUST_CHECK,

    //portal phase events
    EVENT_NETHERSPITE_ROAR = 3,
    EVENT_PORTALS_SPAWN,
    EVENT_BEAM_END_TIMER,
    EVENT_BEAM_INITIAL,
    EVENT_BEAM_PERIODIC,
    EVENT_NETHERBURN,
    EVENT_VOID_ZONE,
    MAX_PORTAL_EVENT,

    //banish phase events
    EVENT_BANISH_END_TIMER = 3,
    EVENT_NETHERBREATH_INITIAL,
    EVENT_NETHERBREATH,
    MAX_BANISH_EVENT,

    MAX_EVENT = 10
};

static const uint32 Beam_Buff[MAX_PORTAL_TYPE][2] =
{
    {SPELL_DOMINANCE_NS,    SPELL_DOMINANCE_PLR},
    {SPELL_SERENITY_NS,     SPELL_SERENITY_PLR},
    {SPELL_PERSEVERENCE_NS, SPELL_PERSEVERENCE_PLR}
};

static const uint32 Beam_Debuff[MAX_PORTAL_TYPE] =
{
    SPELL_EXHAUSTION_DOM,
    SPELL_EXHAUSTION_SER,
    SPELL_EXHAUSTION_PER
};

static const uint32 Portals[MAX_PORTAL_TYPE] =
{
    SPELL_BLUE_PORTAL,
    SPELL_GREEN_PORTAL,
    SPELL_RED_PORTAL
};

static const uint32 Beam_Visuals[MAX_PORTAL_TYPE] =
{
    SPELL_BEAM_DOM,
    SPELL_BEAM_SER,
    SPELL_BEAM_PER
};

static const uint32 uiMaxTimeCounter[3] = {NULL_EVENT, MAX_PORTAL_EVENT, MAX_BANISH_EVENT};

struct MANGOS_DLL_DECL boss_netherspiteAI : public ScriptedAI
{
    boss_netherspiteAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    Phases m_Active_Phase;

    Events m_Active_Event;

    uint32 m_auiTimers[MAX_EVENT];

    uint64 m_auiNpc_BeamGUID[MAX_PORTAL_TYPE];
    uint64 m_auiNpc_PortalGUID[MAX_PORTAL_TYPE];
    uint64 m_auiCurrent_CandidateGUID[MAX_PORTAL_TYPE];
    uint64 m_auiLast_CandidateGUID[MAX_PORTAL_TYPE];
    uint64 m_auiExhaust_CandidateGUID[MAX_PORTAL_TYPE][MAX_RAID_SIZE_10];
    uint64 m_auiRed_Beam_BlockerGUID[MAX_RAID_SIZE_10];

    bool m_bThreat_Increased;

    //0: dominance
    //1: serenity
    //2: perseverence

    void Reset()
    {
        if (m_pInstance)
        {
            //door opens
            m_pInstance->SetData(TYPE_NETHERSPITE, NOT_STARTED);

            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
                pDoor->SetGoState(GO_STATE_ACTIVE);
        }

        m_Active_Phase = NULL_PHASE;
        m_Active_Event = NULL_EVENT;
        m_creature->RemoveAllAuras();

        memset(&m_auiNpc_BeamGUID, 0, sizeof(m_auiNpc_BeamGUID));
        memset(&m_auiNpc_PortalGUID, 0, sizeof(m_auiNpc_PortalGUID));

        memset(&m_auiCurrent_CandidateGUID, 0, sizeof(m_auiCurrent_CandidateGUID));
        memset(&m_auiLast_CandidateGUID, 0, sizeof(m_auiLast_CandidateGUID));

        for (uint8 i=0; i < MAX_PORTAL_TYPE; ++i)
            memset(&m_auiExhaust_CandidateGUID[i], 0, sizeof(m_auiExhaust_CandidateGUID[i]));

        memset(&m_auiRed_Beam_BlockerGUID, 0, sizeof(m_auiRed_Beam_BlockerGUID));
    }

    
    void Aggro(Unit * /*pWho*/)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_NETHERSPITE, IN_PROGRESS);
            //door closes
            if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
                pDoor->SetGoState(GO_STATE_READY);
        }

        SwitchPhase();
        DoMeleeAttackIfReady();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        m_Active_Event = NULL_EVENT;

        for (uint32 i=1; i < uiMaxTimeCounter[m_Active_Phase]; ++i)
        {
            if (m_auiTimers[i])
            {
                if (m_auiTimers[i] <= uiDiff)
                {
                    m_Active_Event = (Events)i;
                    break;
                }
                else m_auiTimers[i] -= uiDiff;
            }
        }

        if (!m_Active_Event)
            return;

        switch (m_Active_Phase)
        {
            case BEAM_PHASE:
                if (m_Active_Event == EVENT_BEAM_END_TIMER)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    SwitchPhase();
                }
                break;
            case BANISH_PHASE:
                if (m_Active_Event == EVENT_BANISH_END_TIMER)
                {
                    m_creature->InterruptNonMeleeSpells(false);
                    SwitchPhase();
                }
                break;
            default:
                break;
        }

        if (m_Active_Phase == BEAM_PHASE || m_Active_Phase == BANISH_PHASE)
        {
            switch (m_Active_Event)
            {
                case EVENT_ENRAGE:
                    DoCastSpellIfCan(m_creature, SPELL_NETHER_INFUSION);
                    m_auiTimers[EVENT_ENRAGE] = 0;
                    break;
                case EVENT_EXHAUST_CHECK:
                    for(uint8 i = 0; i < MAX_PORTAL_TYPE; ++i)
                        DoApplyExhaustion(i);

                    m_auiTimers[EVENT_EXHAUST_CHECK] = 2000;
                    break;
                default:
                    break;
            }
        }

        if (m_Active_Phase == BEAM_PHASE)
        {
            switch (m_Active_Event)
            {
                case EVENT_NETHERSPITE_ROAR:
                    DoScriptText(EMOTE_PHASE_BEAM, m_creature);
                    DoCastSpellIfCan(m_creature, SPELL_NETHERSPITE_ROAR);
                    m_auiTimers[EVENT_NETHERSPITE_ROAR] = 0;
                    break;
                case EVENT_PORTALS_SPAWN:
                    DoSpawnPortals();
                    m_auiTimers[EVENT_PORTALS_SPAWN] = 0;
                    break;
                case EVENT_BEAM_INITIAL:
                    for(uint8 i = 0; i < MAX_PORTAL_TYPE; ++i)
                        DoNetherBeam(i);

                    m_auiTimers[EVENT_BEAM_INITIAL] = 0;
                    m_auiTimers[EVENT_BEAM_PERIODIC] = 1000;
                    break;
                case EVENT_BEAM_PERIODIC:
                    for(uint8 i = 0; i < MAX_PORTAL_TYPE; ++i)
                        DoNetherBeam(i);

                    m_auiTimers[EVENT_BEAM_PERIODIC] = 1000;
                    break;
                case EVENT_NETHERBURN:
                    DoCastSpellIfCan(m_creature, SPELL_NETHERBURN);
                    m_auiTimers[EVENT_NETHERBURN] = 0;
                    break;
                case EVENT_VOID_ZONE:
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_VOID_ZONE, CAST_TRIGGERED);

                    m_auiTimers[EVENT_VOID_ZONE] = 15000;
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }

        else if (m_Active_Phase == BANISH_PHASE)
        {
            switch (m_Active_Event)
            {
                case EVENT_NETHERBREATH_INITIAL:
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_NETHERBREATH);

                    m_auiTimers[EVENT_NETHERBREATH_INITIAL] = 0;
                    m_auiTimers[EVENT_NETHERBREATH] = 5000;
                    break;
                case EVENT_NETHERBREATH:
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        DoCastSpellIfCan(pTarget, SPELL_NETHERBREATH);

                    m_auiTimers[EVENT_NETHERBREATH] = 5000;
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    }

    void JustDied(Unit* /*pKiller*/)
    {
        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_NETHERSPITE, DONE);

        //door opens
        if (GameObject* pDoor = m_pInstance->instance->GetGameObject(m_pInstance->GetData64(DATA_GO_MASSIVE_DOOR)))
            pDoor->SetGoState(GO_STATE_ACTIVE);

        RemovePortals();
    }

    void JustReachedHome()
    {
        RemovePortals();
        RestoreCoordinates();
    }

    void ShuffleCoordinates()
    {
        //we shuffle the portal locations here
        uint8 uiRandom = urand(0, 2);

        if (!uiRandom)
        {
            PortalCoordinates[0] = BasicCoordinates[0];

            if (urand(0, 1))
            {
                PortalCoordinates[1] = BasicCoordinates[1];
                PortalCoordinates[2] = BasicCoordinates[2];
            }
            else
            {
                PortalCoordinates[1] = BasicCoordinates[2];
                PortalCoordinates[2] = BasicCoordinates[1];
            }
        }
        else if (uiRandom == 1)
        {
             PortalCoordinates[0] = BasicCoordinates[2];

            if (urand(0, 1))
            {
                PortalCoordinates[1] = BasicCoordinates[0];
                PortalCoordinates[2] = BasicCoordinates[1];
            }
            else
            {
                PortalCoordinates[1] = BasicCoordinates[1];
                PortalCoordinates[2] = BasicCoordinates[0];
            }
        }
        else
            RestoreCoordinates();
    }

    void RestoreCoordinates()
    {
        for(uint8 i = 0; i < MAX_PORTAL_TYPE; ++i)
            PortalCoordinates[i] = BasicCoordinates[i];
    }

    void DoNetherBeam(uint8 uiColor)
    {
        Unit* pPlayer;

        m_auiCurrent_CandidateGUID[uiColor] = m_creature->GetGUID();
        Unit* pNpc_Beam = m_creature->GetMap()->GetUnit(m_auiNpc_BeamGUID[uiColor]);
        Unit* pNpc_Portal = m_creature->GetMap()->GetUnit(m_auiNpc_PortalGUID[uiColor]);

        if (!pNpc_Beam || !pNpc_Portal)
            return;

        Map *map = m_creature->GetMap();
        if (map->IsDungeon())
        {
            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (PlayerList.isEmpty())
                return;

            //here we calculate possible player blockers, else beam goes to netherspite
            for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
            {
                if (itr->getSource() && itr->getSource()->isAlive())
                {
                    Unit* pPlayer = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID());
                    float fDistance = m_creature->GetDistance(pNpc_Portal);
                    float fAngle = m_creature->GetAngle(pNpc_Portal);
                    float fDistancePlayer = pPlayer->GetDistance(pNpc_Portal);
                    float fAnglePlayer = pPlayer->GetAngle(pNpc_Portal);
                    const float fRadian = 0.2f; //this can be reduced to require more precise placement between netherspite and the portal, for less precision it can be increased.

                    if ((fAngle - fRadian) < fAnglePlayer && fAnglePlayer < (fAngle + fRadian)
                        && fDistancePlayer < fDistance && !pPlayer->HasAura(Beam_Debuff[uiColor]))
                    {
                        if (uiColor == 0 && !pPlayer->HasAura(Beam_Buff[uiColor+1][1]) && !pPlayer->HasAura(Beam_Buff[uiColor+2][1]))
                            m_auiCurrent_CandidateGUID[uiColor] = pPlayer->GetGUID();

                        if (uiColor == 1 && !pPlayer->HasAura(Beam_Buff[uiColor-1][1]) && !pPlayer->HasAura(Beam_Buff[uiColor+1][1]))
                        {
                            if (pPlayer->getPowerType() == POWER_MANA && pPlayer->GetMaxPower(POWER_MANA)>0)  //if player has <=0 maximum mana, beam goes to netherspite
                                m_auiCurrent_CandidateGUID[uiColor] = pPlayer->GetGUID();

                            if (pPlayer->getPowerType() != POWER_MANA)
                                m_auiCurrent_CandidateGUID[uiColor] = pPlayer->GetGUID();
                        }

                        if (uiColor == 2 && !pPlayer->HasAura(Beam_Buff[uiColor-1][1]) && !pPlayer->HasAura(Beam_Buff[uiColor-2][1]))
                            m_auiCurrent_CandidateGUID[uiColor] = pPlayer->GetGUID();
                    }
                }
            }
        }

        if (m_auiCurrent_CandidateGUID[uiColor] == m_creature->GetGUID())
        {
            if (m_auiCurrent_CandidateGUID[uiColor] != m_auiLast_CandidateGUID[uiColor])
            {
                //we need to kill the beamer, or else the beam visual won't disappear
                pNpc_Beam->DealDamage(pNpc_Beam, pNpc_Beam->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                pNpc_Beam = pNpc_Portal->SummonCreature(NPC_PORTAL_CREATURE, PortalCoordinates[uiColor].x, 
                    PortalCoordinates[uiColor].y, PortalCoordinates[uiColor].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                pNpc_Beam->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);    //flag to hide our model
                pNpc_Beam->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                if (pNpc_Beam && pNpc_Beam->isAlive())
                    m_auiNpc_BeamGUID[uiColor] = pNpc_Beam->GetGUID();

                m_auiLast_CandidateGUID[uiColor] = m_creature->GetGUID();
                pNpc_Beam->CastSpell(m_creature, Beam_Visuals[uiColor], true);
            }

            m_creature->CastSpell(m_creature, Beam_Buff[uiColor][0], true);
        }
        else
        {
            if (pPlayer && pPlayer->isAlive())
            {
                if (m_auiCurrent_CandidateGUID[uiColor] != m_auiLast_CandidateGUID[uiColor])
                {
                    pNpc_Beam->DealDamage(pNpc_Beam, pNpc_Beam->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    pNpc_Beam = pNpc_Portal->SummonCreature(NPC_PORTAL_CREATURE, PortalCoordinates[uiColor].x, 
                        PortalCoordinates[uiColor].y, PortalCoordinates[uiColor].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    pNpc_Beam->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);    //flag to hide our model
                    pNpc_Beam->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    if (pNpc_Beam && pNpc_Beam->isAlive())
                        m_auiNpc_BeamGUID[uiColor] = pNpc_Beam->GetGUID();

                    m_auiLast_CandidateGUID[uiColor] = pPlayer->GetGUID();
                    HandleExhaust(m_auiCurrent_CandidateGUID[uiColor],uiColor);

                    if (uiColor == 2)
                        HandleThreatIncrease(m_auiCurrent_CandidateGUID[uiColor]);

                    pNpc_Beam->CastSpell(pPlayer, Beam_Visuals[uiColor], true);
                }
                if (uiColor == 0)
                    pPlayer->CastSpell(pPlayer, Beam_Buff[uiColor][1], true);
                else if (uiColor == 1)
                {
                    pPlayer->ModifyPower(POWER_MANA, pPlayer->GetMaxPower(POWER_MANA));
                    pPlayer->CastSpell(pPlayer, Beam_Buff[uiColor][1], true);
                }
                else if (uiColor == 2)
                {
                    if (pPlayer->GetMaxHealth() <= 1000 && pPlayer->isAlive())
                    {
                        /*we have to do this check here, if we continue and apply one more perseverence buff, player will
                        get negative health (he dies but hes not flagged as dead, causing weird behavior). To prevent this
                        instead of applyng one more stack (which should have killed the player), we manually lower his
                        health -1000 by dealing damage and kill him properly*/
                        m_creature->DealDamage(pPlayer, 1000, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, true); //it seems that durability loss despite being set to true doesn't work
                        return;
                    }

                    pPlayer->CastSpell(pPlayer, Beam_Buff[uiColor][1], true);
                    pPlayer->SetMaxHealth(pPlayer->GetMaxHealth() + 31000);
                    pPlayer->ModifyHealth(pPlayer->GetMaxHealth());
                }
            }
        }
    }

    void HandleExhaust(uint64 uiPlayerGUID, uint8 uiColor)
    {
        bool bExists[MAX_PORTAL_TYPE];

        //storing the possible player's GUIDs that will get the exhaustion debuff
        for (uint8 i=0; i < MAX_PORTAL_TYPE; ++i)
            bExists[i] = false;

        for (uint8 i=0;i < MAX_RAID_SIZE_10; ++i)
        {
            if (m_auiExhaust_CandidateGUID[uiColor][i] == uiPlayerGUID)
            {
                bExists[uiColor] = true;
                break;
            }
        }

        if (!bExists[uiColor])
        {
            for (uint8 i=0;i < MAX_RAID_SIZE_10;++i)
            {
                if (m_auiExhaust_CandidateGUID[uiColor][i] == 0)
                {
                    m_auiExhaust_CandidateGUID[uiColor][i] = uiPlayerGUID;
                    break;
                }
            }
        }
    }

    void HandleThreatIncrease(uint64 uiPlayerGUID)
    {

        m_bThreat_Increased = false;
        for (uint8 i=0; i < MAX_RAID_SIZE_10; ++i)
        {
            if (m_auiRed_Beam_BlockerGUID[i] == uiPlayerGUID)
            {
                m_bThreat_Increased = true;
                break;
            }
        }

        if (!m_bThreat_Increased)
        {
           for (uint8 i=0; i < MAX_RAID_SIZE_10; ++i)
           {
                if (m_auiRed_Beam_BlockerGUID[i] == 0)
                {
                    m_auiRed_Beam_BlockerGUID[i] = uiPlayerGUID;
                    Unit* pPlayer = m_creature->GetMap()->GetUnit(m_auiRed_Beam_BlockerGUID[i]);

                    if (pPlayer)
                        m_creature->getThreatManager().addThreat(pPlayer, 5000000.0f);
                    break;
                }
            }
        }
    }

    void DoApplyExhaustion(uint8 uiColor)
    {

        for (uint8 i=0; i < MAX_RAID_SIZE_10; ++i)
        {
            Unit* pPlayer = m_creature->GetMap()->GetUnit(m_auiExhaust_CandidateGUID[uiColor][i]);

            if (pPlayer && pPlayer->isAlive())
            {
                if (!pPlayer->HasAura(Beam_Buff[uiColor][1]))
                {
                    if (uiColor == 2)
                    {
                        for (uint8 j=0; j < MAX_RAID_SIZE_10; j++)
                        {
                            Unit* pRedBeam_Player = m_creature->GetMap()->GetUnit(m_auiRed_Beam_BlockerGUID[j]);
                            if (pRedBeam_Player && pPlayer == pRedBeam_Player)
                            {
                                if (pRedBeam_Player->isAlive())
                                {
                                    m_creature->getThreatManager().modifyThreatPercent(pRedBeam_Player, -100);
                                    m_creature->getThreatManager().addThreat(pRedBeam_Player, 10.0f);
                                    pRedBeam_Player = NULL;
                                    m_auiRed_Beam_BlockerGUID[j] = 0;
                                }
                                else
                                    m_auiRed_Beam_BlockerGUID[j] = 0;
                            }
                        }
                    }

                    pPlayer->CastSpell(pPlayer, Beam_Debuff[uiColor], true);
                    m_auiExhaust_CandidateGUID[uiColor][i] = 0;
                }
            }
            else
                m_auiExhaust_CandidateGUID[uiColor][i] = 0;
        }
    }

    void RemovePortals()
    {
    Unit* pSummoned;    
        for (uint8 i=0; i < MAX_PORTAL_TYPE; ++i)
        {
           pSummoned = m_creature->GetMap()->GetUnit(m_auiNpc_BeamGUID[i]);
            if (pSummoned && pSummoned->isAlive())
                pSummoned->DealDamage(pSummoned, pSummoned->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

            pSummoned = m_creature->GetMap()->GetUnit(m_auiNpc_PortalGUID[i]);
            if (pSummoned && pSummoned->isAlive())
                pSummoned->DealDamage(pSummoned, pSummoned->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        }
    }

    void SwitchPhase()
    {
        switch(m_Active_Phase)
        {
            case NULL_PHASE: //initialize
                m_Active_Phase = BEAM_PHASE;
                m_creature->RemoveAllAuras();
                AttackStart(m_creature->getVictim());
                DoCastSpellIfCan(m_creature, SPELL_EMPOWERMENT, CAST_TRIGGERED);

                m_auiTimers[EVENT_ENRAGE] = 540000;
                m_auiTimers[EVENT_EXHAUST_CHECK] = 2000;
                m_auiTimers[EVENT_NETHERSPITE_ROAR] = 0;
                m_auiTimers[EVENT_PORTALS_SPAWN] = 4000;
                m_auiTimers[EVENT_BEAM_END_TIMER] = 60000;
                m_auiTimers[EVENT_BEAM_INITIAL] = 14000;
                m_auiTimers[EVENT_BEAM_PERIODIC] = 0;
                m_auiTimers[EVENT_NETHERBURN] = 5500;
                m_auiTimers[EVENT_VOID_ZONE] = 15000;
                break;
            case BEAM_PHASE:  //enter banish phase
                m_Active_Phase = BANISH_PHASE;
                DoCastSpellIfCan(m_creature, SPELL_BANISH_VISUAL, CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_ROOT, CAST_TRIGGERED);
                m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, true);   //adding immunity to melee during banish phase
                DoResetThreat();
                RemovePortals();
                ShuffleCoordinates();
                AttackStart(m_creature->getVictim());
                DoScriptText(EMOTE_PHASE_BANISH, m_creature);

                m_auiTimers[EVENT_BANISH_END_TIMER] = 30000;
                m_auiTimers[EVENT_NETHERBREATH_INITIAL] = 10000;
                m_auiTimers[EVENT_NETHERBREATH] = 0;

                memset(&m_auiLast_CandidateGUID, 0, sizeof(m_auiLast_CandidateGUID));
                break;
            case BANISH_PHASE:  //enter beam phase
                m_Active_Phase = BEAM_PHASE;
                m_creature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, false);  //removing immunity to melee after banish phase
                m_creature->RemoveAurasDueToSpell(SPELL_BANISH_VISUAL, 0);
                m_creature->RemoveAurasDueToSpell(SPELL_ROOT, 0);
                DoResetThreat();
                AttackStart(m_creature->getVictim());

                m_auiTimers[EVENT_NETHERSPITE_ROAR] = 500;
                m_auiTimers[EVENT_PORTALS_SPAWN] = 4000;
                m_auiTimers[EVENT_BEAM_END_TIMER] = 60000;
                m_auiTimers[EVENT_BEAM_INITIAL] = 14000;
                m_auiTimers[EVENT_BEAM_PERIODIC] = 0;
                m_auiTimers[EVENT_NETHERBURN] = 0;
                m_auiTimers[EVENT_VOID_ZONE] = 15000;
                break;
            default:
                break;
        }

        m_Active_Event = NULL_EVENT;
    }

    void DoSpawnPortals()
    {
        Unit* pSummoned;

        for (uint8 i=0; i < MAX_PORTAL_TYPE; ++i)
        {
            pSummoned = m_creature->SummonCreature(NPC_PORTAL_CREATURE, PortalCoordinates[i].x, 
                PortalCoordinates[i].y, PortalCoordinates[i].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            pSummoned->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);    //flag to hide our model
            pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_auiNpc_BeamGUID[i] = pSummoned->GetGUID();

            pSummoned = m_creature->SummonCreature(NPC_PORTAL_CREATURE, PortalCoordinates[i].x, 
                PortalCoordinates[i].y, PortalCoordinates[i].z, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
            pSummoned->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK1);    //flag to hide our model
            pSummoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            pSummoned->CastSpell(pSummoned, Portals[i], true);
            m_auiNpc_PortalGUID[i] = pSummoned->GetGUID();
        }
    }
};

struct MANGOS_DLL_DECL mob_void_zoneAI : public Scripted_NoMovementAI
{
    mob_void_zoneAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bConsumption;
    uint32 m_uiDespawn_Timer;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED);
        m_bConsumption = false;
        m_uiDespawn_Timer = 90000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bConsumption)
        {
            m_bConsumption = true;
            DoCastSpellIfCan(m_creature, SPELL_CONSUMPTION);
        }

        if (m_uiDespawn_Timer <= uiDiff)
            m_creature->ForcedDespawn();
        else
            m_uiDespawn_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_netherspite(Creature* pCreature)
{
    return new boss_netherspiteAI(pCreature);
}

CreatureAI* GetAI_mob_void_zone(Creature* pCreature)
{
    return new mob_void_zoneAI(pCreature);
}

void AddSC_boss_netherspite()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_netherspite";
    newscript->GetAI = &GetAI_boss_netherspite;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_void_zone";
    newscript->GetAI = &GetAI_mob_void_zone;
    newscript->RegisterSelf();
}
