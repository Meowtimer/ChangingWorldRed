/*-- Eigene Zauberkombos --*/

#strict 2

static g_custom_combo_spells;
static g_custom_player_combos;
static g_custom_player_combo0;

// Initialisierung
protected func Initialize()
{
  // Custom combos
	AddMsgBoardCmd("setcombo", "_CCB->SetComboCmd(%player%, \"%s\")");
	AddMsgBoardCmd("clearcombos", "_CCB->ClearCombos(%player%)");
	var i,j,def;
	g_custom_combo_spells = [ELX2, MGTJ, MSSH, ABLA, GRSF, MFWV, GRSG, LAVS, FHRT, MBRG, MFFW];
  g_custom_player_combos = [];
  g_custom_player_combo0 = [];
	return 1;
}

public func SetComboCmd(int plr, string combostr)
{
  // safety
  if (plr<0) return false;
  if (!GetPlayerName(plr)) return false;
  var n = GetLength(combostr);
  // extract spell combos
  var i, s, c, combos = [];
  for (var j=0; j<2; ++j)
  {
    s = "";
    while (i<n)
    {
      c = GetChar(combostr, i++);
      //Log("c=%d", c);
      if (!c || c == 32) break;
      if (c < 0x30 || c > 0x36) return SetComboErr(plr, combostr);
      s = Format("%s%c", s, c);
    }
    //Log("s=%s", s);
    if (!GetLength(s)) return SetComboErr(plr, combostr);
    combos[j] = s;
  }
  if (SetCombo(plr, combos[0], combos[1])) SavePlrCombos(plr);
}

private func SetCombo(int plr, string combo_old, string combo_new, bool quiet)
{
  // Find spell with old combo
  combo_old = Format("%s*", combo_old);
  var def, combo;
  for (def in g_custom_combo_spells)
  {
    combo = Format("%s%s", def->GetSpellClass()->GetClassCombo(), def->GetSpellCombo());
    //Log("WM %s, %s", combo, combo_old);
    if (WildcardMatch(combo, combo_old)) break;
    combo = 0;
  }
  if (!combo) return SetComboErr(plr, combo_old);
  // Set new combo
  var idx = GetIndexOf(def, g_custom_combo_spells);
  if (idx<0) return false;
  g_custom_player_combos[plr][idx] = combo_new;
  // Combo "0" is special
  if (combo_new == "0") g_custom_player_combo0[plr] = def;
  // Message
  if (!quiet)
  {
    PlayerMessage(plr, "Combo for {{%i}} set to %s.", 0, def, combo_new);
    Sound("Ding", true, 0, 100, plr+1);
  }
  return true;
}

public func ClearCombos(int plr)
{
  if (plr<0) return;
  g_custom_player_combos[plr] = [];
  g_custom_player_combo0[plr] = 0;
  SavePlrCombos(plr);
  return true;
}

private func SavePlrCombos(int plr)
{
  var num = 0;
  for (var i=0; i<GetLength(g_custom_player_combos[plr]); ++i)
  {
    var combo = g_custom_player_combos[plr][i];
    if (combo)
    {
      var def = g_custom_combo_spells[i];
      var old_combo = Format("%s%s", def->GetSpellClass()->GetClassCombo(), def->GetSpellCombo());
      var combo_num = eval(old_combo) * 100000 + eval(combo) % 100000;
      SetPlrExtraData(plr, Format("EKCombo%d", num++), combo_num);
    }
  }
  SetPlrExtraData(plr, Format("EKCombo%d", num++), 0);
  return true;
}

private func SetComboErr(int plr, string msg)
{
  if (!msg) msg = "";
  PlayerMessage(plr, "Invalid combo string: %s", 0, msg);
  Sound("Error", true, 0, 100, plr+1);
  return false;
}


protected func InitializePlayer(int iPlr)
{
  // Custom Combos
  g_custom_player_combos[iPlr] = [];
  g_custom_player_combo0[iPlr] = 0;
  var num = 0, combo_num;
  while (combo_num = GetPlrExtraData(iPlr, Format("EKCombo%d", num++)))
    SetCombo(iPlr, Format("%d", combo_num / 100000), Format("%d", combo_num % 100000), true);
}

/* Info */

protected func Activate(iByPlayer)
{
	MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

