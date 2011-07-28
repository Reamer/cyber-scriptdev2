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
SDName: Boss_Volazj 
SD%Complete: 20% 
SDComment: 
SDCategory: Ahn'kahet 
EndScriptData */ 
 
#include "precompiled.h"
#include "ahnkahet.h"
#include "Item.h"


enum 
{ 
    SAY_AGGRO                       = -1619033, 
    SAY_INSANITY                    = -1619034, 
    SAY_SLAY_1                      = -1619035, 
    SAY_SLAY_2                      = -1619036, 
    SAY_SLAY_3                      = -1619037, 
    SAY_DEATH                       = -1619038, 
    WHISPER_AGGRO                   = -1619039, 
    WHISPER_INSANITY                = -1619040, 
    WHISPER_SLAY_1                  = -1619041, 
    WHISPER_SLAY_2                  = -1619042, 
    WHISPER_SLAY_3                  = -1619043, 
    WHISPER_DEATH                   = -1619044, 

    SPELL_INSANITY                  = 57496,
    
    SPELL_INSANITY_PHASE_16         = 57508,
    SPELL_INSANITY_PHASE_32         = 57509,
    SPELL_INSANITY_PHASE_64         = 57510,
    SPELL_INSANITY_PHASE_128        = 57511,
    SPELL_INSANITY_PHASE_256        = 57512,
    
    SPELL_SHIVER                    = 57949, 
    SPELL_SHIVER_H                  = 59978, 
 
    SPELL_SHADOW_BOLT_SALVE         = 57942, 
    SPELL_SHADOW_BOLT_SALVE_H       = 59975, 
 
    SPELL_MIND_FLAY                 = 57941, 
    SPELL_MIND_FLAY_H               = 59974,

    SPELL_CLONE                     = 60352,

    // FIXME: these are not the right clone NPCs!
    CLONE                           = 31627,
    CLONE_H                         = 31627,
    FAC_HOSTILE                     = 16,
    ACHIEVEMENT_QUICK_DEMISE        = 1862
}; 
 
enum clonehealth
{ 
    CLONE_HEALTH_DRUID      = 6101, 
    CLONE_HEALTH_DRUID_H    = CLONE_HEALTH_DRUID * 3, 
    CLONE_HEALTH_PRIEST     = 5204, 
    CLONE_HEALTH_PRIEST_H   = CLONE_HEALTH_PRIEST * 3, 
    CLONE_HEALTH_PALA       = 7934, 
    CLONE_HEALTH_PALA_H     = CLONE_HEALTH_PALA * 3, 
    CLONE_HEALTH_HUNT       = 7399, 
    CLONE_HEALTH_HUNT_H     = CLONE_HEALTH_HUNT * 3, 
    CLONE_HEALTH_SHAMAN     = 7655, 
    CLONE_HEALTH_SHAMAN_H   = CLONE_HEALTH_SHAMAN * 3, 
    CLONE_HEALTH_ROGUE      = 6530, 
    CLONE_HEALTH_ROGUE_H    = CLONE_HEALTH_ROGUE * 3, 
    CLONE_HEALTH_MAGE       = 5129, 
    CLONE_HEALTH_MAGE_H     = CLONE_HEALTH_MAGE * 3, 
    CLONE_HEALTH_WARRIOR    = 10012, 
    CLONE_HEALTH_WARRIOR_H  = CLONE_HEALTH_WARRIOR * 3, 
    CLONE_HEALTH_WARLOCK    = 5477, 
    CLONE_HEALTH_WARLOCK_H  = CLONE_HEALTH_WARLOCK * 3, 
    CLONE_HEALTH_DK         = 9883, 
    CLONE_HEALTH_DK_H       = CLONE_HEALTH_DK * 3, 
}; 
 

/*###### 
## boss_volazj 
######*/ 
 
