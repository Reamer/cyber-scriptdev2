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
SDName: boss_hodir
SD%Complete: 
SDComment: Encounter overview: Hodir casts regular Icicles all the time (as triggered casts from aura, every 2sec,
           on 10man there are 2sec. breaks every 2 icicles).
           Flash Freeze has precast spell, marking random 2-3 already existing .
           When the spell is cast, there is a spell triggered that targets all regular Icicles.
           They spawn Snowpacked Icicles that spawn Snowdrifts and have area aura marking targets to be safe from Flash Freeze.
           All above fixed in core, so refer to commit history of instance_Ulduar file.
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                   = -1603176,
    SAY_SLAY_1                  = -1603174,
    SAY_SLAY_2                  = -1603173,
    SAY_FLASH_FREEZE            = -1603172,
    SAY_FROZEN_BLOWS            = -1603171,
    SAY_DEATH                   = -1603175,
    SAY_BERSERK                 = -1603178,
    SAY_HELP_YOGG               = -1603177,

    EMOTE_FLASH_FREEZE          = -1603179,
    EMOTE_FROZEN_BLOWS          = -1603180,

    SPELL_ENRAGE                = 26662,

    SPELL_FROZEN_BLOWS          = 62478,
    SPELL_FROZEN_BLOWS_H        = 63512,
    SPELL_FREEZE                = 62469,
    SPELL_BITTER_COLD           = 62038,	// SPELL BROKEN!!!!
    SPELL_ICICLE_TARGETING_AURA = 62227,    // spell used for random targeting of Icicle throughout the whole encounter. not used here
    SPELL_ICICLE_FLASH_MARKS    = 62476,    // casts this right befor flash freeze, triggers SPELL_ICICLE_SNOWPACKED. should target Icicles dummy NPCs
    SPELL_ICICLE_FLASH_MARKS_H  = 62477,
    SPELL_ICICLE                = 62234,    // regular Icicle throughout the whole encounter
    SPELL_ICICLE_SNOW_DAMAGE    = 62460,    // regular Icicle throughout the whole encounter, spell triggered by dummy NPC    
    SPELL_ICICLE_DAMAGE_NORMAL  = 62236,
    SPELL_ICICLE_SNOWPACKED_SUM = 62462,    // icicles right before flash freeze that summon snowdrifts
    SPELL_ICICLE_SNOWDRIFT      = 62460,
    SPELL_COLLAPSING_ICICLE     = 69424,    // this spell summons the exact Icicle that falls, deals damage and knocks back
    SPELL_ICE_SHARDS            = 65370,    // icicle damage -> 14k
    SPELL_ICICLE_DUMMY          = 62453,    // cast by Snowpacked Icicle, after this they cast Icicle with snowdrift
    SPELL_SNOWDRIFT             = 62463,
    SPELL_SAFE_AREA_AURA        = 65705,    // snowpacked icicles cast this around on players to make them safe from flash freeze
    SPELL_SAFE_AREA_BUFF        = 62464,
    SPELL_FLASH_FREEZE_KILL     = 62226,    // if target was already Flash Frozen then kill it
    SPELL_FLASH_FREEZE          = 61968,    // main spell cast by Hodir
    SPELL_FLASH_FREEZE_DEBUFF   = 61969,    // stun aura
    SPELL_FLASH_FREEZE_SUMMON   = 61970,    // targets of Flash Freeze summon NPC
    SPELL_FLASH_FREEZE_SUM_NPC  = 61989,    // used by NPC
    SPELL_FLASH_FREEZE_NPC_STUN = 61990,    // used to freeze npcs
    SPELL_FLASH_FREEZE_VIS      = 62148,    // visual effect, triggered after main spell
    SPELL_FLASH_FREEZE_STUN     = 64175,

    NPC_SNOWDRIFT_TARGET    = 33174,
    NPC_ICICLE              = 33169,
    NPC_SNOW_ICICLE         = 33173,
    NPC_COLLAPSING_ICICLE   = 36847,
    NPC_FLASH_FREEZE        = 32926,
    NPC_FLASH_FREEZE_NPC    = 32938,

    ACHIEV_RARE_CACHE       = 3182,
    ACHIEV_RARE_CACHE_H     = 3184,
    ACHIEV_COOLEST_FRIEND   = 2963,
    ACHIEV_COOLEST_FRIEND_H = 2965,
    ACHIEV_GETTING_COLD     = 2967,
    ACHIEV_GETTING_COLD_H   = 2968,
    ACHIEV_CHEESE_FREEZE    = 2961,
    ACHIEV_CHEESE_FREEZE_H  = 2962,

    // helper npcs
    // druid
    NPC_DRUID           = 32941,
    SPELL_WRATH         = 62793,
    SPELL_STARLIGHT     = 62807, // friendly

    // shaman
    NPC_SHAMAN          = 32950,
    SPELL_LAVA_BURST    = 61924,
    SPELL_STORM_CLOUD   = 65123, // friendly
    SPELL_STORM_CLOUD_H = 65133,
    SPELL_STORM_POWER   = 65134, // friendly

    // mage
    NPC_MAGE            = 32946,
    SPELL_FIREBALL      = 61909,
    SPELL_TOASTY_FIRE   = 62823, // friendly -> summon
    NPC_TOASTY_FIRE     = 33342,
    SPELL_TOASTY_FIRE_A = 62821,
    SPELL_MELT_ICE      = 64528,
    SPELL_SIGNED        = 65280,

    // priest
    NPC_PRIEST          = 32948,
    SPELL_SMITE         = 61923,
    SPELL_GREAT_HEAL    = 62809,    //friendly
    SPELL_DISPEL_MAGIC  = 63499,    //friendly
};

