/*-- Changing World --*/ //0.923
#strict 2

// Spielerkonfiguration
static const statSpawnProtection=75;
static const statRelaunches = 5;

// Scoreboard
static const SbrdColRelaunches = 0,
             SbrdColKills = 1,
             SbrdColTeamKills = 2,
             SbrdColSkin = 3;

//Changing World
static const Sektionen_anzahl = 13;



//////////////////////////////////// Script ////////////////////////////////////

/*---------------------------------| Engine |---------------------------------*/

static g_relaunches, g_kills, g_teamkills, g_initstatus,g_teamskin;
static anfangsphase; //damit kein Relaunch abgezogen wird
static Map;          //speichert die zu spielende karte


//Skins
static teamskin_wahl;    //array: Der Skin mit den meisten Stimmen wird für das ganze Team übernommen
static skins;            //Array aller Skins
static speicher;         //array: für team skin
static gesetzte_skins;   //array: Alle gewählten Skins bekommen einen Wert, sodass 2 teams nicht den selben Skin bekommen können
static gesetzte_skins_zaehler; //Zaehlt, wie viele Skins bereits zugeteilt wurden
static SpielerInTeam;    //array, welches Anzahl der Spieler pro Team speichert
static scoreboard_skins; //array: speichert die ID der Skins, mit denen ein Team spielt


//2 DO:
/*



*/


// Initialisierung
protected func Initialize()
{
  g_initstatus = [];
  g_relaunches = [];
  g_kills = [];
  g_teamkills = [];
  g_teamskin = []; //Skin für das Team
  teamskin_wahl = [];
  skins = ["Magus", "Magierin", "Zauberclonk", "Aquaclonk", "GepanzerterRitter",
           "Koenig", "Hydroclonk", "Urwaldclonk", "Priester", "Skelett", "Assasin", "Astronaut",
           "Hazard", "Mario", "SkelettKoenig"];
  var clonkbild = [B_00, B_01, B_02, B_03, B_04, B_05, B_06, B_07, B_08, B_09, B_10, B_11, B_12, B_13, B_14];
  speicher = [];
  gesetzte_skins = CreateArray(GetLength(skins));
  SpielerInTeam = CreateArray(GetTeamCount());
  scoreboard_skins = [];
  
  ScriptGo(1);
  anfangsphase = 1;
  
  if (!ObjectCount(TCEX)) CreateObject(TCEX,50,50,-1);  
  if (!ObjectCount(_CCB)) CreateObject(_CCB,50,50,-1);  
  if (!ObjectCount(_TIM)) CreateObject(_TIM,50,50,-1);  //Timer
  
  // Clonk Statuen am Anfang
  CreateObject(clonkbild[Random(GetLength(clonkbild))], 405, 519, -1);
  
  for(var i = 140; i <= 260; i += 60){
    CreateObject(clonkbild[Random(GetLength(clonkbild))], i, 579, -1);
    CreateObject(clonkbild[Random(GetLength(clonkbild))], i + 400, 579, -1) -> SetObjDrawTransform(-1000, 0,0, 0,1000,0, 0, 0);
  }
    return 1;
}


protected func InitializePlayer(int iPlr, int ix, int iy, object pObj, int iTeam)
{
 //Joinen nicht erlaubt
 if(ScriptCounter() != 0) {EliminatePlayer(iPlr); return false;}
 // Fog of War
 SetFoW(0, iPlr);  // Deaktiviert
 // Scoreboard initialisieren
 InitializeScoreboard();
 // Spieler erzeugen
 CreatePlayer(iPlr);
 // Spieler launchen
 RelaunchPlayer(iPlr,1);
 
 //Zählt, wie viele Spieler in einem Team gehören (für Skin-team-wahl)
 SpielerInTeam[GetPlayerTeam(iPlr)]++;

 //Befindet man sich noch in der Lobby?
 if(anfangsphase)
  {
   var spieler = GetHiRank(iPlr); 
   CreateMenu(MAGE, spieler, 0, C4MN_Extra_None, "Stimme für deinen Teamskin!");
   SetMenuSize(4,2,spieler);
   var leeres_feld = [];
   var grafik = CreateObject(NOTH,0,0,NO_OWNER); //Dient als leeres BIld (und früher
                                                 //auch als Team-über-Kopf-Nachricht)
   for(var a=0;a<GetLength(skins);++a)
    {
     SetGraphics(skins[a],grafik,MAGE);
       AddMenuItem(skins[a], "SetWahl", a, spieler, 0, iPlr, skins[a],4,grafik);
     leeres_feld[a] = 0;
    }
   RemoveObject(grafik);
   var grafik = CreateObject(CXIN,0,0,NO_OWNER);
   //SetGraphics("Graphics",grafik,RMMG);
   AddMenuItem("Zufall", "SetWahl", a, spieler, 0, iPlr, "Zufall",4,grafik);
   RemoveObject(grafik);
   a = GetPlayerTeam(iPlr);
   if(!teamskin_wahl[a])
    {
     teamskin_wahl[a] = leeres_feld;
    }
  }
 return true;
}



