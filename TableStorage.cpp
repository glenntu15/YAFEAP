#include "TableStorage.h"
#include "ErrorManager.h"

TableStorage::TableStorage() : EM(ErrorManager::getInstance())
{

}
TableStorage::~TableStorage()
{

}
// 
// Return 0 -- ok
//        1 -- duplicate ID
int TableStorage::AddGridPoint(int id, double x, double y, double z, char* spc)
{
	int error = 0;
	int len = static_cast<int>(GridTable.size());
	for (int i = 0; i < len; i++) {
		if (GridTable[i].id == id)
			return 1;
	}
	Grid *g = new Grid();
	g->id = id;
	g->x = x;
	g->y = y;
	g->z = z;
	
	GridTable.push_back(*g);

	AddConstraint(id, spc);

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
	int len = static_cast<int>(ElementTable.size());
	for (int i = 0; i < len; i++) {
		if (ElementTable[i].id == id)
			return 1;
	}
	return 0;
}
int TableStorage::AddPbar(int id, int mid, double A, double i1, double i2, double j, double nsm)
{
	int error = 0;
	int len = static_cast<int>(PropertyTable.size());
	for (int i = 0; i < len; i++) {
		if (PropertyTable[i].id == id)
			return 1;
	}
	PBAR* p = new PBAR();
	p->id = id;
	p->mid = mid;
	p->area = A;
	p->I1 = i1;
	p->I2 = i2;
	p->j = j;
	p->nsm = nsm;
	
	PropertyTable.push_back(*p);

	return error;
}
int TableStorage::AddPbarLame(int id)
{
	int error = 0;
	int len = static_cast<int>(PropertyTable.size());
	for (int i = 0; i < len; i++) {
		if (PropertyTable[i].id == id)
			return 1;
	}
	return error;
}
int TableStorage::AddMaterial(int id, double e, double g, double nu, double ro)
{
	int error = 0;
	int len = static_cast<int>(MaterialTable.size());
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
// called with external gid, internal gid added later
int TableStorage::AddForce(int gid, int loadset, double fx, double fy, double fz)
{
	int error = 0;
	int len = static_cast<int>(ForceTable.size());
	for (int i = 0; i < len; i++) {
		if (ForceTable[i].gid == gid) { // duplicate grid
			error = -1;
			return error;
		}
	}
	Force fn;
	fn.gid = gid;
	fn.loadset = loadset;
	fn.fm[0] = fx;
	fn.fm[1] = fy;
	fn.fm[2] = fz;
	ForceTable.push_back(fn);

	return 0;
}

int TableStorage::CleanUpData()
{
	int error = 0;
	int id1, indx;
	const char* gcard = "GRID";
	const char* pcard = "Property";
	const char* mcard = "Material";
//
//	First convert grid number in elements to internal grid numbers
	int len = static_cast<int>(ElementTable.size());

	for (int i = 0; i < len; i++) {
		id1 = ElementTable[i].grida;
		indx = FindGridIndex(id1);
		if (indx < 0)
			EM.ItemNotFound(gcard, id1);
		ElementTable[i].intgrida = indx;
		id1 = ElementTable[i].gridb;
		indx = FindGridIndex(id1);
		if (indx < 0)
			EM.ItemNotFound(gcard, id1);
		ElementTable[i].intgridb = indx;
		
		id1 = ElementTable[i].pid;
		indx = -1;
		// Now make sure it references 
		int plen = static_cast<int>(PropertyTable.size());
		for (int ii = 0; ii < plen; ii++) {
			if (PropertyTable[ii].id == id1) {
				indx = ii;
				ElementTable[i].intpid = ii;
				break;
			}
		}
		if (indx < 0) {
			error = 1;
			EM.ItemNotFound(pcard, id1);
		}
	}
	indx = -1;
	len = NumProperties();
	for (int i = 0; i < len; i++) {
		id1 = PropertyTable[i].mid;
		int mlen = NumMaterials();
		for (int m = 0; m < mlen; m++) {
			if (MaterialTable[m].id == id1) {
				PropertyTable[i].intmid = i;
				indx = i;
				break;
			}
		}
		if (indx < 0) {
			error = 1;
			EM.ItemNotFound(mcard, id1);
		}

	}

	len = NumForces();
	for (int i = 0; i < len; i++) {
		ForceTable[i].intgid = FindGridIndex(ForceTable[i].gid);
	}

//  Check that properties and materials have been entered

	return error;
}
void TableStorage::AddConstraint(int id, char* spc)
{
	int intid = FindGridIndex(id);
	int i0 = int('0');
	if (intid < 0)
		EM.ItemNotFound("GRID", id);
    int len = static_cast<int>(strlen(spc));
	for (int i = 0; i < len; i++) {
		char c = *spc + i;
		if (c != ' ') {
			int n = (int)c - i0 - 1;
			GridTable[intid].constraints[n] = true;
		}

	}
	
}
int inline TableStorage::FindGridIndex(int id)
{
	int len = static_cast<int>(GridTable.size());
	for (int i = 0; i < len; i++) {
		if (GridTable[i].id == id)
			return i;
	}
	return -1; // not found
}
void TableStorage::FillElementTable()
{
	int gid1, gid2;
	double temp;
	Grid g1, g2;
	int nelm = NumElement();
	
	for (int i = 0; i < nelm; i++) {
		gid1 = FindGridIndex(ElementTable[i].grida);
		gid2 = FindGridIndex(ElementTable[i].gridb);
		ElementTable[i].intgrida = gid1;
		ElementTable[i].intgridb = gid2;
		g1 = GridTable[gid1];
		g2 = GridTable[gid2];
		temp = pow((g2.x - g1.x),2) + pow((g2.y - g1.y),2) + pow((g2.z - g1.z),2);
		ElementTable[i].length = sqrt(temp);
	}
}