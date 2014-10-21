/* Richtung wechseln im Sprung */
// Option to turn it on/off by RiQQ

#strict 2
#appendto MAGE

// Einfach immer erlauben, ausser in einigen Spezialsituationen in denen es Aerger macht

local turningAccepted;

protected func Initialize(a,b,c)
{
  // Clonk-Initialisierung
  inherited(a,b,c);
   // Neue Physicals dauerhaft setzen, wenn der Clonk nicht schon höher trainiert hat
  UpdatePhysical ("Breath");
  UpdatePhysical ("Scale");
  UpdatePhysical ("Hangle");
  UpdatePhysical ("Dig");
  UpdatePhysical ("Swim");
  UpdatePhysical ("Push");
  UpdatePhysical ("Fight");
  UpdatePhysical ("Magic");

  ScheduleCall(0, "TimeWaste", 1);
  return(1);
}

private func TimeWaste()
{
  if(GetPlrExtraData(GetOwner(this()),"AirTurning") == 1 || GetPlrExtraData(GetOwner(),"AirTurning") == 2)
  {
      turningAccepted = GetPlrExtraData(GetOwner(this()),"AirTurning");
      SetPlrExtraData(GetOwner(),"AirTurning",turningAccepted);
  }
  else
  {
      turningAccepted = 1;  
  }  
    return(1);
}

protected func ContextTurning(object pCaller)
{
  [Turning in air on/off|Image=MN__]

  if(turningAccepted == 2)
  {
      turningAccepted = 1;
      PlayerMessage(GetOwner(), "Turning in air on!", this());
  }
  else
  {
      turningAccepted = 2;
      PlayerMessage(GetOwner(), "Turning in air off!", this());
  }
  SetPlrExtraData(GetOwner(),"AirTurning",turningAccepted);
  return(1);
}

func ControlLeft()
{
  if(turningAccepted == 1)
  {
    if (GetProcedure() != "SCALE" && GetProcedure() != "DIG")
        SetDir(DIR_Left);
  }
  return _inherited(...);
}

func ControlRight()
{
  if(turningAccepted == 1)
  {
    if (GetProcedure() != "SCALE" && GetProcedure() != "DIG")
        SetDir(DIR_Right);
  }
  return _inherited(...);
}
