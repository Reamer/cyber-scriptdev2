/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_xt002
SD%Complete:
SDComment: correct number of adds in 25man missing; Void Zone Damage; Heart Life;
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //xt yells
    SAY_AGGRO				= -1603038,
    SAY_DEATH				= -1603030,
    SAY_TANCTRUM			= -1603037,
    SAY_SLAY_01				= -1603036,
    SAY_SLAY_02				= -1603035,
    SAY_BERSERK				= -1603031,
    SAY_ADDS				= -1603032,
    SAY_HEART_OPEN			= -1603034,
    SAY_HEART_CLOSE			= -1603033,
    EMOTE_HEART             = -1603350,
    EMOTE_REPAIR            = -1603351,

    //xt-002
    SPELL_TANCTRUM			= 62776,
    SPELL_LIGHT_BOMB_TRIG   = 65598,
    SPELL_LIGHT_BOMB		= 63018,
    SPELL_LIGHT_BOMB_H		= 65121,
    SPELL_GRAVITY_BOMB		= 63024,
    SPELL_GRAVITY_BOMB_H	= 64234,
    SPELL_ENRAGE			= 47008,
    SPELL_STUN				= 3618,
    SPELL_ENERGY_ORB        = 62790,

    // hard mode
    SPELL_HEARTBREAK        = 65737,
    SPELL_HEARTBREAK_H      = 64193,
    SPELL_VOIDZONE          = 64203,
    SPELL_VOIDZONE_H        = 64235,
    SPELL_VOIDZONE_EFFECT   = 46264,
    SPELL_LIFE_SPARK        = 64210,
        
    // Life Spark
    SPELL_SHOCK             = 64230,
    SPELL_STATIC_CHARGED    = 64227,
    SPELL_STATIC_CHARGED_H  = 64236,

    // Aura Void Zone!?

    //heart of the deconstructor
    SPELL_EXPOSED_HEART		= 63849,

    //XS-013 Scrapbot
    SPELL_SCRAP_REPAIR      = 62832,
    
    //XE-321 Boombot
    SPELL_BOOM				= 38831,			// right spell 62834

    //XM-024 Pummeller
    SPELL_CLEAVE			= 8374,
    SPELL_TRAMPLE			= 5568,
    SPELL_UPPERCUT			= 10966,

    //NPC ids
    NPC_HEART				= 33329,
    NPC_SCRAPBOT			= 33343,
    NPC_BOOMBOT				= 33346,
    NPC_PUMMELER			= 33344,
    NPC_VOIDZONE            = 34001,
    NPC_LIFESPARK           = 34004,

    // Achievs
    ACHIEV_HEARTBREAKER         = 3058,
    ACHIEV_HEARTBREAKER_H       = 3059,
    ACHIEV_DECONSTRUCT_FAST     = 2937,
    ACHIEV_DECONSTRUCT_FAST_H   = 2938,
    ACHIEV_NERF_ENGINEERING     = 2931,
    ACHIEV_NERF_ENGINEERING_H   = 2932,
    ACHIEV_NERF_GRAVITY_BOMBS   = 2934,
    ACHIEV_NERF_GRAVITY_BOMBS_H = 2936,
    ACHIEV_NERF_SCRAPBOTS       = 2933,
    ACHIEV_NERF_SCRAPBOTS_H     = 2935,
};

//Positional defines 
struct LocationsXY
{
    float x, y, z, o;
    uint32 id;
};

static LocationsXY SummonLoc[]=
{
    {792.706f, 64.033f, 409.632f},  // lower left
    {879.750f, 64.815f, 409.804f},  // upper left
    {896.488f, -93.018f, 409.731f}, // upper right
    {791.016f, -83.516f, 409.804f}, // lower right
};

// void zone
struct MANGOS_DLL_DECL mob_voidzoneAI : public ScriptedAI
{
    mob_voidzoneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    uint32 Spell_Timer;
    bool m_bIsRegularMode;
    SpellEntry const *spellInfo;

