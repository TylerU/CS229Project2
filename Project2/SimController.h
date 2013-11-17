#include "SimOptions.h"
#include "Grid.h"

using namespace std;

class SimController{
protected:
	int gen;
	SimOptions *opts;

protected:
	void simGeneration();
public:
	SimController(SimOptions *opts);
	virtual string getStateOfCoord(int x, int y);
	virtual void simGenerations(int g);
	void setGridXRange(Range r);
	void setGridYRange(Range r);
	virtual ~SimController();

	inline int getCurGeneration() { return gen; }
};