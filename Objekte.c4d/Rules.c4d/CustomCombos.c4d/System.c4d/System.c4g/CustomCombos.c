/* Custom combos */

#strict 2

#appendto CBMU

static g_custom_combo_spells;
static g_custom_player_combos;
static zaehler;

private func GetSpellCombo(id idSpell, id idClass)
  {
  // Standard-Spellcombo bzw. uberladen
  var plr = GetOwner();
  if (plr>=0 && ObjectCount(_CCB))
    {
 /*    Log("idSpell:%v, dcustom:%v",idSpell, g_custom_combo_spells);
     Log("GetIndexOf(idSpell, g_custom_combo_spells):%v",GetIndexOf(idSpell, g_custom_combo_spells));
     Log("zaehler:%v",zaehler++);*/
    var idx;
    idx = GetIndexOf(idSpell, g_custom_combo_spells);
    var combo = g_custom_player_combos[plr][idx];
    if (combo) return combo;
    }
  return inherited(idSpell, idClass, ...);
  }

