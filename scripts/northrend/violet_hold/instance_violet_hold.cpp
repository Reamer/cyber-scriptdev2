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
SDName: Instance_Violet_Hold
SD%Complete: 50
SDComment: "experimental" use of header/source object
SDCategory: Violet Hold
EndScriptData */

#include "precompiled.h"
#include "violet_hold.h"

/* The Violet Hold encounters:
1 Main Event
2 Erekem
3 Moragg
4 Ichoron
5 Xevozz
6 Lavanthor
7 Zuramat
8 Cyanigosa
*/

instance_violet_hold::instance_violet_hold(Map* pMap) : ScriptedInstance(pMap),
    m_uiSinclariGUID(0),
    m_uiSinclariAltGUID(0),
    m_uiErekemGUID(0),
    m_uiMoraggGUID(0),
    m_uiIchoronGUID(0),
    m_uiXevozzGUID(0),
    m_uiLavanthorGUID(0),
    m_uiZuramatGUID(0),

    m_uiSealDoorGUID(0),
    m_uiErekemDoorGUID(0),
    m_uiErekemDoorLeftGUID(0),
    m_uiErekemDoorRightGUID(0),
    m_uiMoraggDoorGUID(0),
    m_uiIchoronDoorGUID(0),
    m_uiXevozzDoorGUID(0),
    m_uiLavanthorDoorGUID(0),
    m_uiZuramatDoorGUID(0),

    m_uiCellErekemGuard_LGUID(0),
    m_uiCellErekemGuard_RGUID(0),
    m_uiIntroCrystalGUID(0),

    m_uiWorldState(0),
    m_uiWorldStateSealCount(100),
    m_uiWorldStatePortalCount(0),

    m_uiPortalId(0),
    m_uiPortalTimer(0),
    m_uiMaxCountPortalLoc(0),
    m_uiSealDmgSay(0),
    m_uiChosenBoss(0)
{
    Initialize();
}

GameObject* pSealDoor;
std::string m_strInstData;

void instance_violet_hold::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    m_uiMaxCountPortalLoc = (sizeof(afPortalLocation)/sizeof(sPortalData)) - 1;
}
void instance_violet_hold::OnPlayerEnter(Player* pPlayer)
{
    if(m_auiEncounter[0] != NOT_STARTED)
        pPlayer->SendUpdateWorldState(WORLD_STATE_ID,1);
}
void instance_violet_hold::ResetVariables()
{
    m_uiWorldStateSealCount = 100;
    m_uiWorldStatePortalCount = 0;
    m_uiSealDmgSay = 0;
    m_uiChosenBoss = 0;
}

void instance_violet_hold::ResetAll()
{
    ResetVariables();
    UpdateWorldState(false);
    CallGuards(true);
    SetIntroPortals(false);
}

void instance_violet_hold::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_SINCLARI: m_uiSinclariGUID = pCreature->GetGUID(); break;
        case NPC_SINCLARI_ALT: m_uiSinclariAltGUID = pCreature->GetGUID(); break;
        case NPC_DOOR_SEAL: m_uiSealDoorGUID = pCreature->GetGUID(); break;

        case NPC_EREKEM:
            m_uiErekemGUID = pCreature->GetGUID();
            break;
        case NPC_MORAGG:
            m_uiMoraggGUID = pCreature->GetGUID();
            break;
        case NPC_ICHORON:
            m_uiIchoronGUID = pCreature->GetGUID();
            break;
        case NPC_XEVOZZ:
            m_uiXevozzGUID = pCreature->GetGUID();
            break;
        case NPC_LAVANTHOR:
            m_uiLavanthorGUID = pCreature->GetGUID();
            break;
        case NPC_ZURAMAT:
            m_uiZuramatGUID = pCreature->GetGUID();
            break;

        case NPC_PORTAL_INTRO:
            m_lIntroPortalList.push_back(pCreature->GetGUID());
            break;
        case NPC_HOLD_GUARD:
            m_lGuardsList.push_back(pCreature->GetGUID());
            pCreature->setFaction(35);
            break;
    }
}

