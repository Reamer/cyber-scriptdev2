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
SDName: Instance_Ulduar
SD%Complete: 
SDComment: 
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

instance_ulduar::instance_ulduar(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_ulduar::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiHardBoss, 0, sizeof(m_auiHardBoss));
    memset(&m_auiUlduarKeepers, 0, sizeof(m_auiUlduarKeepers));
    memset(&m_auiUlduarTeleporters, 0, sizeof(m_auiUlduarTeleporters));

    for(uint8 i = 0; i < 6; i++)
        m_auiMiniBoss[i] = NOT_STARTED;

    m_uiMimironPhase        = 0;
    m_uiYoggPhase           = 0;
    m_uiVisionPhase         = 0;

    // Leviathan not implemented, so set it as DONE
    SetData(TYPE_LEVIATHAN, DONE);
    SetData(TYPE_LEVIATHAN_TP, DONE);
}

bool instance_ulduar::IsEncounterInProgress() const
{
    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_ulduar::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
    case NPC_LEVIATHAN:
    case NPC_IGNIS:
        break;
    case NPC_IRON_CONSTRUCT:
        m_lIronConstructsGUIDs.push_back(pCreature->GetGUID());
        return;
    case NPC_RAZORSCALE:
    case NPC_COMMANDER:
    case NPC_XT002:
     // Assembly of Iron
    case NPC_STEELBREAKER:
    case NPC_MOLGEIM:
    case NPC_BRUNDIR:
    case NPC_KOLOGARN:
    case NPC_KOLOGARN_BRIDGE_DUMMY:
    case NPC_AURIAYA:
    case NPC_FERAL_DEFENDER:
    case NPC_MIMIRON:
    case NPC_LEVIATHAN_MK:
    // Hodir
    case NPC_HODIR:
    case NPC_HELPER_DRUID:
    case NPC_HELPER_MAGE:
    case NPC_HELPER_SHAMAN:
    case NPC_HELPER_PRIEST:

    //Thorim
    case NPC_THORIM:
    case NPC_RUNIC_COLOSSUS:
        break;
    case NPC_RIGHT_HAND_TRIGGER:
        m_lRightHandTrigger.push_back(pCreature->GetGUID());
        return;
    case NPC_LEFT_HAND_TRIGGER:
        m_lLeftHandTrigger.push_back(pCreature->GetGUID());
        return;
    case NPC_RUNE_GIANT:
    case NPC_JORMUNGAR_BEHEMOTH:
    case NPC_FREYA:
    case NPC_BRIGHTLEAF:
    case NPC_IRONBRACH:
    case NPC_STONEBARK:
    case NPC_VEZAX:
    case NPC_ANIMUS:
    case NPC_YOGGSARON:
    case NPC_SARA:
    case NPC_YOGG_BRAIN:
    case NPC_ALGALON:
        break;
    case NPC_OMINOUS_CLOUD:
        m_lCLoudGUIDs.push_back(pCreature->GetGUID());
        return;


        // used to handle the keepers images
        // set to invisible by default and only made visible if the encounter is done
    case HODIR_IMAGE:
        pCreature->SetVisibility(VISIBILITY_OFF);
        if(m_auiEncounter[8] == DONE)
            pCreature->SetVisibility(VISIBILITY_ON);
        break;
    case FREYA_IMAGE:
        pCreature->SetVisibility(VISIBILITY_OFF);
        if(m_auiEncounter[10] == DONE)
            pCreature->SetVisibility(VISIBILITY_ON);
        break;
    case THORIM_IMAGE:
        pCreature->SetVisibility(VISIBILITY_OFF);
        if(m_auiEncounter[9] == DONE)
            pCreature->SetVisibility(VISIBILITY_ON);
        break;
    case MIMIRON_IMAGE:
        pCreature->SetVisibility(VISIBILITY_OFF);
        if(m_auiEncounter[7] == DONE)
            pCreature->SetVisibility(VISIBILITY_ON);
        break;
    default:
        return;
    }
    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_ulduar::OnObjectCreate(GameObject *pGo)
{
    switch(pGo->GetEntry())
    {
        // doors & other
        // The siege
    case GO_SHIELD_WALL:
        break;
    case GO_LEVIATHAN_GATE:
        if(m_auiEncounter[0] == DONE)
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_XT002_GATE:
        pGo->SetGoState(GO_STATE_READY);
        if(m_auiEncounter[0] == DONE || m_auiEncounter[3] == DONE)
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_BROKEN_HARPOON:
        m_lBreakHarpoonGUID.push_back(pGo->GetObjectGuid());
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
        break;

        // Archivum
    case GO_IRON_ENTRANCE_DOOR:
        break;
    case GO_ARCHIVUM_DOOR:
        pGo->SetGoState(GO_STATE_READY);
        if(m_auiEncounter[4])
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_ARCHIVUM_CONSOLE:
    case GO_UNIVERSE_FLOOR_ARCHIVUM:
        // Celestial Planetarium
    case GO_CELESTIAL_ACCES:
    case GO_CELESTIAL_DOOR:
    case GO_UNIVERSE_FLOOR_CELESTIAL:
    case GO_AZEROTH_GLOBE:
        break;
        // Shattered Hallway
    case GO_KOLOGARN_BRIDGE:
        pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_SHATTERED_DOOR:
        break;
        // The keepers
        // Hodir
    case GO_HODIR_EXIT:
        if(m_auiEncounter[TYPE_HODIR] == DONE)
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_HODIR_ICE_WALL:
        if(m_auiEncounter[TYPE_HODIR] == DONE)
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_HODIR_ENTER:
        pGo->SetGoState(GO_STATE_ACTIVE);
        break;
        // Mimiron
    case GO_MIMIRON_TRAM:
        if (m_auiEncounter[TYPE_AURIAYA] == DONE)
        {
            pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
            pGo->SetGoState(GO_STATE_READY);
        }
        break;
    case GO_MIMIRON_BUTTON:
        if (m_auiEncounter[TYPE_MIMIRON] == NOT_STARTED)
            pGo->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
        break;
    case GO_MIMIRON_DOOR_1:
    case GO_MIMIRON_DOOR_2:
    case GO_MIMIRON_DOOR_3:
    case GO_MIMIRON_ELEVATOR:
    case GO_MIMIRON_TEL1:
    case GO_MIMIRON_TEL2:
    case GO_MIMIRON_TEL3:
    case GO_MIMIRON_TEL4:
    case GO_MIMIRON_TEL5:
    case GO_MIMIRON_TEL6:
    case GO_MIMIRON_TEL7:
    case GO_MIMIRON_TEL8:
    case GO_MIMIRON_TEL9:
        // Thorim
    case GO_DARK_IRON_PORTCULIS:
    case GO_RUNED_STONE_DOOR:
    case GO_THORIM_STONE_DOOR:
    case GO_LIGHTNING_FIELD:
        break;
    case GO_DOOR_LEVER:
        pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
        break;

        // Prison
    case GO_ANCIENT_GATE:
        DoOpenMadnessDoorIfCan();
        break;
    case GO_VEZAX_GATE:
        pGo->SetGoState(GO_STATE_READY);
        if(m_auiEncounter[11])
            pGo->SetGoState(GO_STATE_ACTIVE);
        break;
    case GO_YOGG_GATE:
    case GO_BRAIN_DOOR1:
    case GO_BRAIN_DOOR2:
    case GO_BRAIN_DOOR3:
        // loot
        // Kologarn
    case GO_CACHE_OF_LIVING_STONE:
    case GO_CACHE_OF_LIVING_STONE_H:

        // Hodir
    case GO_CACHE_OF_WINTER:
    case GO_CACHE_OF_RARE_WINTER:
    case GO_CACHE_OF_WINTER_H:
    case GO_CACHE_OF_RARE_WINTER_H:
        // Freya
    case GO_FREYA_GIFT:
    case GO_FREYA_GIFT_HARD:
    case GO_FREYA_GIFT_H:
    case GO_FREYA_GIFT_HARD_H:
        // Thorim
    case GO_CACHE_OF_STORMS:
    case GO_CACHE_OF_RARE_STORMS:
    case GO_CACHE_OF_STORMS_H:
    case GO_CACHE_OF_RARE_STORMS_H:
        // Mimiron
    case GO_CACHE_OF_INOV:
    case GO_CACHE_OF_INOV_HARD:
    case GO_CACHE_OF_INOV_H:
    case GO_CACHE_OF_INOV_HARD_H:
        // Alagon
    case GO_GIFT_OF_OBSERVER:
    case GO_GIFT_OF_OBSERVER_H:
        break;
    default:
        return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_ulduar::OnCreatureDeath(Creature * pCreature)
{
    if (GetData(TYPE_THORIM) == IN_PROGRESS // Thorim
        || pCreature->GetEntry() == NPC_JORMUNGAR_BEHEMOTH // Preadds
        || pCreature->GetEntry() == NPC_CAPTAIN_ALY               
        || pCreature->GetEntry() == NPC_CAPTAIN_HORDE
        || pCreature->GetEntry() == NPC_MERCENARY_ALY
        || pCreature->GetEntry() == NPC_MERCENARY_HORDE) 
    {
        m_lThorimMobsGUIDs.push_back(pCreature->GetGUID());
    }
}

// functions to open or close some doors
void instance_ulduar::OpenDoor(uint32 uiEntry)
{
    if(!uiEntry)
        return;

    GameObject* pGo = GetSingleGameObjectFromStorage(uiEntry);

    if(pGo)
        pGo->SetGoState(GO_STATE_ACTIVE);
}

void instance_ulduar::CloseDoor(uint32 uiEntry)
{
    if(!uiEntry)
        return;

    GameObject* pGo = GetSingleGameObjectFromStorage(uiEntry);

    if(pGo)
        pGo->SetGoState(GO_STATE_READY);
}

// used in order to unlock the door to Vezax and make vezax attackable
void instance_ulduar::DoOpenMadnessDoorIfCan()
{
    if(m_auiEncounter[TYPE_MIMIRON] == DONE && m_auiEncounter[TYPE_HODIR] == DONE && m_auiEncounter[TYPE_THORIM] == DONE && m_auiEncounter[TYPE_FREYA] == DONE)
        OpenDoor(GO_ANCIENT_GATE);
}

void instance_ulduar::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
    case TYPE_LEVIATHAN:
        m_auiEncounter[uiType] = uiData;
        DoUseDoorOrButton(GO_SHIELD_WALL);
        if (uiData == DONE)
            OpenDoor(GO_LEVIATHAN_GATE);
        break;
    case TYPE_IGNIS:
        m_auiEncounter[uiType] = uiData;
        break;
    case TYPE_RAZORSCALE:
        m_auiEncounter[uiType] = uiData;
        break;
    case TYPE_XT002:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE || uiData == FAIL)
            OpenDoor(GO_XT002_GATE);
        else if (uiData == IN_PROGRESS)
            CloseDoor(GO_XT002_GATE);
        break;
    case TYPE_ASSEMBLY:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
        {
            OpenDoor(GO_IRON_ENTRANCE_DOOR);
            OpenDoor(GO_ARCHIVUM_DOOR);
            OpenDoor(GO_SHATTERED_DOOR);
        }
        else if (uiData == IN_PROGRESS)
            CloseDoor(GO_IRON_ENTRANCE_DOOR);
        else if (uiData == FAIL)
            OpenDoor(GO_IRON_ENTRANCE_DOOR);
        break;
    case TYPE_KOLOGARN:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
        {
            if (instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_CACHE_OF_LIVING_STONE, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_CACHE_OF_LIVING_STONE_H, 30*MINUTE);
            }
            if (GameObject* pGo = GetSingleGameObjectFromStorage(GO_KOLOGARN_BRIDGE))
            {
                pGo->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                pGo->SetGoState(GO_STATE_READY);
            }
        }
        break;
    case TYPE_AURIAYA:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
        {
            if (GameObject* pGO = GetSingleGameObjectFromStorage(GO_MIMIRON_TRAM))
            {
                pGO->SetUInt32Value(GAMEOBJECT_LEVEL, 0);
                pGO->SetGoState(GO_STATE_READY);
            }
        }
        break;

        // Keepers
    case TYPE_MIMIRON:
        m_auiEncounter[uiType] = uiData;
        DoUseDoorOrButton(GO_MIMIRON_DOOR_1);
        DoUseDoorOrButton(GO_MIMIRON_DOOR_2);
        DoUseDoorOrButton(GO_MIMIRON_DOOR_3);
        if (uiData == DONE)
        {         
            if (m_auiHardBoss[3] != DONE)
            {
                if (instance->IsRegularDifficulty())
                {
                    DoRespawnGameObject(GO_CACHE_OF_INOV, 30*MINUTE);
                }
                else
                {
                    DoRespawnGameObject(GO_CACHE_OF_INOV_H, 30*MINUTE);
                }
            }
            // used to make the friendly keeper visible
            if(Creature* pImage = GetSingleCreatureFromStorage(KEEPER_MIMIRON))
                pImage->SetVisibility(VISIBILITY_ON);
            DoOpenMadnessDoorIfCan();
        }
        break;
    case TYPE_HODIR:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
        {
            OpenDoor(GO_HODIR_ENTER);
            DoUseDoorOrButton(GO_HODIR_ICE_WALL);
            DoUseDoorOrButton(GO_HODIR_EXIT);
            if(instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_CACHE_OF_WINTER, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_CACHE_OF_WINTER_H, 30*MINUTE);
            }
            // used to make the friendly keeper visible
            if(Creature* pImage = GetSingleCreatureFromStorage(KEEPER_HODIR))
                pImage->SetVisibility(VISIBILITY_ON);

            DoOpenMadnessDoorIfCan();
        }
        else if (uiData == IN_PROGRESS)
            CloseDoor(GO_HODIR_ENTER);
        else if (uiData == FAIL)
            OpenDoor(GO_HODIR_ENTER);
        break;
    case TYPE_THORIM:
        m_auiEncounter[uiType] = uiData;
        DoUseDoorOrButton(GO_LIGHTNING_DOOR);
        if (uiData == IN_PROGRESS)
            DoUseDoorOrButton(GO_DARK_IRON_PORTCULIS);
        if (uiData == DONE)
        {
            if(m_auiHardBoss[5] != DONE)
            {
                if (instance->IsRegularDifficulty())
                {
                    DoRespawnGameObject(GO_CACHE_OF_STORMS, 30*MINUTE);
                }
                else
                {
                    DoRespawnGameObject(GO_CACHE_OF_STORMS_H, 30*MINUTE);
                }
            }
            // used to make the friendly keeper visible
            if(Creature* pImage = GetSingleCreatureFromStorage(KEEPER_THORIM))
                pImage->SetVisibility(VISIBILITY_ON);
            DoOpenMadnessDoorIfCan();
        }
        break;
    case TYPE_FREYA:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
        {
            // do this in order to see how many elders were alive and spawn the correct chest
            // hard mode drop
            if(m_auiHardBoss[6] != DONE)
            {
                if (instance->IsRegularDifficulty())
                {
                    DoRespawnGameObject(GO_FREYA_GIFT, 30*MINUTE);
                }
                else
                {
                    DoRespawnGameObject(GO_FREYA_GIFT_H, 30*MINUTE);
                }
            }
            // used to make the friendly keeper visible
            if(Creature* pImage = GetSingleCreatureFromStorage(KEEPER_FREYA))
                pImage->SetVisibility(VISIBILITY_ON);
            DoOpenMadnessDoorIfCan();
        }
        break;
        // Prison
    case TYPE_VEZAX:
        m_auiEncounter[uiType] = uiData;
        if (uiData == DONE)
            DoUseDoorOrButton(GO_VEZAX_GATE);
        break;
    case TYPE_YOGGSARON:
        m_auiEncounter[uiType] = uiData;
        DoUseDoorOrButton(GO_YOGG_GATE);
        break;

        // Celestial Planetarium
    case TYPE_ALGALON:
        m_auiEncounter[uiType] = uiData;
        DoUseDoorOrButton(GO_CELESTIAL_DOOR);
        DoUseDoorOrButton(GO_UNIVERSE_FLOOR_CELESTIAL);
        if (uiData == DONE)
            DoRespawnGameObject(GO_GIFT_OF_OBSERVER, 30*MINUTE);
        break;

        // Hard modes
    case TYPE_LEVIATHAN_HARD:
        m_auiHardBoss[0] = uiData;
        break;
    case TYPE_XT002_HARD:
        m_auiHardBoss[1] = uiData;
        break;
    case TYPE_ASSEMBLY_HARD:
        m_auiHardBoss[2] = uiData;
        break;
    case TYPE_MIMIRON_HARD:
        m_auiHardBoss[3] = uiData;
        if(uiData == DONE)
        {
            if (instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_CACHE_OF_INOV_HARD, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_CACHE_OF_INOV_HARD_H, 30*MINUTE);
            }
        }
        break;
    case TYPE_HODIR_HARD:
        m_auiHardBoss[4] = uiData;
        if(uiData == DONE)
        {
            if (instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_CACHE_OF_RARE_WINTER, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_CACHE_OF_RARE_WINTER_H, 30*MINUTE);
            }
        }
        break;
    case TYPE_THORIM_HARD:
        m_auiHardBoss[5] = uiData;
        if(uiData == DONE)
        {
            if (instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_CACHE_OF_RARE_STORMS, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_CACHE_OF_RARE_STORMS_H, 30*MINUTE);
            }            
        }
        break;
    case TYPE_FREYA_HARD:
        m_auiHardBoss[6] = uiData;
        if(uiData == DONE)
        {
            if (instance->IsRegularDifficulty())
            {
                DoRespawnGameObject(GO_FREYA_GIFT_HARD, 30*MINUTE);
            }
            else
            {
                DoRespawnGameObject(GO_FREYA_GIFT_HARD_H, 30*MINUTE);
            }
        }
        break;
    case TYPE_VEZAX_HARD:
        m_auiHardBoss[7] = uiData;
        break;
    case TYPE_YOGGSARON_HARD:
        m_auiHardBoss[8] = uiData;
        break;

        // Ulduar keepers
    case TYPE_KEEPER_HODIR:
        m_auiUlduarKeepers[0] = uiData;
        break;
    case TYPE_KEEPER_THORIM:
        m_auiUlduarKeepers[1] = uiData;
        break;
    case TYPE_KEEPER_FREYA:
        m_auiUlduarKeepers[2] = uiData;
        break;
    case TYPE_KEEPER_MIMIRON:
        m_auiUlduarKeepers[3] = uiData;
        break;

        // teleporters
    case TYPE_LEVIATHAN_TP:
        m_auiUlduarTeleporters[0] = uiData;
        break;
    case TYPE_XT002_TP:
        m_auiUlduarTeleporters[1] = uiData;
        break;
    case TYPE_MIMIRON_TP:
        m_auiUlduarTeleporters[2] = uiData;
        break;

        // mini boss
    case TYPE_RUNIC_COLOSSUS:
        m_auiMiniBoss[0] = uiData;
        if (uiData == DONE)
            OpenDoor(GO_RUNED_STONE_DOOR);
        else
            CloseDoor(GO_RUNED_STONE_DOOR);
        break;
    case TYPE_RUNE_GIANT:
        m_auiMiniBoss[1] = uiData;
        if (uiData == DONE)
            OpenDoor(GO_THORIM_STONE_DOOR);
        else
            CloseDoor(GO_THORIM_STONE_DOOR);
        break;
    case TYPE_LEVIATHAN_MK:
        m_auiMiniBoss[2] = uiData;
        break;
    case TYPE_VX001:
        m_auiMiniBoss[3] = uiData;
        /*if (uiData == DONE)     // just for animation :)
        {
            for(uint8 i = 0; i < 9; i++)
                DoUseDoorOrButton(m_uiMimironTelGUID[i]);
        }*/ 
        break;
    case TYPE_AERIAL_UNIT:
        m_auiMiniBoss[4] = uiData;
        break;
    case TYPE_YOGG_BRAIN:
        m_auiMiniBoss[5] = uiData;
        break;

        //phases
    case TYPE_MIMIRON_PHASE:
        m_uiMimironPhase = uiData;
        break;
    case TYPE_YOGG_PHASE:
        m_uiYoggPhase = uiData;
        break;
    case TYPE_VISION_PHASE:
        m_uiVisionPhase = uiData;
        break;
    }

    if (uiData == DONE || uiData == FAIL)
    {
        OUT_SAVE_INST_DATA;

        // save all encounters, hard bosses and keepers
        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
            << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiHardBoss[0] << " "
            << m_auiHardBoss[1] << " " << m_auiHardBoss[2] << " " << m_auiHardBoss[2] << " "
            << m_auiHardBoss[4] << " " << m_auiHardBoss[5] << " " << m_auiHardBoss[6] << " "
            << m_auiHardBoss[7] << " " << m_auiHardBoss[8] << " " << m_auiUlduarKeepers[0] << " "
            << m_auiUlduarKeepers[1] << " " << m_auiUlduarKeepers[2] << " " << m_auiUlduarKeepers[3] << " "
            << m_auiUlduarTeleporters[0] << " " << m_auiUlduarTeleporters[1] << " " << m_auiUlduarTeleporters[2];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

// TODO: implement all achievs here!
bool instance_ulduar::CheckAchievementCriteriaMeet(uint32 criteria_id, const Player *source)
{
    switch(criteria_id)
    {
    case ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET:
        break;
    }
    return false;
}

bool instance_ulduar::CheckConditionCriteriaMeet(Player const* source, uint32 map_id, uint32 instance_condition_id)
{
    if (map_id != instance->GetId())
        return false;

    if (GetData(instance_condition_id) == DONE)
        return true;

    return false;
}

uint32 instance_ulduar::GetData(uint32 uiType)
{
    switch(uiType)
    {
    case TYPE_LEVIATHAN:
        return m_auiEncounter[0];
    case TYPE_IGNIS:
        return m_auiEncounter[1];
    case TYPE_RAZORSCALE:
        return m_auiEncounter[2];
    case TYPE_XT002:
        return m_auiEncounter[3];
    case TYPE_ASSEMBLY:
        return m_auiEncounter[4];
    case TYPE_KOLOGARN:
        return m_auiEncounter[5];
    case TYPE_AURIAYA:
        return m_auiEncounter[6];
    case TYPE_MIMIRON:
        return m_auiEncounter[7];
    case TYPE_HODIR:
        return m_auiEncounter[8];
    case TYPE_THORIM:
        return m_auiEncounter[9];
    case TYPE_FREYA:
        return m_auiEncounter[10];
    case TYPE_VEZAX:
        return m_auiEncounter[11];
    case TYPE_YOGGSARON:
        return m_auiEncounter[12];
    case TYPE_ALGALON:
        return m_auiEncounter[13];

    // hard modes
    case TYPE_LEVIATHAN_HARD:
        return m_auiHardBoss[0];
    case TYPE_XT002_HARD:
        return m_auiHardBoss[1];
    case TYPE_ASSEMBLY_HARD:
        return m_auiHardBoss[2];
    case TYPE_MIMIRON_HARD:
        return m_auiHardBoss[3];
    case TYPE_HODIR_HARD:
        return m_auiHardBoss[4];
    case TYPE_THORIM_HARD:
        return m_auiHardBoss[5];
    case TYPE_FREYA_HARD:
        return m_auiHardBoss[6];
    case TYPE_VEZAX_HARD:
        return m_auiHardBoss[7];
    case TYPE_YOGGSARON_HARD:
        return m_auiHardBoss[8];

        // ulduar keepers
    case TYPE_KEEPER_HODIR:
        return m_auiUlduarKeepers[0];
    case TYPE_KEEPER_THORIM:
        return m_auiUlduarKeepers[1];
    case TYPE_KEEPER_FREYA:
        return m_auiUlduarKeepers[2];
    case TYPE_KEEPER_MIMIRON:
        return m_auiUlduarKeepers[3];

        // teleporters
    case TYPE_LEVIATHAN_TP:
        return m_auiUlduarTeleporters[0];
    case TYPE_XT002_TP:
        return m_auiUlduarTeleporters[1];
    case TYPE_MIMIRON_TP:
        return m_auiUlduarTeleporters[2];

        // mini boss
    case TYPE_RUNE_GIANT:
        return m_auiMiniBoss[1];
    case TYPE_RUNIC_COLOSSUS:
        return m_auiMiniBoss[0];
    case TYPE_LEVIATHAN_MK:
        return m_auiMiniBoss[2];
    case TYPE_VX001:
        return m_auiMiniBoss[3];
    case TYPE_AERIAL_UNIT:
        return m_auiMiniBoss[4];
    case TYPE_YOGG_BRAIN:
        return m_auiMiniBoss[5];

    case TYPE_MIMIRON_PHASE:
        return m_uiMimironPhase;
    case TYPE_YOGG_PHASE:
        return m_uiYoggPhase;
    case TYPE_VISION_PHASE:
        return m_uiVisionPhase;
    }

    return 0;
}

const char* instance_ulduar::Save()
{
    return m_strInstData.c_str();
}

void instance_ulduar::Load(const char* strIn)
{
    if (!strIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(strIn);

    std::istringstream loadStream(strIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
    >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
    >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
    >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiHardBoss[0] >> m_auiHardBoss[1]
    >> m_auiHardBoss[2] >> m_auiHardBoss[3] >> m_auiHardBoss[4] >> m_auiHardBoss[5]
    >> m_auiHardBoss[6] >> m_auiHardBoss[7] >> m_auiHardBoss[8] >> m_auiUlduarKeepers[0]
    >> m_auiUlduarKeepers[1] >> m_auiUlduarKeepers[2] >> m_auiUlduarKeepers[3] >> m_auiUlduarTeleporters[0]
    >> m_auiUlduarTeleporters[1] >> m_auiUlduarTeleporters[2];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_ulduar(Map* pMap)
{
    return new instance_ulduar(pMap);
}

void AddSC_instance_ulduar()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "instance_ulduar";
    newscript->GetInstanceData = &GetInstanceData_instance_ulduar;
    newscript->RegisterSelf();
}