// Script for the Flash freeze which is enchasing the npcs in ice at the begginign of the fight
// this needs some fixing on spells
struct MANGOS_DLL_DECL mob_npc_flashFreezeAI : public ScriptedAI
{
    mob_npc_flashFreezeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        //pCreature->SetDisplayId(25865);     // invisible
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    uint64 m_uiVictimsGUID;

    uint64  m_uiFreezeTimer;
    bool    m_bIsFreeze;
    uint32  m_uiCheckTimer;

    void Reset()
    {
        m_uiVictimsGUID = 0;
        m_uiCheckTimer = 1000;
    }

    void Aggro(Unit *who)
    {
        if (Creature* pHodir = GetClosestCreatureWithEntry(m_creature, NPC_HODIR, 100.0f))
        {
            pHodir->AI()->AttackStart(who);
            pHodir->AddThreat(who, 0.0f);
        }
    }

    void TeleportMeToCreature(Creature* pCreature)
    {
        m_creature->GetMap()->CreatureRelocation(m_creature, pCreature->GetPositionX(), pCreature->GetPositionY(), pCreature->GetPositionZ(), 0);
        m_creature->Relocate(pCreature->GetPositionX(), pCreature->GetPositionY(), pCreature->GetPositionZ(), 0);
    }

    void FreezeVictim(uint64 pVictimGUID)
    {
        if (!pVictimGUID)
            return;
        if (Map* pMap = m_creature->GetMap())
        {
            if (Creature* pVictim = pMap->GetCreature(pVictimGUID))
            {
                if (pVictim->isAlive() && !pVictim->HasAura(SPELL_FLASH_FREEZE_NPC_STUN, EFFECT_INDEX_0))
                {
                    TeleportMeToCreature(pVictim);
                    //DoCast(m_creature, SPELL_FLASH_FREEZE_NPC_STUN, true);
                    DoCast(pVictim, SPELL_FLASH_FREEZE_NPC_STUN, true);
                    m_uiVictimsGUID = pVictimGUID;
                }
            }
        }       
    }

