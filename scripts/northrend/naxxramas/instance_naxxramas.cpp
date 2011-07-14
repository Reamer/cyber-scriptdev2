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
SDName: Instance_Naxxramas
SD%Complete: 90%
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

instance_naxxramas::instance_naxxramas(Map* pMap) : ScriptedInstance(pMap),

    m_bBlaumeuxDead(false),
    m_bRivendareDead(false),
    m_bZeliekDead(false),
    m_bKorthazzDead(false),

    m_fChamberCenterX(0.0f),
    m_fChamberCenterY(0.0f),
    m_fChamberCenterZ(0.0f),

    m_uiArachnophobiaTimer(0),

    m_bTheSafetyDance(false),
    m_bMommaSaidKnockYouOut(false),
    m_bSporeLoser(false),
    m_bShocking(false),
    m_bTheHundredClub(false),
    m_bJustCantGetEnough(false),
    m_bMonsterRun(true)
{
    Initialize();
}

void instance_naxxramas::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

void instance_naxxramas::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_ANUB_REKHAN:
        case NPC_FAERLINA:
        case NPC_MAEXXNA:
        case NPC_PATCHWERK:
        case NPC_GROBBULUS:
        case NPC_GLUTH:
        case NPC_THADDIUS:
        case NPC_STALAGG:
        case NPC_FEUGEN:
        case NPC_NOTH:
        case NPC_HEIGAN:
        case NPC_LOATHEB:
        case NPC_RAZUVIOUS:
        case NPC_ZELIEK:
        case NPC_THANE:
        case NPC_BLAUMEUX:
        case NPC_RIVENDARE:
        case NPC_GOTHIK:
        case NPC_SAPPHIRON:
        case NPC_KELTHUZAD:
            break;
        case NPC_SUB_BOSS_TRIGGER:
            m_lGothTriggerList.push_back(pCreature->GetGUID());
            return;
        case NPC_WORSHIPPER:
        case NPC_FOLLOWER:
            m_lFaerlinaAddsGUID.push_back(pCreature->GetObjectGuid());
            return;
        case NPC_DEATH_KNIGHT_UNDERSTUDY:
            m_lDeathKnightUnderstudyGUID.push_back(pCreature->GetObjectGuid());
            return;
    }
    m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
}