struct MANGOS_DLL_DECL boss_volazjAI : public ScriptedAI 
{ 
    boss_volazjAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData(); 
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty(); 
        Reset(); 
    } 
 
    instance_ahnkahet* m_pInstance; 
    bool m_bIsRegularMode; 
 
    bool m_bIsInInsanity; 
    bool phase66; 
    bool phase33; 
    bool clone16;
    bool clone32;
    bool clone64;
    bool clone128;
    bool clone256;
 
    GUIDList p_lCloneGuid; 
    GUIDList p_lClone16Guid;
    GUIDList p_lClone32Guid;
    GUIDList p_lClone64Guid;
    GUIDList p_lClone128Guid;
    GUIDList p_lClone256Guid;
 
    uint32 insanityEndTimer; 
    uint32 insanityTimer; 
    uint32 shadowBoltSalveTimer; 
    uint32 shiverTimer; 
    uint32 mindFlayTimer; 
    uint32 achievementTimer;

    uint8  count;
    float m_fHealthPercent;
 
    void Reset() 
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
        insanityEndTimer = 1000; 
        achievementTimer = 120000; // 2minutes
        shadowBoltSalveTimer = 6000;
        shiverTimer = 13000;
        mindFlayTimer = 9000;
        p_lCloneGuid.clear();
        p_lClone16Guid.clear();
        p_lClone32Guid.clear();
        p_lClone64Guid.clear();
        p_lClone128Guid.clear();
        p_lClone256Guid.clear();

        m_bIsInInsanity = false;
        phase66 = false;
        phase33 = false;
        clone16 = false;
        clone32 = false;
        clone64 = false;
        clone128 = false;
        clone256 = false;

        count   = 0;
        m_fHealthPercent = 66.0f;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, NOT_STARTED);
        RemoveInsanity();
    } 
 
    void Aggro(Unit* pWho) 
    { 
        DoScriptText(SAY_AGGRO, m_creature);

        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
            if (Player* target = itr->getSource()) 
                DoScriptText(WHISPER_AGGRO,m_creature,target);
        
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, IN_PROGRESS);
    } 
 
    void KilledUnit(Unit* pVictim) 
    { 
        int32 textId = 0;

        switch(urand(0, 2)) 
        { 
            case 0: textId = SAY_SLAY_1; break; 
            case 1: textId = SAY_SLAY_2; break; 
            case 2: textId = SAY_SLAY_3; break; 
        } 

        DoScriptText(textId,m_creature);

        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
            if (Player* target = itr->getSource()) 
                DoScriptText(textId-6,m_creature,target);
    } 
 
    void JustDied(Unit* pKiller) 
    { 
        DoScriptText(SAY_DEATH, m_creature); 
        if (m_pInstance)
            m_pInstance->SetData(TYPE_VOLAZJ, DONE);
        RemoveInsanity();
    }

    void RemoveInsanity(){
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
        { 
            if (Player* target = itr->getSource()) 
            { 
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_16);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_32);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_64);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_128);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_256);
            } 
        }
    }

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_INSANITY && target->GetTypeId() == TYPEID_PLAYER)
        {
            switch(count)
            {
                case 0:
                {
                    target->CastSpell(target, SPELL_INSANITY_PHASE_16, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                    CreateClonesForUnitInPhase((Player*)target, 16);
                    break;
                }
                case 1:
                {
                    target->CastSpell(target, SPELL_INSANITY_PHASE_32, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                    CreateClonesForUnitInPhase((Player*)target, 32);
                    break;
                }
                case 2:
                {
                    target->CastSpell(target, SPELL_INSANITY_PHASE_64, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                    CreateClonesForUnitInPhase((Player*)target, 64);
                    break;
                }
                case 3:
                {
                    target->CastSpell(target, SPELL_INSANITY_PHASE_128, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                    CreateClonesForUnitInPhase((Player*)target, 128);
                    break;
                }
                case 4:
                {
                    target->CastSpell(target, SPELL_INSANITY_PHASE_256, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                    CreateClonesForUnitInPhase((Player*)target, 256);
                    break;
                }
                default:
                    target->MonsterSay("Fehler",0);
            }
            m_bIsInInsanity = true;
            count++;
        }
    }

    void CreateClonesForUnitInPhase(Player* pTargetPhase, int pPhase)
    {
        if (Map* pMap = m_creature->GetMap())
        {
            Map::PlayerList const &group = pMap->GetPlayers();
            for (Map::PlayerList::const_iterator itr = group.begin(); itr != group.end(); ++itr) 
            {
                if (Player* pPlayerToClone = itr->getSource())
                {
                    if (!pPlayerToClone->isAlive())
                        continue;
                    if (pPlayerToClone->GetObjectGuid() == pTargetPhase->GetObjectGuid())
                        continue;
                    if (pPlayerToClone->isGameMaster())
                        continue;

                    Creature* pClone = m_creature->SummonCreature(m_bIsRegularMode ? CLONE : CLONE_H, pPlayerToClone->GetPositionX(), pPlayerToClone->GetPositionY(), pPlayerToClone->GetPositionZ(), pPlayerToClone->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                    if (pClone) 
                    {
                        pClone->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_UNK_6);
                        // Very cool CLONE!!!
                        pPlayerToClone->CastSpell(pClone, SPELL_CLONE, true);
                        if (Item* pMainhand = pPlayerToClone->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND))
                            pClone->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 0, pMainhand->GetEntry());
                        if (Item* pOffhand = pPlayerToClone->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND))
                            pClone->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, pOffhand->GetEntry());
                        if (Item* pRange = pPlayerToClone->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_RANGED))
                            pClone->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, pRange->GetEntry());

                        pClone->setFaction(FAC_HOSTILE);
                        switch (pPlayerToClone->getClass()) 
                        { 
                            case CLASS_PRIEST:
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_PRIEST : CLONE_HEALTH_PRIEST_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_PALADIN: 
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_PALA : CLONE_HEALTH_PALA_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase);
                                break;
                            case CLASS_WARLOCK: 
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_WARLOCK : CLONE_HEALTH_WARLOCK_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_MAGE:
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_MAGE : CLONE_HEALTH_MAGE_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_ROGUE: 
                                pClone->setPowerType(POWER_ENERGY);
                                pClone->SetMaxPower(POWER_ENERGY,45000);
                                pClone->SetPower(POWER_ENERGY,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_ROGUE : CLONE_HEALTH_ROGUE_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase);
                                break;
                            case CLASS_WARRIOR:
                                pClone->setPowerType(POWER_RAGE);
                                pClone->SetMaxPower(POWER_RAGE,45000);
                                pClone->SetPower(POWER_RAGE,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_WARRIOR : CLONE_HEALTH_WARRIOR_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase);
                                break;
                            case CLASS_DRUID:
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_DRUID : CLONE_HEALTH_DRUID_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_SHAMAN:
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_SHAMAN : CLONE_HEALTH_SHAMAN_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_HUNTER:
                                pClone->setPowerType(POWER_MANA);
                                pClone->SetMaxPower(POWER_MANA,45000);
                                pClone->SetPower(POWER_MANA,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_HUNT : CLONE_HEALTH_HUNT_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase, 20.0f);
                                break;
                            case CLASS_DEATH_KNIGHT:
                                pClone->setPowerType(POWER_RUNIC_POWER);
                                pClone->SetMaxPower(POWER_RUNIC_POWER,45000);
                                pClone->SetPower(POWER_RUNIC_POWER,45000);
                                pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_DK : CLONE_HEALTH_DK_H);
                                pClone->GetMotionMaster()->MoveChase(pTargetPhase);
                                break;
                            default: break;
                        }
                        if (m_bIsRegularMode)
                        {
                            pClone->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, 400.0f );
                            pClone->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, 600.0f );
                        } else {
                            pClone->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, 800.0f );
                            pClone->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, 1100.0f );
                        }
                        pClone->SetLevel(pPlayerToClone->getLevel() + 2);
                        pClone->SetHealth(pClone->GetMaxHealth()); 
                        pClone->Attack(pTargetPhase, true);
                        pClone->AddThreat(pTargetPhase, 10000.0f);
                        pClone->SetInCombatWith(pTargetPhase);
                        p_lCloneGuid.push_back(pClone->GetObjectGuid()); 

                        switch (pPhase)
                        {
                            case 16:
                                pClone->SetPhaseMask(16, true);
                                p_lClone16Guid.push_back(pClone->GetObjectGuid());
                                break;
                            case 32:
                                pClone->SetPhaseMask(32, true);
                                p_lClone32Guid.push_back(pClone->GetObjectGuid());
                                break;
                            case 64:
                                pClone->SetPhaseMask(64, true);
                                p_lClone64Guid.push_back(pClone->GetObjectGuid());
                                break;
                            case 128:
                                pClone->SetPhaseMask(128, true);
                                p_lClone128Guid.push_back(pClone->GetObjectGuid());
                                break;
                            case 256:
                                pClone->SetPhaseMask(256, true);
                                p_lClone256Guid.push_back(pClone->GetObjectGuid());
                                break;
                            default:
                                pTargetPhase->MonsterSay("Fehler in der Phasenzuweisung.", 0);
                        }
                    }
                }
            }            
        }
    }

    bool cloneAlive()
    {
        if (!p_lCloneGuid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lCloneGuid.begin(); itr != p_lCloneGuid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    bool clone16Alive()
    {
        if (!p_lClone16Guid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lClone16Guid.begin(); itr != p_lClone16Guid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    bool clone32Alive()
    {
        if (!p_lClone32Guid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lClone32Guid.begin(); itr != p_lClone32Guid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    bool clone64Alive()
    {
        if (!p_lClone64Guid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lClone64Guid.begin(); itr != p_lClone64Guid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    bool clone128Alive()
    {
        if (!p_lClone128Guid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lClone128Guid.begin(); itr != p_lClone128Guid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }

    bool clone256Alive()
    {
        if (!p_lClone256Guid.empty() && m_pInstance)
        {
            for (GUIDList::iterator itr = p_lClone256Guid.begin(); itr != p_lClone256Guid.end(); ++itr)
            {
                if (Creature* pClone = m_pInstance->instance->GetCreature(*itr))
                {
                    if (pClone->isAlive())
                        return true;
                }
            }
        }

        return false;
    }
 
    void UpdateAI(const uint32 uiDiff) 
    {
        if (!m_bIsInInsanity)
            if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
 
        if (m_bIsInInsanity) 
        { 
            if (insanityEndTimer < uiDiff) 
            { 
                if (!cloneAlive()) 
                {  
                    m_bIsInInsanity = false; 
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim()); 
                    m_creature->Attack(m_creature->getVictim(), true); 
                    insanityEndTimer = 1000; 
                    clone16 = clone32 = clone64 = clone128 = clone256 = false;
                    return;
                }else 
                    insanityEndTimer = 1000; 
            }else insanityEndTimer -= uiDiff;

            if (!clone16Alive() && !clone16)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                { 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                    { 
                        if (target->HasAura(SPELL_INSANITY_PHASE_16))
                        {
                            if (!clone32)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_32, true);
                            else if (!clone64)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_64, true);
                            else if (!clone128)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_128, true);
                            else if (!clone256)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_256, true);
                            target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_16);
                        }
                    } 
                }

                clone16 = true;
            }

            if (!clone32Alive() && !clone32)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                { 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                    { 
                        if (target->HasAura(SPELL_INSANITY_PHASE_32))
                        {
                            if (!clone16)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_16, true);
                            else if (!clone64)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_64, true);
                            else if (!clone128)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_128, true);
                            else if (!clone256)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_256, true);
                            target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_32);
                        }
                    } 
                }

                clone32 = true;
            }

            if (!clone64Alive() && !clone64)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                { 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                    { 
                        if (target->HasAura(SPELL_INSANITY_PHASE_64))
                        {
                            if (!clone16)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_16, true);
                            else if (!clone32)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_32, true);
                            else if (!clone128)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_128, true);
                            else if (!clone256)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_256, true);
                            target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_64);
                        }
                    } 
                }

                clone64 = true;
            }

            if (!clone128Alive() && !clone128)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                { 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                    { 
                        if (target->HasAura(SPELL_INSANITY_PHASE_128))
                        {
                            if (!clone16)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_16, true);
                            else if (!clone32)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_32, true);
                            else if (!clone64)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_64, true);
                            else if (!clone256)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_256, true);
                            target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_128);
                        }
                    } 
                }

                clone128 = true;
            }

            if (!clone256Alive() && !clone256)
            {
                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                { 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                    { 
                        if (target->HasAura(SPELL_INSANITY_PHASE_256))
                        {
                            if (!clone16)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_16, true);
                            else if (!clone32)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_32, true);
                            else if (!clone64)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_64, true);
                            else if (!clone128)
                                target->CastSpell(target, SPELL_INSANITY_PHASE_128, true);
                            target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_256);
                        }
                    } 
                }

                clone256 = true;
            }
        } 
        else 
        { 
            if (m_creature->GetHealthPercent() < m_fHealthPercent && (!phase66 || !phase33)) 
            {
                m_fHealthPercent = m_fHealthPercent - 33.0f;
                DoScriptText(SAY_INSANITY, m_creature);

                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                        DoScriptText(WHISPER_INSANITY,m_creature,target);

                m_creature->InterruptNonMeleeSpells(true);
                
                if (phase66)
                    phase33 = true;
                else
                    phase66 = true;

                DoCastSpellIfCan(m_creature, SPELL_INSANITY); 
                count = 0;
                m_creature->GetMotionMaster()->Clear(); 
                m_creature->GetMotionMaster()->MoveIdle(); 
            } 
 
            if (mindFlayTimer < uiDiff) 
            { 
                if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_MIND_FLAY : SPELL_MIND_FLAY_H) == CAST_OK)
                    mindFlayTimer = urand(10000, 12000); 
            }else mindFlayTimer -= uiDiff; 
 
            if (shadowBoltSalveTimer < uiDiff) 
            { 
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHADOW_BOLT_SALVE : SPELL_SHADOW_BOLT_SALVE_H) == CAST_OK)
                    shadowBoltSalveTimer = urand(5000, 10000); 
            }else shadowBoltSalveTimer -= uiDiff; 
 
            if (shiverTimer < uiDiff) 
            { 
                if (DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), m_bIsRegularMode ? SPELL_SHIVER : SPELL_SHIVER_H) == CAST_OK)
                    shiverTimer = urand(13000, 14000); 
            }else shiverTimer -= uiDiff;
            
            DoMeleeAttackIfReady();     
        }    
    } 
}; 
 