    void JustDied(Unit* Killer)
    {            
        if (Creature* pVictim = m_creature->GetMap()->GetCreature(m_uiVictimsGUID))
        {
            if (pVictim->isAlive() && pVictim->HasAura(SPELL_FLASH_FREEZE_NPC_STUN, EFFECT_INDEX_0))
            {
                pVictim->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_NPC_STUN);
                if (Creature* pHodir = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_HODIR)))
                    pVictim->AddThreat(pHodir, 100.0f);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiCheckTimer <= uiDiff)
        {
            if (Unit* pVictim = m_creature->GetMap()->GetUnit(m_uiVictimsGUID))
            {
                if (!pVictim->isAlive())
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), 0, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, 0 , false);
            }
            m_uiCheckTimer = 1000;
        }else m_uiCheckTimer -= uiDiff;
    }
};

// Hodir
struct MANGOS_DLL_DECL boss_hodirAI : public ScriptedAI
{
    boss_hodirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    instance_ulduar* m_pInstance;

    // hard mode timer
    uint32 m_uiSpeedKillTimer;

    uint32 m_uiEnrageTimer;
    uint32 m_uiFlashFreezeTimer;
    uint32 m_uiFlashFreezeCastTimer;
    uint32 m_uiFrozenBlowsTimer;
    uint32 m_uiFreezeTimer;

    bool m_bIsOutro;
    uint32 m_uiOutroTimer;
    uint32 m_uiStep;

    uint64 m_uiHelperGUID[4];
    uint64 m_uiDruidHelperGUID;
    uint64 m_uiMageHelperGUID;
    uint64 m_uiPriestHelperGUID;


    std::vector<uint64> lHelperGUID;
    std::vector<uint64> lflashfreezeMobGUID;
    