void instance_naxxramas::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_ARAC_ANUB_DOOR:
            break;
        case GO_ARAC_ANUB_GATE:
            if (m_auiEncounter[TYPE_ANUB_REKHAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARAC_FAER_WEB:
            break;
        case GO_ARAC_FAER_DOOR:
            if (m_auiEncounter[TYPE_FAERLINA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARAC_MAEX_INNER_DOOR:
            break;
        case GO_ARAC_MAEX_OUTER_DOOR:
            if (m_auiEncounter[TYPE_FAERLINA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        case GO_PLAG_NOTH_ENTRY_DOOR:
            break;
        case GO_PLAG_NOTH_EXIT_DOOR:
            if (m_auiEncounter[TYPE_NOTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_HEIG_ENTRY_DOOR:
            if (m_auiEncounter[TYPE_NOTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_HEIG_EXIT_DOOR:
            if (m_auiEncounter[TYPE_HEIGAN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_LOAT_DOOR:
            break;
        case GO_ERUPTION_ONE_OBJECT:
            m_lEruptionObjectOneGUIDs.push_back(pGo->GetObjectGuid());
            return;
        case GO_ERUPTION_TWO_OBJECT:
            m_lEruptionObjectTwoGUIDs.push_back(pGo->GetObjectGuid());
            return;
        case GO_ERUPTION_THREE_OBJECT:
            m_lEruptionObjectThreeGUIDs.push_back(pGo->GetObjectGuid());
            return;
        case GO_ERUPTION_FOUR_OBJECT:
            m_lEruptionObjectFourGUIDs.push_back(pGo->GetObjectGuid());
            return;

        case GO_MILI_GOTH_ENTRY_GATE:
            break;
        case GO_MILI_GOTH_EXIT_GATE:
            if (m_auiEncounter[TYPE_GOTHIK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MILI_GOTH_COMBAT_GATE:
            break;
        case GO_MILI_HORSEMEN_DOOR:
            if (m_auiEncounter[TYPE_GOTHIK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        case GO_CHEST_HORSEMEN_NORM:
        case GO_CHEST_HORSEMEN_HERO:
            break;

        case GO_CONS_PATH_EXIT_DOOR:
            if (m_auiEncounter[TYPE_PATCHWERK] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_GLUT_EXIT_DOOR:
            if (m_auiEncounter[TYPE_GLUTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_THAD_DOOR:
            if (m_auiEncounter[TYPE_GLUTH] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;

        case GO_KELTHUZAD_WATERFALL_DOOR:
            if (m_auiEncounter[TYPE_SAPPHIRON] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_KELTHUZAD_EXIT_DOOR:
            break;

        case GO_ARAC_EYE_RAMP:
            if (m_auiEncounter[TYPE_MAEXXNA] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_PLAG_EYE_RAMP:
            if (m_auiEncounter[TYPE_LOATHEB] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_MILI_EYE_RAMP:
            if (m_auiEncounter[TYPE_FOUR_HORSEMEN] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_CONS_EYE_RAMP:
            if (m_auiEncounter[TYPE_THADDIUS] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARAC_PORTAL:
        case GO_PLAG_PORTAL:
        case GO_MILI_PORTAL:
        case GO_CONS_PORTAL:
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

bool instance_naxxramas::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;

    return false;
}

void instance_naxxramas::OnPlayerDeath(Player* pPlayer)
{
    if (IsEncounterInProgress())
    {
        m_bMonsterRun = false;
        pPlayer->MonsterSay("MonsterRun-Erfolg gibs nicht mehr.", 0);
        if (GetData(TYPE_HEIGAN) == IN_PROGRESS)
        {
            m_bTheSafetyDance = false;
        }
    }
}


void instance_naxxramas::OnPlayerLeave(Player* pPlayer)
{
    m_bMonsterRun = false;
    pPlayer->MonsterSay("MonsterRun-Erfolg gibs nicht mehr.", 0);
}
bool instance_naxxramas::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRITERIA_THE_SAFETY_DANCE:
        case ACHIEV_CRITERIA_THE_SAFETY_DANCE_H:
            return m_bTheSafetyDance;
        case ACHIEV_CRITERIA_MOMMA_SAID_KNOCK_YOU_OUT:
        case ACHIEV_CRITERIA_MOMMA_SAID_KNOCK_YOU_OUT_H:
            return m_bMommaSaidKnockYouOut;
        case ACHIEV_CRITERIA_SPORE_LOSER:
        case ACHIEV_CRITERIA_SPORE_LOSER_H:
            return m_bSporeLoser;
        case ACHIEV_CRITERIA_SHOCKING:
        case ACHIEV_CRITERIA_SHOCKING_H:
            return m_bShocking;
        case ACHIEV_CRITERIA_THE_HUNDRED_CLUB:
        case ACHIEV_CRITERIA_THE_HUNDRED_CLUB_H:
            return m_bTheHundredClub;
        case ACHIEV_CRITERIA_JUST_CANT_GET_ENOUGH:
        case ACHIEV_CRITERIA_JUST_CANT_GET_ENOUGH_H:
            return m_bJustCantGetEnough;

        case ACHIEV_CRITERIA_THE_IMMORTAL_1:
        case ACHIEV_CRITERIA_THE_IMMORTAL_2:
        case ACHIEV_CRITERIA_THE_IMMORTAL_3:
        case ACHIEV_CRITERIA_THE_IMMORTAL_4:
        case ACHIEV_CRITERIA_THE_IMMORTAL_5:
        case ACHIEV_CRITERIA_THE_UNDYING_1:
        case ACHIEV_CRITERIA_THE_UNDYING_2:
        case ACHIEV_CRITERIA_THE_UNDYING_3:
        case ACHIEV_CRITERIA_THE_UNDYING_4:
        case ACHIEV_CRITERIA_THE_UNDYING_5:
            return IsAllEncounterDone() ? m_bMonsterRun : false;
        default:
            return 0;
    }
}

bool instance_naxxramas::IsAllEncounterDone()
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        if (m_auiEncounter[i] != DONE)
            return false; 
    return true;
}

void instance_naxxramas::SetAchiev(uint32 uiType, bool get)
{
    switch(uiType)
    {
        case TYPE_HEIGAN:
            m_bTheSafetyDance = get;
            break;
        case TYPE_FAERLINA:
            m_bMommaSaidKnockYouOut = get;
            break;
        case TYPE_LOATHEB:
            m_bSporeLoser = get;
            break;
        case TYPE_THADDIUS:
            m_bShocking = get;
            break;
        case TYPE_SAPPHIRON:
            m_bTheHundredClub = get;
            break;
        case TYPE_KELTHUZAD:
            m_bJustCantGetEnough = get;
            break;
        default:
            break;
    }
}
void instance_naxxramas::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ANUB_REKHAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARAC_ANUB_DOOR);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_ARAC_ANUB_GATE);
            break;
        case TYPE_FAERLINA:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARAC_FAER_WEB);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARAC_FAER_DOOR);
                DoUseDoorOrButton(GO_ARAC_MAEX_OUTER_DOOR);
            }
            break;
        case TYPE_MAEXXNA:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARAC_MAEX_INNER_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARAC_EYE_RAMP);
                DoRespawnGameObject(GO_ARAC_PORTAL, 30*MINUTE);
                if (GameObject* pTele = GetSingleGameObjectFromStorage(GO_ARAC_PORTAL))
                    pTele->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
                DoTaunt();
            }
            break;
        case TYPE_NOTH:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_NOTH_ENTRY_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_PLAG_NOTH_EXIT_DOOR);
                DoUseDoorOrButton(GO_PLAG_HEIG_ENTRY_DOOR);
            }
            break;
        case TYPE_HEIGAN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_HEIG_ENTRY_DOOR);
            if (uiData == DONE)
                DoUseDoorOrButton(GO_PLAG_HEIG_EXIT_DOOR);
            break;
        case TYPE_LOATHEB:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_PLAG_LOAT_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_PLAG_EYE_RAMP);
                DoRespawnGameObject(GO_PLAG_PORTAL, 30*MINUTE);
                if (GameObject* pTele = GetSingleGameObjectFromStorage(GO_PLAG_PORTAL))
                    pTele->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
                DoTaunt();
            }
            break;
        case TYPE_RAZUVIOUS:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_GOTHIK:
            m_auiEncounter[uiType] = uiData;
            switch(uiData)
            {
                case IN_PROGRESS:
                case FAIL:
                    DoUseDoorOrButton(GO_MILI_GOTH_ENTRY_GATE);
                    DoUseDoorOrButton(GO_MILI_GOTH_COMBAT_GATE);
                    break;
                case SPECIAL:
                    DoUseDoorOrButton(GO_MILI_GOTH_COMBAT_GATE);
                    break;
                case DONE:
                    DoUseDoorOrButton(GO_MILI_GOTH_ENTRY_GATE);
                    DoUseDoorOrButton(GO_MILI_GOTH_EXIT_GATE);
                    DoUseDoorOrButton(GO_MILI_HORSEMEN_DOOR);
                    break;
            }
            break;
        case TYPE_BLAUMEUX:
            if (uiData == DONE)
            {
                m_bBlaumeuxDead = true;
                if (m_bRivendareDead && m_bZeliekDead && m_bKorthazzDead)
                    SetData(TYPE_FOUR_HORSEMEN, DONE);
            }
            break;
        case TYPE_RIVENDARE:
            if (uiData == DONE)
            {
                m_bRivendareDead = true;
                if (m_bBlaumeuxDead && m_bZeliekDead && m_bKorthazzDead)
                    SetData(TYPE_FOUR_HORSEMEN, DONE);
            }
            break;
        case TYPE_ZELIEK:
            if (uiData == DONE)
            {
                m_bZeliekDead = true;
                if (m_bBlaumeuxDead && m_bRivendareDead && m_bKorthazzDead)
                    SetData(TYPE_FOUR_HORSEMEN, DONE);
            }
            break;
        case TYPE_KORTHAZZ:
            if (uiData == DONE)
            {
                m_bKorthazzDead = true;
                if (m_bBlaumeuxDead && m_bRivendareDead && m_bZeliekDead)
                    SetData(TYPE_FOUR_HORSEMEN, DONE);
            }
            break;
        case TYPE_FOUR_HORSEMEN:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_MILI_HORSEMEN_DOOR);
            if (uiData == DONE)
            {
                if (instance)
                {
                    bool m_bIsRegularMode = instance->IsRegularDifficulty();
                    DoCompleteAchievement(m_bIsRegularMode ? 568 : 569);
                }                
                DoUseDoorOrButton(GO_MILI_EYE_RAMP);
                DoRespawnGameObject(GO_MILI_PORTAL, 30*MINUTE);
                if (GameObject* pTele = GetSingleGameObjectFromStorage(GO_MILI_PORTAL))
                    pTele->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);

                DoRespawnGameObject(instance->IsRegularDifficulty() ? GO_CHEST_HORSEMEN_NORM : GO_CHEST_HORSEMEN_HERO, 30*MINUTE);
                DoTaunt();
            }
            break;
        case TYPE_PATCHWERK:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_CONS_PATH_EXIT_DOOR);
            break;
        case TYPE_GROBBULUS:
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_GLUTH:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_CONS_GLUT_EXIT_DOOR);
                DoUseDoorOrButton(GO_CONS_THAD_DOOR);
            }
            break;
        case TYPE_THADDIUS:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_CONS_THAD_DOOR);
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_CONS_EYE_RAMP);
                DoRespawnGameObject(GO_CONS_PORTAL, 30*MINUTE);
                if (GameObject* pTele = GetSingleGameObjectFromStorage(GO_CONS_PORTAL))
                    pTele->RemoveFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
                DoTaunt();
            }
            break;
        case TYPE_SAPPHIRON:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
                DoUseDoorOrButton(GO_KELTHUZAD_WATERFALL_DOOR);
            break;
        case TYPE_KELTHUZAD:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_KELTHUZAD_EXIT_DOOR);
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3] << " " << m_auiEncounter[4] << " " << m_auiEncounter[5] << " "
            << m_auiEncounter[6] << " " << m_auiEncounter[7] << " " << m_auiEncounter[8] << " "
            << m_auiEncounter[9] << " " << m_auiEncounter[10] << " " << m_auiEncounter[11] << " "
            << m_auiEncounter[12] << " " << m_auiEncounter[13] << " " << m_auiEncounter[14];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_naxxramas::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3]
        >> m_auiEncounter[4] >> m_auiEncounter[5] >> m_auiEncounter[6] >> m_auiEncounter[7]
        >> m_auiEncounter[8] >> m_auiEncounter[9] >> m_auiEncounter[10] >> m_auiEncounter[11]
        >> m_auiEncounter[12] >> m_auiEncounter[13] >> m_auiEncounter[14];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_naxxramas::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ANUB_REKHAN:
            return m_auiEncounter[0];
        case TYPE_FAERLINA:
            return m_auiEncounter[1];
        case TYPE_MAEXXNA:
            return m_auiEncounter[2];
        case TYPE_NOTH:
            return m_auiEncounter[3];
        case TYPE_HEIGAN:
            return m_auiEncounter[4];
        case TYPE_LOATHEB:
            return m_auiEncounter[5];
        case TYPE_RAZUVIOUS:
            return m_auiEncounter[6];
        case TYPE_GOTHIK:
            return m_auiEncounter[7];
        case TYPE_FOUR_HORSEMEN:
            return m_auiEncounter[8];
        case TYPE_PATCHWERK:
            return m_auiEncounter[9];
        case TYPE_GROBBULUS:
            return m_auiEncounter[10];
        case TYPE_GLUTH:
            return m_auiEncounter[11];
        case TYPE_THADDIUS:
            return m_auiEncounter[12];
        case TYPE_SAPPHIRON:
            return m_auiEncounter[13];
        case TYPE_KELTHUZAD:
            return m_auiEncounter[14];
    }
    return 0;
}

