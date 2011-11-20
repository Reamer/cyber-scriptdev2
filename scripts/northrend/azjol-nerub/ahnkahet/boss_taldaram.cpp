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
    SPELL_CONJURE_FLAME_ORB         = 55931,
    SPELL_BLOODTHIRST               = 55968,
    SPELL_VANISH                    = 55964,
    SPELL_EMBRACE_OF_THE_VAMPYR     = 55959,
    SPELL_EMBRACE_OF_THE_VAMPYR_H   = 59513,

    SPELL_FLAME_ORB_SPAWN_EFFECT    = 55891, // Orb Grow up
    SPELL_FLAME_ORB_VISUAL          = 55928, // Flame orb effect
    SPELL_FLAME_ORB_DEATH           = 55947, // Despawn effect
    SPELL_FLAME_ORB                 = 57750, // Flame orb damage
    SPELL_FLAME_ORB_H               = 58937,

    SPELL_FLAME_SPHERE_PERIODIC     = 55926,
    SPELL_FLAME_SPHERE_VISUAL       = 55928,
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
        Reset();
    }

    instance_ahnkahet* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSummonFlameOrbTimer;
    uint32 m_uiVanishTimer;
    uint32 m_uiBloodthirstTimer;
    uint32 m_uiEmbraceOfTheVampyrTimer;
    uint32 m_uiEmbraceOfTheVampyrInterruptDamage;

    uint8 m_uiOrbCounter;

    void Reset()
    {
        m_uiEmbraceOfTheVampyrInterruptDamage = 0;
        m_uiSummonFlameOrbTimer = 12000;
        m_uiVanishTimer = 14000;
        m_uiEmbraceOfTheVampyrTimer = m_uiVanishTimer + 2600;
        m_uiBloodthirstTimer = 10000;
        m_uiOrbCounter = 0;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->RemoveAurasDueToSpell(SPELL_BEAM_VISUAL);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, FAIL);
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
        if (m_pInstance)
            m_pInstance->SetData(TYPE_TALDARAM, DONE);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_uiEmbraceOfTheVampyrInterruptDamage > m_bIsRegularMode ? 20000 : 40000)
        {
            m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
            m_creature->InterruptNonMeleeSpells(false);
            m_uiEmbraceOfTheVampyrInterruptDamage = 0;
        }
        if(pDoneBy->GetCreatorGuid() == m_creature->GetObjectGuid())
        {
            uiDamage = 0;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        float destx = 0.0f;
        float desty = 0.0f;
        float destz = 0.0f;
        float srcx = 0.0f;
        float srcy = 0.0f;
        float srcz = 0.0f;
        m_creature->GetPosition(srcx, srcy, srcz);
        m_creature->GetClosePoint(destx,desty,destz, 0, 40.0f, 120.0 * m_uiOrbCounter++);
        m_creature->GetTerrain()->CheckPathAccurate(srcx, srcy, srcz, destx,desty,destz, NULL);
        pSummoned->GetMotionMaster()->MovePoint(0, destx, desty, destz, false);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiSummonFlameOrbTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_CONJURE_FLAME_ORB) == CAST_OK)
            {
                m_uiSummonFlameOrbTimer = 16000 + rand()%10000;
                m_uiOrbCounter = 0;
            }
        }
        else
            m_uiSummonFlameOrbTimer -= uiDiff;

        if (m_uiBloodthirstTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLOODTHIRST) == CAST_OK)
                m_uiBloodthirstTimer = m_bIsRegularMode ? 15000 : 8000;
        }
        else
            m_uiBloodthirstTimer -= uiDiff;

        if(m_uiVanishTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature,SPELL_VANISH) == CAST_OK)
            {
                m_uiVanishTimer = 25000;
                m_uiEmbraceOfTheVampyrTimer = 2600;
            }
        }
        else
            m_uiVanishTimer -= uiDiff;

        if (m_uiEmbraceOfTheVampyrTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, uint32(0), SELECT_FLAG_PLAYER))
            {
                m_creature->NearTeleportTo(pTarget->GetPositionX() + 3.0f, pTarget->GetPositionY() + 3.0f, pTarget->GetPositionZ(), pTarget->GetOrientation());
                DoCast(pTarget, m_bIsRegularMode ? SPELL_EMBRACE_OF_THE_VAMPYR : SPELL_EMBRACE_OF_THE_VAMPYR_H);
                m_uiEmbraceOfTheVampyrInterruptDamage = 0;
                m_uiEmbraceOfTheVampyrTimer = m_uiVanishTimer +2600;
            }
        }
        else
            m_uiEmbraceOfTheVampyrTimer -= uiDiff;

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

    uint32 m_uiDespawn_Timer;

    void Reset()
    {
        m_uiDespawn_Timer = 23000;
        //hack to set model invisible
        m_creature->SetDisplayId(10045);
        m_creature->SetLevitate(true);
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
            if (DoCastSpellIfCan(m_creature, SPELL_FLAME_ORB_DEATH) == CAST_OK);
                m_creature->ForcedDespawn(1000);
        }
        else
            m_uiDespawn_Timer -= uiDiff;

        DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FLAME_ORB : SPELL_FLAME_ORB_H, CAST_AURA_NOT_PRESENT);

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
    pGo->SetFlag(GAMEOBJECT_FLAGS, GO_FLAG_NO_INTERACT);
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
