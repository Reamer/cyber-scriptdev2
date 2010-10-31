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
SDName: Boss_Zuljin
SD%Complete: 90
SDComment:
SDCategory: Zul'Aman
EndScriptData */

#include "precompiled.h"
#include "zulaman.h"

enum
{
    SAY_INTRO                       = -1568056,
    SAY_AGGRO                       = -1568057,
    SAY_BEAR_TRANSFORM              = -1568058,
    SAY_EAGLE_TRANSFORM             = -1568058,
    SAY_LYNX_TRANSFORM              = -1568060,
    SAY_DRAGONHAWK_TRANSFORM        = -1568061,
    SAY_FIRE_BREATH                 = -1568062,
    SAY_BERSERK                     = -1568053,
    SAY_KILL1                       = -1568064,
    SAY_KILL2                       = -1568065,
    SAY_DEATH                       = -1568066,

    // Troll Form
    SPELL_WHIRLWIND                 = 17207,
    SPELL_GRIEVOUS_THROW            = 43093,                //removes debuff after full healed

    // Bear Form
    SPELL_CREEPING_PARALYSIS        = 43095,                //should cast on the whole raid
    SPELL_OVERPOWER                 = 43456,                //use after melee attack dodged

    // Eagle Form
    SPELL_ENERGY_STORM              = 43983,                //enemy area aura, trigger 42577
    SPELL_ZAP_INFORM                = 42577,
    SPELL_ZAP_DAMAGE                = 43137,                //1250 damage
    SPELL_SUMMON_CYCLONE            = 43112,                //summon four feather vortex
    CREATURE_FEATHER_VORTEX         = 24136,                // 24136
    SPELL_CYCLONE_VISUAL            = 43119,                //trigger 43147 visual
    SPELL_CYCLONE_PASSIVE           = 43120,                //trigger 43121 (4y aoe) every second

    // Lynx Form
    SPELL_CLAW_RAGE_HASTE           = 42583,
    SPELL_CLAW_RAGE_TRIGGER         = 43149,
    SPELL_CLAW_RAGE_DAMAGE          = 43150,
    SPELL_LYNX_RUSH_HASTE           = 43152,
    SPELL_LYNX_RUSH_DAMAGE          = 43153,

    // Dragonhawk Form
    SPELL_FLAME_WHIRL               = 43213,                //trigger two spells
    SPELL_FLAME_BREATH              = 43215,
    SPELL_SUMMON_PILLAR             = 43216,                //summon 24187
    CREATURE_COLUMN_OF_FIRE         = 24187,
    SPELL_PILLAR_TRIGGER            = 43218,                //trigger 43217
                                    
    // Cosmetic
    SPELL_SPIRIT_AURA               = 42466,
    SPELL_SIPHON_SOUL               = 43501,

    // Transforms
    SPELL_SHAPE_OF_THE_BEAR         = 42594,
    SPELL_SHAPE_OF_THE_EAGLE        = 42606,
    SPELL_SHAPE_OF_THE_LYNX         = 42607,
    SPELL_SHAPE_OF_THE_DRAGONHAWK   = 42608,

    SPELL_BERSERK                   = 45078,

    WEAPON_ID                       = 33975,

    PHASE_TROLL                     = 1,
    PHASE_BEAR                      = 2,
    PHASE_EAGLE                     = 3,
    PHASE_LYNX                      = 4,
    PHASE_DRAGONHAWK                = 5
    
};

//coords for going for changing form
const float CENTER_X = 120.148811f;
const float CENTER_Y = 703.713684f;
const float CENTER_Z = 45.111477f;

struct MANGOS_DLL_DECL boss_zuljinAI : public ScriptedAI
{

    uint32 m_uiPhase;
    bool Phase3Reached;


   // Troll Form timer

       uint32 m_uiSPELL_WHIRLWIND_timer;
       uint32 m_uiSPELL_GRIEVOUS_THROW_timer;

   // Bear Form timer
       uint32 m_uiSPELL_OVERPOWER_timer;
       uint32 m_uiSPELL_CREEPING_PARALYSIS_timer;

   // Eagle Form
        Creature* FEATHER_VORTEX[4];
        Unit*     PLAYERS[12]; 
        uint32 PLAYER_counter;
        uint32 FEATHER_VORTEX_TARGET_timer;
        uint32 RAID_MANA[12];

