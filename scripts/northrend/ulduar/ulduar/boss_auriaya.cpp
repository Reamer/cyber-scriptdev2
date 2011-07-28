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
SDName: boss_auriaya
SD%Complete:
SDComment: need correct setstack for feral defender buff
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_AGGRO       = -1603110,
    SAY_SLAY_1      = -1603112,
    SAY_SLAY_2      = -1603111,
    SAY_BERSERK     = -1603113,
    SAY_DEATH       = -1603114,
    EMOTE_SCREECH   = -1603115,
    EMOTE_DEFENDER  = -1603116,

    //auriaya
    SPELL_BERSERK               = 47008,
    SPELL_GUARDIAN_SWARM        = 64396,    // broken
    SPELL_SENTINEL_BLAST        = 64389,
    SPELL_SENTINEL_BLAST_H      = 64678,
    SPELL_SONIC_SCREECH         = 64422,
    SPELL_SONIC_SCREECH_H       = 64688,
    SPELL_FEAR                  = 64386,

    // summon Feral Defender
    SPELL_SUMMON_DEFENDER       = 64448,
    SPELL_SUMMON_DEFENDER_AURA  = 64449,
    NPC_FERAL_DEFENDER_STALKER  = 34096,

    //feral defender
    SPELL_FEIGN_DEATH           = 57685,
    SPELL_FERAL_ESSENCE         = 64455,
    SPELL_FERAL_POUNCE          = 64478,
    SPELL_FERAL_POUNCE_H        = 64669,
    SPELL_FERAL_RUSH            = 64496,
    SPELL_FERAL_RUSH_H          = 64674,

    //sanctum sentry
    SPELL_RIP_FLESH             = 64375,
    SPELL_RIP_FLESH_H           = 64667,
    SPELL_SAVAGE_POUNCE         = 64666,
    SPELL_SAVAGE_POUNCE_H       = 64374,
    SPELL_STRENGHT_OF_PACK      = 64369,
    SPELL_STRENGHT_OF_PACK_BUFF = 64381,
    //seeping feral essence
    SPELL_SUMMON_SEEPING_ESSENCE= 64457,
    AURA_VOID_ZONE              = 64458,
    AURA_VOID_ZONE_H            = 64676,
    //NPC ids
    MOB_VOID_ZONE               = 34098,
    MOB_FERAL_DEFENDER          = 34035,
    MOB_GUARDIAN_SWARN          = 34034,

    ACHIEV_CRAZY_CAT_LADY       = 3006,
    ACHIEV_CRAZY_CAT_LADY_H     = 3007,

    ACHIEV_NINE_LIVES           = 3076,
    ACHIEV_NINE_LIVES_H         = 3077,
};

bool m_bCrazyCatLady;
bool m_bNineLives;

// Seeping Feral Essence
struct MANGOS_DLL_DECL mob_seeping_feral_essenceAI : public ScriptedAI
{
    mob_seeping_feral_essenceAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        SetCombatMovement(false);
        pCreature->setFaction(14);
        pCreature->SetDisplayId(11686);     // make invisible
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature, m_bIsRegularMode ? AURA_VOID_ZONE : AURA_VOID_ZONE_H);
        m_creature->SetRespawnDelay(7*DAY);
    }

    void UpdateAI(const uint32 diff)
    {
    }
};

CreatureAI* GetAI_mob_seeping_feral_essence(Creature* pCreature)
{
    return new mob_seeping_feral_essenceAI(pCreature);
}

// Sanctum Sentry
struct MANGOS_DLL_DECL mob_sanctum_sentryAI : public ScriptedAI
{
    mob_sanctum_sentryAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsFollowing;

    uint32 m_uiRip_Flesh_Timer;
    uint32 m_uiJump_Timer;

    void Reset()
    {
        m_uiRip_Flesh_Timer = 13000;
        m_uiJump_Timer = 0;
        m_bIsFollowing = false;
    }

