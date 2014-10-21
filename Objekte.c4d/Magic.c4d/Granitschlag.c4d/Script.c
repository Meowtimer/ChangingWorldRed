/*-- Granitschlag --*/

#strict

public func Activate(pCaster)
{
var i = CreateContents(GRBL,pCaster);
var m = FindContents(WOOD,pCaster);
    if(GetDir(pCaster)==DIR_Left())
    {
        Exit(i,-5,-5,-50,-10,-5,-50);
        if(m)
        {
        RemoveObject(m);
        i->CreateContents(ROCK);
    }
    }
        
    if(GetDir(pCaster)==DIR_Right())
    {
        Exit(i,+5,-5,50,+10,-5,50);
                if(m)
        {
        RemoveObject(m);
        i->CreateContents(ROCK);
    }
    }
    RemoveObject();
    CreateParticle("PSpark", GetX(pCaster)-GetX(), GetY(pCaster)-GetY(), 0, 0, 250);
    CreateParticle("PSpark", GetX(pCaster)-GetX()-10, GetY(pCaster)-GetY()+5, 0, 0, 150);
    CreateParticle("PSpark", GetX(pCaster)-GetX()+10, GetY(pCaster)-GetY()+5, 0, 0, 150);
    Sound("Blast3");
    return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("232"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
