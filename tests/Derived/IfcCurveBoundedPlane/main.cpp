

#include <ifc2x3/ExpressDataSet.h>
#include <ifc2x3/IfcCurveBoundedPlane.h>
#include <ifc2x3/IfcElementarySurface.h>
#include "../../tests.h"

#include <sstream>
#include <string>
//#include <math.h>
// uncomment the following line to test compilation types checks
// #define TEST_COMPILATION_ERRORS

#ifndef LOG_DEBUG_ENABLE
#define LOG_DEBUG_ENABLE
#endif
int main(int n, char **p)
{
	Step::RefPtr<ifc2x3::ExpressDataSet> eds ;
	try
	{
		std::cout << "Creating ExpressDataSet" << std::endl;
		eds = new ifc2x3::ExpressDataSet;
	}
	catch (const std::exception& e)
	{
		TEST_FAILURE((std::string("Exception : ") + e.what()).c_str());
	}

	TEST_VALIDITY(eds);

///////////////////////////////////////////////////////////////////////////////////////////////////////

	Step::RefPtr<ifc2x3::IfcCurveBoundedPlane> CurveBoundedPlane = eds->createIfcCurveBoundedPlane();
	TEST_VALIDITY(CurveBoundedPlane);
	TEST_ASSERT(CurveBoundedPlane->getDim()==0);	

///////////////////////////////////////////////////////////////////////////////////////////////////////

	Step::RefPtr<ifc2x3::IfcPlane> BasisSurface = eds->createIfcPlane();
	TEST_VALIDITY(BasisSurface);

	
	Step::RefPtr<ifc2x3::IfcDirection> axis = eds->createIfcDirection();
	axis->getDirectionRatios().push_back(1);
	axis->getDirectionRatios().push_back(0);
	axis->getDirectionRatios().push_back(0);


	Step::RefPtr<ifc2x3::IfcDirection> RefDirection = eds->createIfcDirection();
	RefDirection->getDirectionRatios().push_back(1);
	RefDirection->getDirectionRatios().push_back(1);
	RefDirection->getDirectionRatios().push_back(0);

	Step::RefPtr<ifc2x3::IfcCartesianPoint> Location = eds->createIfcCartesianPoint();
	ifc2x3::List_IfcLengthMeasure_1_3 Coordinates;
	Coordinates.push_back(0);
	Coordinates.push_back(0);
	Coordinates.push_back(0);
	Location->setCoordinates(Coordinates);

	Step::RefPtr<ifc2x3::IfcAxis2Placement3D> Pos = eds->createIfcAxis2Placement3D();
	Pos->setAxis(axis);
	Pos->setRefDirection(RefDirection);
	Pos->setLocation(Location);

	BasisSurface->setPosition(Pos);

	CurveBoundedPlane->setBasisSurface(BasisSurface);

	TEST_ASSERT(CurveBoundedPlane->getDim()==3);	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << std::endl << "Failure : " << failure_results << " Success : " << success_results << std::endl;

	return failure_results;
}