    void Reset()
    {
        //respawn all npc's
        if (m_pInstance)
            for (std::list<uint64>::iterator i = m_pInstance->m_lHodirMobsGUIDs.begin(); i != m_pInstance->m_lHodirMobsGUIDs.end(); i++)
                if (Creature *pTmp = m_pInstance->instance->GetCreature(*i))
                    if (!pTmp->isAlive())
                        pTmp->Respawn();
        m_uiSpeedKillTimer      = 0;
        m_uiEnrageTimer         = 480000;
        m_uiFlashFreezeTimer    = 50000;
        m_uiFlashFreezeCastTimer= 90000;
        m_uiFrozenBlowsTimer    = 60000;
        m_uiFreezeTimer         = urand(15000, 20000);
        m_uiOutroTimer          = 10000;
        m_uiStep                = 1;
        m_bIsOutro              = false;

        lHelperGUID.clear();
        lflashfreezeMobGUID.clear();

        if (lHelperGUID.empty())
        {
            std::list<Creature*> lHelpers;
            GetCreatureListWithEntryInGrid(lHelpers, m_creature, NPC_DRUID, DEFAULT_VISIBILITY_INSTANCE);
            GetCreatureListWithEntryInGrid(lHelpers, m_creature, NPC_SHAMAN, DEFAULT_VISIBILITY_INSTANCE);
            GetCreatureListWithEntryInGrid(lHelpers, m_creature, NPC_MAGE, DEFAULT_VISIBILITY_INSTANCE);
            GetCreatureListWithEntryInGrid(lHelpers, m_creature, NPC_PRIEST, DEFAULT_VISIBILITY_INSTANCE);
            if (!lHelpers.empty())
            {

                for(std::list<Creature*>::iterator iter = lHelpers.begin(); iter != lHelpers.end(); ++iter)
                {
                    if ((*iter))
                    {
                        (*iter)->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_NPC_STUN);
                        //(*iter)->MonsterYell("Ich bin Helfer und wurde erkannt", 0);
                        lHelperGUID.push_back((*iter)->GetGUID());
                    }
                }
            }
        }
        if (lflashfreezeMobGUID.empty())
        {
            std::list<Creature*> lFlashFreezeMobs;
            GetCreatureListWithEntryInGrid(lFlashFreezeMobs, m_creature, NPC_FLASH_FREEZE_NPC, DEFAULT_VISIBILITY_INSTANCE);
            if (!lFlashFreezeMobs.empty())
            {
                for(std::list<Creature*>::iterator iter = lFlashFreezeMobs.begin(); iter != lFlashFreezeMobs.end(); ++iter)
                {
                    if ((*iter))
                    {
                        (*iter)->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_NPC_STUN);
                        //(*iter)->MonsterYell("Ich bin Blitzeis und wurde erkannt", 0);
                        lflashfreezeMobGUID.push_back((*iter)->GetGUID());
                    }                
                }
            }
        }
        FreezeAllHelper();
    }

    void JustReachedHome()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_HODIR, FAIL);
    }

    void FreezeAllHelper()
    {
        if (lHelperGUID.size() == lflashfreezeMobGUID.size())
        {
            if (Map* pMap =  m_creature->GetMap())
            {
                for (uint8 i = 0; i < lHelperGUID.size(); i++)
                {
                    if (Creature* pFreezeMob = pMap->GetCreature(lflashfreezeMobGUID.at(i)))
                    {
                        if(mob_npc_flashFreezeAI* pFreezeMobAI = (mob_npc_flashFreezeAI*) pFreezeMob->AI())
                        {
                            pFreezeMobAI->FreezeVictim(lHelperGUID.at(i));
                        }
                    }
                }                
            }
        }
        else
        {
            m_creature->MonsterYell("Die Anzahl der Helfer ist ungleich der FreezeMobs", 0);
        }
    }

    void Aggro(Unit *who)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_HODIR, IN_PROGRESS);

        DoScriptText(SAY_AGGRO, m_creature);

        DoCast(m_creature, SPELL_BITTER_COLD, true);
        DoCast(m_creature, SPELL_ICICLE_TARGETING_AURA, true);
    }

    void DoOutro()
    {
        if(m_pInstance)
        {
            if(m_uiSpeedKillTimer < 180000)
            {
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
                m_pInstance->SetData(TYPE_HODIR, DONE);
            }
            else
                m_pInstance->SetData(TYPE_HODIR, DONE);
        }
        m_creature->ForcedDespawn();
    }

    // for debug only
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
        {
            m_pInstance->SetData(TYPE_HODIR, DONE);
            if(m_uiSpeedKillTimer > 0)
                m_pInstance->SetData(TYPE_HODIR_HARD, DONE);
        }
    }

    void DamageTaken(Unit *done_by, uint32 &uiDamage)
    {
        if(m_creature->GetHealthPercent() < 1.0f)
        {
            uiDamage = 0;
            m_bIsOutro = true;
        }
    }

    void KilledUnit(Unit* pWho)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void SpellHitTarget(Unit *pTarget, const SpellEntry *spellProto)
    {
        if (spellProto->Id == SPELL_FLASH_FREEZE)
        {
            if (!pTarget->HasAura(SPELL_SAFE_AREA_BUFF, EFFECT_INDEX_0))
            {
                if (pTarget->GetTypeId() == TYPEID_PLAYER)
                {
                    if (pTarget->HasAura(SPELL_FLASH_FREEZE_DEBUFF, EFFECT_INDEX_0))
                        DoCastSpellIfCan(pTarget, SPELL_FLASH_FREEZE_KILL, true);
                    else
                        pTarget->CastSpell(pTarget, SPELL_FLASH_FREEZE_SUMMON, true);
                }
                if (pTarget->GetTypeId() == TYPEID_UNIT)
                {
                    if (pTarget->HasAura(SPELL_FLASH_FREEZE_NPC_STUN, EFFECT_INDEX_0))
                        DoCastSpellIfCan(pTarget, SPELL_FLASH_FREEZE_KILL, true);
                    else
                    {
                        for (uint8 i = 0; i < lHelperGUID.size(); i++)
                        {
                            if (pTarget->GetGUID() == lHelperGUID.at(i))
                            {
                                if (Map* pMap =  m_creature->GetMap())
                                {
                                    if (Creature* pFreezeMob = pMap->GetCreature(lflashfreezeMobGUID.at(i)))
                                    {
                                        if (!pFreezeMob->isAlive())
                                            pFreezeMob->Respawn();
                                        if(mob_npc_flashFreezeAI* pFreezeMobAI = (mob_npc_flashFreezeAI*) pFreezeMob->AI())
                                        {
                                            pFreezeMobAI->FreezeVictim(lHelperGUID.at(i));
                                        }
                                    }                
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bIsOutro)
        {
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;

            // reset if fighting only the npcs
            // this gets bugged if some of the npcs get top aggro
            if(m_creature->getVictim()->GetTypeId() != TYPEID_PLAYER)
                EnterEvadeMode();

            // hard mode check
            m_uiSpeedKillTimer += uiDiff;

            // Flash freeze
            if(m_uiFlashFreezeTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_FLASH_FREEZE, CAST_INTERRUPT_PREVIOUS) == CAST_OK)
                {
                    DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_ICICLE_FLASH_MARKS : SPELL_ICICLE_FLASH_MARKS_H, CAST_TRIGGERED);
                    DoScriptText(EMOTE_FLASH_FREEZE, m_creature);
                    DoScriptText(SAY_FLASH_FREEZE, m_creature);
                    m_uiFlashFreezeTimer = 50000;
                }
            }else m_uiFlashFreezeTimer -= uiDiff;

            // frozen blows
            if(m_uiFrozenBlowsTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FROZEN_BLOWS : SPELL_FROZEN_BLOWS_H) == CAST_OK)
                {
                    DoScriptText(SAY_FROZEN_BLOWS, m_creature);
                    DoScriptText(EMOTE_FROZEN_BLOWS, m_creature);
                    m_uiFrozenBlowsTimer = urand(50000, 60000);
                }
            }else m_uiFrozenBlowsTimer -= uiDiff;

            // freeze
            if(m_uiFreezeTimer < uiDiff)
            {
                if(Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_FREEZE);
                m_uiFreezeTimer = urand(5000, 10000);
            }else m_uiFreezeTimer -= uiDiff;

            // enrage
            if(m_uiEnrageTimer < uiDiff)
            {
                DoScriptText(SAY_BERSERK, m_creature);
                DoCast(m_creature, SPELL_ENRAGE);
                m_uiEnrageTimer = 30000;
            }else m_uiEnrageTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
        // outro
        if(m_bIsOutro)
        {
            switch(m_uiStep)
            {
            case 1:
                m_creature->setFaction(35);
                m_creature->RemoveAllAuras();
                m_creature->DeleteThreatList();
                m_creature->CombatStop(true);
                m_creature->InterruptNonMeleeSpells(false);
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->GetMotionMaster()->MovePoint(0, 1984.64f, -206.37f, 432.68f);
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            case 3:
                DoScriptText(SAY_DEATH, m_creature);
                ++m_uiStep;
                m_uiOutroTimer = 5000;
                break;
            case 5:
                DoOutro();
                ++m_uiStep;
                m_uiOutroTimer = 10000;
                break;
            }
        }
        else return;

        if (m_uiOutroTimer <= uiDiff)
        {
            ++m_uiStep;
            m_uiOutroTimer = 330000;
        }else m_uiOutroTimer -= uiDiff;
    }
};

// script for Icicles
struct MANGOS_DLL_DECL mob_icicleAI : public ScriptedAI
{
    mob_icicleAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();

        SetCombatMovement(false);
        m_uiSpellId = 0;
        m_uiActionTimer = 10000;

        switch (m_creature->GetEntry())
        {
            case NPC_ICICLE:
                m_creature->SetDisplayId(28470);
                m_uiActionTimer = 2000;
                m_uiSpellId = SPELL_ICICLE_DAMAGE_NORMAL;
                break;
            case NPC_SNOW_ICICLE:
                m_creature->SetDisplayId(28470);
                m_uiSpellId = SPELL_ICICLE_SNOW_DAMAGE;
                m_uiActionTimer = 3000;
                break;
            case NPC_SNOWDRIFT_TARGET:
                m_creature->SetDisplayId(15880); // invinsible
                DoCastSpellIfCan(m_creature, SPELL_SAFE_AREA_AURA, CAST_TRIGGERED);
                break;
            default:
                break;
        }
        m_creature->SetRespawnDelay(7*DAY*IN_MILLISECONDS);
    }

    instance_ulduar *m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiActionTimer;
    uint32 m_uiSpellId;
    float x, y, z;

    void Reset() {}
    void AttackStart(Unit* pWho) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiActionTimer <= uiDiff)
        {
            DoCastSpellIfCan(m_creature, m_uiSpellId, CAST_TRIGGERED);
            DoCastSpellIfCan(m_creature, SPELL_ICICLE_DUMMY);
            m_uiActionTimer = 30000;
        }else m_uiActionTimer -= uiDiff;
    }
};