protected func Script1()
{
 ScriptGo(0);
 return true;
}

//Wird vom Timer aufgerufen
public func TimerFunktion(){ //TeamSkinWahlenEnde darf nicht sofort aufgerufen werden
    ScriptGo(1);               //ansonsten manchmal Absturz
    return true;
}

protected func Script3()
{
 ScriptGo(0);
 TeamSkinWahlenEnde();
 return true;
}

//wird aufgerufen, wenn ein Spieler einen Skin wählt
protected func SetWahl(int skin, int iPlr)
{
  ++teamskin_wahl[GetPlayerTeam(iPlr)][skin];
  if(--SpielerInTeam[GetPlayerTeam(iPlr)] <= 0){ //Ist ein Team schon fertig mit seiner Wahl?, dann lege Skin jetzt schon fest!
    SucheSkinMitMeistenVotes(iPlr);
    for(var i = 1; i <= GetTeamCount(); ++i) {
      if(SpielerInTeam[i]) break;
    }
    if(i > GetTeamCount()){//Alle Teams fertig? Dann gehts 'sofort' los!
      Log("Jeder hat seinen Skin gewählt. Das Spiel startet in Kürze!");
      FindObject(_TIM) -> Fertig();
    }
  }
  return true;
}


public func TeamSkinWahlenEnde(){ //Wird vom Timer aufgerufen (nach 10 Frames Verzögerung)
    WerteSkinwahlenAus();
    LadeSektion();
    for(var a = 0; a < GetPlayerCount(); ++a)
   {
    RelaunchPlayer(GetPlayerByIndex(a));
    FindObject(_CCB)->InitializePlayer(GetPlayerByIndex(a));
   }
    anfangsphase = 0; //Es kann losgehen!
    return true;
}


protected func WerteSkinwahlenAus()
{
 for(var a = 0;a < GetPlayerCount(); ++a)
  { //Wenn es Teams gibt
     var y = 0;
     for(var x = 0; x < GetLength(speicher); ++x) //kam zuvor schon jemand, der im selben Team ist?
      {
       if(GetPlayerTeam(GetPlayerByIndex(a)) == GetPlayerTeam(x))
        {
         SetSkin(speicher[x],GetPlayerByIndex(a)); 
         y = 1;
         break;
        }
      }
     if(y == 1) continue;
     SucheSkinMitMeistenVotes(a); 
  }
  return true;
}


protected func SucheSkinMitMeistenVotes(int iPlr){ 
  var big = []; //big[0] = die Stimmen für den Skin mit den meisten votes (big[1] und eventuell auch: big[2], ...)
  for(var b = 0; b < GetLength(skins); ++b)
   {
    if(!teamskin_wahl[GetPlayerTeam(GetPlayerByIndex(iPlr))][b])  continue; //Ein Skin hat keine Stimme? Dann sofort weiter
    if(gesetzte_skins[b])  continue; //Skin wurde bereits zugeteilt
    if(teamskin_wahl[GetPlayerTeam(GetPlayerByIndex(iPlr))][b] > big[0]) //Ein Skin hat mehr Stimmen als der aktuelle?
     {
      big = [];
      big[0] = teamskin_wahl[GetPlayerTeam(GetPlayerByIndex(iPlr))][b];
      big[1] = b;
     }
    else
     {
      if(teamskin_wahl[GetPlayerTeam(GetPlayerByIndex(iPlr))][b] == big[0]) //Ein Skin hat genau so viele Stimmen wie der aktuelle?
       {
        big[GetLength(big)] = b;
       }
     }
   }
  if(!big[0]) //gar nichts wurde gewählt? Dann Zufall!
   {
    SetSkin(GetLength(skins) + 1,GetPlayerByIndex(iPlr));
   }
  else
   {
    //Log("Es wurde min. ein Skin gewählt.");    
    var z = Random(GetLength(big) - 1);
    SetSkin(big[z+1],GetPlayerByIndex(iPlr));
   }
  return true; 
}


