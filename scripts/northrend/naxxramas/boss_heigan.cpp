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
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDisruption_Timer;
    uint32 m_uiFeaver_Timer;
    uint32 m_uiErupt_Timer;
    uint32 m_uiPhase_Timer;
    uint32 m_uiPlagueCloudTimer;

    uint32 m_uiSafeSpot;
    uint32 m_uiFastTimer;
    uint32 m_uiSlowTimer;
    bool m_bforward;

    uint8 phase;

    void Reset()
    {
        if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        phase = 0;
        m_uiSafeSpot = 1;
        m_uiFastTimer = 3500;
        m_uiSlowTimer = 10500;
        m_bforward = true;

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_HEIGAN, NOT_STARTED);
            m_pInstance->SetAchiev(TYPE_HEIGAN, false);
        }
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if(phase != PHASE_GROUND)
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
        if(phase == PHASE_GROUND)
        {
            m_creature->InterruptNonMeleeSpells(false);
            m_uiFeaver_Timer = 20000;
            m_uiPhase_Timer = 90000;
            m_uiSlowTimer = 10500; 
            m_uiSafeSpot = 1;
            m_uiDisruption_Timer = 5000+urand(0,4000);
            if(m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoStartMovement(m_creature->getVictim());
        }else if(phase == PHASE_PLATFORM)
        {
            m_creature->InterruptNonMeleeSpells(true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->Clear(false);
            m_creature->GetMotionMaster()->MoveIdle();
            DoCast(m_creature, SPELL_TELEPORT_SELF);

            m_uiPhase_Timer = 45000;
            m_uiSafeSpot = 1;
            m_uiFastTimer = 3500;
            m_uiPlagueCloudTimer = 1000;
        }
    }
    void Aggro(Unit *who)
    {
        SetPhase(1);
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_HEIGAN, IN_PROGRESS);
            m_pInstance->SetAchiev(TYPE_HEIGAN, true);
        }
    }

    void KilledUnit(Unit* victim)
    {
        if(m_pInstance)
            m_pInstance->SetAchiev(TYPE_HEIGAN, false);
        DoScriptText(SAY_SLAY, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_HEIGAN, DONE);
    }

    std::list<GameObject*> GetGameObjectsByEntry(uint32 entry)
    {
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.SetNoCreate();
 
        std::list<GameObject*> gameobjectList;
 
        AllGameObjectsWithEntryInRangeCheck check(m_creature, entry, 100);
        MaNGOS::GameObjectListSearcher<AllGameObjectsWithEntryInRangeCheck> searcher(gameobjectList, check);
        TypeContainerVisitor<MaNGOS::GameObjectListSearcher<AllGameObjectsWithEntryInRangeCheck>, GridTypeMapContainer> visitor(searcher);
 
        cell.Visit(pair, visitor, *(m_creature->GetMap()), *m_creature, 100);
 
        return gameobjectList;
    }

    //Let's Dance!
    void DoErupt(uint32 safePlace)
    {
        uint64 heiganGUID = m_pInstance->GetData64(NPC_HEIGAN);
        Map::PlayerList const &PlList = m_pInstance->instance->GetPlayers();
        if (PlList.isEmpty())
            return;
 
        if(safePlace != 1)
        {
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
            if (m_pInstance)
            {
                for (std::list<uint64>::iterator itr = m_pInstance->m_lEruptionObjectOneGUIDs.begin(); itr != m_pInstance->m_lEruptionObjectOneGUIDs.end(); ++itr)
                {
                    if (GameObject* pEruption = m_creature->GetMap()->GetGameObject((*itr)))
                    {
                        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                        {
                            if (Player* pPlayer = i->getSource())
                            {
                                if (pPlayer->isGameMaster())
                                    continue;
         
                                if (pPlayer->isAlive())
                                {
                                    if(pPlayer->GetDistance(pEruption) <= 4.0f)
                                        pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, pEruption->GetGUID()); 
                                }
                            }
                        }
                    }
                }
            }
        }
        //Change direction of dance
        else m_bforward = true;
        if(safePlace != 2)
        {
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
            if (m_pInstance)
            {
                for (std::list<uint64>::iterator itr = m_pInstance->m_lEruptionObjectTwoGUIDs.begin(); itr != m_pInstance->m_lEruptionObjectTwoGUIDs.end(); ++itr)
                {
                    if (GameObject* pEruption = m_creature->GetMap()->GetGameObject((*itr)))
                    {
                        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                        {
                            if (Player* pPlayer = i->getSource())
                            {
                                if (pPlayer->isGameMaster())
                                    continue;
         
                                if (pPlayer->isAlive())
                                {
                                    if(pPlayer->GetDistance(pEruption) <= 4.0f)
                                        pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, pEruption->GetGUID()); 
                                }
                            }
                        }
                    }
                }
            }
        }
        if(safePlace != 3)
        {
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
            if (m_pInstance)
            {
                for (std::list<uint64>::iterator itr = m_pInstance->m_lEruptionObjectThreeGUIDs.begin(); itr != m_pInstance->m_lEruptionObjectThreeGUIDs.end(); ++itr)
                {
                    if (GameObject* pEruption = m_creature->GetMap()->GetGameObject((*itr)))
                    {
                        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                        {
                            if (Player* pPlayer = i->getSource())
                            {
                                if (pPlayer->isGameMaster())
                                    continue;
         
                                if (pPlayer->isAlive())
                                {
                                    if(pPlayer->GetDistance(pEruption) <= 4.0f)
                                        pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, pEruption->GetGUID()); 
                                }
                            }
                        }
                    }
                }
            }
        }
        if(safePlace != 4)
        {
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
            if (m_pInstance)
            {
                for (std::list<uint64>::iterator itr = m_pInstance->m_lEruptionObjectFourGUIDs.begin(); itr != m_pInstance->m_lEruptionObjectFourGUIDs.end(); ++itr)
                {
                    if (GameObject* pEruption = m_creature->GetMap()->GetGameObject((*itr)))
                    {
                        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
                        {
                            if (Player* pPlayer = i->getSource())
                            {
                                if (pPlayer->isGameMaster())
                                    continue;
         
                                if (pPlayer->isAlive())
                                {
                                    if(pPlayer->GetDistance(pEruption) <= 4.0f)
                                        pPlayer->CastSpell(pPlayer, SPELL_ERUPTION, true, 0, 0, pEruption->GetGUID()); 
                                }
                            }
                        }
                    }
                }
            }
        //Let's dance back!
        }else m_bforward=false;
    }
    void UpdateAI(const uint32 diff)
    {
        if(phase == 0)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return; 

        if (m_uiPhase_Timer < diff)
        {
            if (phase == PHASE_GROUND)
                SetPhase(PHASE_PLATFORM);
            else SetPhase(PHASE_GROUND);
        }else m_uiPhase_Timer -= diff;

        if (phase == PHASE_GROUND)
        {
            if (m_uiDisruption_Timer < diff)
            {
                //DoCastSpellIfCan(m_creature->getVictim(), SPELL_DISRUPTION);
                m_uiDisruption_Timer = 5000+rand()%10000;
            }else m_uiDisruption_Timer -= diff;

            if (m_uiFeaver_Timer < diff)
            {
                //DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DECREPIT_FEVER_N : SPELL_DECREPIT_FEVER_H);
                m_uiFeaver_Timer = 30000+rand()%10000;
            }else m_uiFeaver_Timer -= diff;

            if(m_uiSlowTimer < diff)
            {
                DoErupt(m_uiSafeSpot);
                if(m_bforward)
                    m_uiSafeSpot++;
                else
                    m_uiSafeSpot--;
                m_uiSlowTimer = 10500;
            }else m_uiSlowTimer-=diff;
        }

        if (phase == PHASE_PLATFORM)
        {
            if (m_uiPlagueCloudTimer < diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_PLAGUE_CLOUD);
                m_uiPlagueCloudTimer  = 999999;
            }else m_uiPlagueCloudTimer -= diff;

            if(m_uiFastTimer < diff)
            {
                DoErupt(m_uiSafeSpot);
                if(m_bforward)
                    m_uiSafeSpot++;
                else
                    m_uiSafeSpot--;
                m_uiFastTimer = 3500;
            }else m_uiFastTimer-=diff;
        }

        DoMeleeAttackIfReady();
    }
};


CreatureAI* GetAI_boss_heigan(Creature* pCreature)
{
    return new boss_heiganAI(pCreature);
}

void AddSC_boss_heigan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_heigan";
    newscript->GetAI = &GetAI_boss_heigan;
    newscript->RegisterSelf();
}