void instance_naxxramas::SetGothTriggers()
{
    Creature* pGoth = GetSingleCreatureFromStorage(NPC_GOTHIK);

    if (!pGoth)
        return;

    for(GUIDList::iterator itr = m_lGothTriggerList.begin(); itr != m_lGothTriggerList.end(); ++itr)
    {
        if (Creature* pTrigger = instance->GetCreature(*itr))
        {
            GothTrigger pGt;
            pGt.bIsAnchorHigh = (pTrigger->GetPositionZ() >= (pGoth->GetPositionZ() - 5.0f));
            pGt.bIsRightSide = IsInRightSideGothArea(pTrigger);

            m_mGothTriggerMap[pTrigger->GetGUID()] = pGt;
        }
    }
}

Creature* instance_naxxramas::GetClosestAnchorForGoth(Creature* pSource, bool bRightSide)
{
    std::list<Creature* > lList;

    for (UNORDERED_MAP<uint64, GothTrigger>::iterator itr = m_mGothTriggerMap.begin(); itr != m_mGothTriggerMap.end(); ++itr)
    {
        if (!itr->second.bIsAnchorHigh)
            continue;

        if (itr->second.bIsRightSide != bRightSide)
            continue;

        if (Creature* pCreature = instance->GetCreature(itr->first))
            lList.push_back(pCreature);
    }

    if (!lList.empty())
    {
        lList.sort(ObjectDistanceOrder(pSource));
        return lList.front();
    }

    return NULL;
}

