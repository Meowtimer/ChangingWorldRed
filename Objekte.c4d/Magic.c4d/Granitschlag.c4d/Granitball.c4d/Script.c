/*-- Granitball --*/

#strict
protected func Initialize()
{
 // Sicht verfolgen
  SetPlrView(GetOwner(),this());
  
}


public func ExplodeSize() 
{ 
  return(1); 
}

protected func Hit()
{
   Smoke(0, 0, RandomX(100,150));
   Smoke(0, 0, RandomX(100,150));
     CreateParticle("Fire", RandomX(-3,3), RandomX(-4,3), RandomX(-2,2), RandomX(-2,2), RandomX(80,100), RGB(100,100,100));

    CastObjects(_GSR, 8+Random(3), 40-Random(Random(Random(40))), 0, -6);
    CastObjects(_GSR, 8+Random(3), 40-Random(Random(Random(40))), 0, -6);
    CastObjects(_GSR, 5+Random(5), 30-Random(Random(Random(30))), 0, -3);
    CastObjects(_GSR, 5+Random(5), 30-Random(Random(Random(30))), 0, -3);
  Sound("RockBreak*");
  return(RemoveObject());
}



protected func Effect()
{
	
	
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(100,150), RGB(100,105,100));;
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(80,125), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(75,100), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(100,150), RGB(100,105,100));;
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(80,125), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(75,100), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(100,150), RGB(100,105,100));;
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(80,125), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(75,100), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(100,150), RGB(100,105,100));;
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(80,125), RGB(100,100,100));
	CreateParticle("BSpark", RandomX(-3,3), RandomX(-2,3), RandomX(-2,2), RandomX(-2,2), RandomX(75,100), RGB(100,100,100));
	}