//Legt den Skin eines Spielers / des Teams fest
protected func SetSkin(int skin, int iPlr)
{
  if(skin >= GetLength(skins)){ //Zufall oder nichts gewählt?, dann random!
    var a = Random(GetLength(skins) - gesetzte_skins_zaehler);
    
    //Es gibt mehr Teams als Skins? Dann eben doppelte Skins
    if(gesetzte_skins_zaehler >= GetLength(skins)) a = Random(GetLength(skins));
    
    for(var b = 0; b <= a; ++b){
      if(gesetzte_skins[b]) ++a;
    }
    SetSkin(b - 1,iPlr);
    return true;
  }
  for(var i = 0; i < GetPlayerCount(); ++i){
    if(GetPlayerTeam(i) == GetPlayerTeam(iPlr)){
      SetGraphics(skins[skin],GetHiRank(i));
      g_teamskin[i] = skins[skin];
      speicher[i]   = skin;
      var zusatz = "";
      if(skin < 10) zusatz = "0";
      scoreboard_skins[i] = Format("{{B_%s%d}}", zusatz, skin);
    } 
  }
  gesetzte_skins_zaehler++;
  gesetzte_skins[skin] = true;
  return true;
}



//Ladet die zu spielende Karte
public func LadeSektion(int ikarte)
{
 var zusatz = "";
 var a = Random(Sektionen_anzahl) + 1;
 if(ikarte > 0 && ikarte <= Sektionen_anzahl) a = ikarte;
 //else a = 5; //Manipulation!
 if(a<100) zusatz = "0";
 if(a<10)  zusatz = "00";
 Log("Karte Nr. %v wird geladen...",a);
 LoadScenarioSection(Format("%s%v", zusatz, a));
 Map=a;
 if (!ObjectCount(_CCB)) CreateObject(_CCB,50,50,-1);
  
 if(a ==  1) {SetSkyAdjust(RGB(150,250,150));}
 if(a ==  2) {SetSkyAdjust(RGB(70,60,20)); SetMatAdjust(RGBa(180,180,180));}
 //SkyDark(10): SetSkyAdjust(RGB(210,200,160)); //a == 2
 if(a ==  3) {SetSkyAdjust(RGB(250,220,200));}
 if(a ==  4) {SetSkyAdjust(RGB(160,160,160));}
 if(a ==  7) Map_Xeron(); 
 if(a ==  8) Clonkfussball();
 if(a == 10) Frindus_map2();
 if(a == 11) Alx_map1();
 if(a == 13) KampfDerKatapulte(); 
 return true;
}




protected func RelaunchPlayer(int iPlr, int ungueltig_rela)
{
  if (iPlr<0) return;
  // Clonk
  var pClonk = GetCrew(iPlr);
  // Noch Relaunches übrig?
  if (!pClonk)
  {
    if(!anfangsphase)
     {
      if (--g_relaunches[iPlr] >= 0)
       {
        // Scoreboard aktualisieren
        UpdateScoreboard(iPlr);
       }
      else
       {
         // Scoreboard aktualisieren
         UpdateScoreboard(iPlr);
         // Raus ohne speichern
         Log("$LogPlrEliminated$", GetTaggedPlayerName(iPlr));
         Sound("Eliminated", true);
         if (IsNetwork() && !GetScenNetworkGame())
         // Netzwerk-Hack: Kommando nur auf Host-Computer ausfuehren
         // Sonst werden alle Clients aktiviert...
         EliminatePlayer(iPlr, true);
         return true;
       }
     }
    else
      UpdateScoreboard(iPlr);
  }
  // Relaunchen
  pClonk = Redefine(pClonk, MAGE, iPlr, ungueltig_rela);
  SetGraphics(g_teamskin[iPlr],pClonk); //Skin wird natürlich beibehalten!
  AddEffect("KillSymbols",pClonk,50,1);
  
  // Sichtweite
  // SetPlrViewRange(400, pClonk); // Deaktiviert
  return false;
}


