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
SDName: Boss_Thaddius
SD%Complete: 90
SDComment: Placeholder. Includes Feugen & Stalagg.
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    // Stalagg
    SAY_STAL_AGGRO                = -1533023,
    SAY_STAL_SLAY                 = -1533024,
    SAY_STAL_DEATH                = -1533025,

    SPELL_POWERSURGE              = 54529,
    SPELL_POWERSURGE_H            = 28134,

    //Feugen
    SAY_FEUG_AGGRO                = -1533026,
    SAY_FEUG_SLAY                 = -1533027,
    SAY_FEUG_DEATH                = -1533028,

    SPELL_STATIC_FIELD            = 28135,
    SPELL_STATIC_FIELD_H          = 54528,

    //both
    SPELL_WARSTOMP                = 28125,

    //Thaddus
    SAY_GREET                     = -1533029,
    SAY_AGGRO1                    = -1533030,
    SAY_AGGRO2                    = -1533031,
    SAY_AGGRO3                    = -1533032,
    SAY_SLAY                      = -1533033,
    SAY_ELECT                     = -1533034,
    SAY_DEATH                     = -1533035,
    SAY_SCREAM1                   = -1533036,
    SAY_SCREAM2                   = -1533037,
    SAY_SCREAM3                   = -1533038,
    SAY_SCREAM4                   = -1533039,

    SPELL_BALL_LIGHTNING          = 28299,

    SPELL_POLARITYSHIFT           = 28089,

    SPELL_CHAIN_LIGHTNING         = 28167,
    SPELL_CHAIN_LIGHTNING_H       = 54531,

    SPELL_BESERK                  = 26662,

    //generic
    C_TESLA_COIL                  = 16218                   //the coils (emotes "Tesla Coil overloads!")
};

static const float StalaggPos[] = {3449.0613f, -2933.2119f, 312.0044f};
static const float  FeugenPos[] = {3506.7844f, -2989.0639f, 312.0093f};

struct MANGOS_DLL_DECL boss_thaddiusAI: public ScriptedAI
{
    boss_thaddiusAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool    m_bIsRegularMode;

    bool    m_bCanAggro;
    bool    m_bOneIsDeath;
    uint32  m_uiPreBossTimer;


    uint32  m_uiPolarityShiftTimer;
    uint32  m_uiChainLightningTimer;
    uint32  m_uiBerserkTimer;
    uint32  m_uiBallLightningTimer;


    void Reset()
    {
        PreBossRespawn();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_STUNNED);
        
        m_bCanAggro      = false;
        m_bOneIsDeath    = false;
        m_uiPreBossTimer = 5000;

        m_uiPolarityShiftTimer  = 10000;
        m_uiChainLightningTimer = 12000+urand(0,5000);
        m_uiBerserkTimer        = 365000;
        m_uiBallLightningTimer  = 3000;

