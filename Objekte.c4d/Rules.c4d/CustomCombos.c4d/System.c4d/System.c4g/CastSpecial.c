/* Zaubern auf Spezial2 */

#strict 2

#appendto MAGE

static g_custom_player_combo0;

func ControlSpecial2()
{
  if (ObjectCount(_CCB))
  {
    var plr = GetOwner();
    if (plr>=0)
    {
      var spell;
      if (spell = g_custom_player_combo0[plr])
      {
        return DoMagic(spell);
      }
    }
  }
  return inherited(...);
}