protected func OnClonkDeath(object pClonk, int iKilledBy)
{
  if(anfangsphase) return 0;
  //Killsymbols importieren
  var iKillEffectNumber=GetEffect("KillSymbols",pClonk);
  var sKillSymbols="";
  if(iKillEffectNumber) {
    var i;
        for(i=0;EffectVar(i,pClonk,iKillEffectNumber);i++) {
         //Log("%i",EffectVar(i,pClonk,iKillEffectNumber));
            if(GetType(EffectVar(i,pClonk,iKillEffectNumber)) == C4V_C4ID)
        sKillSymbols=Format("%s{{%i}}",sKillSymbols,EffectVar(i,pClonk,iKillEffectNumber));
    }
        if(GetType(EffectVar(i,pClonk,iKillEffectNumber))) sKillSymbols=Format(" (%s)",sKillSymbols);   //Einklammern
  } 
   
  //Aufräumen, Schedules entfernen
  //Hm man hat ja garkeinen Clonk => Workaround, dumme globals. 
  /*
  ClearScheduleCall(pClonk,"RemoveEffect");
  ClearScheduleCall(pClonk,"RegroupEffectVars");
  if(GetEffect("IntSchedule",pClonk)) Log("BuH");
  while(GetEffect("IntSchedule",pClonk)) RemoveEffect("IntSchedule",pClonk); */
 
  //Nicht bei Disconnects
  if(!GetPlayerName(GetOwner(pClonk))) return(1);
  
  // Spieler
  var iPlr = GetOwner(pClonk);

  // Tod durch den Besitzer (= Selbstmord)
  if (iKilledBy == iPlr)
  {
    // Nachricht
    Log("$LogPlrSuicide$", GetTaggedPlayerName(iPlr),sKillSymbols);
  }
  // Tod durch einen Teamkollegen :/
  else if ((GetPlayerTeam(iKilledBy) == GetPlayerTeam(iPlr))&&GetTeamCount())
  {
    // Nachricht
    Log("$LogPlrTeamkill$", GetTaggedPlayerName(iKilledBy), GetTaggedPlayerName(iPlr),sKillSymbols);
    g_teamkills[iKilledBy]++; // Scoreboard: Teamkills
    // Scoreboard aktualisieren
    UpdateScoreboard(iKilledBy);
  }
  // Anderer Tod
  else if (iKilledBy < 0)
  {
    // Nachricht
    Log("$LogPlrDeath$", GetTaggedPlayerName(iPlr),sKillSymbols);
  }
  // Tod durch einen Gegner
  else if (iKilledBy >= 0)
  {
    // Nachricht
    Log("$LogPlrMurder$", GetTaggedPlayerName(iKilledBy), GetTaggedPlayerName(iPlr),sKillSymbols);
    // Spielervariablen
    g_kills[iKilledBy]++; // Scoreboard: Kills
    // Scoreboard aktualisieren
    UpdateScoreboard(iKilledBy);
  }
  return(1);
}

/*---------------------------------| Spielerfunktionen |----------------------*/

protected func CreatePlayer(int iPlr)
{
  // Spielervariablen
  g_initstatus[iPlr] = 1; // Initialisationsstatus
  g_relaunches[iPlr] = statRelaunches;  // Relaunches
  g_kills[iPlr] = 0; // Kills
  g_teamkills[iPlr] = 0; // Teamkills
  // Scoreboard
  UpdateScoreboard(iPlr);
  DoScoreboardShow(1, iPlr);
  return(1);
}

protected func RemovePlayer(int iPlr)
{ 
  // Spielervariablen
  g_initstatus[iPlr] = 0;  // Initialisationsstatus
  g_relaunches[iPlr] = -1;  // Relaunches
  // Scoreboard
  UpdateScoreboard(iPlr);
  return(1);
}

/*---------------------------------| Scoreboardfunktionen |-------------------*/