void instance_violet_hold::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_DOOR_SEAL:
            m_uiSealDoorGUID = pGo->GetGUID();
            pSealDoor = pGo;
            DoUseDoorOrButton(pGo->GetGUID());
            break;
        case GO_DOOR_EREKEM:
            m_uiErekemDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_EREKEM_LEFT:
            m_uiErekemDoorLeftGUID = pGo->GetGUID();
            break;
        case GO_DOOR_EREKEM_RIGHT:
            m_uiErekemDoorRightGUID = pGo->GetGUID();
            break;
        case GO_DOOR_MORAGG:
            m_uiMoraggDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_ICHORON:
            m_uiIchoronDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_XEVOZZ:
            m_uiXevozzDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_LAVANTHOR:
            m_uiLavanthorDoorGUID = pGo->GetGUID();
            break;
        case GO_DOOR_ZURAMAT:
            m_uiZuramatDoorGUID = pGo->GetGUID();
            break;

        case GO_INTRO_CRYSTAL:
            m_uiIntroCrystalGUID = pGo->GetGUID();
            break;
    }
}

void instance_violet_hold::UpdateWorldState(bool bEnable)
{
    if (bEnable)
        m_uiWorldState = 1;
    else
        m_uiWorldState = 0;

    DoUpdateWorldState(WORLD_STATE_ID, m_uiWorldState);
    DoUpdateWorldState(WORLD_STATE_SEAL, m_uiWorldStateSealCount);
    DoUpdateWorldState(WORLD_STATE_PORTALS, m_uiWorldStatePortalCount);
}

void instance_violet_hold::SetData(uint32 uiType, uint32 uiData)
{
    debug_log("SD2: instance_violet_hold: SetData got type % u, data %u.", uiType, uiData);

    switch(uiType)
    {
        case TYPE_MAIN:
        {
            if (uiData == m_auiEncounter[0])
                return;

            switch(uiData)
            {
                case NOT_STARTED:
                    ResetAll();
                    break;
                case IN_PROGRESS:
                    
                    //DoUseDoorOrButton(m_uiSealDoorGUID); // is not working for unknown reason
                    if(pSealDoor)
                        pSealDoor->UseDoorOrButton();
                    UpdateWorldState();
                    m_uiPortalId = urand(0, 2);
                    m_uiPortalTimer = 15000;
                    break;
                case FAIL:
                    /*if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                        pSinclari->Respawn();
                        ResetAll();*/
                case DONE:
                    UpdateWorldState(false);
                    //DoUseDoorOrButton(m_uiSealDoorGUID);
                    if(pSealDoor)
                        pSealDoor->ResetDoorOrButton();
                    if(Creature* pDoorSeal = instance->GetCreature(m_uiSealDoorGUID))
                        pDoorSeal->ForcedDespawn();
                    break;
                case SPECIAL:
                    break;
            }
            m_auiEncounter[0] = uiData;
            break;
        }
        case TYPE_SEAL:
            m_auiEncounter[1] = uiData;
            if(uiData == SPECIAL && m_auiEncounter[TYPE_MAIN] == IN_PROGRESS) 
            {
                --m_uiWorldStateSealCount;
                if(Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                {
                    if(m_uiSealDmgSay==0 && m_uiWorldStateSealCount<=75)
                    {
                        ++m_uiSealDmgSay;
                        DoScriptText(SAY_SEAL_75,pSinclari);
                    }
                    else if(m_uiSealDmgSay==1 && m_uiWorldStateSealCount<=50)
                    {
                        ++m_uiSealDmgSay;
                        DoScriptText(SAY_SEAL_50,pSinclari);
                    }
                    else if(m_uiSealDmgSay==2 && m_uiWorldStateSealCount<=5)
                    {
                        ++m_uiSealDmgSay;
                        DoScriptText(SAY_SEAL_5,pSinclari);
                    }
                    UpdateWorldState();
                    if(m_uiWorldStateSealCount <= 0)
                        SetData(TYPE_MAIN,FAIL);
                }
            }
            break;
        case TYPE_PORTAL:
        {
            if(m_auiEncounter[TYPE_MAIN] == IN_PROGRESS)
                switch(uiData)
                {
                    case SPECIAL:                               // timer to next
                        m_uiPortalTimer = 90000;
                        break;
                    case DONE:                                  // portal done, set timer to 5 secs
                        m_uiPortalTimer = 5000;
                        break;
                }
            m_auiEncounter[2] = uiData;
            break;
        }
        case TYPE_EREKEM: 
        case TYPE_MORAGG:
        case TYPE_ICHORON:
        case TYPE_XEVOZZ:
        case TYPE_LAVANTHOR:
        case TYPE_ZURAMAT:
            m_auiEncounter[uiType] = uiData;
            if(uiData == DONE)
                m_uiPortalTimer = 20000;
            break;
        case TYPE_CYANIGOSA:
            m_auiEncounter[uiType] = uiData;
            if(uiData == DONE)
                SetData(TYPE_MAIN,DONE);
            break;
    }
    if (uiData == DONE)
    {
        //check if boss was completed
        for(int i = TYPE_EREKEM;i<TYPE_CYANIGOSA+1;i++)
            if(m_auiEncounter[i]==IN_PROGRESS || m_auiEncounter[i]==SPECIAL)
                return;

            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;

            for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                saveStream << m_auiEncounter[i] << " ";

            m_strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
    }
}
uint32 instance_violet_hold::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_MAIN:
            return m_auiEncounter[0];
        case TYPE_EREKEM:
            return m_auiEncounter[3];
        case TYPE_MORAGG:
            return m_auiEncounter[4];
        case TYPE_ICHORON:
            return m_auiEncounter[5];
        case TYPE_XEVOZZ:
            return m_auiEncounter[6];
        case TYPE_LAVANTHOR:
            return m_auiEncounter[7];
        case TYPE_ZURAMAT:
            return m_auiEncounter[8];
        case TYPE_RAND_BOSS_ID:
        {
                uint8 m_uiBoss = 0;
                do
                {
                    m_uiBoss = urand(3,8);
                } while (m_uiBoss == m_uiChosenBoss);
                m_uiChosenBoss = m_uiBoss;
                return m_uiBoss;
        }
        case TYPE_SEAL: return m_uiWorldStateSealCount;
    }
    return 0;
}
void instance_violet_hold::SetIntroPortals(bool bDeactivate)
{
    for(std::list<uint64>::iterator i = m_lIntroPortalList.begin(); i != m_lIntroPortalList.end(); ++i)
    {
        if (Creature* pPortal = instance->GetCreature(*i))
        {
            if (bDeactivate)
                pPortal->ForcedDespawn();
            else
                pPortal->Respawn();
        }
    }
}

