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
SDName: Boss_Heigan
SD%Complete: 65
SDComment: Missing traps dance
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    PHASE_GROUND            = 1,
    PHASE_PLATFORM          = 2,

    SAY_AGGRO1              = -1533109,
    SAY_AGGRO2              = -1533110,
    SAY_AGGRO3              = -1533111,
    SAY_SLAY                = -1533112,
    SAY_TAUNT1              = -1533113,
    SAY_TAUNT2              = -1533114,
    SAY_TAUNT3              = -1533115,
    SAY_TAUNT4              = -1533117,
    SAY_CHANNELING          = -1533116,
    SAY_DEATH               = -1533118,
    EMOTE_TELEPORT          = -1533136,
    EMOTE_RETURN            = -1533137,

    SPELL_ERUPTION          = 29371,                        //Spell used by floor pieces to cause damage to players

    //Spells by boss
    SPELL_DECREPIT_FEVER_N  = 29998,
    SPELL_DECREPIT_FEVER_H  = 55011,
    SPELL_DISRUPTION        = 29310,
    SPELL_TELEPORT_SELF     = 30211,
    SPELL_PLAGUE_CLOUD      = 29350,

    //Spell by eye stalks
    SPELL_MIND_FLAY         = 26143,

    HEIGAN_TRIGGER          = 45101
};

#define POS_X 2793.86f
#define POS_Y -3707.38f
#define POS_Z 276.627f
#define POS_O 0.593f

#define TRIGGER_X 2769.f
#define TRIGGER_Y -3671.f
#define TRIGGER_Z 273.667f
#define TRIGGER_O 0.f


struct MANGOS_DLL_DECL boss_heiganAI : public ScriptedAI
{
    boss_heiganAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 Disruption_Timer;
    uint32 Feaver_Timer;
    uint32 Erupt_Timer;
    uint32 Phase_Timer;
    uint32 PlagueCloudTimer;

    uint32 eruptSection;
    bool eruptDirection;

    uint8 phase;

