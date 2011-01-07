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
 
    bool isInInsanity; 
    bool phase66; 
    bool phase33; 
    bool clone16;
    bool clone32;
    bool clone64;
    bool clone128;
    bool clone256;
 
    std::list<uint64> cloneGUIDList; 
    std::list<uint64> clone16GUIDList;
    std::list<uint64> clone32GUIDList;
    std::list<uint64> clone64GUIDList;
    std::list<uint64> clone128GUIDList;
    std::list<uint64> clone256GUIDList;
 
    uint32 insanityEndTimer; 
    uint32 insanityTimer; 
    uint32 createMirrorTimer; 
    uint32 shadowBoltSalveTimer; 
    uint32 shiverTimer; 
    uint32 mindFlayTimer; 
    uint32 achievementTimer;
 
    void Reset() 
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
        insanityEndTimer = 9999999; 
        createMirrorTimer = 9999999;
        achievementTimer = 120000; // 2minutes
        shadowBoltSalveTimer = 6000;
        shiverTimer = 13000;
        mindFlayTimer = 9000;
        cloneGUIDList.clear();
        clone16GUIDList.clear();
        clone32GUIDList.clear();
        clone64GUIDList.clear();
        clone128GUIDList.clear();
        clone256GUIDList.clear();

        isInInsanity = false;
        phase66 = false;
        phase33 = false;
        clone16 = false;
        clone32 = false;
        clone64 = false;
        clone128 = false;
        clone256 = false;

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
            if (Unit *target = pMap->GetUnit(itr->getSource()->GetGUID())) 
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
            if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
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
            if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
            { 
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_16);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_32);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_64);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_128);
                target->RemoveAurasDueToSpell(SPELL_INSANITY_PHASE_256);
            } 
        }
    }

    void setPlayersPhase()
    {
        uint8 i = 1;

        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &players = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        {
            if(Player* target = pMap->GetPlayer(itr->getSource()->GetGUID())) 
            {
                switch(i)
                {
                    case 1:
                    {
                        target->CastSpell(target, SPELL_INSANITY_PHASE_16, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                        break;
                    }
                    case 2:
                    {
                        target->CastSpell(target, SPELL_INSANITY_PHASE_32, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                        break;
                    }
                    case 3:
                    {
                        target->CastSpell(target, SPELL_INSANITY_PHASE_64, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                        break;
                    }
                    case 4:
                    {
                        target->CastSpell(target, SPELL_INSANITY_PHASE_128, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                        break;
                    }
                    case 5:
                    {
                        target->CastSpell(target, SPELL_INSANITY_PHASE_256, true, NULL, NULL, m_creature->GetObjectGuid(), NULL);
                        break;
                    }
                    default:
                        break;
                }
                i++;
            }
        }
    }
 
    void createClassMirrors() 
    {
        Map* pMap = m_creature->GetMap();
        Map::PlayerList const &group1 = pMap->GetPlayers();
        for (Map::PlayerList::const_iterator itr = group1.begin(); itr != group1.end(); ++itr) 
        {
            Map::PlayerList const &group2 = group1;
            for (Map::PlayerList::const_iterator ittr = group2.begin(); ittr != group2.end(); ++ittr)
            {
                if (Player* pPlayer1 = pMap->GetPlayer(itr->getSource()->GetGUID()))
                {
                    if (Player* pPlayer2 = pMap->GetPlayer(ittr->getSource()->GetGUID()))
                    {
                        if (pPlayer1->isAlive() && pPlayer2->isAlive() && (pPlayer1->GetGUID() != pPlayer2->GetGUID()) && !pPlayer1->isGameMaster() && !pPlayer2->isGameMaster())
                        {
                            Creature* pClone = m_creature->SummonCreature(m_bIsRegularMode ? CLONE : CLONE_H, pPlayer2->GetPositionX(), pPlayer2->GetPositionY(), pPlayer2->GetPositionZ(), pPlayer2->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                            if (pClone) 
                            {
                                pClone->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PASSIVE);
                                pClone->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);
                                pClone->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
                                pClone->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_UNK_6);

                                pClone->SetName(pPlayer2->GetName());
                                pClone->SetDisplayId(pPlayer2->GetDisplayId());
                                pClone->setFaction(FAC_HOSTILE);

                                switch (pPlayer2->getClass()) 
                                { 
                                    case CLASS_PRIEST:
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_PRIEST : CLONE_HEALTH_PRIEST_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_PALADIN: 
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_PALA : CLONE_HEALTH_PALA_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1);
                                        break;
                                    case CLASS_WARLOCK: 
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_WARLOCK : CLONE_HEALTH_WARLOCK_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_MAGE:
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_MAGE : CLONE_HEALTH_MAGE_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_ROGUE: 
                                        pClone->setPowerType(POWER_ENERGY);
                                        pClone->SetMaxPower(POWER_ENERGY,45000);
                                        pClone->SetPower(POWER_ENERGY,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_ROGUE : CLONE_HEALTH_ROGUE_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1);
                                        break;
                                    case CLASS_WARRIOR:
                                        pClone->setPowerType(POWER_RAGE);
                                        pClone->SetMaxPower(POWER_RAGE,45000);
                                        pClone->SetPower(POWER_RAGE,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_WARRIOR : CLONE_HEALTH_WARRIOR_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1);
                                        break;
                                    case CLASS_DRUID:
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_DRUID : CLONE_HEALTH_DRUID_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_SHAMAN:
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_SHAMAN : CLONE_HEALTH_SHAMAN_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_HUNTER:
                                        pClone->setPowerType(POWER_MANA);
                                        pClone->SetMaxPower(POWER_MANA,45000);
                                        pClone->SetPower(POWER_MANA,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_HUNT : CLONE_HEALTH_HUNT_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1, 20.0f);
                                        break;
                                    case CLASS_DEATH_KNIGHT:
                                        pClone->setPowerType(POWER_RUNIC_POWER);
                                        pClone->SetMaxPower(POWER_RUNIC_POWER,45000);
                                        pClone->SetPower(POWER_RUNIC_POWER,45000);
                                        pClone->SetMaxHealth(m_bIsRegularMode ? CLONE_HEALTH_DK : CLONE_HEALTH_DK_H);
                                        pClone->GetMotionMaster()->MoveChase(pPlayer1);
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
                                pClone->SetLevel(pPlayer2->getLevel() + 2);
                                pClone->SetHealth(pClone->GetMaxHealth()); 
                                pClone->Attack(pPlayer1, true);
                                pClone->AddThreat(pPlayer1, 10000.0f);
                                pClone->SetInCombatWith(pPlayer1);
                                cloneGUIDList.push_back(pClone->GetGUID()); 
                     
                    
                                if (pPlayer1->HasAura(SPELL_INSANITY_PHASE_16)) //Phase 16
                                {
                                    pClone->SetPhaseMask(16, true);
                                    clone16GUIDList.push_back(pClone->GetGUID());
                                }
                                else if (pPlayer1->HasAura(SPELL_INSANITY_PHASE_32))//Phase 32
                                {
                                    pClone->SetPhaseMask(32, true);
                                    clone32GUIDList.push_back(pClone->GetGUID());                            
                                }
                                else if (pPlayer1->HasAura(SPELL_INSANITY_PHASE_64))//Phase 64
                                {
                                    pClone->SetPhaseMask(64, true);
                                    clone64GUIDList.push_back(pClone->GetGUID());
                                }
                                else if (pPlayer1->HasAura(SPELL_INSANITY_PHASE_128))//Phase 128
                                {
                                    pClone->SetPhaseMask(128, true);
                                    clone128GUIDList.push_back(pClone->GetGUID());
                                }
                                else if (pPlayer1->HasAura(SPELL_INSANITY_PHASE_256))//Phase 256
                                {
                                    pClone->SetPhaseMask(256, true);
                                    clone256GUIDList.push_back(pClone->GetGUID());
                                }
                            }
                        }
                    }
                }
            } 
        } 
    } 

    bool cloneAlive()
    {
        if (!cloneGUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = cloneGUIDList.begin(); itr != cloneGUIDList.end(); ++itr)
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
        if (!clone16GUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = clone16GUIDList.begin(); itr != clone16GUIDList.end(); ++itr)
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
        if (!clone32GUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = clone32GUIDList.begin(); itr != clone32GUIDList.end(); ++itr)
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
        if (!clone64GUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = clone64GUIDList.begin(); itr != clone64GUIDList.end(); ++itr)
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
        if (!clone128GUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = clone128GUIDList.begin(); itr != clone128GUIDList.end(); ++itr)
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
        if (!clone256GUIDList.empty() && m_pInstance)
        {
            for (std::list<uint64>::iterator itr = clone256GUIDList.begin(); itr != clone256GUIDList.end(); ++itr)
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
        if (!isInInsanity)
            if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
 
        if (isInInsanity) 
        { 
            if (insanityEndTimer < uiDiff) 
            { 
                if (!cloneAlive()) 
                {  
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
                    isInInsanity = false; 
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim()); 
                    m_creature->Attack(m_creature->getVictim(), true); 
                    insanityEndTimer = 9999999; 
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
            if (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.66 && !phase66) 
            { 
                DoScriptText(SAY_INSANITY, m_creature);

                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                        DoScriptText(WHISPER_INSANITY,m_creature,target);

                m_creature->InterruptNonMeleeSpells(true);
                phase66 = true; 
                DoCastSpellIfCan(m_creature, SPELL_INSANITY); 
                createMirrorTimer = 5000; 
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
                m_creature->GetMotionMaster()->Clear(); 
                m_creature->GetMotionMaster()->MoveIdle(); 
            } 
 
            if (m_creature->GetHealth() < m_creature->GetMaxHealth() * 0.33 && !phase33) 
            {
                DoScriptText(SAY_INSANITY, m_creature);

                Map* pMap = m_creature->GetMap();
                Map::PlayerList const &players = pMap->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr) 
                    if (Unit *target = m_creature->GetMap()->GetUnit(itr->getSource()->GetGUID())) 
                        DoScriptText(WHISPER_INSANITY,m_creature,target);

                m_creature->InterruptNonMeleeSpells(true);
                phase33 = true; 
                DoCastSpellIfCan(m_creature, SPELL_INSANITY); 
                createMirrorTimer = 5000; 
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE); 
                m_creature->GetMotionMaster()->Clear(); 
                m_creature->GetMotionMaster()->MoveIdle(); 
            } 
            if (createMirrorTimer)
            {
                if (createMirrorTimer < uiDiff) 
                { 
                    isInInsanity = true; 
                    setPlayersPhase();
                    createClassMirrors();
                    createMirrorTimer = 0; 
                    insanityEndTimer = 5000;
                }else createMirrorTimer -= uiDiff; 
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

 
    uint32 spellPriestTimer; 
    uint32 spellRogueTimer; 
    uint32 spellMageTimer; 
    uint32 spellDeathKnightTimer; 
    uint32 spellShamanTimer; 
    uint32 spellWarlockTimer; 
    uint32 spellDruidTimer; 
    uint32 spellWarriorTimer; 
    uint32 spellPaladinTimer; 
    uint32 spellHunterTimer; 
        
    void Reset()
    {
        spellPriestTimer       = 5000;    
        spellRogueTimer        = 5000;
        spellMageTimer         = 5000;
        spellDeathKnightTimer  = 5000;
        spellShamanTimer       = 5000;
        spellWarlockTimer      = 5000;
        spellDruidTimer        = 5000;
        spellWarriorTimer      = 5000;
        spellPaladinTimer      = 5000;
        spellHunterTimer       = 5000;
    } 

    void UpdateAI(const uint32 uiDiff) 
    {
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetMaxHealth() == CLONE_HEALTH_DRUID || m_creature->GetMaxHealth() == CLONE_HEALTH_DRUID_H) 
        {
            if (spellDruidTimer < uiDiff) 
            {
                int randomSpell = urand(0, 2); 
                switch (randomSpell) 
                { 
                    case 0: 
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DRUID_1 : SPELL_DRUID_1_H) == CAST_OK) //Moon Fire
                            spellDruidTimer = 5000;
                        break; 
                    case 1:
                        if (!m_creature->HasAura(m_bIsRegularMode ? SPELL_DRUID_2 : SPELL_DRUID_2_H))
                            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_2 : SPELL_DRUID_2_H) == CAST_OK) //Regrowth
                                spellDruidTimer = 5000;
                        break; 
                    case 2:
                        if (!m_creature->HasAura(m_bIsRegularMode ? SPELL_DRUID_3 : SPELL_DRUID_3_H))
                            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_DRUID_3 : SPELL_DRUID_3_H) == CAST_OK) //Rejuvenation
                                spellDruidTimer = 5000;
                        break; 
                    default:
                        break;
                }
            } else
                spellDruidTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_MAGE || m_creature->GetMaxHealth() == CLONE_HEALTH_MAGE_H) 
        {
            if (spellMageTimer < uiDiff) 
            {
                int randomSpell = urand(0,1);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGE_1 : SPELL_MAGE_1_H) == CAST_OK) //Fireball
                            spellMageTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MAGE_2 : SPELL_MAGE_2_H) == CAST_OK) //Frostbolt
                            spellMageTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellMageTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_PALA || m_creature->GetMaxHealth() == CLONE_HEALTH_PALA_H) 
        { 
            if (spellPaladinTimer < uiDiff) 
            {
                int randomSpell = urand(0,2);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PALA_1 : SPELL_PALA_1_H) == CAST_OK) // Consecration
                            spellPaladinTimer = urand(4000, 5000);
                        break;
                    case 1:
                        if (m_creature->GetHealth() < (m_creature->GetMaxHealth() - 2000))
                            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PALA_2 : SPELL_PALA_2_H) == CAST_OK) // Holy Light
                                spellPaladinTimer = urand(4000, 5000);
                        break;
                    case 2:
                        if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_PALA_3) == CAST_OK) // Divine Storm
                            spellPaladinTimer = urand(4000, 5000);
                        break;
                    default:
                        break;
                } 
            }else spellPaladinTimer -= uiDiff; 
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_HUNT || m_creature->GetMaxHealth() == CLONE_HEALTH_HUNT_H) 
        {
            if (spellHunterTimer < uiDiff) 
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ?  SPELL_HUNTER_1 :  SPELL_HUNTER_1_H) == CAST_OK) //Shoot
                    spellHunterTimer = urand(3000, 5000);
                if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 3.0f))
                {
                    DoCast(m_creature->getVictim(), SPELL_HUNTER_2, true); //Wing Clip
                    spellHunterTimer = urand(5000, 7000);
                }
            }else spellHunterTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_WARLOCK || m_creature->GetMaxHealth() == CLONE_HEALTH_WARLOCK_H) 
        {
            if (spellWarlockTimer < uiDiff) 
            {
                int randomSpell = urand(0,2);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_1 : SPELL_WARLOCK_1_H) == CAST_OK) //Shadow Bolt
                            spellWarlockTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (!m_creature->getVictim()->HasAura(m_bIsRegularMode ? SPELL_WARLOCK_2 : SPELL_WARLOCK_2_H))
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_2 : SPELL_WARLOCK_2_H) == CAST_OK) //Corruption
                                spellWarlockTimer = urand(4000, 6000);
                        break;
                    case 2:
                        if (!m_creature->getVictim()->HasAura(m_bIsRegularMode ? SPELL_WARLOCK_3 : SPELL_WARLOCK_3_H))
                            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARLOCK_3 : SPELL_WARLOCK_3_H) == CAST_OK) //Immolate
                                spellWarlockTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellWarlockTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_DK || m_creature->GetMaxHealth() == CLONE_HEALTH_DK_H) 
        { 
            if (spellDeathKnightTimer < uiDiff) 
            { 
                int randomSpell = urand(0,2);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DK_1) == CAST_OK) //Heart Strike
                            spellDeathKnightTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DK_2 : SPELL_DK_2_H) == CAST_OK) //Plague Strike
                            spellDeathKnightTimer = urand(4000, 6000);
                        break;
                    case 2:
                        if (!m_bIsRegularMode)
                            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_DK_3_H) == CAST_OK) //Icy Touch
                                spellDeathKnightTimer = urand(4000, 6000);
                        break;                               
                    default:
                        break;
                }
            }else spellDeathKnightTimer -= uiDiff;  
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_WARRIOR || m_creature->GetMaxHealth() == CLONE_HEALTH_WARRIOR_H) 
        { 
            if (spellWarriorTimer < uiDiff) 
            { 
                int randomSpell = urand(0,1);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARRIOR_1 : SPELL_WARRIOR_1_H) == CAST_OK) //Whirlwind
                            spellWarriorTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_WARRIOR_2 : SPELL_WARRIOR_2_H) == CAST_OK) //Thunderclap
                            spellWarriorTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellWarriorTimer -= uiDiff; 
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_PRIEST || m_creature->GetMaxHealth() == CLONE_HEALTH_PRIEST_H) 
        {
            if (spellPriestTimer < uiDiff) 
            {
                int randomSpell = urand(0,1);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PRIEST_1 : SPELL_PRIEST_1_H) == CAST_OK) //Renew
                            spellPriestTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PRIEST_2 : SPELL_PRIEST_2_H) == CAST_OK) //Shadow Word: Pain
                            spellPriestTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellPriestTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_SHAMAN || m_creature->GetMaxHealth() == CLONE_HEALTH_SHAMAN_H) 
        {
            if (spellShamanTimer < uiDiff) 
            {
                int randomSpell = urand(0,1);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHAMAN_1 : SPELL_SHAMAN_1_H) == CAST_OK) //Lightnig Bolt
                            spellShamanTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHAMAN_2 : SPELL_SHAMAN_2_H) == CAST_OK) //Chain Lightning
                            spellShamanTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellShamanTimer -= uiDiff;
        } 
        else if (m_creature->GetMaxHealth() == CLONE_HEALTH_ROGUE || m_creature->GetMaxHealth() == CLONE_HEALTH_ROGUE_H) 
        { 
            if (spellRogueTimer < uiDiff) 
            {
                int randomSpell = urand(0,1);
                switch (randomSpell)
                {
                    case 0:
                        if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ROGUE_1 : SPELL_ROGUE_1_H) == CAST_OK) //Hemorrhage
                            spellRogueTimer = urand(4000, 6000);
                        break;
                    case 1:
                        if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_ROGUE_2) == CAST_OK) //Sinister Strike
                            spellRogueTimer = urand(4000, 6000);
                        break;
                    default:
                        break;
                }
            }else spellRogueTimer -= uiDiff;
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