        if (m_pInstance)
            m_pInstance->SetAchiev(TYPE_THADDIUS, false);
    }

    void PreBossDeath()
    {
        if(m_bOneIsDeath)
        {
            DoScriptText(SAY_GREET,m_creature);
            m_bCanAggro      = true;
            m_uiPreBossTimer = 15000 + urand(0,5000);
        }
        else
        {
            m_bOneIsDeath = true;
            m_uiPreBossTimer = 5000;
        }
    }

    void PreBossRespawn()
    {
        if(Creature* Stalagg = m_pInstance->GetSingleCreatureFromStorage(NPC_STALAGG))
            if(Stalagg->isDead())
                Stalagg->Respawn();
        if(Creature* Feugen = m_pInstance->GetSingleCreatureFromStorage(NPC_FEUGEN))
            if(Feugen->isDead())
                Feugen->Respawn();
    }

    void Aggro(Unit* pVictim)
    {
        switch(urand(0,2))
        {
            case 0:  DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1:  DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2:  DoScriptText(SAY_AGGRO3, m_creature); break;
            default: break;
        }
        if (m_pInstance)
        {
            m_pInstance->SetAchiev(TYPE_THADDIUS, true);
            m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_THADDIUS, FAIL);
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_THADDIUS, DONE);
            m_pInstance->SetData(TYPE_STALAGG, DONE);
            m_pInstance->SetData(TYPE_FEUGEN, DONE);
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if(urand(0,1))
            DoScriptText(SAY_SLAY,m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_bCanAggro)
        {
            if(m_bOneIsDeath)
            {
                if(m_uiPreBossTimer <= diff)
                {
                    PreBossRespawn();
                    m_bOneIsDeath = false;
                } else m_uiPreBossTimer -= diff;
            }
        }
        else
        {
            if(m_bOneIsDeath)
            {
                if(m_uiPreBossTimer <= diff)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_STUNNED);
                    m_bOneIsDeath = false;
                }
                else m_uiPreBossTimer -= diff;
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer <= diff)
            {
                DoCast(m_creature, SPELL_BESERK);
                m_uiBerserkTimer = 0;
            }else m_uiBerserkTimer -= diff;
        } 

        if(m_uiPolarityShiftTimer <= diff)
        {
            if (DoCastSpellIfCan(m_creature,SPELL_POLARITYSHIFT) == CAST_OK)
            {
                DoScriptText(SAY_ELECT,m_creature);
                m_uiPolarityShiftTimer = 30000;
            }
        } else m_uiPolarityShiftTimer -= diff;

        if(m_uiChainLightningTimer <= diff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                DoCast(pTarget,m_bIsRegularMode?SPELL_CHAIN_LIGHTNING:SPELL_CHAIN_LIGHTNING_H);
            m_uiChainLightningTimer = 12000+urand(0,5000);
        }else m_uiChainLightningTimer -= diff;

        if(!m_creature->getVictim()->IsWithinDistInMap(m_creature,ATTACK_DISTANCE))
        {
            if(m_uiBallLightningTimer <= diff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCast(pTarget,SPELL_BALL_LIGHTNING);
                m_uiBallLightningTimer = 2500;
            }else m_uiBallLightningTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_thaddius(Creature* pCreature)
{
    return new boss_thaddiusAI(pCreature);
}

struct MANGOS_DLL_DECL mob_stalaggAI: public ScriptedAI
{
    mob_stalaggAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    
    uint32  m_uiPullTimer;
    uint32  m_uiPowerSurgeTimer;
    uint32  m_uiEnrageCastTimer;
    uint32  m_uiCheckAggroTimer;
    uint32  m_uiWarStompTimer;

    ObjectGuid StalaggTelsa;

    bool    m_bJustEnraged;
    bool    m_bIsFeugenClose;
    bool    m_bJustSpawnedVisual;

    

    void Reset()
    {
        if (Creature* pTelsa = m_creature->GetMap()->GetCreature(StalaggTelsa))
            pTelsa->ForcedDespawn();

        m_bJustSpawnedVisual= false;

        m_uiPullTimer       = 30000;
        m_uiPowerSurgeTimer = 10000;
        m_bJustEnraged      = false;
        m_bIsFeugenClose    = false;
        m_uiEnrageCastTimer = 1000;

        m_uiWarStompTimer   = 10000+urand(0,5000);

        m_uiCheckAggroTimer = 5000;
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
        {
            if (Creature* pThaddius = m_pInstance->GetSingleCreatureFromStorage(NPC_THADDIUS))
            {
                if (boss_thaddiusAI* pThaddiusAI = (boss_thaddiusAI*)pThaddius->AI())
                {
                    pThaddiusAI->PreBossDeath();
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_bJustSpawnedVisual)
        {
            if(GameObject* NearTeslaCoil = GetClosestGameObjectWithEntry(m_creature,181478,100.0f))
            {
                float gx,gy,gz;
                NearTeslaCoil->GetPosition(gx,gy,gz);
                if(Creature* TeslaVisual = m_creature->SummonCreature(C_TESLA_COIL,gx,gy,gz,0.0f,TEMPSUMMON_DEAD_DESPAWN,0))
                {
                    TeslaVisual->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PASSIVE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
                    TeslaVisual->GetMotionMaster()->MovePoint(0,gx,gy,gz);
                    TeslaVisual->CastSpell(m_creature,45537,true);
                    StalaggTelsa = TeslaVisual->GetObjectGuid();
                    m_bJustSpawnedVisual = true;
                }
            }
        }

        if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if(Creature* Feugen = m_pInstance->GetSingleCreatureFromStorage(NPC_FEUGEN))
        {
            if(Feugen->IsWithinDist(m_creature,20.0f))
                m_bIsFeugenClose = true;
            else
                m_bIsFeugenClose = false;
        }

        if( (m_creature->GetPositionZ() < 309.58f) || m_bIsFeugenClose)
        {
            if(!m_bJustEnraged)
            {
                m_bJustEnraged = true;
            }
            else
            {
                if(m_uiEnrageCastTimer <= diff)
                {
                    m_creature->CastSpell(m_creature->getVictim(),17364,true);
                    m_uiEnrageCastTimer = 500;
                } else m_uiEnrageCastTimer -= diff;
            }
        }
        else
            m_bJustEnraged = false;

        if(m_uiWarStompTimer <= diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP,true);
            m_uiWarStompTimer = 10000+urand(0,5000);
        } else m_uiWarStompTimer -= diff;

        if(m_uiCheckAggroTimer)
        {
            if(m_uiCheckAggroTimer <= diff)
            {                
                if(Creature* Feugen = m_pInstance->GetSingleCreatureFromStorage(NPC_FEUGEN))
                {
                    if(!Feugen->getVictim())
                    {
                        Feugen->AI()->AttackStart(m_creature->getVictim());
                        Feugen->AddThreat(m_creature->getVictim(),1000.0f);
                    }
                }
                m_uiCheckAggroTimer = 0;
            } else m_uiCheckAggroTimer -= diff;
        }

        if(m_uiPowerSurgeTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode? SPELL_POWERSURGE : SPELL_POWERSURGE_H,true);
            m_uiPowerSurgeTimer = 15000+urand(0,5000);
        } else m_uiPowerSurgeTimer -= diff;

        if(m_uiPullTimer <= diff)
        {
            if(Unit* StalaggVictim = m_creature->getVictim())
            {
                float ThreatStalaggVictim = m_creature->getThreatManager().getThreat(StalaggVictim);
                if(Creature* Feugen = m_pInstance->GetSingleCreatureFromStorage(NPC_FEUGEN))
                {
                    if (Unit* FeugenVictim = Feugen->getVictim())
                    {
                        float ThreatFeugenVictim = Feugen->getThreatManager().getThreat(FeugenVictim);
                        //swap
                        StalaggVictim->NearTeleportTo(FeugenPos[0],FeugenPos[1],FeugenPos[2],0.0f);
                        Feugen->AddThreat(StalaggVictim, ThreatFeugenVictim);
                        Feugen->getThreatManager().modifyThreatPercent(FeugenVictim, -100);
                        FeugenVictim->NearTeleportTo(StalaggPos[0],StalaggPos[1],StalaggPos[2], 0.0f);
                        m_creature->AddThreat(FeugenVictim, ThreatStalaggVictim);
                        m_creature->getThreatManager().modifyThreatPercent(StalaggVictim, -100);

                    }
                }
            }
            m_uiPullTimer = 30000;
        } else m_uiPullTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_stalagg(Creature* pCreature)
{
    return new mob_stalaggAI(pCreature);
}

struct MANGOS_DLL_DECL mob_feugenAI: public ScriptedAI
{
    mob_feugenAI(Creature* pCreature): ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool    m_bIsRegularMode;

    bool    m_bJustSpawnedVisual;
    bool    m_bJustEnraged;
    bool    m_bIsStalaggClose;

    
    uint32  m_uiStaticFieldTimer;
    uint32  m_uiEnrageCastTimer;
    uint32  m_uiWarStompTimer;
    uint32  m_uiCheckAggroTimer;

    ObjectGuid FeugenTelsa;

    void Reset()
    {
        if (Creature* pTemp = m_creature->GetMap()->GetCreature(FeugenTelsa))
            pTemp->ForcedDespawn();

        m_bJustSpawnedVisual= false;
        m_bJustEnraged      = false;
        m_bIsStalaggClose   = false;

        m_uiStaticFieldTimer= 3000;
        m_uiEnrageCastTimer = 1000;
        m_uiWarStompTimer   = 10000+urand(0,5000);
        m_uiCheckAggroTimer = 5000;
    }

    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
        {
            if (Creature* pThaddius = m_pInstance->GetSingleCreatureFromStorage(NPC_THADDIUS))
            {
                if (boss_thaddiusAI* pThaddiusAI = (boss_thaddiusAI*)pThaddius->AI())
                {
                    pThaddiusAI->PreBossDeath();
                }
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!m_bJustSpawnedVisual)
        {
            if(GameObject* NearTeslaCoil = GetClosestGameObjectWithEntry(m_creature,181477,100.0f))
            {
                float gx,gy,gz;
                NearTeslaCoil->GetPosition(gx,gy,gz);
                if(Creature* TeslaVisual = m_creature->SummonCreature(C_TESLA_COIL,gx,gy,gz,0.0f,TEMPSUMMON_DEAD_DESPAWN,0))
                {
                    TeslaVisual->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_PASSIVE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_NON_ATTACKABLE);
                    TeslaVisual->GetMotionMaster()->MovePoint(0,gx,gy,gz);
                    TeslaVisual->CastSpell(m_creature,45537,true);
                    FeugenTelsa = TeslaVisual->GetObjectGuid();
                    m_bJustSpawnedVisual = true;
                }
            }
        }

        if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
            return;

        if(Creature* Stalagg = m_pInstance->GetSingleCreatureFromStorage(NPC_STALAGG))
        {
            if(Stalagg->IsWithinDist(m_creature,20.0f))
                m_bIsStalaggClose = true;
            else
                m_bIsStalaggClose = false;
        }

        if(m_creature->GetPositionZ() < 309.58f || m_bIsStalaggClose)
        {
            if(!m_bJustEnraged)
            {
                m_bJustEnraged = true;
            } else
            {
                if(m_uiEnrageCastTimer <= diff)
                {
                    m_creature->CastSpell(m_creature->getVictim(),17364,true);
                    m_uiEnrageCastTimer = 500;
                } else m_uiEnrageCastTimer -= diff;
            }
        } else
            m_bJustEnraged = false;

        if(m_uiCheckAggroTimer)
        {
            if(m_uiCheckAggroTimer <= diff)
            {
                if(Creature* Stalagg = m_pInstance->GetSingleCreatureFromStorage(NPC_STALAGG))
                {
                    if(!Stalagg->getVictim())
                    {
                        Stalagg->AI()->AttackStart(m_creature->getVictim());
                        Stalagg->AddThreat(m_creature->getVictim(),1000.0f);
                    }
                }
                m_uiCheckAggroTimer = 0;
            } else m_uiCheckAggroTimer -= diff;
        }

        if(m_uiWarStompTimer <= diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP,true);
            m_uiWarStompTimer = 10000+urand(0,5000);
        } else m_uiWarStompTimer -= diff;

        if(m_uiStaticFieldTimer <= diff)
        {
            DoCast(m_creature,m_bIsRegularMode ? SPELL_STATIC_FIELD : SPELL_STATIC_FIELD_H,true);
            m_uiStaticFieldTimer = 3000;
        } else m_uiStaticFieldTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_feugen(Creature* pCreature)
{
    return new mob_feugenAI(pCreature);
}

void AddSC_boss_thaddius()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "boss_thaddius";
    newscript->GetAI = &GetAI_boss_thaddius;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_stalagg";
    newscript->GetAI = &GetAI_mob_stalagg;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_feugen";
    newscript->GetAI = &GetAI_mob_feugen;
    newscript->RegisterSelf();
}
