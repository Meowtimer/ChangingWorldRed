/*-- LocalPlayerFinder --*/
// Manages all players and finds out which onea are local

#strict 2

local unknown_plrs, local_plrs, remote_plrs;

func Initialize()
{
  unknown_plrs = [];
  local_plrs = [];
  remote_plrs = [];
  // Add already joined players
  for (var i=0; i<GetPlayerCount(C4PT_User); ++i)
    unknown_plrs[GetLength(unknown_plrs)] = GetPlayerByIndex(i, C4PT_User);
  if (GetLength(unknown_plrs)) SetAction("Active");
}

func InitializePlayer(int plr)
{
  if (GetPlayerType(plr) != C4PT_User) return;
  //Log("Init plr %d", plr);
  unknown_plrs[GetLength(unknown_plrs)] = plr;
  SetAction("Active");
}

func CheckTime()
{
  // Determine all unknown_plrs
  for (var plr in unknown_plrs)
  {
    var cursor = GetCursor(plr);
    if (!cursor) continue;
    var mnu = GetMenu(cursor);
    var is_local = (GetPlayerVal("ViewWealth", 0, plr)>95);
    if (!mnu && !is_local)
    {
      // No menu open and wealth display is off: Open our test menu
      // Note: Command object must be cursor, not this, for wealth display on correct player.
      //  Wtf engine bugs?
      CreateMenu(GetID(), cursor, cursor, C4MN_Extra_Value, "", 0, 0, 1, GetID());
    }
    else if (mnu == GetID())
    {
      // This menu is open: Query view
      if (is_local)
      {
        local_plrs[GetLength(local_plrs)] = plr;
      }
      else
      {
        remote_plrs[GetLength(remote_plrs)] = plr;
      }
      RemoveFromArray(unknown_plrs, plr);
      GameCallEx("OnPlayerLocalityFound", plr, is_local);
      CloseMenu(cursor);
    }
    else
    {
      // Another menu is open - keep in unknown and wait for menu to close
    }
  }
  if (!GetLength(unknown_plrs)) SetAction("Idle");
}

func RemovePlayer(int plr)
{
  // Game callback: Remove player from all lists
  //Log("Remove plr %d", plr);
  var idx;
  RemoveFromArray(unknown_plrs, plr);
  RemoveFromArray(local_plrs, plr);
  RemoveFromArray(remote_plrs, plr);
}

func RemoveFromArray(&arr, int plr)
{
  // If plr is found in arr, remove that element
  var idx = GetIndexOf(plr, arr);
  if (idx<0) return false;
  var new_len = GetLength(arr)-1;
  arr[idx] = arr[new_len];
  SetLength(arr, new_len);
  return true;
}

static const PLRLOC_Invalid = 0, // invalid player number
              PLRLOC_Unknown = 1, // locality not determined yet
              PLRLOC_Local   = 2, // player at local computer
              PLRLOC_Remote  = 3; // player at remote computer

global func GetPlayerLocality(int plr)
{
  // find out where a player is located
  var lp69 = FindObject(LP69);
  if (!lp69) lp69 = CreateObject(LP69, AbsX(50), AbsY(50), NO_OWNER);
  return lp69->GetPlayerLocality2(plr);
}

public func GetPlayerLocality2(int plr)
{
  if (GetIndexOf(plr, unknown_plrs) >= 0) return PLRLOC_Unknown;
  if (GetIndexOf(plr, local_plrs) >= 0) return PLRLOC_Local;
  if (GetIndexOf(plr, remote_plrs) >= 0) return PLRLOC_Remote;
  return PLRLOC_Invalid;
}

global func GetLocalPlayers()
{
  // return an array of all players that play locally on this computer
  var lp69 = FindObject(LP69);
  if (!lp69) lp69 = CreateObject(LP69, AbsX(50), AbsY(50), NO_OWNER);
  return lp69->GetLocalPlayers2();
}

func GetLocalPlayers2()
{
  
  return local_plrs;
}