   // Lynx Form timer

       uint32 m_uiSPELL_CLAW_RAGE_timer;
       uint32 SPELL_CLAW_RAGE_DAMAGE_timer;
       uint32 m_uiSPELL_LYNX_RUSH_timer;
       uint32 LYNX_RUSH_counter;
       uint32 SPELL_CLAW_RAGE_counter;
       Unit*  CLAW_RAGE_TARGET;

   // Dragonhawk Form timer

       uint32 m_uiSPELL_FLAME_WHIRL_timer;         
       uint32 m_uiSPELL_FLAME_BREATH_timer;
       uint32 m_uiSPELL_PILLAR_timer;



    boss_zuljinAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
        
    }

    ScriptedInstance* m_pInstance;

    void Reset()
    {
       
        DespawnVortex();
        Phase3Reached = false;
        m_uiPhase = 1;

        m_uiSPELL_GRIEVOUS_THROW_timer = 8000;
        m_uiSPELL_WHIRLWIND_timer = 14000;

        m_uiSPELL_CREEPING_PARALYSIS_timer = 3000;
        m_uiSPELL_OVERPOWER_timer = 6000;

        FEATHER_VORTEX_TARGET_timer = 4000;
        PLAYER_counter = 0;

        m_uiSPELL_CLAW_RAGE_timer = 7000;
        m_uiSPELL_LYNX_RUSH_timer = 4000;
        LYNX_RUSH_counter = 9;

        m_uiSPELL_FLAME_WHIRL_timer = 4000;         
        m_uiSPELL_FLAME_BREATH_timer = 8000;
        m_uiSPELL_PILLAR_timer = 3000;
    }

    void GetPlayers()
    {
        ThreatList const& tList = m_creature->getThreatManager().getThreatList();
        std::vector<Unit *> targets;

        if (tList.empty())
            return;

        //begin + 1 , so we don't target the one with the highest threat
        ThreatList::const_iterator itr = tList.begin();
        std::advance(itr, 0);
        for(; itr!= tList.end(); ++itr)                    //store the threat list in a different container
        {
            Unit *target = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                                                            //only on alive players
            if (target && target->GetTypeId() == TYPEID_PLAYER && target->getPowerType() == POWER_MANA){
                PLAYERS[PLAYER_counter] = target;
                PLAYER_counter ++;
            }
        }

        for(uint32 i = 0; i < PLAYER_counter; i++)
        {
            RAID_MANA[i] = PLAYERS[i]->GetPower(POWER_MANA);
        }
    }

    void EnergyStormDamage()
    {
        for(uint32 i = 0; i < PLAYER_counter; i++)
        {
            if(RAID_MANA[i] > PLAYERS[i]->GetPower(POWER_MANA))
            {
                m_creature->CastSpell(PLAYERS[i],SPELL_ZAP_DAMAGE,false);
            }
            RAID_MANA[i] = PLAYERS[i]->GetPower(POWER_MANA);
        }
    }

    void DespawnVortex()
    {
        for(int i = 0; i < 4; i++)
        {
            if(Phase3Reached)
            {
                FEATHER_VORTEX[i]->SetVisibility(VISIBILITY_OFF);
                FEATHER_VORTEX[i]->setDeathState(JUST_DIED);
            }
        }
    }

    void NextVortexTarget()
    {
        for(int i = 0; i < 4; i++)
        {
            if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                FEATHER_VORTEX[i]->GetMotionMaster()->Clear(false);
                FEATHER_VORTEX[i]->GetMotionMaster()->MoveFollow(pTarget,0,0);   
            }
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_KILL1 : SAY_KILL2, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_ZULJIN, DONE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiPhase < 2 && m_creature->GetHealthPercent() < 80)
        {            
            m_uiPhase = 2;
            DoResetThreat();

            m_creature->Relocate(CENTER_X, CENTER_Y, CENTER_Z);         
            m_creature->CastSpell(m_creature,SPELL_SHAPE_OF_THE_BEAR,true);
        }

        if(m_uiPhase < 3 && m_creature->GetHealthPercent() < 60)
        {            
            m_uiPhase = 3;
            m_creature->CastSpell(m_creature,SPELL_SHAPE_OF_THE_EAGLE,true);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MovePoint(1,CENTER_X,CENTER_Y,CENTER_Z);

            m_creature->CastSpell(m_creature,SPELL_ENERGY_STORM,false);

            for(int i = 0; i < 4; i++)
            {
                FEATHER_VORTEX[i] = m_creature->SummonCreature(CREATURE_FEATHER_VORTEX,CENTER_X+i*10,CENTER_Y+i*10,CENTER_Z,0,TEMPSUMMON_CORPSE_DESPAWN,1000000000);
                FEATHER_VORTEX[i]->setFaction(m_creature->getFaction()); 
                FEATHER_VORTEX[i]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                FEATHER_VORTEX[i]->SetLevel(73);
                FEATHER_VORTEX[i]->SetSpeedRate(MOVE_WALK,3);
 //               FEATHER_VORTEX[i]->SetSpeedRate(MOVE_RUN,10);
                FEATHER_VORTEX[i]->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
           
                

                Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                FEATHER_VORTEX[i]->GetMotionMaster()->Clear(false);
                FEATHER_VORTEX[i]->GetMotionMaster()->MoveFollow(pTarget,0,0);
            }
            for(int i = 0; i < 4; i++)
            {
                FEATHER_VORTEX[i]->CastSpell(FEATHER_VORTEX[i],SPELL_CYCLONE_PASSIVE,true,0,0,m_creature->GetGUID());
                FEATHER_VORTEX[i]->CastSpell(FEATHER_VORTEX[i],SPELL_CYCLONE_VISUAL,true,0,0,m_creature->GetGUID());

            }
            GetPlayers();
            Phase3Reached = true;

            //m_creature->Relocate(CENTER_X, CENTER_Y, CENTER_Z);
            //m_creature->SendMonsterMove(CENTER_X, CENTER_Y, CENTER_Z, false);
        }

        if(m_uiPhase < 4 && m_creature->GetHealthPercent() < 40)
        {
            DespawnVortex();
            m_uiPhase = 4;
            m_creature->CastSpell(m_creature,SPELL_SHAPE_OF_THE_LYNX,true);
            m_creature->GetMotionMaster()->Clear();
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            DoResetThreat();

            //m_creature->Relocate(CENTER_X, CENTER_Y, CENTER_Z);
            //m_creature->SendMonsterMove(CENTER_X, CENTER_Y, CENTER_Z, false);
        }
        if(m_uiPhase < 5 && m_creature->GetHealthPercent() < 20)
        {            
            m_uiPhase = 5;
            LYNX_RUSH_counter = 9;
            m_creature->CastSpell(m_creature,SPELL_SHAPE_OF_THE_DRAGONHAWK,true);
            DoResetThreat();

            //m_creature->Relocate(CENTER_X, CENTER_Y, CENTER_Z);
            //m_creature->SendMonsterMove(CENTER_X, CENTER_Y, CENTER_Z, false);
            //m_creature->GetMotionMaster()->Mutate(new TargetedMovementGenerator<Creature>(*m_creature->getVictim()));
        }

        switch(m_uiPhase)
        {        
            case PHASE_TROLL:
                 if(m_uiSPELL_GRIEVOUS_THROW_timer < uiDiff)
                 {
                     DoCastSpellIfCan(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0), SPELL_GRIEVOUS_THROW, false);                        
                     m_uiSPELL_GRIEVOUS_THROW_timer = 15000;
                 }else m_uiSPELL_GRIEVOUS_THROW_timer -= uiDiff;

                 if(m_uiSPELL_WHIRLWIND_timer < uiDiff)
                 {
                     DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND, false);                        
                     m_uiSPELL_WHIRLWIND_timer = 15000;
                 }else m_uiSPELL_WHIRLWIND_timer -= uiDiff;

                break;

            case PHASE_BEAR:
                 if(m_uiSPELL_OVERPOWER_timer < uiDiff)
                 {
                     DoCastSpellIfCan(m_creature->getVictim(),SPELL_OVERPOWER, false);                        
                     m_uiSPELL_OVERPOWER_timer = 6000;
                 }else m_uiSPELL_OVERPOWER_timer -= uiDiff;

                 if(m_uiSPELL_CREEPING_PARALYSIS_timer < uiDiff)
                 {
                     DoCastSpellIfCan(m_creature,SPELL_CREEPING_PARALYSIS, false);                        
                     m_uiSPELL_CREEPING_PARALYSIS_timer = 12000;
                 }else m_uiSPELL_CREEPING_PARALYSIS_timer -= uiDiff;

                break;

            case PHASE_EAGLE:
                EnergyStormDamage();

                if(FEATHER_VORTEX_TARGET_timer < uiDiff)
                {
                     NextVortexTarget();                        
                     FEATHER_VORTEX_TARGET_timer = 5000;
                 }else FEATHER_VORTEX_TARGET_timer -= uiDiff;

                break;

            case PHASE_LYNX:
                if(m_uiSPELL_CLAW_RAGE_timer < uiDiff)
                {
                    if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    {
                        DoCastSpellIfCan(pTarget,SPELL_CLAW_RAGE_HASTE, false);
                        DoCastSpellIfCan(pTarget,SPELL_CLAW_RAGE_DAMAGE, false);
                        m_uiSPELL_CLAW_RAGE_timer = 12000;
                    }
                }else m_uiSPELL_CLAW_RAGE_timer -= uiDiff;

                if(m_uiSPELL_LYNX_RUSH_timer < uiDiff)
                {
                    for (uint8 i = 0; i < LYNX_RUSH_counter; i++)
                    {
                        if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            DoCastSpellIfCan(pTarget, SPELL_CLAW_RAGE_HASTE, false); //Lynx rush haste do  not work
                            DoCastSpellIfCan(pTarget ,SPELL_LYNX_RUSH_DAMAGE, false);
                        }
                    }
                    m_uiSPELL_LYNX_RUSH_timer = 12000;
                }else m_uiSPELL_LYNX_RUSH_timer -= uiDiff;
        
                break;

            case PHASE_DRAGONHAWK:
                if(m_uiSPELL_FLAME_BREATH_timer < uiDiff)
                {
                    if(m_uiSPELL_FLAME_WHIRL_timer < 13000)
                    {
                        DoCastSpellIfCan(m_creature->getVictim(),SPELL_FLAME_BREATH, false);
                        m_uiSPELL_FLAME_BREATH_timer = urand(7000,15000);
                    }
                }else m_uiSPELL_FLAME_BREATH_timer -= uiDiff;

                if(m_uiSPELL_FLAME_WHIRL_timer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature,SPELL_FLAME_WHIRL, false);
                    m_uiSPELL_FLAME_WHIRL_timer = 15000;
                }else m_uiSPELL_FLAME_WHIRL_timer -= uiDiff;
                 
                if(m_uiSPELL_PILLAR_timer < uiDiff)
                {
                    if(m_uiSPELL_FLAME_WHIRL_timer < 13000)
                    {
                        if(Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        {
                            Creature* pillar_dummy = m_creature->SummonCreature(CREATURE_COLUMN_OF_FIRE,pTarget->GetPositionX(),pTarget->GetPositionY(), pTarget->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000);                         
                            pillar_dummy->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            pillar_dummy->setFaction(m_creature->getFaction());                           
                            pillar_dummy->SetLevel(73);
                            pillar_dummy->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            pillar_dummy->SetVisibility(VISIBILITY_OFF); //wenns script fertig ist, damit man den dummy nicht sieht
                            pillar_dummy->CastSpell(pillar_dummy,SPELL_PILLAR_TRIGGER,true,0,0,m_creature->GetGUID());
                            pillar_dummy->GetMotionMaster()->Clear(false);
                            pillar_dummy->GetMotionMaster()->MoveIdle();
                            m_uiSPELL_PILLAR_timer = 3333;
                        }                      
                    }
                }else m_uiSPELL_PILLAR_timer -= uiDiff;

            break;
        }

        if(SPELL_CLAW_RAGE_counter == 0 && LYNX_RUSH_counter == 0 && m_uiPhase != PHASE_EAGLE)
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_zuljin(Creature* pCreature)
{
    return new boss_zuljinAI(pCreature);
}

void AddSC_boss_zuljin()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_zuljin";
    newscript->GetAI = &GetAI_boss_zuljin;
    newscript->RegisterSelf();
}
