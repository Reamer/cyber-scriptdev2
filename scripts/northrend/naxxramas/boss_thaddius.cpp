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

    SPELL_CHARGE_POSITIVE_DMGBUFF = 29659,
    SPELL_CHARGE_POSITIVE_NEARDMG = 28062,

    SPELL_CHARGE_NEGATIVE_DMGBUFF = 29660,
    SPELL_CHARGE_NEGATIVE_NEARDMG = 28085,

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
    boss_thaddiusAI(Creature* c): ScriptedAI(c)
    {
        m_pInstance = (instance_naxxramas*)m_creature->GetInstanceData();
        m_bIsRegularMode = m_creature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint64 StalaggGUID, FeugenGUID;

    uint32 PreBossTimer;
    bool CanAggro;

    uint32 PolarityShiftTimer;
    uint32 PolarityClearTimer;
    uint32 PolarityDamageTimer;
    uint32 ChainLightningTimer;
    uint32 BerserkTimer;
    uint32 BallLightningTimer;

    std::list<Player*> PlusList;
    std::list<Player*> MinusList;

    void Reset()
    {
        PreBossRespawn();
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_STUNNED);
        StalaggGUID = 0;
        FeugenGUID = 0;
        
        CanAggro = false;
        PreBossTimer = 0;

        PolarityShiftTimer = 10000;
        PolarityClearTimer = 0;
        PolarityDamageTimer = 1000;
        ChainLightningTimer = 12000+urand(0,5000);
        BerserkTimer = 365000;
        BallLightningTimer = 3000;

        RemoveCharges();

        PlusList.clear();
        MinusList.clear();
    }

    void PreBossDeath()
    {
        if(PreBossTimer)
        {
            DoScriptText(SAY_GREET,m_creature);
            CanAggro = true;
            PreBossTimer = 15000+urand(0,5000);
            return;
        }

        PreBossTimer = 5000;
    }

    void PreBossRespawn()
    {
        if(Creature* Stalagg = m_pInstance->instance->GetCreature(StalaggGUID))
            if(Stalagg->isDead())
                Stalagg->Respawn();
        if(Creature* Feugen = m_pInstance->instance->GetCreature(FeugenGUID))
            if(Feugen->isDead())
                Feugen->Respawn();
    }

    void RemoveCharges()
    {
        Map::PlayerList const& plList = m_pInstance->instance->GetPlayers();

        if(plList.isEmpty())
            return;

        for(Map::PlayerList::const_iterator ittr = plList.begin(); ittr != plList.end(); ++ittr)
        {
            if(ittr->getSource())
            {
                ittr->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_POSITIVE_DMGBUFF);
                ittr->getSource()->RemoveAurasDueToSpell(SPELL_CHARGE_NEGATIVE_DMGBUFF);
            }
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* spell)
    {
        if(spell->Id == SPELL_POLARITYSHIFT && pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->isAlive())
        {
            pTarget->RemoveAura(SPELL_CHARGE_POSITIVE_DMGBUFF, EFFECT_INDEX_0);
            pTarget->RemoveAura(SPELL_CHARGE_NEGATIVE_DMGBUFF, EFFECT_INDEX_0);

            if(urand(1,4) > 2) //positive
            {
                PlusList.push_back((Player*)pTarget);
                pTarget->CastSpell(pTarget,SPELL_CHARGE_POSITIVE_DMGBUFF,true);
            }
            else               //negative
            {
                MinusList.push_back((Player*)pTarget);
                pTarget->CastSpell(pTarget,SPELL_CHARGE_NEGATIVE_DMGBUFF,true);
            }
        }
    }

    void Aggro(Unit* pVictim)
    {
        switch(urand(1,3))
        {
        case 1:  DoScriptText(SAY_AGGRO1, m_creature); break;
        case 2:  DoScriptText(SAY_AGGRO2, m_creature); break;
        default: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        RemoveCharges();
    }

    void KilledUnit(Unit* pVictim)
    {
        if(urand(0,1))
            DoScriptText(SAY_SLAY,m_creature);

        pVictim->RemoveAura(SPELL_CHARGE_POSITIVE_DMGBUFF, EFFECT_INDEX_0);
        pVictim->RemoveAura(SPELL_CHARGE_NEGATIVE_DMGBUFF, EFFECT_INDEX_0);
    }

    void UpdateAI(const uint32 diff)
    {
        if(!CanAggro)
        {
            if(PreBossTimer)
            {
                if(PreBossTimer <= diff)
                {
                    PreBossRespawn();
                    PreBossTimer = 0;
                } else PreBossTimer -= diff;
            }
        }
        else
        {
            if(PreBossTimer)
            {
                if(PreBossTimer <= diff)
                {
                    Map::PlayerList const& plList = m_pInstance->instance->GetPlayers();
                    if(plList.isEmpty())
                    {
                        error_log("ThaddiusAI: cannot start attacking, player list is empty!");
                        PreBossTimer = 0;
                        return;
                    }
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE + UNIT_FLAG_NOT_SELECTABLE + UNIT_FLAG_STUNNED);
                    if(plList.begin()->getSource())
                        m_creature->AI()->AttackStart(plList.begin()->getSource());
                    PreBossTimer = 0;
                } else PreBossTimer -= diff;
            }
        }

        if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
        {
            if(m_creature->GetHealth() < m_creature->GetMaxHealth())
                EnterEvadeMode();
            return;
        }

        if (BerserkTimer)
        {
            if (BerserkTimer <= diff)
            {
                DoCast(m_creature, SPELL_BESERK);
                BerserkTimer = 0;
            }else BerserkTimer -= diff;
        } 

        if(PolarityShiftTimer <= diff)
        {
            DoScriptText(SAY_ELECT,m_creature);

            DoCast(m_creature,SPELL_POLARITYSHIFT);
            PolarityShiftTimer = 30000;
            PolarityClearTimer = 500;
            PolarityDamageTimer = 8000;
        } else PolarityShiftTimer -= diff;

        if(PolarityClearTimer)
        {
            if(PolarityClearTimer <= diff)
            {
                PlusList.clear();
                MinusList.clear();
                PolarityClearTimer = 0;
            } else PolarityClearTimer -= diff;
        }

        if(PolarityDamageTimer <= diff)
        {
            if(!PlusList.empty())
            {
                for(std::list<Player*>::const_iterator itr = PlusList.begin(); itr != PlusList.end(); ++itr)
                {
                    int32 charge = 0;
                    Map::PlayerList const& plList = m_pInstance->instance->GetPlayers();
                    for(Map::PlayerList::const_iterator ittr = plList.begin(); ittr != plList.end(); ++ittr)
                    {
                        if(ittr->getSource() && ittr->getSource()->isAlive() && !ittr->getSource()->HasAura(SPELL_CHARGE_POSITIVE_DMGBUFF,EFFECT_INDEX_0) && ittr->getSource()->IsWithinDistInMap((*itr), 10.0f))
                            (*itr)->CastSpell((*itr),SPELL_CHARGE_POSITIVE_NEARDMG,true);
                        if(ittr->getSource() && ittr->getSource()->isAlive() && ittr->getSource()->HasAura(SPELL_CHARGE_POSITIVE_DMGBUFF,EFFECT_INDEX_0) && ittr->getSource()->IsWithinDistInMap((*itr), 10.0f))
                            ++charge;
                    }
                    (*itr)->RemoveAura(SPELL_CHARGE_POSITIVE_DMGBUFF, EFFECT_INDEX_0);
                    for( int32 i = 0; i < charge; i++)
                    {
                        (*itr)->CastSpell((*itr), SPELL_CHARGE_POSITIVE_DMGBUFF,true);
                    }


                }
            }
            if(!MinusList.empty())
            {
                for(std::list<Player*>::const_iterator itr = MinusList.begin(); itr != MinusList.end(); ++itr)
                {
                    int32 charge = 0;
                    Map::PlayerList const& plList = m_pInstance->instance->GetPlayers();
                    for(Map::PlayerList::const_iterator ittr = plList.begin(); ittr != plList.end(); ++ittr)
                    {
                        if(ittr->getSource() && ittr->getSource()->isAlive() && !ittr->getSource()->HasAura(SPELL_CHARGE_NEGATIVE_DMGBUFF,EFFECT_INDEX_0) && ittr->getSource()->IsWithinDistInMap((*itr), 10.0f))
                            (*itr)->CastSpell((*itr),SPELL_CHARGE_NEGATIVE_NEARDMG,true);
                        if(ittr->getSource() && ittr->getSource()->isAlive() && ittr->getSource()->HasAura(SPELL_CHARGE_NEGATIVE_DMGBUFF,EFFECT_INDEX_0) && ittr->getSource()->IsWithinDistInMap((*itr), 10.0f))
                            ++charge;
                    }
                    (*itr)->RemoveAura(SPELL_CHARGE_NEGATIVE_DMGBUFF, EFFECT_INDEX_0);
                    for( int32 i = 0; i < charge; i++)
                    {
                        (*itr)->CastSpell((*itr), SPELL_CHARGE_NEGATIVE_DMGBUFF,true);
                    }
                }
            }
            PolarityDamageTimer = 1000;
        } else PolarityDamageTimer -= diff;

        if(ChainLightningTimer <= diff)
        {
            DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),m_bIsRegularMode?SPELL_CHAIN_LIGHTNING:SPELL_CHAIN_LIGHTNING_H);
            ChainLightningTimer = 12000+urand(0,5000);
        }else ChainLightningTimer -= diff;

        if(!m_creature->getVictim()->IsWithinDistInMap(m_creature,ATTACK_DISTANCE))
        {
            if(BallLightningTimer <= diff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0),SPELL_BALL_LIGHTNING);
                BallLightningTimer = 2500;
            }else BallLightningTimer -= diff;
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
    mob_stalaggAI(Creature* c): ScriptedAI(c)
    {
        m_pInstance = (instance_naxxramas*)m_creature->GetInstanceData();
        m_bIsRegularMode = m_creature->GetMap()->IsRegularDifficulty();

        if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
            ThaddiusAI->StalaggGUID = m_creature->GetGUID();

        Reset();
    }

    boss_thaddiusAI* GetThaddiusAI()
    {
        if(m_pInstance)
            if(Creature* Thaddius = GetClosestCreatureWithEntry(m_creature,NPC_THADDIUS,1000.0f))//m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_THADDIUS)))
                return (boss_thaddiusAI*)Thaddius->AI();

        return NULL;
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    bool JustSpawnedVisual;

    uint32 PullTimer;
    uint32 PowerSurgeTimer;
    bool JustEnraged;
    bool isFeugenClose;
    uint32 EnrageCastTimer;

    uint32 CheckAggroTimer;

    uint32 WarStompTimer;

    uint64 StalaggTelsa;

    void Reset()
    {
        if (Creature* pTemp = m_creature->GetMap()->GetCreature(StalaggTelsa))
            pTemp->ForcedDespawn();

        JustSpawnedVisual = false;

        PullTimer = 30000;
        PowerSurgeTimer = 10000;
        JustEnraged = false;
        isFeugenClose = false;
        EnrageCastTimer = 1000;

        WarStompTimer = 10000+urand(0,5000);

        CheckAggroTimer = 5000;

        /*if(Creature* TeslaVisual = GetClosestCreatureWithEntry(m_creature,C_TESLA_COIL,30.0f))
            TeslaVisual->CastSpell(m_creature,45537,true);*/ //mob falls down

    }

    void JustDied(Unit* pKiller)
    {
        boss_thaddiusAI* ThaddiusAI = GetThaddiusAI();
        if(!ThaddiusAI)
            return;

        ThaddiusAI->PreBossDeath();
    }

    void UpdateAI(const uint32 diff)
    {
        if(!JustSpawnedVisual)
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
                    StalaggTelsa = TeslaVisual->GetGUID();
                    JustSpawnedVisual = true;
                }
            }
        }

        static boss_thaddiusAI* ThaddiusAI;
        if((ThaddiusAI = GetThaddiusAI()) && !ThaddiusAI->StalaggGUID)
            ThaddiusAI->StalaggGUID = m_creature->GetGUID();

        if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
        {
            if(m_creature->GetHealth() < m_creature->GetMaxHealth())
                EnterEvadeMode();
            return;
        }

        if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
        {
            if(Creature* Feugen = m_pInstance->instance->GetCreature(ThaddiusAI->FeugenGUID))
            {
                if(Feugen->IsWithinDist(m_creature,20.0f))
                    isFeugenClose = true;
                else
                    isFeugenClose = false;
            }
        }

        if( (m_creature->GetPositionZ() < 309.58f) || isFeugenClose)
        {
            if(!JustEnraged)
            {
                JustEnraged = true;
            }
            else
            {
                if(EnrageCastTimer <= diff)
                {
                    m_creature->CastSpell(m_creature->getVictim(),17364,true);
                    EnrageCastTimer = 500;
                } else EnrageCastTimer -= diff;
            }
        }
        else
            JustEnraged = false;

        if(WarStompTimer <= diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP,true);
            WarStompTimer = 10000+urand(0,5000);
        } else WarStompTimer -= diff;

        if(CheckAggroTimer)
        {
            if(CheckAggroTimer <= diff)
            {
                if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
                {
                    if(Creature* Feugen = m_pInstance->instance->GetCreature(ThaddiusAI->FeugenGUID))
                    {
                        if(!Feugen->getVictim())
                        {
                            Feugen->AI()->AttackStart(m_creature->getVictim());
                            Feugen->AddThreat(m_creature->getVictim(),1000.0f);
                        }
                    }
                }
                CheckAggroTimer = 0;
            } else CheckAggroTimer -= diff;
        }

        if(PowerSurgeTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode? SPELL_POWERSURGE : SPELL_POWERSURGE_H,true);
            PowerSurgeTimer = 15000+urand(0,5000);
        } else PowerSurgeTimer -= diff;

        if(PullTimer <= diff)
        {
            if(Unit* StalaggVictim = m_creature->getVictim())
            {
                //workaround for non working move type
                if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
                {
                    Unit* FeugenVictim = NULL;
                    Unit* StalaggVictim = m_creature->getVictim();
                    float ThreatFeugenVictim = NULL;
                    float ThreatStalaggVictim = m_creature->getThreatManager().getThreat(StalaggVictim);
                    if(Creature* Feugen = m_pInstance->instance->GetCreature(ThaddiusAI->FeugenGUID))
                    {
                        FeugenVictim = Feugen->getVictim();
                        if (FeugenVictim)
                        {
                            ThreatFeugenVictim = Feugen->getThreatManager().getThreat(FeugenVictim);
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
                /*m_creature->GetMotionMaster()->MoveIdle();
                ((Player*)Victim)->NearTeleportTo(Victim->GetPositionX(),Victim->GetPositionY(),Victim->GetPositionZ()+5.0f,Victim->GetOrientation());

                float vsin = sin(Victim->GetAngle(FeugenPos[0],FeugenPos[1]));
                float vcos = cos(Victim->GetAngle(FeugenPos[0],FeugenPos[1]));

                WorldPacket data(SMSG_MOVE_KNOCK_BACK, (8+4+4+4+4+4));
                data.appendPackGUID(Victim->GetGUID());
                data << uint32(1);                                      // Sequence
                data << float(vcos);                                    // x direction
                data << float(vsin);                                    // y direction
                data << float(Victim->GetDistance2d(FeugenPos[0],FeugenPos[1]));
                data << float(-10.0);                                   // Z Movement speed

                ((Player*)Victim)->GetSession()->SendPacket(&data);*/
            }
            PullTimer = 30000;
        } else PullTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_stalagg(Creature* pCreature)
{
    return new mob_stalaggAI(pCreature);
}

struct MANGOS_DLL_DECL mob_feugenAI: public ScriptedAI
{
    mob_feugenAI(Creature* c): ScriptedAI(c)
    {
        m_pInstance = (instance_naxxramas*)m_creature->GetInstanceData();
        m_bIsRegularMode = m_creature->GetMap()->IsRegularDifficulty();

        if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
            ThaddiusAI->FeugenGUID = m_creature->GetGUID();

        Reset();
    }

    boss_thaddiusAI* GetThaddiusAI()
    {
        if(m_pInstance)
            if(Creature* Thaddius = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_THADDIUS)))
                return (boss_thaddiusAI*)Thaddius->AI();

        return NULL;
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    bool JustSpawnedVisual;

    uint32 StaticFieldTimer;
    bool JustEnraged;
    uint32 EnrageCastTimer;

    uint32 WarStompTimer;

    uint32 CheckAggroTimer;

    uint64 FeugenTelsa;

    void Reset()
    {
        if (Creature* pTemp = m_creature->GetMap()->GetCreature(FeugenTelsa))
            pTemp->ForcedDespawn();

        JustSpawnedVisual = false;

        StaticFieldTimer = 3000;
        JustEnraged = false;
        EnrageCastTimer = 1000;

        WarStompTimer = 10000+urand(0,5000);

        CheckAggroTimer = 5000;

        /*if(Creature* TeslaVisual = GetClosestCreatureWithEntry(m_creature,C_TESLA_COIL,30.0f))
            TeslaVisual->CastSpell(m_creature,45537,true);*/ //mob falls down
    }

    void JustDied(Unit* pKiller)
    {
        boss_thaddiusAI* ThaddiusAI = GetThaddiusAI();
        if(!ThaddiusAI)
            return;

        ThaddiusAI->PreBossDeath();
    }

    void UpdateAI(const uint32 diff)
    {
        if(!JustSpawnedVisual)
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
                    FeugenTelsa = TeslaVisual->GetGUID();
                    JustSpawnedVisual = true;
                }
            }
        }

        static boss_thaddiusAI* ThaddiusAI;
        if((ThaddiusAI = GetThaddiusAI()) && !ThaddiusAI->FeugenGUID)
            ThaddiusAI->FeugenGUID = m_creature->GetGUID();

        if(!m_creature->getVictim() || !m_creature->SelectHostileTarget())
        {
            if(m_creature->GetHealth() < m_creature->GetMaxHealth())
                EnterEvadeMode();
            return;
        }

        if( (m_creature->GetPositionZ() < 309.58f/* || m_pInstance->instance->GetCreature(GetThaddiusAI()->FeugenGUID)->IsWithinDist(m_creature,20.0f) */))
        {
            if(!JustEnraged)
            {
                JustEnraged = true;
            } else
            {
                if(EnrageCastTimer <= diff)
                {
                    m_creature->CastSpell(m_creature->getVictim(),17364,true);
                    EnrageCastTimer = 500;
                } else EnrageCastTimer -= diff;
            }
        } else
            JustEnraged = false;

        if(CheckAggroTimer)
        {
            if(CheckAggroTimer <= diff)
            {
                if(boss_thaddiusAI* ThaddiusAI = GetThaddiusAI())
                {
                    if(Creature* Stalagg = m_pInstance->instance->GetCreature(ThaddiusAI->StalaggGUID))
                    {
                        if(!Stalagg->getVictim())
                        {
                            Stalagg->AI()->AttackStart(m_creature->getVictim());
                            Stalagg->AddThreat(m_creature->getVictim(),1000.0f);
                        }
                    }
                }
                CheckAggroTimer = 0;
            } else CheckAggroTimer -= diff;
        }

        if(WarStompTimer <= diff)
        {
            DoCast(m_creature,SPELL_WARSTOMP,true);
            WarStompTimer = 10000+urand(0,5000);
        } else WarStompTimer -= diff;

        if(StaticFieldTimer <= diff)
        {
            DoCast(m_creature,m_bIsRegularMode ? SPELL_STATIC_FIELD : SPELL_STATIC_FIELD_H,true);
            StaticFieldTimer = 3000;
        } else StaticFieldTimer -= diff;

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