    void Reset()
    {
        spellInfo = (SpellEntry*)GetSpellStore()->LookupEntry(m_bIsRegularMode ? SPELL_VOIDZONE : SPELL_VOIDZONE_H);
        Spell_Timer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
		// should be an aura here. Couldn't find it
		// hacky way, needs fixing!
        if (Spell_Timer < diff)
        {
            if (spellInfo)
            {
                int32 damage = spellInfo->CalculateSimpleValue(EFFECT_INDEX_1);
                m_creature->CastCustomSpell(m_creature, SPELL_VOIDZONE_EFFECT, &damage, 0, 0, true);
            }
            Spell_Timer = 1000;
        }else Spell_Timer -= diff;  
    }
};

CreatureAI* GetAI_mob_voidzone(Creature* pCreature)
{
    return new mob_voidzoneAI(pCreature);
}

// lifespark
struct MANGOS_DLL_DECL mob_lifesparkAI : public ScriptedAI
{
    mob_lifesparkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiSpell_Timer;

    void Reset()
    {  
        DoCast(m_creature, m_bIsRegularMode ? SPELL_STATIC_CHARGED : SPELL_STATIC_CHARGED_H);
        m_uiSpell_Timer = 1000;
    }

    void UpdateAI(const uint32 uidiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uidiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHOCK);
            m_uiSpell_Timer = urand(4000, 7000);
        } else m_uiSpell_Timer -= uidiff;
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lifespark(Creature* pCreature)
{
    return new mob_lifesparkAI(pCreature);
}

// XM-024 Pummeller
struct MANGOS_DLL_DECL mob_pummelerAI : public ScriptedAI
{
    mob_pummelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 Spell_Timer;

    void Reset()
    {        
        AttackStart(m_pInstance->GetPlayerInMap(true, false));
        Spell_Timer = urand(5000, 10000);
    }
    
    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Spell_Timer < diff)
        {
            switch(urand(0, 2))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                    break;
                case 1:
                    DoCast(m_creature->getVictim(), SPELL_TRAMPLE);
                    break;
                case 2:
                    DoCast(m_creature->getVictim(), SPELL_UPPERCUT);
                    break;
            }
            Spell_Timer = urand(5000, 10000);
        }else Spell_Timer -= diff;        

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_pummeler(Creature* pCreature)
{
    return new mob_pummelerAI(pCreature);
}