    void Reset()
    {
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        phase = 0;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, NOT_STARTED);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if(phase != 1)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho, 0.0f);
            DoStartMovement(pWho);
        }

    }
    void SetPhase(uint8 tPhase)
    {
        phase = tPhase;
        if(phase == 1)
        {
            m_creature->InterruptNonMeleeSpells(false);
            Feaver_Timer = 20000;
            Phase_Timer = 90000;
            Disruption_Timer = 5000+rand()%10000;
            if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoStartMovement(m_creature->getVictim());
        }else if(phase == 2)
        {
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            DoCast(m_creature, SPELL_TELEPORT_SELF);

            Phase_Timer = 45000;
            PlagueCloudTimer = 2000;
        }
    }
    void Aggro(Unit *who)
    {
		m_creature->SummonCreature(HEIGAN_TRIGGER, TRIGGER_X, TRIGGER_Y, TRIGGER_Z, TRIGGER_O, TEMPSUMMON_CORPSE_DESPAWN, 0);
        SetPhase(1);
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if(m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, IN_PROGRESS);
    }

    void KilledUnit(Unit* victim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if(phase == 0)
            return;

        if (Phase_Timer < diff)
        {
            if ( phase == 1)
                SetPhase(2);
            else
                SetPhase(1);
        }else Phase_Timer -= diff;

        if ( phase == 2 && PlagueCloudTimer < diff)
        {
            DoCast(m_creature, SPELL_PLAGUE_CLOUD);
            PlagueCloudTimer  = 999999;
        }
        else
            PlagueCloudTimer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() || phase != 1)
            return;
        if (Disruption_Timer < diff)
        {
            DoCast(m_creature->getVictim(), SPELL_DISRUPTION);
            Disruption_Timer = 5000+rand()%10000;
        }else Disruption_Timer -= diff;

        if (Feaver_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DECREPIT_FEVER_N : SPELL_DECREPIT_FEVER_H);
            Feaver_Timer = 30000+rand()%10000;
        }else Feaver_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_heigan_eruptionAI : public ScriptedAI
{
    npc_heigan_eruptionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Reset();
    }
    ScriptedInstance* pInstance;
 
    uint32 phase;
    uint32 safeSpot;
    uint32 fastTimer;
    uint32 phaseTimer;
    uint32 slowTimer;
    bool forward;
    std::list<GameObject*> GetGameObjectsByEntry(uint32 entry)
    {
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();
 
        std::list<GameObject*> gameobjectList;

		AllGameObjectsWithEntryInRangeCheck check(m_creature, entry, 100);
        MaNGOS::GameObjectListSearcher<AllGameObjectsWithEntryInRangeCheck> searcher(gameobjectList, check);
        TypeContainerVisitor<MaNGOS::GameObjectListSearcher<AllGameObjectsWithEntryInRangeCheck>, GridTypeMapContainer> visitor(searcher);
 
        cell.Visit(pair, visitor, *(m_creature->GetMap()));
 
        return gameobjectList;
    }
    //Let's Dance!
    void DoErupt(uint32 safePlace)
    {
        uint64 heiganGUID = pInstance->GetData64(NPC_HEIGAN);
        Map::PlayerList const &PlList = pInstance->instance->GetPlayers();
        if (PlList.isEmpty())
            return;
 
        if(safePlace != 1)
        {
            std::list<GameObject*> eruptGOs = GetGameObjectsByEntry(181678);
            //Visual part of eruption
            for (int32 i = 181510; i <= 181526; i++)
            {
                if (i == 181513 || i == 181512 || i == 181511 || i == 181525 || i == 181514 || i == 181515 || i == 181516)
                    continue;
                std::list<GameObject*> visualGO = GetGameObjectsByEntry(i);
                for (std::list<GameObject*>::iterator itr = visualGO.begin(); itr != visualGO.end(); ++itr)
                {
                    if((*itr))
                        //Required GO Custom Animation Patch for this
                    {
                        WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM,8+4);
                        data << (*itr)->GetGUID();
                        data << 0;
                        (*itr)->SendMessageToSet(&data,true);
                    }
                }
            }
            //Damage part of eruption
            for (std::list<GameObject*>::iterator itr = eruptGOs.begin(); itr != eruptGOs.end(); ++itr)
            {
                if(!(*itr))
                    continue;
                for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                {
                    if (Player* pPlayer = i->getSource())
                    {
                        if (pPlayer->isGameMaster())
                            continue;
 
                        if (pPlayer->isAlive())
                        {
                            if(pPlayer->GetDistance((*itr)) <= 4.0f)
                                //We use originalCaster for deal damage by Plague Fissure
                                pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, m_creature->GetGUID());                        }
                    }
                }
            }
        }
        //Change direction of dance
        else forward = true;
        if(safePlace != 2)
        {
            std::list<GameObject*> eruptGOs = GetGameObjectsByEntry(181676);
            for (int32 i = 181511; i <= 181531; i++)
            {
                if ((i > 181516 && i < 181525) || (i == 181526))
                    continue;
                std::list<GameObject*> visualGO = GetGameObjectsByEntry(i);
                for (std::list<GameObject*>::iterator itr = visualGO.begin(); itr != visualGO.end(); ++itr)
                {
                    if((*itr))
                    {
                        WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM,8+4);
                        data << (*itr)->GetGUID();
                        data << 0;
                        (*itr)->SendMessageToSet(&data,true);
                    }
                }
            }
            for (std::list<GameObject*>::iterator itr = eruptGOs.begin(); itr != eruptGOs.end(); ++itr)
            {
                if(!(*itr))
                    continue;
                for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                {
                    if (Player* pPlayer = i->getSource())
                    {
                        if (pPlayer->isGameMaster())
                            continue;
 
                        if (pPlayer->isAlive())
                        {
                            if(pPlayer->GetDistance((*itr)) <= 4.0f)
                                pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, m_creature->GetGUID());                        }
                    }
                }
            }
        }
        if(safePlace != 3)
        {
            std::list<GameObject*> eruptGOs = GetGameObjectsByEntry(181677);
            for (int32 i = 181532; i <= 181545; i++)
            {
                if (i >= 181537 && i <= 181539)
                    continue;
                std::list<GameObject*> visualGO = GetGameObjectsByEntry(i);
                for (std::list<GameObject*>::iterator itr = visualGO.begin(); itr != visualGO.end(); ++itr)
                {
                    if((*itr))
                    {
                        WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM,8+4);
                        data << (*itr)->GetGUID();
                        data << 0;
                        (*itr)->SendMessageToSet(&data,true);
                    }
                }
            }
            for (std::list<GameObject*>::iterator itr = eruptGOs.begin(); itr != eruptGOs.end(); ++itr)
            {
                if(!(*itr))
                    continue;
                for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                {
                    if (Player* pPlayer = i->getSource())
                    {
                        if (pPlayer->isGameMaster())
                            continue;
 
                        if (pPlayer->isAlive())
                        {
                            if(pPlayer->GetDistance((*itr)) <= 4.0f)
                                pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, m_creature->GetGUID());                        }
                    }
                }
            }
        }
        if(safePlace != 4)
        {
            std::list<GameObject*> eruptGOs = GetGameObjectsByEntry(181695);
            for (int32 i = 181537; i <= 181552; i++)
            {
                if (i > 181539 && i < 181545)
                    continue;
                std::list<GameObject*> visualGO = GetGameObjectsByEntry(i);
                for (std::list<GameObject*>::iterator itr = visualGO.begin(); itr != visualGO.end(); ++itr)
                {
                    if((*itr))
                    {
                        WorldPacket data(SMSG_GAMEOBJECT_CUSTOM_ANIM,8+4);
                        data << (*itr)->GetGUID();
                        data << 0;
                        (*itr)->SendMessageToSet(&data,true);
                    }
                }
            }
            for (std::list<GameObject*>::iterator itr = eruptGOs.begin(); itr != eruptGOs.end(); ++itr)
            {
                if(!(*itr))
                    continue;
                for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                {
                    if (Player* pPlayer = i->getSource())
                    {
                        if (pPlayer->isGameMaster())
                            continue;
 
                        if (pPlayer->isAlive())
                        {
                            if(pPlayer->GetDistance((*itr)) <= 4.0f)
                                pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, m_creature->GetGUID());
                        }
                    }
                }
            }
        //Let's dance back!
        }else forward=false;
    }
 
    void Reset()
    {
        phase = 1;
        safeSpot = 1;
        fastTimer = 3500;
        slowTimer = 10500;
        phaseTimer = 90000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }
    void Aggro(Unit* who)
    {
        //This is just for dance. It doesn't attack anybody.
        DoStopAttack();
        SetCombatMovement(false);
    }
    void UpdateAI(const uint32 diff)
    {
        if(m_creature->GetMapId() != 533)
            return;

        if(pInstance->GetData(TYPE_HEIGAN) != IN_PROGRESS)
        {
            m_creature->ForcedDespawn();
        }
 
        if (phase == 1)
        {
            if(phaseTimer < diff)
            {
                // Let's fast dance
                phase = 2;
                phaseTimer = 45000;
                safeSpot = 1;
            }
            else
            {
                phaseTimer-=diff;
                if(slowTimer < diff)
                {
                    DoErupt(safeSpot);
                    if(forward)
                        safeSpot++;
                    else
                        safeSpot--;
                    slowTimer = 10500;
                }else slowTimer-=diff;
            }
        }
        else if(phase == 2)
        {
            if(phaseTimer < diff)
            {
                // Slow dance again
                phase = 1;
                phaseTimer = 90000;
                safeSpot = 1;
            }
            else 
            {
                phaseTimer-=diff;
                if(fastTimer < diff)
                {
                    DoErupt(safeSpot);
                    if(forward)
                        safeSpot++;
                    else
                        safeSpot--;
                    fastTimer = 3500;
                }else fastTimer-=diff;
            }
        }
    }
};

CreatureAI* GetAI_boss_heigan(Creature* pCreature)
{
    return new boss_heiganAI(pCreature);
}

CreatureAI* GetAI_npc_heigan_eruptionAI(Creature* pCreature)
{
    return new npc_heigan_eruptionAI(pCreature);
}

void AddSC_boss_heigan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_heigan";
    newscript->GetAI = &GetAI_boss_heigan;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_heigan_eruption";
    newscript->GetAI = &GetAI_npc_heigan_eruptionAI;
    newscript->RegisterSelf();
}