    void SpellHit(Unit *pCaster, const SpellEntry *spellInfo)
    {
        if (spellInfo->Id == SPELL_STRENGHT_OF_PACK_BUFF)
        {
            uint32 stack = 0;
            if (m_pInstance)
            {
                for (GUIDList::iterator itr = m_pInstance->m_lSanctumSentryGuids.begin(); itr != m_pInstance->m_lSanctumSentryGuids.end(); ++itr)
                {
                    if (Creature* pSanity = m_creature->GetMap()->GetCreature(*itr))
                    {
                        if (pSanity->isAlive() && (m_creature->GetDistance(pSanity) < 10.0f))
                            ++stack;
                    }
                }
            }

            if (SpellAuraHolder *holder = m_creature->GetSpellAuraHolder(SPELL_STRENGHT_OF_PACK_BUFF))
                holder->SetStackAmount(stack);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_AURIAYA))
            {
                if (pTemp->isAlive())
                    pTemp->SetInCombatWithZone();
            }

            for (GUIDList::iterator itr = m_pInstance->m_lSanctumSentryGuids.begin(); itr != m_pInstance->m_lSanctumSentryGuids.end(); ++itr)
            {
                if (Creature* pSanity = m_creature->GetMap()->GetCreature(*itr))
                {
                    if (pSanity->isAlive())
                        pSanity->SetInCombatWithZone();
                }
            }
        }


        DoCast(m_creature, SPELL_STRENGHT_OF_PACK);
    }

    void JustDied(Unit* pKiller)
    {
        m_bCrazyCatLady = false;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() != FOLLOW_MOTION_TYPE)
                if (m_pInstance)
                    if (Creature *pAuriaya = m_pInstance->GetSingleCreatureFromStorage(NPC_AURIAYA))
                        m_creature->GetMotionMaster()->MoveFollow(pAuriaya, 5.0f, urand(60, 250)/100.0f);
            return;
        }

        if (m_uiRip_Flesh_Timer < diff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_RIP_FLESH : SPELL_RIP_FLESH_H);
            m_uiRip_Flesh_Timer = 13000;
        }else m_uiRip_Flesh_Timer -= diff;

        if (m_uiJump_Timer < diff)
        {
            if (!m_creature->IsWithinDistInMap(m_creature->getVictim(), 8) && m_creature->IsWithinDistInMap(m_creature->getVictim(), 25))
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SAVAGE_POUNCE : SPELL_SAVAGE_POUNCE_H);
            m_uiJump_Timer = 1000;
        }else m_uiJump_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_sanctum_sentry(Creature* pCreature)
{
    return new mob_sanctum_sentryAI(pCreature);
}

// Feral Defender
struct MANGOS_DLL_DECL mob_feral_defenderAI : public ScriptedAI
{
    mob_feral_defenderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiPounce_Timer;
    uint32 m_uiRush_Start_Timer;
    uint32 m_uiRush_Finish_Timer;
    uint32 m_uiRush_Delay;
    uint32 m_uiRevive_Delay;

    bool m_bIsRush;
    bool m_bIsDead;

    bool m_bHasAura;

    GUIDList m_lVoidZones;

    void Reset()
    {
        m_uiPounce_Timer        = 5000;
        m_uiRush_Start_Timer    = 9000;
        m_bIsRush               = false;
        m_bIsDead               = false;
        m_bHasAura              = false;
        m_creature->SetRespawnDelay(DAY);

        if (!m_lVoidZones.empty())
        {
            if (m_pInstance)
            {
                for(GUIDList::iterator itr = m_lVoidZones.begin(); itr != m_lVoidZones.end(); itr++)
                {
                    if (Creature *pTmp = m_pInstance->instance->GetCreature(*itr))
                        pTmp->ForcedDespawn();
                }
            }
        }
        m_lVoidZones.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoCast(m_creature, SPELL_FERAL_ESSENCE);
    }

    void JustDied(Unit* pKiller)
    {
        m_bNineLives = true;
    }

