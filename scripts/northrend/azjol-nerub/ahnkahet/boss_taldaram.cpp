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
SDName: Boss_Taldaram
SD%Complete: 20%
SDComment:
SDCategory: Ahn'kahet
EndScriptData */

#include "precompiled.h"
#include "ahnkahet.h"

enum
{
    SAY_AGGRO                       = -1619008,
    SAY_VANISH_1                    = -1619009,
    SAY_VANISH_2                    = -1619010,
    SAY_FEED_1                      = -1619011,
    SAY_FEED_2                      = -1619012,
    SAY_SLAY_1                      = -1619013,
    SAY_SLAY_2                      = -1619014,
    SAY_SLAY_3                      = -1619015,
    SAY_DEATH                       = -1619016,

    SPELL_BEAM_VISUAL               = 60342,      // Used when taldram levitates before encounter
    SPELL_CONJURE_FLAME_ORB         = 55931,      // Dummy spell, dont do anything except cast
    SPELL_BLOODTHIRST               = 55968,
    SPELL_VANISH                    = 55964,      // Don't work, Creature goes outfight (hp boost)
    SPELL_EMBRACE_OF_THE_VAMPYR     = 55959,
    SPELL_EMBRACE_OF_THE_VAMPYR_H   = 59513,

    SPELL_FLAME_ORB_SPAWN_EFFECT    = 55891, // Orb Grow up
    SPELL_FLAME_ORB_VISUAL          = 55928, // Flame orb effect
    SPELL_FLAME_ORB_DEATH           = 55947, // Despawn effect
    SPELL_FLAME_ORB                 = 57750, // Flame orb damage    
    SPELL_FLAME_ORB_H               = 58937,

    SPELL_FLAME_SPHERE_PERIODIC     = 55926,
    SPELL_FLAME_SPHERE_VISUAL       = 55928,

    NPC_FLAME_ORB                   = 30702,

    FLAME_ORB_Z                     = 17,

    FLAME_ORB_UP_X                  = 383,
    FLAME_ORB_UP_Y                  = -984,

    FLAME_ORB_DOWN_X                = 632,
    FLAME_ORB_DOWN_Y                = -684,

    FLAME_ORB_RIGHT_X               = 350,
    FLAME_ORB_RIGHT_Y               = -705,

    FLAME_ORB_LEFT_X                = 613,
    FLAME_ORB_LEFT_Y                = -966,
};

/*######
## boss_taldaram
######*/

struct MANGOS_DLL_DECL boss_taldaramAI : public ScriptedAI
{
    boss_taldaramAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        damageToInterrupt = m_bIsRegularMode ? 20000 : 40000;
        Reset();
    }

    instance_ahnkahet* m_pInstance;
    bool m_bIsRegularMode;

    bool isInVanish;
    bool isInVampyrMode;

    uint32 summonFlameOrbTimer;
    uint32 vanishTimer;
    uint32 bloodthirstTimer;
    uint32 embraceOfTheVampyrTimer;
    uint32 embraceOfTheVampyrInterruptDamage;
    uint32 embraceOfTheVampyrFinishedTimer;
    uint32 damageToInterrupt;

    void Reset()
    {
        summonFlameOrbTimer = 12000;
        vanishTimer = 14000;
        bloodthirstTimer = 10000;
        embraceOfTheVampyrFinishedTimer = 20000;
        isInVanish = false;
        isInVampyrMode = false;
        m_creature->SetVisibility(VISIBILITY_ON);
    
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->RemoveAurasDueToSpell(SPELL_BEAM_VISUAL);
        
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, IN_PROGRESS);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SLAY_3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        m_creature->SetVisibility(VISIBILITY_ON);
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (isInVampyrMode)
            embraceOfTheVampyrInterruptDamage += uiDamage;

        if (embraceOfTheVampyrInterruptDamage > damageToInterrupt)
        {
            m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
            m_creature->InterruptNonMeleeSpells(false);    
            summonFlameOrbTimer = 4000;
            isInVampyrMode = false;
            embraceOfTheVampyrInterruptDamage = 0;
        }
        if(pDoneBy->GetEntry() == NPC_FLAME_ORB)
        {
            uiDamage = 0;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (pWho->IsWithinDist(m_creature, 10.0f, true))
            {
                if (m_creature->isAlive())
                {
                    if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                    {
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                    }
                    AttackStart(pWho);
                }
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!isInVanish && !isInVampyrMode)
        {
            // Summon Flame Orb
            if(summonFlameOrbTimer <= uiDiff)
            {
                for(int i = 0; i < 3; ++i)
                {
                    m_creature->SummonCreature(NPC_FLAME_ORB, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 7, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    if(m_bIsRegularMode)
                        break;
                }
                DoCast(m_creature, SPELL_CONJURE_FLAME_ORB);
                vanishTimer += 10000;
                summonFlameOrbTimer = 16000 + rand()%10000;
            }else summonFlameOrbTimer -= uiDiff;
            
            if (bloodthirstTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOODTHIRST) == CAST_OK)
                    bloodthirstTimer = m_bIsRegularMode ? 15000 : 8000;
            }else bloodthirstTimer -= uiDiff;
        }


        if (!isInVampyrMode)
            if(vanishTimer < uiDiff)
            {
                //m_creature->CastSpell(m_creature,SPELL_VANISH,true);
                m_creature->SetVisibility(VISIBILITY_OFF);
                isInVanish = true;
                embraceOfTheVampyrTimer = 2000;
                vanishTimer = 200000;
            }else vanishTimer -= uiDiff;
        
        if (isInVanish)
        {
            if (embraceOfTheVampyrTimer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                {
                    if (pTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        m_creature->SetVisibility(VISIBILITY_ON);
                        m_creature->NearTeleportTo(pTarget->GetPositionX() + 3.0f, pTarget->GetPositionY() + 3.0f, pTarget->GetPositionZ(), pTarget->GetOrientation());
                        m_creature->RemoveAurasDueToSpell(SPELL_VANISH);
                        DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_EMBRACE_OF_THE_VAMPYR : SPELL_EMBRACE_OF_THE_VAMPYR_H);
                        embraceOfTheVampyrInterruptDamage = 0;
                        isInVampyrMode = true;
                        summonFlameOrbTimer += 20000;
                        isInVanish = false;
                        vanishTimer = 25000;
                    }
                }
            }else embraceOfTheVampyrTimer -= uiDiff;
        }

        // set VampyrMode false when Spell not stopped by damage
        if(isInVampyrMode)
            if(embraceOfTheVampyrFinishedTimer < uiDiff)
            {
                isInVampyrMode = false;
                embraceOfTheVampyrFinishedTimer = 20000;
            } else embraceOfTheVampyrFinishedTimer -= uiDiff;
            
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_taldaram(Creature* pCreature)
{
    return new boss_taldaramAI(pCreature);
}

