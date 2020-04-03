#include "TableStorage.h"

TableStorage::TableStorage()
{

}
TableStorage::~TableStorage()
{

}
// 
// Return 0 -- ok
//        1 -- duplicate ID
int TableStorage::AddGridPoint(int id, double x, double y, double z, int spc)
{
	int error = 0;
	int len = GridTable.size();
	for (int i = 0; i < len; i++) {
		if (GridTable[i].id == id)
			return 1;
	}
	Grid *g = new Grid();
	g->id = id;
	g->x = x;
	g->y = y;
	g->z = z;
	g->spc = spc;
	GridTable.push_back(*g);

	return error;

}
int TableStorage::AddCbarElement(int id, int pid, int iga, int igb, double x, double y, double z, int PA, int PB) {
	int error = 0;
	Element* e = new Element();
	e->id = id;
	e->pid = pid;
	e->grida = iga;
	e->gridb = igb;
	// Skip orient vector for now
	e->pinFlagsa = PA;
	e->pinFlagsb = PB;

	error = CheckForDupEID(id);
	if (error > 0)
		return error;

	ElementTable.push_back(*e);
	return error;

}
int TableStorage::CheckForDupEID(int id)
{
	int len = ElementTable.size();
	for (int i = 0; i < len; i++) {
		if (ElementTable[i].id == id)
			return 1;
	}
	return 0;
}
int TableStorage::AddPbar(int id, int mid, double A, double i1, double i2, double j, double nsm)
{
	int error = 0;
	int len = PropertyTable.size();
	for (int i = 0; i < len; i++) {
		if (PropertyTable[i].id == id)
			return 1;
	}
	PBAR* p = new PBAR();
	p->id = id;
	p->mid = mid;
	p->Area = A;
	p->I1 = i1;
	p->I2 = i2;
	p->J = j;
	p->Nsm = nsm;

	PropertyTable.push_back(*p);

	return error;
}
int TableStorage::AddPbarLame(int id)
{
	int error = 0;
	int len = PropertyTable.size();
	for (int i = 0; i < len; i++) {
		if (PropertyTable[i].id == id)
			return 1;
	}
	return error;
}
int TableStorage::AddMaterial(int id, double e, double g, double nu, double ro)
{
	int error = 0;
	int len = MaterialTable.size();
	for (int i = 0; i < len; i++) {
		if (MaterialTable[i].id == id)
			return 1;
	}

	MAT1 *m = new MAT1();
	m->id = id;
	m->e = e;
	m->g = g;
	m->nu = nu;
	m->rho = ro;

	MaterialTable.push_back(*m);

	return error;
}