    // feign death
    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_AURIAYA) == DONE)
            {
                Reset();
                m_creature->ForcedDespawn(1000);
                return;
            }
        }
        if (uiDamage > m_creature->GetHealth())
        {
            uiDamage = 0;
            m_creature->CastStop();
            m_creature->RemoveArenaAuras(true);
            //DoSpawnCreature(MOB_VOID_ZONE, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_DEAD_DESPAWN, 0))
            DoCast(m_creature, SPELL_SUMMON_SEEPING_ESSENCE, true);
            DoCast(m_creature, SPELL_FEIGN_DEATH, true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            if (m_creature->HasAura(SPELL_FERAL_ESSENCE))
            {
                // remove 1 stack of the aura
                if(SpellAuraHolder* strenght = m_creature->GetSpellAuraHolder(SPELL_FERAL_ESSENCE))
                {
                    if(strenght->ModStackAmount(-1))
                        m_creature->RemoveAurasDueToSpell(SPELL_FERAL_ESSENCE);
                }

                m_uiRevive_Delay = 35000;
                m_bIsDead = true;
            }
        }
    }

    void JustReachedHome()
    {
        m_creature->ForcedDespawn();
    }
    
    void JustSummoned(Creature *pCreature)
    {
        m_lVoidZones.push_back(pCreature->GetObjectGuid());
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_bIsDead)
        {
            if (m_uiRevive_Delay < diff)
            {
                m_creature->SetHealth(m_creature->GetMaxHealth());
                m_creature->RemoveAurasDueToSpell(SPELL_FEIGN_DEATH);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_bIsDead = false;
                m_uiPounce_Timer = 0;
                m_uiRush_Start_Timer = 9000;
            }else m_uiRevive_Delay -= diff;

            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // hacky way of stacking aura, needs fixing
        if (!m_bHasAura)
        {
            if(SpellAuraHolder* essence = m_creature->GetSpellAuraHolder(SPELL_FERAL_ESSENCE))
            {
                if(essence->GetStackAmount() < 9)
                {
                    m_bHasAura = true;
                    essence->SetStackAmount(9);
                }
            }
        }

        if (m_uiPounce_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_FERAL_POUNCE : SPELL_FERAL_POUNCE_H);
                m_creature->AI()->AttackStart(target);
            }
            m_uiPounce_Timer = 5000;
        }else m_uiPounce_Timer -= diff;

        if (m_uiRush_Start_Timer < diff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(target, m_bIsRegularMode ? SPELL_FERAL_RUSH : SPELL_FERAL_RUSH_H);
                m_creature->AI()->AttackStart(target);
            }
            m_uiRush_Start_Timer    = 35000;
            m_uiRush_Finish_Timer   = m_bIsRegularMode ? 2500 : 5000;
            m_uiRush_Delay          = 500;
            m_bIsRush               = true;
        }else m_uiRush_Start_Timer -= diff;

        if (m_bIsRush)
        {
            if (m_uiRush_Delay <= diff)
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    DoCast(target, m_bIsRegularMode ? SPELL_FERAL_RUSH : SPELL_FERAL_RUSH_H);
                    m_creature->AI()->AttackStart(target);
                }
                m_uiRush_Delay = 500;
            }else m_uiRush_Delay -= diff;
        }

        if (m_uiRush_Finish_Timer < diff)
            m_bIsRush = false;
        else m_uiRush_Finish_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_feral_defender(Creature* pCreature)
{
    return new mob_feral_defenderAI(pCreature);
}

// Auriaya
struct MANGOS_DLL_DECL boss_auriayaAI : public ScriptedAI
{
    boss_auriayaAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiEnrage_Timer;
    uint32 m_uiSwarm_Timer;
    uint32 m_uiSonic_Screech_Timer;
    uint32 m_uiSentinel_Blast_Timer;
    uint32 m_uiFear_Timer;
    uint32 m_uiSummon_Timer;
    uint8 m_uiSwarmcount;

    bool m_bHasBerserk;
    bool m_bIsDefender;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_uiEnrage_Timer        = 600000;
        m_uiSwarm_Timer         = urand(50000, 60000);
        m_uiSonic_Screech_Timer = 65000;
        m_uiSentinel_Blast_Timer= 42000;
        m_uiFear_Timer          = 40000;
        m_uiSummon_Timer        = 60000;
        m_uiSwarmcount          = 10;
        m_bHasBerserk           = false;
        m_bIsDefender           = false;

        // achievs
        m_bCrazyCatLady = true;
        m_bNineLives    = false;
        