/*######
## mob_flame_orb
######*/

struct MANGOS_DLL_DECL mob_taldaram_flame_orbAI : public ScriptedAI
{
    mob_taldaram_flame_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ahnkahet*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ahnkahet* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bIsFlying;
    int8 direction;
    float m_fPosiZ;
    

    uint32 m_uiDespawn_Timer;
    uint32 m_uiCast_Timer;

    void Reset()
    {
        m_fPosiZ = m_creature->GetPositionZ();
        m_uiDespawn_Timer = 13000;
        m_uiCast_Timer = 3000;
        direction = -1;
        m_bIsFlying = false;
        //hack to set model invisible
        m_creature->SetDisplayId(10045);
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        DoCast(m_creature, SPELL_FLAME_ORB_VISUAL);
        DoCast(m_creature, SPELL_FLAME_ORB_SPAWN_EFFECT);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    void AttackStart(Unit* pWho)
    {
        return;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        // Despawn Timer
        if(m_uiDespawn_Timer <= uiDiff)
        {
            DoCast(m_creature, SPELL_FLAME_ORB_DEATH);
            m_creature->ForcedDespawn();
        }else m_uiDespawn_Timer -= uiDiff;

        // Fly timer
        if(m_uiCast_Timer <= uiDiff)
        {
            if(m_bIsFlying)
                return;

            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_ORB : SPELL_FLAME_ORB_H);
            direction = urand(0,3);
            switch(direction)
            {
                case 0: // Up
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_UP_X, FLAME_ORB_UP_Y, m_fPosiZ);
                    break;
                case 1: // Down
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_DOWN_X, FLAME_ORB_DOWN_Y, m_fPosiZ);
                    break;
                case 2: // Right
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_RIGHT_X, FLAME_ORB_RIGHT_Y, m_fPosiZ);
                    break;
                case 3: // Left
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_LEFT_X, FLAME_ORB_LEFT_Y, m_fPosiZ);
                    break;
                default:
                    m_creature->GetMotionMaster()->MovePoint(0, FLAME_ORB_UP_X, FLAME_ORB_UP_Y, m_fPosiZ);
                    break;

            }
            m_bIsFlying = true;
        }else m_uiCast_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_taldaram_flame_orb(Creature* pCreature)
{
    return new mob_taldaram_flame_orbAI(pCreature);
}


/*######
## go_nerubian_device
######*/

bool GOUse_go_nerubian_device(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = (instance_ahnkahet*)pGo->GetInstanceData();

    if (!pInstance)
        return false;

    pInstance->SetData(TYPE_TALDARAM, SPECIAL);
    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_UNK1);
    return false;
}

void AddSC_boss_taldaram()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_taldaram";
    newscript->GetAI = &GetAI_boss_taldaram;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_taldaram_flame_orb";
    newscript->GetAI = &GetAI_mob_taldaram_flame_orb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_nerubian_device";
    newscript->pGOUse = &GOUse_go_nerubian_device;
    newscript->RegisterSelf();
}
