/*-- AFK-rule --*/

#strict 2

protected func Activate(iByPlayer)
{
	MessageWindow(GetDesc(), iByPlayer);
  return 1;
}

func OnPlayerLocalityFound(int plr, bool loc)
{
  if (loc)
  {
    // --- ASYNC BEGIN ---
    // A local player has been found. Recolor crew.
    var i,crew;
    while (crew = GetCrew(plr,i++)) RecolorCrew(crew);
    // --- ASYNC END ---
  }
}

func RecolorCrew(object clonk) // ASYNC!
{
  // Recolor crew: In case of team colors, recolor local clonk to have a different color
  var plr = GetController(clonk);
  var team = GetPlayerTeam(plr);
  if (team)
  {
    var clr = GetTeamColor(team);
    if (clr == GetColorDw(clonk))
    {
      var hsl = RGB2HSL(clr);
      hsl = (hsl & 0xffff00) | Min((hsl & 0xff)+40,0xff);
      var new_clr = HSL2RGB(hsl);
      //Log("Color of %v from %x to %x.", clonk, clr, new_clr);
      return SetColorDw(new_clr, clonk);
    }
  }
  return false;
}

func CheckRecolorCrew(object clonk)
{
  // Recolor crew if local player
  var plr = GetController(clonk);
  if (GetPlayerLocality(plr) == PLRLOC_Local)
  {
    // --- ASYNC BEGIN ---
    RecolorCrew(clonk);
    // --- ASYNC END ---
  }
}