        for (GUIDList::iterator itr = m_pInstance->m_lSanctumSentryGuids.begin(); itr != m_pInstance->m_lSanctumSentryGuids.end(); ++itr)
        {
            if (Creature* pSanity = m_creature->GetMap()->GetCreature(*itr))
            {
                if (!pSanity->isAlive())
                {
                    pSanity->Respawn();
                    pSanity->NearTeleportTo(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 0.0f);
                }
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        //death yell
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, DONE);

        // hacky way to complete achievements; use only if you have this function
        if (m_bCrazyCatLady)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_CRAZY_CAT_LADY : ACHIEV_CRAZY_CAT_LADY_H);
        }

        if (m_bNineLives)
        {
            if(m_pInstance)
                m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_NINE_LIVES : ACHIEV_NINE_LIVES_H);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, IN_PROGRESS);

        for (GUIDList::iterator itr = m_pInstance->m_lSanctumSentryGuids.begin(); itr != m_pInstance->m_lSanctumSentryGuids.end(); ++itr)
        {
            if (Creature* pSanity = m_creature->GetMap()->GetCreature(*itr))
            {
                if (pSanity->isAlive())
                    pSanity->SetInCombatWithZone();
            }
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_1 : SAY_SLAY_2, m_creature);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_AURIAYA, FAIL);
    }

    void JustSummoned(Creature *pCreature)
    {
        if (pCreature->GetEntry() == NPC_FERAL_DEFENDER_STALKER)
        {
            DoCast(pCreature, SPELL_SUMMON_DEFENDER_AURA);
            pCreature->ForcedDespawn(4000);
        }
        else if (pCreature->GetEntry() == MOB_FERAL_DEFENDER)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                pCreature->AI()->AttackStart(pTarget);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFear_Timer < uiDiff)
        {
            DoScriptText(EMOTE_SCREECH, m_creature);
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature, SPELL_FEAR);
            m_uiFear_Timer = 35000;
            m_uiSentinel_Blast_Timer = 2500;
            if (m_uiSonic_Screech_Timer < 4000)
                m_uiSonic_Screech_Timer = 4000;
        }else m_uiFear_Timer -= uiDiff;

        if (m_uiSentinel_Blast_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_SENTINEL_BLAST : SPELL_SENTINEL_BLAST_H);
            m_uiSentinel_Blast_Timer = urand(30000, 40000);
        }else m_uiSentinel_Blast_Timer -= uiDiff;

        if (!m_bIsDefender)
        {
            if (m_uiSummon_Timer < uiDiff)
            {
                m_creature->CastStop();
                //DoCast(m_creature, SPELL_SUMMON_DEFENDER, true);
                // should be summoned by spell, but creature can be summoned in not wanted position
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    m_creature->SummonCreature(NPC_FERAL_DEFENDER_STALKER, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 7*DAY*IN_MILLISECONDS);
                    DoScriptText(EMOTE_DEFENDER, m_creature);
                    m_bIsDefender = true;
                }
            }else m_uiSummon_Timer -= uiDiff;
        }

        if (m_uiSonic_Screech_Timer < uiDiff)
        {
            m_creature->InterruptNonMeleeSpells(true);
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SONIC_SCREECH : SPELL_SONIC_SCREECH_H);
            m_uiSonic_Screech_Timer = 27000;
        }else m_uiSonic_Screech_Timer -= uiDiff;

        // summon swarm, spell needs core fix
        if (m_uiSwarm_Timer < uiDiff)
        {
            for(int i = 0; i < 10; i++)
            {
                float angle = (float) rand()*360/RAND_MAX + 1;
                float homeX = m_creature->GetPositionX() + 10*cos(angle*(M_PI/180));
                float homeY = m_creature->GetPositionY() + 10*sin(angle*(M_PI/180));
                if (Creature* pTemp = m_creature->SummonCreature(MOB_GUARDIAN_SWARN, homeX, homeY, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoCast(pTarget, SPELL_GUARDIAN_SWARM);
                        pTemp->AddThreat(pTarget, 100.0f);
                        pTemp->SetInCombatWithZone();
                    }
                }
            }
            m_uiSwarm_Timer = 35000;
        }else m_uiSwarm_Timer -= uiDiff;

        if (m_uiEnrage_Timer < uiDiff && !m_bHasBerserk)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            m_creature->CastStop();
            DoCast(m_creature, SPELL_BERSERK);
            m_bHasBerserk = true;
        }else m_uiEnrage_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_auriaya(Creature* pCreature)
{
    return new boss_auriayaAI(pCreature);
}

void AddSC_boss_auriaya()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_auriaya";
    NewScript->GetAI = GetAI_boss_auriaya;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_seeping_feral_essence";
    NewScript->GetAI = &GetAI_mob_seeping_feral_essence;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_sanctum_sentry";
    NewScript->GetAI = &GetAI_mob_sanctum_sentry;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_feral_defender";
    NewScript->GetAI = &GetAI_mob_feral_defender;
    NewScript->RegisterSelf();
}