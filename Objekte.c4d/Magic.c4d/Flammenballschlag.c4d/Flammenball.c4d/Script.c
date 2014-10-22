/*-- Granitball --*/

#strict
protected func Initialize()
{
	// Sicht verfolgen
	SetPlrView(GetOwner(), this);
	Incinerate(this);
	Schedule("Extinguish(FindObjectOwner(MAGE,GetOwner(this())))", 10, 0);
	Schedule("DoEnergy(2,FindObjectOwner(MAGE,GetOwner(this())))", 10, 0);
	AddEffect("SpawnFlame", this, 50, 5, this);
}


private func ExplodeSize()
{
	return 30;
}

protected func Hit()
{
	CreateParticle
	(
		"Fire",
		RandomX(-3, 3),
		RandomX(-4, 3),
		RandomX(-2, 2),
		RandomX(-2, 2),
		RandomX(80, 100),
		RGB(100, 100, 100)
	);
	SetYDir(Random(1) - 1, this, 1);
	Sound("Fire");
	return 1;
}

public func FxSpawnFlameTimer(obj, effect)
{
	CreateObject(FLAM);
	return 0;
}

global func Earth()
{
	var c = GetHiRank(1);
	DrawMaterialQuad
	(
		"Earth-Rough",
		c->GetX() - 100,
		c->GetY() - 100,
		c->GetX() + 100,
		c->GetY() - 100,
		c->GetX() + 100,
		c->GetY() + 100,
		c->GetX() - 100,
		c->GetY() + 100
	);
}

protected func Effect()
{
	var i = 0;
	var pObject = FindObject(0, -15, -15, 30, 30, OCF_Inflammable);
	if (GetXDir() == 0 && GetYDir() == 0)
		return Boom();
	if (pObject)
		Incinerate(pObject);
	CreateParticle
	(
		"Fire",
		RandomX(-3, 3),
		RandomX(-4, 3),
		RandomX(-2, 2),
		RandomX(-2, 2),
		RandomX(80, 100),
		RGB(100, 10, 10)
	);
	while (i < GetVertexNum() - 1)
	{
		DrawParticleLine
		(
			"Fire",
			GetVertex(i) - GetX(),
			GetVertex(i, 1) - GetY(),
			GetVertex(i + 1) - GetX(),
			GetVertex(i + 1, 1) - GetY(),
			6,
			60,
			RGB(255, 10, 0)
		);
		DrawParticleLine
		(
			"Blast",
			GetVertex(i) - GetX(),
			GetVertex(i, 1) - GetY(),
			GetVertex(i + 1) - GetX(),
			GetVertex(i + 1, 1) - GetY(),
			3,
			25,
			RGB(250, 10, 15)
		);
		i++;
	}
	CreateParticle
	(
		"PSpark",
		RandomX(-4, 4),
		RandomX(-3, 4),
		RandomX(-3, 3),
		RandomX(-3, 4),
		RandomX(100, 150),
		RGB(200, 15, 15)
	);
	CreateParticle
	(
		"PSpark",
		RandomX(-4, 4),
		RandomX(-3, 4),
		RandomX(-3, 3),
		RandomX(-3, 4),
		RandomX(75, 80),
		RGB(190, 5, 20)
	);
	CreateParticle
	(
		"PSpark",
		RandomX(-4, 4),
		RandomX(-3, 4),
		RandomX(-3, 3),
		RandomX(-3, 4),
		RandomX(25, 50),
		RGB(255, 15, 5)
	);
}

private func Boom()
{
	Explode(ExplodeSize(), this);
	CastObjects(FLAM, 12, 25, 0, 0);
}


