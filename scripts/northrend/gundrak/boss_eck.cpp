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
SDName: boss_ech_the_Ferocious
SD%Complete: 1
SDComment: This Boss is only available on hc mod if Moorabi was killed....
SDCategory: gundrak (HC)
EndScriptData */

#include "precompiled.h"
#include "gundrak.h"

enum
{    
    //  ECK TALKS NEED TO BE EDIT
    EMOTE_BERSERK               =   -1533021,
    
    // ECK SPELLS
    SPELL_ECK_RESIDUE_DEBUFF    =   55817, 
    SPELL_ECK_SPITE             =   55814,

    SPELL_ECK_SPRING_WHERE		=   55837,        // needs targetcoordinates in db. after Spring is casted Thread list will be resetted
    SPELL_ECK_SPRING			=	55815,        //trigger spell 55837
    
    SPELL_ECK_BIT               =   55813,
    SPELL_ECK_BERSERK           =   55816,
    
};

/*######
## boss_eck_the_ferocious
######*/

struct MANGOS_DLL_DECL boss_eck_the_ferociousAI : public ScriptedAI
{
    boss_eck_the_ferociousAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool m_bBerserk;

    uint32 m_uiEckSpiteTimer;
    uint32 m_uiEckSpringTimer;
    uint32 m_uiEckBitTimer;
    uint32 m_uiEckBerserkTimer;
        
    void Reset()
    {
        m_uiEckSpiteTimer            = 15000;                 //incorrect    Timer ?
        m_uiEckSpringTimer            = 8000;                     //incorrect Timer ?
        m_uiEckBitTimer                = 5000;                     //incorrect Timer ?
        m_uiEckBerserkTimer            = urand(60000 , 90000);  //enrange at 20% HP or after 60-90 seconds
        m_bBerserk = false;

        //we need to remove debuff if raid fail
        if (m_creature->HasAura(SPELL_ECK_RESIDUE_DEBUFF))
             m_creature->RemoveAurasDueToSpell(SPELL_ECK_RESIDUE_DEBUFF);
    }
    void Aggro(Unit* pWho)
    {
		if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, IN_PROGRESS);
    }
    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_ECK, DONE);
    }
    void SpellHitTarget (Unit* pUnit, const SpellEntry* pSpellEntry)
    {
        if (pSpellEntry->Id == SPELL_ECK_SPITE && pUnit->GetTypeId() == TYPEID_PLAYER
            && !pUnit->HasAura(SPELL_ECK_RESIDUE_DEBUFF))
            pUnit->CastSpell(pUnit, SPELL_ECK_RESIDUE_DEBUFF, true);
    }
    
    void UpdateAI(const uint32 uiDiff)
    {    
        /* if Hc mod is on creature should be there else not
        if(!m_bIsRegularMode)
        {
                m_creature->SetVisibility(VISIBILITY_ON);
        }*/
                            
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if (m_uiEckSpiteTimer < uiDiff)
        {
            //dunno if this is right
            DoCastSpellIfCan(m_creature, SPELL_ECK_SPITE);
                
            m_uiEckSpiteTimer = 15000;
        }
        else
            m_uiEckSpiteTimer -= uiDiff;

        if (m_uiEckSpringTimer < uiDiff)
        {
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
			{
                DoCastSpellIfCan(target, SPELL_ECK_SPRING);
				m_creature->DeleteThreatList();
				m_creature->AddThreat(target,0.0f);
				m_uiEckSpringTimer = 8000;
			}
        }
        else
            m_uiEckSpringTimer -= uiDiff;

        if (m_uiEckBitTimer < uiDiff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_ECK_BIT);
            m_uiEckBitTimer = 5000;
        }
        else
            m_uiEckBitTimer -= uiDiff;

        if (!m_bBerserk)
        {
            if (m_creature->GetHealthPercent() <= 20.0f)
            {
                DoScriptText(EMOTE_BERSERK, m_creature);
                DoCastSpellIfCan(m_creature, SPELL_ECK_BERSERK);
                m_bBerserk = true;
            }
        }

        if (!m_bBerserk)
        {
            if (m_uiEckBerserkTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_ECK_BERSERK);
                DoScriptText(EMOTE_BERSERK, m_creature);
                m_bBerserk = true;
            }
            else
                m_uiEckBerserkTimer -= uiDiff;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eck_the_ferocious(Creature* pCreature)
{
    return new boss_eck_the_ferociousAI(pCreature);
}

void AddSC_boss_eck_the_ferocious()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_eck_the_ferocious";
    newscript->GetAI = &GetAI_boss_eck_the_ferocious;
    newscript->RegisterSelf();
}