CreatureAI* GetAI_boss_volazj(Creature* pCreature) 
{ 
    return new boss_volazjAI(pCreature); 
} 
 
enum 
{ 
    SPELL_DK_1              = 55978, //Heart Strike
    SPELL_DK_2              = 50688, //Plague Strike
    SPELL_DK_2_H            = 60186, //Plague Strike
    SPELL_DK_3_H            = 50349, //Icy Touch

    SPELL_DRUID_1           = 27737, //Moon Fire
    SPELL_DRUID_1_H         = 47072, //Moon Fire
    SPELL_DRUID_2           = 9857,  //Regrowth
    SPELL_DRUID_2_H         = 48442, //Regrowth
    SPELL_DRUID_3           = 26982, //Rejuvenation
    SPELL_DRUID_3_H         = 48441, //Rejuvenation

    SPELL_HUNTER_1          = 22907, //Shoot
    SPELL_HUNTER_1_H        = 20463, //Shoot
    SPELL_HUNTER_2          = 44286, //Wing Clip

    SPELL_MAGE_1            = 71500, //Fireball
    SPELL_MAGE_1_H          = 24374, //Fireball
    SPELL_MAGE_2            = 31622, //Frostbolt
    SPELL_MAGE_2_H          = 21369, //Frostbolt