// script for Flash freeze
struct MANGOS_DLL_DECL mob_flashFreezeAI : public ScriptedAI
{
    mob_flashFreezeAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        m_bIsFrozen = false;
        m_uiCheckTimer = 1000;
        DoCast(m_creature, SPELL_FLASH_FREEZE_VIS, true);
        Reset();
    }

    instance_ulduar *m_pInstance;
    bool m_bIsFrozen;
    uint32 m_uiCheckTimer;

    void Reset(){}
    void AttackStart(Unit* pWho){}

    void JustDied(Unit* Killer)
    {
        if (!m_pInstance)
            return;

        if (Unit* pVictim = m_pInstance->instance->GetUnit(m_creature->GetCreatorGuid()))
            pVictim->RemoveAurasDueToSpell(SPELL_FLASH_FREEZE_DEBUFF);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bIsFrozen && m_pInstance)
        {
            if (Unit* pVictim = m_pInstance->instance->GetUnit(m_creature->GetCreatorGuid()))
            {
                DoCastSpellIfCan(pVictim, SPELL_FLASH_FREEZE_DEBUFF, CAST_TRIGGERED);
                m_bIsFrozen = true;
            }
        }

        if (m_uiCheckTimer <= uiDiff)
        {
            if (Unit* pVictim = m_pInstance->instance->GetUnit(m_creature->GetCreatorGuid()))
            {
                if (!pVictim->isAlive())
                    m_creature->ForcedDespawn();
            }
            m_uiCheckTimer = 1000;
        }else m_uiCheckTimer -= uiDiff;
    }
};