void instance_violet_hold::SpawnPortal()
{
    if (const sPortalData* pData = GetPortalData())
    {
        if (Creature* pController = instance->GetCreature(m_uiSinclariAltGUID))
        {
            uint32 uiPortalEntry;

            switch(pData->pPortalType)
            {
                case PORTAL_TYPE_NORM: uiPortalEntry = NPC_PORTAL; break;
                case PORTAL_TYPE_SQUAD:
                case PORTAL_TYPE_BOSS: uiPortalEntry = NPC_PORTAL_ELITE; break;
            }

            pController->SummonCreature(uiPortalEntry, pData->fX, pData->fY, pData->fZ, pData->fOrient, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 1800*IN_MILLISECONDS);
        }
    }
}

void instance_violet_hold::SetPortalId()
{
    if (IsCurrentPortalForTrash())
    {
        int iTemp = rand()%(m_uiMaxCountPortalLoc - 1);

        if (iTemp >= m_uiPortalId)
            ++iTemp;

        debug_log("SD2: instance_violet_hold: SetPortalId %i, old was id %u.", iTemp, m_uiPortalId);

        m_uiPortalId = iTemp;
    }
    else
    {
        debug_log("SD2: instance_violet_hold: SetPortalId %u (is boss), old was id %u.", m_uiMaxCountPortalLoc, m_uiPortalId);
        m_uiPortalId = m_uiMaxCountPortalLoc;
    }
}

void instance_violet_hold::CallGuards(bool bRespawn)
{
    for(std::list<uint64>::iterator i = m_lGuardsList.begin(); i != m_lGuardsList.end(); ++i)
    {
        if (Creature* pGuard = instance->GetCreature(*i))
        {
            if (bRespawn)
            {
                pGuard->Respawn();
            }
            else if (pGuard->isAlive())
            {
                pGuard->AI()->EnterEvadeMode();

                if (Creature* pSinclari = instance->GetCreature(m_uiSinclariGUID))
                    pGuard->GetMotionMaster()->MoveFollow(pSinclari, 0.0f, 0.0f);

                pGuard->ForcedDespawn(20000);
            }
        }
    }
}

