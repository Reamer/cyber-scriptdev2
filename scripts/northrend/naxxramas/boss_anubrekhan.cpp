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
SDName: Boss_Anubrekhan
SD%Complete: 95
SDComment:
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

enum
{
    SAY_GREET                   = -1533000,
    SAY_AGGRO1                  = -1533001,
    SAY_AGGRO2                  = -1533002,
    SAY_AGGRO3                  = -1533003,
    SAY_TAUNT1                  = -1533004,
    SAY_TAUNT2                  = -1533005,
    SAY_TAUNT3                  = -1533006,
    SAY_TAUNT4                  = -1533007,
    SAY_SLAY                    = -1533008,

    SPELL_IMPALE                = 28783,                    //May be wrong spell id. Causes more dmg than I expect
    SPELL_IMPALE_H              = 56090,
    SPELL_LOCUSTSWARM           = 28785,                    //This is a self buff that triggers the dmg debuff
    SPELL_LOCUSTSWARM_H         = 54021,

    //spellId invalid
    SPELL_SUMMONGUARD           = 29508,                    //Summons 1 crypt guard at targeted location

    SPELL_SELF_SPAWN_5          = 29105,                    //This spawns 5 corpse scarabs ontop of us (most likely the pPlayer casts this on death)
    SPELL_SELF_SPAWN_10         = 28864,                    //This is used by the crypt guards when they die

    SPELL_FRENZY                = 8269,
    SPELL_ACID_SPIT             = 28969,
    SPELL_ACID_SPIT_H           = 56098,
    SPELL_CLEAVE                = 40504,

    NPC_CRYPT_GUARD             = 16573
};

struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_naxxramas*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bHasTaunted = false;
        Reset();
    }

    instance_naxxramas* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiImpaleTimer;
    uint32 m_uiLocustSwarmTimer;
    uint32 m_uiCryptGuardTimer;
    bool   m_bHasTaunted;

    void Reset()
    {
        m_uiImpaleTimer = 15000;                            // 15 seconds
        m_uiLocustSwarmTimer = urand(80000, 120000);        // Random time between 80 seconds and 2 minutes for initial cast
        m_uiCryptGuardTimer = 0;                       
    }

    void KilledUnit(Unit* pVictim)
    {
        //Force the player to spawn corpse scarabs via spell
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, true);

        if (urand(0, 4))
            return;

        DoScriptText(SAY_SLAY, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, IN_PROGRESS);

        if (m_bIsRegularMode)
            m_uiCryptGuardTimer = 20000;
        else
        {
            SummonGuard();
            SummonGuard();
            m_uiCryptGuardTimer = 30000;
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, DONE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANUB_REKHAN, FAIL);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 60.0f))
        {
            switch(urand(0, 4))
            {
                case 0: DoScriptText(SAY_GREET, m_creature); break;
                case 1: DoScriptText(SAY_TAUNT1, m_creature); break;
                case 2: DoScriptText(SAY_TAUNT2, m_creature); break;
                case 3: DoScriptText(SAY_TAUNT3, m_creature); break;
                case 4: DoScriptText(SAY_TAUNT4, m_creature); break;
            }
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void SummonGuard()
    {
        m_creature->SummonCreature(NPC_CRYPT_GUARD,m_creature->GetPositionX()+rand()%5,
                                                   m_creature->GetPositionY()+rand()%5,
                                                   m_creature->GetPositionZ(),0,
                                                   TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,60000);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Impale
        if (m_uiImpaleTimer < uiDiff)
        {
            //Cast Impale on a random target
            //Do NOT cast it when we are afflicted by locust swarm
            if (!m_creature->HasAura(SPELL_LOCUSTSWARM) || !m_creature->HasAura(SPELL_LOCUSTSWARM_H))
            {
                if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1))
                    DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
                    m_uiImpaleTimer = 15000;
            }         
        }else m_uiImpaleTimer -= uiDiff;

        // Locust Swarm
        if (m_uiLocustSwarmTimer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_LOCUSTSWARM :SPELL_LOCUSTSWARM_H);
            m_uiLocustSwarmTimer = 80000;
            //spawn crypt guards with swarm due boss will bug through floor
            m_uiCryptGuardTimer = 2000;
        }else m_uiLocustSwarmTimer -= uiDiff;

        // Summon crypt guard
        if (m_uiCryptGuardTimer < uiDiff)
        {
            SummonGuard();
            if (!m_bIsRegularMode)
                SummonGuard();

            //DoCastSpellIfCan(m_creature, SPELL_SUMMONGUARD);
            m_uiCryptGuardTimer = 30000;
        }else m_uiCryptGuardTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_crypt_guardAI : public ScriptedAI
{
    mob_crypt_guardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint32 AcidSpit_Timer;
    uint32 Cleave_Timer;
    uint32 Berserk_Timer;

    void Reset()
    {
        AcidSpit_Timer = 10000 + rand()%1000;
        Cleave_Timer = 5000 + rand()%5000;
        Berserk_Timer = 120000;
    }

    void KilledUnit(Unit* pVictim)
    {
        //Force the player to spawn corpse scarabs via spell
        if (pVictim->GetTypeId() == TYPEID_PLAYER)
            pVictim->CastSpell(pVictim, SPELL_SELF_SPAWN_5, true);
    }

    void JustDied(Unit* Killer)
    {
        m_creature->CastSpell(m_creature, SPELL_SELF_SPAWN_10, true);
    }

    void Aggro(Unit *who)
    {
        if (m_pInstance)
        {
            if (Creature* pAnubRekhan = ((Creature*)m_creature->GetMap()->GetUnit( m_pInstance->GetData64(NPC_ANUB_REKHAN))))
                if (pAnubRekhan->isAlive() && !pAnubRekhan->getVictim())
                    pAnubRekhan->AI()->AttackStart(who);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Berserk_Timer)
            if (Berserk_Timer < diff)
            {
                DoCast(m_creature, SPELL_FRENZY);
                Berserk_Timer = 0;
            }else Berserk_Timer -= diff;

        if (AcidSpit_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ACID_SPIT : SPELL_ACID_SPIT_H);
            AcidSpit_Timer = 10000 + rand()%1000;
        }else AcidSpit_Timer -= diff;

        if (Cleave_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 5000 + rand()%5000;
        }else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubrekhan(Creature* pCreature)
{
    return new boss_anubrekhanAI(pCreature);
}

CreatureAI* GetAI_mob_crypt_guard(Creature* pCreature)
{
    return new mob_crypt_guardAI(pCreature);
}

void AddSC_boss_anubrekhan()
{
    Script* NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_anubrekhan";
    NewScript->GetAI = &GetAI_boss_anubrekhan;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_crypt_guard";
    NewScript->GetAI = &GetAI_mob_crypt_guard;
    NewScript->RegisterSelf();
}