// XE-321 Boombot
struct MANGOS_DLL_DECL mob_boombotAI : public ScriptedAI
{
    mob_boombotAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {        
        AttackStart(m_pInstance->GetPlayerInMap(true, false));
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            DoCast(m_creature, SPELL_BOOM);
        }
    }

    void DoMeleeAttackIfReady()
    {
        //If we are within range melee the target
        if (m_creature->IsWithinDistInMap(m_creature->getVictim(), ATTACK_DISTANCE))
            DoCast(m_creature, SPELL_BOOM);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_boombot(Creature* pCreature)
{
    return new mob_boombotAI(pCreature);
}

// Heart of the Deconstructor
struct MANGOS_DLL_DECL mob_xtheartAI : public ScriptedAI
{
    mob_xtheartAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 m_uiDeathTimer;
    uint32 m_uiTotalDamage;

    void Reset()
    {
        DoCast(m_creature, SPELL_EXPOSED_HEART);
        m_creature->SetRespawnDelay(DAY);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_uiTotalDamage = 0;
        m_uiDeathTimer = 30000;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        m_uiTotalDamage += (uiDamage *2);
    }

    void JustDied(Unit* pKiller)
    {
        // used for hard mode loot only when hard mode loot is within the heart's corpse
		// remove this for revision
        m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
        if(m_pInstance)
            m_pInstance->SetData(TYPE_XT002_HARD, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        // despawns the creature if not killed in 30 secs
        if(m_uiDeathTimer < diff)
        {
            // pass damage to boss
            if (Creature* pTemp = m_creature->GetMap()->GetCreature( m_pInstance->GetData64(NPC_XT002)))
            {
                if (pTemp->isAlive())
                    pTemp->DealDamage(pTemp, m_uiTotalDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
            m_creature->ForcedDespawn();
        }
        else
            m_uiDeathTimer -= diff;
    }
};

CreatureAI* GetAI_mob_xtheart(Creature* pCreature)
{
    return new mob_xtheartAI(pCreature);
}

//XT-002 Deconstructor
struct MANGOS_DLL_DECL boss_xt_002AI : public ScriptedAI
{
    boss_xt_002AI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPhase;

    // spell timers
    uint32 m_uiHeart_Timer;
    uint32 m_uiLight_Bomb_Timer;
    uint32 m_uiGravity_Bomb_Timer;
    uint32 m_uiTanctrum_Timer;
    uint32 m_uiEnrage_Timer;
    uint32 m_uiRange_Check_Timer;
    uint32 m_uiVoidZoneTimer;
    uint32 m_uiLifeSparkTimer;

    // Summon
    uint32 m_uiSummonTimer;
    bool   m_bSayAdds;
    std::list<uint64> m_lScrapbotsGUIDList;
    std::list<uint64> m_lBoombotsGUIDList;
    std::list<uint64> m_lPummelerGUIDList;

    // health timers
    uint32 m_uiHealthPercent;
    uint32 m_uiHpDelayTimer;
    bool m_bIsEnrage;

    uint64 pLightBombTarGUID;
    uint64 pGravityBombTarGUID;
    uint64 m_uiXtHeartGUID;

    bool m_bIsHardMode;
    bool m_bHasMoreHealth;

    uint32 uiEncounterTimer;
    bool m_bIsEngineer;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_uiPhase               = 1;
        // spell timers
        m_uiLight_Bomb_Timer    = 5000;     // 7 seconds the first 14 secs all after(7 secs in 25man)
        m_uiGravity_Bomb_Timer  = 30000;    // 11 seconds first 18 secs all after(11 secs in 25man)
        m_uiTanctrum_Timer      = 35000;    // 38 seconds first 40 secs all after
        m_uiEnrage_Timer        = 600000;   // 10 min
        m_uiRange_Check_Timer   = 1000;
        m_uiVoidZoneTimer       = 60000;
        m_uiLifeSparkTimer      = 60000;

        // Summon
        m_uiSummonTimer         = 5000;
        m_bSayAdds              = false;
        m_lScrapbotsGUIDList.clear();
        m_lBoombotsGUIDList.clear();
        m_lPummelerGUIDList.clear();

        // health timers
        m_uiHealthPercent       = 75;


        m_bIsEnrage             = false;
        m_bIsHardMode           = false;
        m_bHasMoreHealth        = false;


        pLightBombTarGUID       = 0;
        pGravityBombTarGUID     = 0;
        m_uiXtHeartGUID         = 0;

        uiEncounterTimer        = 0;
        m_bIsEngineer           = true;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, DONE);
            if(m_bIsHardMode)
            {
                m_pInstance->SetData(TYPE_XT002_HARD, DONE);
                // make the heart give the loot for hard mode
				// hacky way of giving hard mode loot, used only when hard mode loot is within the heart's corpse
				// PLEASE REMOVE FOR REVISION!
                if(Creature* pHeart = m_pInstance->instance->GetCreature(m_uiXtHeartGUID))
                    pHeart->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }

        DoScriptText(SAY_DEATH, m_creature);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        if (!m_lScrapbotsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lScrapbotsGUIDList.begin(); itr != m_lScrapbotsGUIDList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->ForcedDespawn();
        }
        if (!m_lBoombotsGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lBoombotsGUIDList.begin(); itr != m_lBoombotsGUIDList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->ForcedDespawn();
        }
        if (!m_lPummelerGUIDList.empty())
        {
            for(std::list<uint64>::iterator itr = m_lPummelerGUIDList.begin(); itr != m_lPummelerGUIDList.end(); ++itr)
                if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    pTemp->ForcedDespawn();
        }

        // hacky way to complete achievements; use only if you have this function
        // Deconstruct Fast
        if (uiEncounterTimer < 205000)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_DECONSTRUCT_FAST : ACHIEV_DECONSTRUCT_FAST_H);
        }

        // Heartbreaker
        if (m_bIsHardMode)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_HEARTBREAKER : ACHIEV_HEARTBREAKER_H);
        }

        // Nerf Engineer
        if(m_bIsEngineer)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_NERF_ENGINEERING : ACHIEV_NERF_ENGINEERING_H);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, IN_PROGRESS);
            if(m_pInstance->GetData(TYPE_XT002_TP) != DONE)
                m_pInstance->SetData(TYPE_XT002_TP, DONE);
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, FAIL);
            m_pInstance->SetData(TYPE_XT002_HARD, NOT_STARTED);
        }

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 1))
        {
            case 0: DoScriptText(SAY_SLAY_01, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_02, m_creature); break;
        }
    }

    void SummonAdds()
    {
        if (m_bSayAdds == false)
        {
            DoScriptText(SAY_ADDS, m_creature);
            m_bSayAdds = true;
        }
        
        for (int j = 0; j < (m_bIsRegularMode ? 3 : 5); ++j)
        {
            uint32 addentry = 0;
            switch(urand(0,2))
            {
                case 0: addentry = NPC_SCRAPBOT; break;
                case 1: addentry = NPC_BOOMBOT;  break;
                case 2: addentry = NPC_PUMMELER; break;
                default:addentry = NPC_SCRAPBOT;
            }
            uint8 i = urand(0, 4);
            if (Creature* pTemp = m_creature->SummonCreature(addentry, SummonLoc[i].x + urand(0, 10), SummonLoc[i].y + urand(0, 10), SummonLoc[i].z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
            {
                DoCast(pTemp, SPELL_ENERGY_ORB, true);
                switch (addentry)
                {
                    case NPC_SCRAPBOT:
                        pTemp->GetMotionMaster()->MoveFollow(m_creature, 0.0f, 0.0f);
                        m_lScrapbotsGUIDList.push_back(pTemp->GetGUID());
                        break;
                    case NPC_BOOMBOT:
                        m_lBoombotsGUIDList.push_back(pTemp->GetGUID());
                        break;
                    case NPC_PUMMELER:
                        m_lPummelerGUIDList.push_back(pTemp->GetGUID());
                        break;
                }
            }
        }
    }

    void SwitchIntoPhaseOne()
    {
        m_uiPhase = 1;
        DoScriptText(SAY_HEART_CLOSE, m_creature);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveAurasDueToSpell(SPELL_STUN);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        AttackStart(m_creature->getVictim());
        
        m_uiLight_Bomb_Timer = 7000;
        m_uiGravity_Bomb_Timer = 11000;
        m_uiTanctrum_Timer = 38000;

    }

    void UpdateAI(const uint32 uiDiff)
    {
        // both phases
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // enrage timer
        if (m_uiEnrage_Timer < uiDiff && !m_bIsEnrage)
        {
            DoCast(m_creature, SPELL_ENRAGE, true);
            if (m_creature->HasAura(SPELL_ENRAGE))
            {
                m_bIsEnrage = true;
                DoScriptText(SAY_BERSERK, m_creature);
            }
            else
                m_uiEnrage_Timer = 5000;
        }else m_uiEnrage_Timer -= uiDiff;

        // Achiev timer
        uiEncounterTimer += uiDiff;

        if (m_uiPhase == 1)
        {
            // light bomb
            if (m_uiLight_Bomb_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    // fix spell range
                    DoCast(pTarget, m_bIsRegularMode ? SPELL_LIGHT_BOMB : SPELL_LIGHT_BOMB_H, true);
                    pLightBombTarGUID = pTarget->GetGUID();
                }

			    // spawn a life spark from the target
                if(m_bIsHardMode)
                    m_uiLifeSparkTimer = 9000;

                m_uiLight_Bomb_Timer = urand(10000, 14000);
            }else m_uiLight_Bomb_Timer -= uiDiff;

            // graviti bomb
            if (m_uiGravity_Bomb_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    // fix spell range
                    m_creature->CastSpell(pTarget, m_bIsRegularMode ? SPELL_GRAVITY_BOMB : SPELL_GRAVITY_BOMB_H, true);
                    pGravityBombTarGUID = pTarget->GetGUID();
                }

			    // spawn a void zone from the target
                if(m_bIsHardMode)
                    m_uiVoidZoneTimer = 9000;

                m_uiGravity_Bomb_Timer = urand(25000, 30000); 
            }else m_uiGravity_Bomb_Timer -= uiDiff;

            if (m_uiTanctrum_Timer < uiDiff)
            {
                DoCast(m_creature, SPELL_TANCTRUM);
                DoScriptText(SAY_TANCTRUM, m_creature);
                m_uiTanctrum_Timer = 60000;
            }else m_uiTanctrum_Timer -= uiDiff;

            if(m_bIsHardMode)
            {
			    // the spell doesn't increase the boss' heart. Override
                if(m_uiHpDelayTimer < uiDiff && m_bHasMoreHealth)
                {
                    m_creature->SetHealth(m_creature->GetMaxHealth());
                    m_bHasMoreHealth = false;
                }else m_uiHpDelayTimer -= uiDiff;

                /*if (m_uiLifeSparkTimer < uiDiff)              // Nun im Core
                {
                    if (Unit* pTarget = m_creature->GetMap()->GetUnit( pLightBombTarGUID))
                        DoCast(pTarget, SPELL_LIFE_SPARK, true);
                    m_uiLifeSparkTimer = 60000;
                }else m_uiLifeSparkTimer -= uiDiff;

                if (m_uiVoidZoneTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->GetMap()->GetUnit( pGravityBombTarGUID))
                        DoCast(pTarget,m_bIsRegularMode ? SPELL_VOIDZONE : SPELL_VOIDZONE_H, true);     
                    m_uiVoidZoneTimer = 60000;
                }else m_uiVoidZoneTimer -= uiDiff;*/ 
            }
            if (m_creature->GetHealthPercent() < m_uiHealthPercent && !m_bIsHardMode)
            {
                m_uiHeart_Timer = 30000;
                m_creature->CastStop();
                m_uiHealthPercent = m_uiHealthPercent - 25;
                m_uiPhase = 2;
                DoScriptText(SAY_HEART_OPEN, m_creature);
                DoCast(m_creature, SPELL_STUN);
                DoScriptText(EMOTE_HEART, m_creature);
                m_creature->SetStandState(UNIT_STAND_STATE_KNEEL);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                // timers
                m_uiSummonTimer         = 5000;
                m_bSayAdds              = false;

                if(Creature *Heart = m_creature->SummonCreature(NPC_HEART, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 900000))
                    m_uiXtHeartGUID = Heart->GetGUID();
            }

            DoMeleeAttackIfReady();

        }
        else if(m_uiPhase == 2)
        {
            if (m_uiSummonTimer < uiDiff)
            {
                SummonAdds();
                m_uiSummonTimer = urand(3000, 5000);
            }else m_uiSummonTimer -= uiDiff;

            // Switch into Phase 1
            if (m_uiHeart_Timer < uiDiff)
            {
                SwitchIntoPhaseOne();
            }else m_uiHeart_Timer -= uiDiff;

            if (m_pInstance->GetData(TYPE_XT002_HARD) == IN_PROGRESS)
            {
                SwitchIntoPhaseOne();
                DoCast(m_creature, m_bIsRegularMode ? SPELL_HEARTBREAK : SPELL_HEARTBREAK_H);
                m_uiHpDelayTimer = 500;
                m_bHasMoreHealth = true;
                m_bIsHardMode = true;
            }
        }

        // adds range check
        if (m_uiRange_Check_Timer < uiDiff)
        {
            if (!m_lScrapbotsGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lScrapbotsGUIDList.begin(); itr != m_lScrapbotsGUIDList.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    {
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            DoCast(pTemp, SPELL_SCRAP_REPAIR, true);
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                            m_bIsEngineer = false;
                            DoScriptText(EMOTE_REPAIR, m_creature);
                        }
                    }
                }
            }
            m_uiRange_Check_Timer = 1000;
        }else m_uiRange_Check_Timer -= uiDiff;

    }
};

CreatureAI* GetAI_boss_xt_002(Creature* pCreature)
{
    return new boss_xt_002AI(pCreature);
}

void AddSC_boss_xt002()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_xt_002";
    NewScript->GetAI = GetAI_boss_xt_002;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_pummeler";
    NewScript->GetAI = &GetAI_mob_pummeler;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_boombot";
    NewScript->GetAI = &GetAI_mob_boombot;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_xtheart";
    NewScript->GetAI = &GetAI_mob_xtheart;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_voidzone";
    NewScript->GetAI = &GetAI_mob_voidzone;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_lifespark";
    NewScript->GetAI = &GetAI_mob_lifespark;
    NewScript->RegisterSelf();
}