    SPELL_ROGUE_1           = 45897, //Hemorrhage
    SPELL_ROGUE_1_H         = 30478, //Hemorrhage
    SPELL_ROGUE_2           = 57640, //Sinister Strike

    SPELL_SHAMAN_1          = 15208, //Lightnig Bolt
    SPELL_SHAMAN_1_H        = 48895, //Lightnig Bolt
    SPELL_SHAMAN_2          = 10605, //Chain Lightning
    SPELL_SHAMAN_2_H        = 24680, //Chain Lightning

    SPELL_PRIEST_1          = 57777, //Renew
    SPELL_PRIEST_1_H        = 46192, //Renew
    SPELL_PRIEST_2          = 23952, //Shadow Word: Pain
    SPELL_PRIEST_2_H        = 41355, //Shadow Word: Pain
 
    SPELL_WARRIOR_1         = 31910, //Whirlwind
    SPELL_WARRIOR_1_H       = 29852, //Whirlwind
    SPELL_WARRIOR_2         = 60019, //Thunderclap
    SPELL_WARRIOR_2_H       = 36214, //Thunderclap

    SPELL_WARLOCK_1         = 24668, //Shadow Bolt
    SPELL_WARLOCK_1_H       = 31618, //Shadow Bolt
    SPELL_WARLOCK_2         = 30938, //Corruption
    SPELL_WARLOCK_2_H       = 60016, //Corruption
    SPELL_WARLOCK_3         = 44267, //Immolate
    SPELL_WARLOCK_3_H       = 20294, //Immolate
   
