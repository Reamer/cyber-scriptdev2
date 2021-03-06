/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
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
SDName: Boss_Grilek
SD%Complete: 100
SDComment:
SDCategory: Zul'Gurub
EndScriptData */

#include "precompiled.h"
#include "zulgurub.h"

#define SPELL_AVARTAR                24646                  //The Enrage Spell
#define SPELL_GROUNDTREMOR            6524

struct MANGOS_DLL_DECL boss_grilekAI : public ScriptedAI
{
    boss_grilekAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 Avartar_Timer;
    uint32 GroundTremor_Timer;

    void Reset()
    {
        Avartar_Timer = urand(15000, 25000);
        GroundTremor_Timer = urand(8000, 16000);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Avartar_Timer
        if (Avartar_Timer < diff)
        {

            DoCastSpellIfCan(m_creature, SPELL_AVARTAR);
            Unit* target = NULL;

            target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);

            if (m_creature->getThreatManager().getThreat(m_creature->getVictim()))
                m_creature->getThreatManager().modifyThreatPercent(m_creature->getVictim(),-50);
            if (target)
                AttackStart(target);

            Avartar_Timer = urand(25000, 35000);
        }else Avartar_Timer -= diff;

        //GroundTremor_Timer
        if (GroundTremor_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_GROUNDTREMOR);
            GroundTremor_Timer = urand(12000, 16000);
        }else GroundTremor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_grilek(Creature* pCreature)
{
    return new boss_grilekAI(pCreature);
}

void AddSC_boss_grilek()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_grilek";
    pNewScript->GetAI = &GetAI_boss_grilek;
    pNewScript->RegisterSelf();
}