// Helper npcs
struct MANGOS_DLL_DECL npc_hodir_helperAI : public ScriptedAI
{
    npc_hodir_helperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar *pInstance;

    bool m_bIsRegularMode;

    uint32 m_uiSpellTimer;
    uint32 m_uiCreatureEntry;

    void Reset()
    {
        m_uiSpellTimer = 5000;
        m_uiCreatureEntry = m_creature->GetEntry();
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            DoStartMovement(pWho, 20.0f);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < uiDiff)
        {
            switch (m_uiCreatureEntry)
            {
                case NPC_PRIEST:                     // Priest
                    switch(urand(0, 4))
                    {
                        case 0:
                            if(Creature *pHodir = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HODIR)))
                                DoCast(pHodir, SPELL_SMITE);
                            break;
                        case 1:
                        case 2:
                        case 3:
                            DoCast(m_creature, SPELL_GREAT_HEAL);
                            break;
                        case 4:
                            DoCast(m_creature, SPELL_DISPEL_MAGIC);
                            break;
                    }
                    break;
                case NPC_DRUID:                     // Druid
                    if (roll_chance_i(80))
                        if(Creature *pHodir = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HODIR)))
                            DoCast(pHodir, SPELL_WRATH);
                    else
                        DoCast(m_creature, SPELL_STARLIGHT);
                    break;
                case NPC_SHAMAN:                     // Shaman
                    if (roll_chance_i(70))
                    {
                        if(Creature *pHodir = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HODIR)))
                            DoCast(pHodir, SPELL_LAVA_BURST);
                    }
                    else
                    {
                        Unit *pTemp = pInstance->GetPlayerInMap(1,1);
                        if (pTemp && m_creature->GetDistance(pTemp) < 40)
                            DoCast(pTemp, m_bIsRegularMode ? SPELL_STORM_CLOUD : SPELL_STORM_CLOUD_H);
                    }
                    break;
                case NPC_MAGE:                     // Mage
                    switch(urand(0, 4))
                    {
                        case 0:
                            if(Creature *pHodir = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HODIR)))
                                DoCast(pHodir, SPELL_FIREBALL);
                            break;
                        case 1:
                        case 2:
                            if(Creature *pHodir = m_creature->GetMap()->GetCreature(pInstance->GetData64(NPC_HODIR)))
                                DoCast(pHodir, SPELL_SIGNED);
                            break;
                        case 3:
                            if(Creature *pTemp = GetClosestCreatureWithEntry(m_creature, NPC_FLASH_FREEZE, 50.0f))
                                DoCast(pTemp, SPELL_MELT_ICE);
                            break;
                        case 4:
                            DoCast(m_creature, SPELL_TOASTY_FIRE);
                            break;
                    }
                    break;
            }
            m_uiSpellTimer = urand(2000, 5000);
        }else m_uiSpellTimer -= uiDiff;
    }
};