    SPELL_PALA_1            = 32773, //Consecration
    SPELL_PALA_1_H          = 59998, //Consecration
    SPELL_PALA_2            = 32769, //Holy Light
    SPELL_PALA_2_H          = 29562, //Holy Light
    SPELL_PALA_3            = 58127  //Divine Storm
}; 
struct MANGOS_DLL_DECL mob_volazj_cloneAI : public ScriptedAI 
{ 
    mob_volazj_cloneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    { 
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData(); 
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty(); 
        Reset(); 
    } 
 
    instance_ahnkahet* m_pInstance; 
 
    bool m_bIsRegularMode;

 
    uint32 m_uiSpellTimer;
        
    void Reset()
    {
        m_uiSpellTimer       = 5000;    
    } 

    void UpdateAI(const uint32 uiDiff) 
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (m_creature->GetMaxHealth())
        { 
            case CLONE_HEALTH_DRUID:
            case CLONE_HEALTH_DRUID_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0, 2); 
                    switch (randomSpell) 
                    { 
                        case 0: 
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DRUID_1 : SPELL_DRUID_1_H) == CAST_OK) //Moon Fire
                                m_uiSpellTimer = 5000;
                            break; 
                        case 1:
                            if (!m_creature->HasAura(m_bIsRegularMode ? SPELL_DRUID_2 : SPELL_DRUID_2_H))
                                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_2 : SPELL_DRUID_2_H) == CAST_OK) //Regrowth
                                    m_uiSpellTimer = 5000;
                            break; 
                        case 2:
                            if (!m_creature->HasAura(m_bIsRegularMode ? SPELL_DRUID_3 : SPELL_DRUID_3_H))
                                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_3 : SPELL_DRUID_3_H) == CAST_OK) //Rejuvenation
                                    m_uiSpellTimer = 5000;
                            break; 
                        default:
                            break;
                    }
                } else
                    m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_MAGE:
            case CLONE_HEALTH_MAGE_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,1);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGE_1 : SPELL_MAGE_1_H) == CAST_OK) //Fireball
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGE_2 : SPELL_MAGE_2_H) == CAST_OK) //Frostbolt
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;                
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_PALA:
            case CLONE_HEALTH_PALA_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,2);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PALA_1 : SPELL_PALA_1_H) == CAST_OK) // Consecration
                                m_uiSpellTimer = urand(4000, 5000);
                            break;
                        case 1:
                            if (m_creature->GetHealth() < (m_creature->GetMaxHealth() - 2000))
                                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PALA_2 : SPELL_PALA_2_H) == CAST_OK) // Holy Light
                                    m_uiSpellTimer = urand(4000, 5000);
                            break;
                        case 2:
                            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PALA_3) == CAST_OK) // Divine Storm
                                m_uiSpellTimer = urand(4000, 5000);
                            break;
                        default:
                            break;
                    } 
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_HUNT:
            case CLONE_HEALTH_HUNT_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ?  SPELL_HUNTER_1 :  SPELL_HUNTER_1_H) == CAST_OK) //Shoot
                        m_uiSpellTimer = urand(3000, 5000);
                    if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 3.0f))
                    {
                        DoCast(m_creature->getVictim(), SPELL_HUNTER_2, true); //Wing Clip
                        m_uiSpellTimer = urand(5000, 7000);
                    }
                }else m_uiSpellTimer -= uiDiff;
                
                break;
            }
            case CLONE_HEALTH_WARLOCK:
            case CLONE_HEALTH_WARLOCK_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,2);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_1 : SPELL_WARLOCK_1_H) == CAST_OK) //Shadow Bolt
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (!m_creature->getVictim()->HasAura(m_bIsRegularMode ? SPELL_WARLOCK_2 : SPELL_WARLOCK_2_H))
                                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_2 : SPELL_WARLOCK_2_H) == CAST_OK) //Corruption
                                    m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 2:
                            if (!m_creature->getVictim()->HasAura(m_bIsRegularMode ? SPELL_WARLOCK_3 : SPELL_WARLOCK_3_H))
                                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_3 : SPELL_WARLOCK_3_H) == CAST_OK) //Immolate
                                    m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_DK:
            case CLONE_HEALTH_DK_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                { 
                    int randomSpell = urand(0,2);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DK_1) == CAST_OK) //Heart Strike
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DK_2 : SPELL_DK_2_H) == CAST_OK) //Plague Strike
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 2:
                            if (!m_bIsRegularMode)
                                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DK_3_H) == CAST_OK) //Icy Touch
                                    m_uiSpellTimer = urand(4000, 6000);
                            break;                               
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_WARRIOR:
            case CLONE_HEALTH_WARRIOR_H:
            {

                if (m_uiSpellTimer < uiDiff) 
                { 
                    int randomSpell = urand(0,1);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARRIOR_1 : SPELL_WARRIOR_1_H) == CAST_OK) //Whirlwind
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARRIOR_2 : SPELL_WARRIOR_2_H) == CAST_OK) //Thunderclap
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff; 

                break;
            }
            case CLONE_HEALTH_PRIEST:
            case CLONE_HEALTH_PRIEST_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,1);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PRIEST_1 : SPELL_PRIEST_1_H) == CAST_OK) //Renew
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PRIEST_2 : SPELL_PRIEST_2_H) == CAST_OK) //Shadow Word: Pain
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_SHAMAN:
            case CLONE_HEALTH_SHAMAN_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,1);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHAMAN_1 : SPELL_SHAMAN_1_H) == CAST_OK) //Lightnig Bolt
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHAMAN_2 : SPELL_SHAMAN_2_H) == CAST_OK) //Chain Lightning
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            case CLONE_HEALTH_ROGUE:
            case CLONE_HEALTH_ROGUE_H:
            {
                if (m_uiSpellTimer < uiDiff) 
                {
                    int randomSpell = urand(0,1);
                    switch (randomSpell)
                    {
                        case 0:
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ROGUE_1 : SPELL_ROGUE_1_H) == CAST_OK) //Hemorrhage
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        case 1:
                            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ROGUE_2) == CAST_OK) //Sinister Strike
                                m_uiSpellTimer = urand(4000, 6000);
                            break;
                        default:
                            break;
                    }
                }else m_uiSpellTimer -= uiDiff;

                break;
            }
            default:
                break;
        }

        DoMeleeAttackIfReady(); 
    } 
}; 
 
CreatureAI* GetAI_mob_volazj_clone(Creature* pCreature) 
{ 
    return new mob_volazj_cloneAI(pCreature); 
} 
 
void AddSC_boss_volazj() 
{ 
    Script *newscript; 
 
    newscript = new Script; 
    newscript->Name = "boss_volazj"; 
    newscript->GetAI = &GetAI_boss_volazj; 
    newscript->RegisterSelf(); 
 
    newscript = new Script; 
    newscript->Name = "mob_volazj_clone"; 
    newscript->GetAI = &GetAI_mob_volazj_clone; 
    newscript->RegisterSelf(); 
}