public func InitializeScoreboard()
{
  // Scoreboardstruktur
  SetScoreboardData(SBRD_Caption, SBRD_Caption, "$SbrdTitle$");
  SetScoreboardData(SBRD_Caption, SbrdColRelaunches, "{{MAGE}}");
  SetScoreboardData(SBRD_Caption, SbrdColKills, "{{FRFS}}");
  SetScoreboardData(SBRD_Caption, SbrdColTeamKills, "{{MONS}}");
  SetScoreboardData(SBRD_Caption, SbrdColSkin, "SKIN"); //'SKIN' wird nicht angezeigt :(
  return(1);
}

public func UpdateScoreboard(int iPlr)
{
  // Scoreboard aktualisieren
  var iPlrID = GetPlayerID(iPlr);
  // Spieler noch nicht initialisiert oder schon eliminiert?
  //if (!g_initstatus[iPlr])
  if(!GetPlayerName(iPlr))
  {
    SetScoreboardData(iPlrID, SBRD_Caption);  // Spielername
    SetScoreboardData(iPlrID, SbrdColRelaunches);  // Relaunches
    SetScoreboardData(iPlrID, SbrdColKills);  // Kills

  }
  // Sonst updaten
  else
  {
    SetScoreboardData(iPlrID, SBRD_Caption, Format("%s", GetTaggedPlayerName(iPlr)));  // Spielername
    SetScoreboardData(iPlrID, SbrdColRelaunches, Format("%d", g_relaunches[iPlr]), g_relaunches[iPlr]);  // Relaunches
    if (g_relaunches[iPlr] < 0) SetScoreboardData(iPlrID, SbrdColRelaunches, "$SbrdPlayerEliminated$", g_relaunches[iPlr]);  // Out
    SetScoreboardData(iPlrID, SbrdColKills, Format("%d", g_kills[iPlr]), g_kills[iPlr]);  // Kills
    SetScoreboardData(iPlrID, SbrdColTeamKills, Format("%d", g_teamkills[iPlr]), g_teamkills[iPlr]);  // Teamkills
    SetScoreboardData(iPlrID, SbrdColSkin, scoreboard_skins[iPlr]);  // Skins
  }
  //Nach den Relaunches und sekundär nach Kills sortieren
  SortScoreboard(SbrdColTeamKills, 0);
  SortScoreboard(SbrdColKills, 1);
  SortScoreboard(SbrdColRelaunches, 1);
  return(1);
}

/*---------------------------------| Funktionen |-----------------------------*/

global func AntiStuck(object pObj) {
  while(Stuck(pObj))
    SetPosition(GetX(pObj), GetY(pObj)-1, pObj);
    return 1;
}