void instance_naxxramas::GetGothSummonPointCreatures(std::list<Creature*> &lList, bool bRightSide)
{
    for (UNORDERED_MAP<uint64, GothTrigger>::iterator itr = m_mGothTriggerMap.begin(); itr != m_mGothTriggerMap.end(); ++itr)
    {
        if (itr->second.bIsAnchorHigh)
            continue;

        if (itr->second.bIsRightSide != bRightSide)
            continue;

        if (Creature* pCreature = instance->GetCreature(itr->first))
            lList.push_back(pCreature);
    }
}

bool instance_naxxramas::IsInRightSideGothArea(Unit* pUnit)
{
    if (GameObject* pCombatGate = GetSingleGameObjectFromStorage(GO_MILI_GOTH_COMBAT_GATE))
        return (pCombatGate->GetPositionY() >= pUnit->GetPositionY());

    error_log("SD2: left/right side check, Gothik combat area failed.");
    return true;
}

void instance_naxxramas::SetChamberCenterCoords(float fX, float fY, float fZ)
{
    m_fChamberCenterX = fX;
    m_fChamberCenterY = fY;
    m_fChamberCenterZ = fZ;
}

void instance_naxxramas::DoTaunt()
{
    Creature* pKelThuzad = GetSingleCreatureFromStorage(NPC_KELTHUZAD);

    if (pKelThuzad && pKelThuzad->isAlive())
    {
        uint8 uiWingsCleared = 0;

        if (m_auiEncounter[2] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[5] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[8] == DONE)
            ++uiWingsCleared;

        if (m_auiEncounter[12] == DONE)
            ++uiWingsCleared;

        switch(uiWingsCleared)
        {
            case 1: DoScriptText(SAY_KELTHUZAD_TAUNT1, pKelThuzad); break;
            case 2: DoScriptText(SAY_KELTHUZAD_TAUNT2, pKelThuzad); break;
            case 3: DoScriptText(SAY_KELTHUZAD_TAUNT3, pKelThuzad); break;
            case 4: DoScriptText(SAY_KELTHUZAD_TAUNT4, pKelThuzad); break;
        }
    }
}

InstanceData* GetInstanceData_instance_naxxramas(Map* pMap)
{
    return new instance_naxxramas(pMap);
}

bool AreaTrigger_at_naxxramas(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pAt->id == AREATRIGGER_KELTHUZAD)
    {
        if (pPlayer->isGameMaster() || pPlayer->isDead())
            return false;

        instance_naxxramas* pInstance = (instance_naxxramas*)pPlayer->GetInstanceData();

        if (!pInstance)
            return false;

        pInstance->SetChamberCenterCoords(pAt->x, pAt->y, pAt->z);

        if (pInstance->GetData(TYPE_KELTHUZAD) == NOT_STARTED)
        {
            if (Creature* pKelthuzad = pInstance->instance->GetCreature(pInstance->GetData64(NPC_KELTHUZAD)))
            {
                if (pKelthuzad->isAlive())
                {
                    pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
                    pKelthuzad->SetInCombatWithZone();
                }
            }
        }
    }

    return false;
}

void AddSC_instance_naxxramas()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_naxxramas";
    pNewScript->GetInstanceData = &GetInstanceData_instance_naxxramas;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_naxxramas";
    pNewScript->pAreaTrigger = &AreaTrigger_at_naxxramas;
    pNewScript->RegisterSelf();
}