void instance_violet_hold::ProcessActivationCrystal(Unit* pUser, bool bIsIntro)
{
    if (Creature* pSummon = pUser->SummonCreature(NPC_DEFENSE_SYSTEM, fDefenseSystemLoc[0], fDefenseSystemLoc[1], fDefenseSystemLoc[2], fDefenseSystemLoc[3], TEMPSUMMON_TIMED_DESPAWN, 10000))
    {
        pSummon->CastSpell(pSummon, SPELL_DEFENSE_SYSTEM_VISUAL, true);

        // TODO: figure out how the rest work
        // NPC's NPC_DEFENSE_DUMMY_TARGET are probably channeling some spell to the defense system
    }

    if (bIsIntro)
        DoUseDoorOrButton(m_uiIntroCrystalGUID);

    // else, kill (and despawn?) certain trash mobs. Also boss affected, but not killed.
}

uint32 instance_violet_hold::GetRandomPortalEliteEntry()
{
    return (urand(0, 1) ? NPC_PORTAL_GUARDIAN : NPC_PORTAL_KEEPER);
}

uint32 instance_violet_hold::GetRandomMobForNormalPortal()
{
    switch(urand(1, 4))
    {
        case 1: return NPC_AZURE_INVADER;
        case 2: return NPC_MAGE_HUNTER;
        case 3: return NPC_AZURE_SPELLBREAKER;
        case 4: return NPC_AZURE_BINDER;
    }

    return 0;
}

uint64 instance_violet_hold::GetData64(uint32 uiData)
{
    switch(uiData)
    {
        case DATA_EREKEM:
            return m_uiErekemGUID;
        case DATA_MORAGG:
            return m_uiMoraggGUID;
        case DATA_ICHORON:
            return m_uiIchoronGUID;
        case DATA_XEVOZZ:
            return m_uiXevozzGUID;
        case DATA_LAVANTHOR:
            return m_uiLavanthorGUID;
        case DATA_ZURAMAT:
            return m_uiZuramatGUID;
        case DATA_SINCLARI:
            return m_uiSinclariGUID;
        case DATA_NPC_SEAL_DOOR:
            return m_uiSealDoorGUID;
        case DATA_SEAL_DOOR:
            return m_uiSealDoorGUID;
        case DATA_EREKEM_DOOR:
            return m_uiErekemDoorGUID;
        case DATA_EREKEM_DOOR_L:
            return m_uiErekemDoorLeftGUID;
        case DATA_EREKEM_DOOR_R:
            return m_uiErekemDoorRightGUID;
        case DATA_MORAGG_DOOR:
            return m_uiMoraggDoorGUID;
        case DATA_ICHORON_DOOR:
            return m_uiIchoronDoorGUID;
        case DATA_XEVOZZ_DOOR:
            return m_uiXevozzDoorGUID;
        case DATA_LAVANTHOR_DOOR:
            return m_uiLavanthorDoorGUID;
        case DATA_ZURAMAT_DOOR:
            return m_uiZuramatDoorGUID;
    }
    return 0;
}

void instance_violet_hold::Update(uint32 uiDiff)
{
    if (!(m_auiEncounter[0] == IN_PROGRESS || m_auiEncounter[0] == SPECIAL))
        return;

    if (m_uiPortalTimer)
    {
        if (m_uiPortalTimer <= uiDiff)
        {
            DoUpdateWorldState(WORLD_STATE_PORTALS, ++m_uiWorldStatePortalCount);

            SetPortalId();
            SpawnPortal();

            m_uiPortalTimer = 0;
        }
        else
            m_uiPortalTimer -= uiDiff;
    }
}

InstanceData* GetInstanceData_instance_violet_hold(Map* pMap)
{
    return new instance_violet_hold(pMap);
}

void AddSC_instance_violet_hold()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_violet_hold";
    newscript->GetInstanceData = GetInstanceData_instance_violet_hold;
    newscript->RegisterSelf();
}