public func Redefine(object pClonk, id idTo, int iPlr, int ungueltig_rela)
{
  // Welcher Clonk?
  if (!pClonk && this()) pClonk = this();
  var pNew = pClonk;
  // Neuer Clonk
  if (GetID(pClonk) != idTo)
  {
  
    //Nützlich, wenn es keinen Platz gibt einen Wipf zu erzeugen
    VorWipfErzeugung();

    // Neuer Clonk erstellen, nicht zu nahe am Gegner
    pNew = PlaceAnimal(idTo);   

    Var(0)=PlaceAnimal(WIPF);
    var j=100;
    var posx=GetX(Var(0));
    var posy=GetY(Var(0));
    SetPosition(posx,posy,pNew);

    while((((GetLength(FindObjects(Find_Distance(225, posx, posy), Find_Hostile(iPlr))) && j>30) //Feind in der Nähe? Dann nochmal (außer nach über 70 Versuchen)
             || Stuck(Var(0))                                                                    //Stuckt man?
             || (GetMaterial(GetX(Var(0)),GetY(Var(0))) == Material("Tunnel"))                   //Spawnt man im Hintergrund: Tunnel?
             )&& (j--)                                                                           //alle obigen Bedingungen nur für die ersten 100 Versuche
             )|| MapBedingungen(posx, posy)                                                      //Spezielle Bedingungen für maps (müssen immer erfüllt sein)
             )
        {
       RemoveObject(Var(0));
       Var(0)=PlaceAnimal(WIPF);
       posx=GetX(Var(0));
       posy=GetY(Var(0));
       SetPosition(posx,posy,pNew);
      }
    
    if(GetY(pNew) > (LandscapeHeight()*7)/10) {
      // Eine magische Brücke unter dem König erschaffen
      TieferSpawn(posx, posy);
     }
     
    RemoveObject(Var(0));
     
    // Alten Clonk übernehmen
    if (pClonk)
    {
      // Position
      pNew -> SetPosition(GetX(pClonk), GetY(pClonk));
      // Inhalte übernehmen
      pNew -> GrabContents(pClonk);
      // Aktion übernehmen
      pNew -> SetAction(GetAction(), GetActionTarget(0, pClonk), GetActionTarget(1, pClonk));
      // Richtung übernehmen
      pNew -> SetDir(GetDir(pClonk));
      // Kommandorichtung übernehmen
      pNew -> SetComDir(GetComDir(pClonk));
      //AntiStuck
      while(Stuck(pNew))
        pNew->SetPosition(GetX(), GetY()-1);
      //kurze Unverwundbarkeit (verhindert Spawnkills, keine anderen Effekte)
      if(anfangsphase) AddEffect("JustSpawned", pNew,360, 1);
      else             AddEffect("JustSpawned", pNew,102, 1);
      // Fertig; Diesen Clonk nun entfernen
      RemoveObject(pClonk);
    }
  }

  // Position
  var iX = AbsX(GetX(pNew)), iY = AbsY(GetY(pNew));
  //RelaunchPosition(iX, iY, GetPlayerTeam(iPlr));
  pNew -> SetPosition(iX, iY);
  // Besitzer
  pNew -> SetOwner(iPlr);
  // Crewmitglied
  pNew -> MakeCrewMember(pNew, iPlr);
  // Volle Startenergie
  pNew -> DoEnergy(10000);
  // Volle Zauberenergie
  if (GetPhysical("Magic",, pNew)/*&&FrameCounter()>1*/) pNew -> DoMagicEnergy(10000,, 1);
  
  // Für den Spieler
  SetCursor(iPlr, pNew);
  SetPlrView(iPlr, pNew);
  //AntiStuck
  AntiStuck(pNew);
  //kurze Unverwundbarkeit (verhindert Spawnkills, keine anderen Effekte)
  if(anfangsphase) AddEffect("JustSpawned", pNew,(statSpawnProtection+10)*3, 1);
  else             AddEffect("JustSpawned", pNew,statSpawnProtection+10, 1);
  return(pNew);
}


/** EFFEKTE **/

global func FxJustSpawnedTimer(object pTarget,int iEffectNumber,int iEffectTime) {
 var iMaxTime=statSpawnProtection;
 if(anfangsphase) SetClrModulation(RGBa(255,255,90+ iEffectTime /3,180-iEffectTime / 2),pTarget);
 else             SetClrModulation(RGBa(255,255,90+iEffectTime * 2,180-iEffectTime*2),pTarget);
 if(iEffectTime>iMaxTime && (!anfangsphase)) return(-1);
 return(1);
 } 

global func FxJustSpawnedDamage() { return(0); }

global func FxJustSpawnedStop(pTarget)
{
    SetCategory(C4D_Living,pTarget);
    SetClrModulation(RGBa(255,255,255,0),pTarget);  
    return 1;
}

global func FxJustSpawnedEffect(string e){
  if (WildcardMatch(e,"*Fire*")) return -1;
}

global func FxJustSpawnedStart(pTarget)
{
    SetClrModulation(RGBa(255,255,90,180),pTarget);
    SetCategory(C4D_Vehicle,pTarget);
    return 1;
}

/* Relaunchposition */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  return(1);
}







/* ein paar maps brauchen Extras */

protected func Map_Xeron()
{
 for(var chest in FindObjects(Find_ID(CHST)))
  {
   while(Contents(0,chest)) RemoveObject(Contents(0,chest));
   FillChest(chest);
  }
 CreateObject(_SC_,1,1,-1);
}

protected func FillChest(object chest)
{
    var count = Random(3)+2;
    while(count--)
    {
        var playerID = RandChestID();
        if(FindObject2(Find_ID(playerID), Find_Container(chest)))
            count++;
        else
            CreateContents(playerID, chest);
    }
}

