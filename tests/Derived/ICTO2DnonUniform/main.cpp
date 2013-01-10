

#include <ifc2x3/ExpressDataSet.h>
#include <ifc2x3/IfcCartesianTransformationOperator2DnonUniform.h>
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

	LOG_DEBUG(std::endl << "Test with no Scale" << std::endl);
	Step::RefPtr<ifc2x3::IfcCartesianTransformationOperator2DnonUniform> CTOperator2DnonUniform1 = eds->createIfcCartesianTransformationOperator2DnonUniform();
	TEST_VALIDITY(CTOperator2DnonUniform1);
	
	Step::RefPtr< ifc2x3::IfcDirection > Axis11 = eds->createIfcDirection();
	ifc2x3::List_Real_2_3 ListReal11;
	ListReal11.push_back(1.0);
	ListReal11.push_back(0.0);
	Axis11->setDirectionRatios(ListReal11);

	CTOperator2DnonUniform1->setAxis1(Axis11);

	Step::RefPtr< ifc2x3::IfcDirection > Axis21 = eds->createIfcDirection();
	ifc2x3::List_Real_2_3 ListReal21;
	ListReal21.push_back(0.0);
	ListReal21.push_back(1.0);
	Axis21->setDirectionRatios(ListReal21);

	CTOperator2DnonUniform1->setAxis2(Axis21);

	
	LOG_DEBUG("getScale2 = "<< Step::isUnset( CTOperator2DnonUniform1->getScale2()) << std::endl);
	TEST_ASSERT(Step::isUnset( CTOperator2DnonUniform1->getScale2()));
	LOG_DEBUG("getScl2 = "<< CTOperator2DnonUniform1->getScl2() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform1->getScl2()==1.0);
	LOG_DEBUG("getScl = "<< CTOperator2DnonUniform1->getScl() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform1->getScl()==1.0);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
	LOG_DEBUG(std::endl << "Test with CTOperator2DnonUniform2->setScale(2.5);" << std::endl);
	Step::RefPtr<ifc2x3::IfcCartesianTransformationOperator2DnonUniform> CTOperator2DnonUniform2 = eds->createIfcCartesianTransformationOperator2DnonUniform();
	TEST_VALIDITY(CTOperator2DnonUniform2);

	Step::RefPtr< ifc2x3::IfcDirection > Axis12 = eds->createIfcDirection();
	ifc2x3::List_Real_2_3 ListReal12;
	ListReal12.push_back(2.0);
	ListReal12.push_back(1.0);
	Axis12->setDirectionRatios(ListReal12);

	CTOperator2DnonUniform2->setAxis1(Axis12);

	Step::RefPtr< ifc2x3::IfcDirection > Axis22 = eds->createIfcDirection();
	ifc2x3::List_Real_2_3 ListReal22;
	ListReal22.push_back(4.0);
	ListReal22.push_back(1.0);
	Axis22->setDirectionRatios(ListReal22);

	CTOperator2DnonUniform2->setAxis2(Axis22);

	CTOperator2DnonUniform2->setScale(2.5);
	LOG_DEBUG("getScale2 = "<< Step::isUnset( CTOperator2DnonUniform2->getScale2()) << std::endl);
	TEST_ASSERT(Step::isUnset( CTOperator2DnonUniform2->getScale2()));
	LOG_DEBUG("getScl2 = "<< CTOperator2DnonUniform2->getScl2() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform2->getScl2()==2.5);
	LOG_DEBUG("getScl = "<< CTOperator2DnonUniform2->getScl() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform2->getScl()==2.5);


	CTOperator2DnonUniform2->setScale2(4.5);
	LOG_DEBUG(std::endl << "Test with CTOperator2DnonUniform2->setScale2(4.5);" << std::endl);

	LOG_DEBUG("getScale2 = "<< CTOperator2DnonUniform2->getScale2() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform2->getScale2()==4.5);
	LOG_DEBUG("getScl2 = "<< CTOperator2DnonUniform2->getScl2() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform2->getScl2()==4.5);
	LOG_DEBUG("getScl = "<< CTOperator2DnonUniform2->getScl() << std::endl);
	TEST_ASSERT(CTOperator2DnonUniform2->getScl()==2.5);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << std::endl << "Failure : " << failure_results << " Success : " << success_results << std::endl;

	return failure_results;
}