// Toasty fire. Used by mage
struct MANGOS_DLL_DECL mob_toasty_fireAI : public ScriptedAI
{
    mob_toasty_fireAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        //pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    void Reset()
    {
        DoCast(m_creature, SPELL_TOASTY_FIRE_A);
    }

    void UpdateAI(const uint32 diff) {}
};

CreatureAI* GetAI_boss_hodir(Creature* pCreature)
{
    return new boss_hodirAI(pCreature);
}

CreatureAI* GetAI_mob_icicle(Creature* pCreature)
{
    return new mob_icicleAI(pCreature);
}

CreatureAI* GetAI_mob_flashFreeze(Creature* pCreature)
{
    return new mob_flashFreezeAI(pCreature);
}

CreatureAI* GetAI_npc_hodir_helper(Creature* pCreature)
{
    return new npc_hodir_helperAI(pCreature);
}

CreatureAI* GetAI_mob_toasty_fire(Creature* pCreature)
{
    return new mob_toasty_fireAI(pCreature);
}

CreatureAI* GetAI_mob_npc_flashFreeze(Creature* pCreature)
{
    return new mob_npc_flashFreezeAI(pCreature);
}

void AddSC_boss_hodir()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hodir";
    newscript->GetAI = &GetAI_boss_hodir;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flashFreeze";
    newscript->GetAI = &GetAI_mob_flashFreeze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_npc_flashFreeze";
    newscript->GetAI = &GetAI_mob_npc_flashFreeze;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_icicle";
    newscript->GetAI = &GetAI_mob_icicle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_toasty_fire";
    newscript->GetAI = &GetAI_mob_toasty_fire;
    newscript->RegisterSelf();

    /*newscript = new Script;
    newscript->Name = "npc_hodir_priest";
    newscript->GetAI = &GetAI_npc_hodir_priest;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_mage";
    newscript->GetAI = &GetAI_npc_hodir_mage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_hodir_druid";
    newscript->GetAI = &GetAI_npc_hodir_druid;
    newscript->RegisterSelf();
 
    newscript = new Script;
    newscript->Name = "npc_hodir_shaman";
    newscript->GetAI = &GetAI_npc_hodir_shaman;
    newscript->RegisterSelf();*/ 

    newscript = new Script;
    newscript->Name = "npc_hodir_helper";
    newscript->GetAI = &GetAI_npc_hodir_helper;
    newscript->RegisterSelf();
}