protected func RandChestID()
{
    var rand = [COKI,SPER,STFN,SFLN,SWOR,AXE1,BOW1,GUNP,WBRL,ARMR, SPER,STFN,SFLN,SWOR,AXE1,EFLN];
    return(rand[Random(GetLength(rand))]);
}

protected func Clonkfussball()
{
 var i=40;
 while(i<LandscapeWidth()-40)
  {
     CreateObject(GRAS,i,10,NO_OWNER);
     i+=Random(2)+5;
    }
}

protected func Alx_map1()
{
  AddEffect("ThrowObjectA3", 0, 30, 120, 0);
  var x=[];
  var y=[];
  for(var i=0; i<20; i++) {
    var pObj=PlaceAnimal(WIPF);
    x[i]=GetX(pObj);
    y[i]=GetY(pObj);
    RemoveObject(pObj);
  }
  
  for(var a=0; a<20; a++) 
    CreateObject(KBSH, x[a], y[a]+10, NO_OWNER);
  
  for(var a=0; a<20; a++) {
    if(Random(1)) 
        CreateObject(_VEG, x[a]-10, y[a]+10, NO_OWNER);
    else
        CreateObject(_VEG, x[a]+10, y[a]+10, NO_OWNER);
  }
  CreateObject(ANST, 0, 0, -1);
}

protected func KampfDerKatapulte()
{
  if(!FindObject(MELE)) CreateObject(MELE);
  if(!FindObject(GOAL)) CreateObject(GOAL);  
  if(!FindObject(_NOB)) CreateObject(_NOB);
  SetSkyAdjust(RGBa(255,255,255,0)); 
  return true;
}

protected func Frindus_map2(){
  CreateObject(CCRS,405,520,-1);
  Schedule("FindObject(CCRS,385,480,40,40) -> Set(1, 100, 0, 0, 255, 1)",1); //muss verzögert werden
  return true;
}

global func FxThrowObjectA3Timer() {
  var x=Random(LandscapeWidth());
  var y=Random(LandscapeHeight());
  var aRandomObjects=[ROCK, FLNT, GOLD, BONE];
  var pObj=CreateObject(aRandomObjects[Random(GetLength(aRandomObjects)+1)-1], x, y, NO_OWNER);
  SetR(Random(90), pObj); //Rotation
  SetRDir(Random(100), pObj);
  SetXDir(Random(100), pObj);
  SetYDir(Random(100), pObj);
}

/* Map Extra Objekte Ende */


/* Spawnregeln: In machen Maps kann man eingeschlossen werden */

global func Frindus2(int iX, int iY) {
   if(Map!=10) return false;
   if(iY>260 && iY<370 && iX>170 && iX <290) return true;
   if(iY>260 && iY<370 && iX>520 && iX <640) return true;
   return false;
}

global func Alx1(int iX, int iY) {
   if(Map!=11) return false;
   if(!GetPathLength(iX, iY, (LandscapeWidth()*12)/100, (LandscapeHeight()*55)/100))
     return true; //thx to Zapper
   return false;
}

global func MapBedingungen(int iX, int iY){
   if(Map == 10) return(Frindus2(iX,iY));
   if(Map == 11) return(Alx1(iX, iY));
   return false;
}

/* Spawnregel Ende */


/* Brücke zaubern, falls man zu tief spawnt */

protected func TieferSpawn(int iX, int iY){
  //Maps, in denen man nicht aus der Map fallen kann, brauchen das nicht
  if(Map == 10 || Map == 11 || Map == 12 || Map == 13) return false;
  CreateObject(FBRG, iX, iY+16, -1); 
  return true;
}

/* Tiefe-Spawn-Brücke Ende */


/* Vor der Wipf Erzeugung: In manchen Maps kann ein Wipf nicht platziert werden, da es z.b. nur Himmel gibt */
     
protected func VorWipfErzeugung(){
  if(Map == 13){
    var riX = 15 + Random(LandscapeWidth()-70);
    var riY = 50 + Random(LandscapeHeight()-100);
    DrawMaterialQuad("FlyAshes-RoughDark",riX, riY, riX+40, riY, riX+40, riY+20, riX, riY+20);
  }
  return true;
}

/* Vor der Wipf Erzeugung Ende */





