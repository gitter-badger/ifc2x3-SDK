

#include <ifc2x3/ExpressDataSet.h>
#include <ifc2x3/IfcStructuralLinearActionVarying.h>
#include <ifc2x3/IfcShapeAspect.h>
#include "../../tests.h"

#include <sstream>
#include <string>
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

    Step::RefPtr< ifc2x3::IfcStructuralLinearActionVarying > StructuralLinearActionVarying = eds->createIfcStructuralLinearActionVarying();
    TEST_ASSERT(StructuralLinearActionVarying->getVaryingAppliedLoads().empty());

    Step::RefPtr< ifc2x3::IfcShapeAspect > ShapeAspect = eds->createIfcShapeAspect();
    TEST_VALIDITY(ShapeAspect);
    Step::RefPtr< ifc2x3::IfcShapeModel > ShapeModelRepresentation1 = eds->createIfcShapeRepresentation();
    Step::RefPtr< ifc2x3::IfcShapeModel > ShapeModelRepresentation2 = eds->createIfcShapeRepresentation();
    ShapeAspect->getShapeRepresentations().push_back(ShapeModelRepresentation1);
    ShapeAspect->getShapeRepresentations().push_back(ShapeModelRepresentation2);
    ShapeAspect->setName("ShapeAspect");
    ShapeAspect->setDescription("IfcShapeAspect");
    ShapeAspect->setProductDefinitional(Step::LTrue);
    Step::RefPtr< ifc2x3::IfcProductDefinitionShape > ProductDefinitionShape = eds->createIfcProductDefinitionShape();
    ShapeAspect->setPartOfProductDefinitionShape(ProductDefinitionShape);
    StructuralLinearActionVarying->setVaryingAppliedLoadLocation(ShapeAspect);

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadLinearForce > load =
                eds->createIfcStructuralLoadLinearForce();

        load->setLinearForceX(1);
        load->setLinearForceY(1);
        load->setLinearForceZ(1);
        load->setLinearMomentX(1);
        load->setLinearMomentY(1);
        load->setLinearMomentZ(1);
        StructuralLinearActionVarying->getSubsequentAppliedLoads().push_back(load);
    }

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadPlanarForce > load =
                eds->createIfcStructuralLoadPlanarForce();

        load->setPlanarForceX(2);
        load->setPlanarForceY(2);
        load->setPlanarForceZ(2);

        StructuralLinearActionVarying->getSubsequentAppliedLoads().push_back(load);
    }

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadSingleDisplacement > load =
                eds->createIfcStructuralLoadSingleDisplacement();

        load->setDisplacementX(3);
        load->setDisplacementY(3);
        load->setDisplacementZ(3);
        load->setRotationalDisplacementRX(3);
        load->setRotationalDisplacementRY(3);
        load->setRotationalDisplacementRZ(3);

        StructuralLinearActionVarying->getSubsequentAppliedLoads().push_back(load);
    }

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadSingleForce > load =
                eds->createIfcStructuralLoadSingleForce();

        load->setForceX(4);
        load->setForceY(4);
        load->setForceZ(4);
        load->setMomentX(4);
        load->setMomentY(4);
        load->setMomentZ(4);

        StructuralLinearActionVarying->getSubsequentAppliedLoads().push_back(load);
    }

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadTemperature > load =
                eds->createIfcStructuralLoadTemperature();

        load->setDeltaT_Constant(5);
        load->setDeltaT_Y(5);
        load->setDeltaT_Z(5);

        StructuralLinearActionVarying->getSubsequentAppliedLoads().push_back(load);
    }

    ifc2x3::List_IfcStructuralLoad_2_n ListIfcStructuralLoad = StructuralLinearActionVarying->getVaryingAppliedLoads();
    if(dynamic_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[0].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadLinearForce> StructuralLoadLinearForce = static_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[0].get());
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceX()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceY()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceZ()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentX()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentY()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentZ()==1);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadPlanarForce* > (ListIfcStructuralLoad[1].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadPlanarForce> StructuralLoadPlanarForce = static_cast<ifc2x3::IfcStructuralLoadPlanarForce* > (ListIfcStructuralLoad[1].get());
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceX()==2);
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceY()==2);
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceZ()==2);

    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadSingleDisplacement* > (ListIfcStructuralLoad[2].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadSingleDisplacement> StructuralLoadSingleDisplacement = static_cast<ifc2x3::IfcStructuralLoadSingleDisplacement* > (ListIfcStructuralLoad[2].get());
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementX()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementY()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementZ()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRX()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRY()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRZ()==3);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadSingleForce* > (ListIfcStructuralLoad[3].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadSingleForce> StructuralLoadSingleForce = static_cast<ifc2x3::IfcStructuralLoadSingleForce* > (ListIfcStructuralLoad[3].get());
        TEST_ASSERT(StructuralLoadSingleForce->getForceX()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getForceY()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getForceZ()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentX()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentY()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentZ()==4);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadTemperature* > (ListIfcStructuralLoad[4].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadTemperature> StructuralLoadTemperature = static_cast<ifc2x3::IfcStructuralLoadTemperature* > (ListIfcStructuralLoad[4].get());
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Constant()==5);
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Y()==5);
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Z()==5);
    }

    {
        Step::RefPtr< ifc2x3::IfcStructuralLoadLinearForce > load =
                eds->createIfcStructuralLoadLinearForce();

        load->setLinearForceX(0);
        load->setLinearForceY(0);
        load->setLinearForceZ(0);
        load->setLinearMomentX(0);
        load->setLinearMomentY(0);
        load->setLinearMomentZ(0);
        StructuralLinearActionVarying->setAppliedLoad(load);
    }

    ListIfcStructuralLoad = StructuralLinearActionVarying->getVaryingAppliedLoads();
    if(dynamic_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[0].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadLinearForce> StructuralLoadLinearForce = static_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[0].get());
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceX()==0);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceY()==0);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceZ()==0);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentX()==0);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentY()==0);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentZ()==0);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[1].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadLinearForce> StructuralLoadLinearForce = static_cast<ifc2x3::IfcStructuralLoadLinearForce* > (ListIfcStructuralLoad[1].get());
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceX()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceY()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearForceZ()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentX()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentY()==1);
        TEST_ASSERT(StructuralLoadLinearForce->getLinearMomentZ()==1);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadPlanarForce* > (ListIfcStructuralLoad[2].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadPlanarForce> StructuralLoadPlanarForce = static_cast<ifc2x3::IfcStructuralLoadPlanarForce* > (ListIfcStructuralLoad[2].get());
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceX()==2);
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceY()==2);
        TEST_ASSERT(StructuralLoadPlanarForce->getPlanarForceZ()==2);

    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadSingleDisplacement* > (ListIfcStructuralLoad[3].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadSingleDisplacement> StructuralLoadSingleDisplacement = static_cast<ifc2x3::IfcStructuralLoadSingleDisplacement* > (ListIfcStructuralLoad[3].get());
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementX()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementY()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getDisplacementZ()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRX()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRY()==3);
        TEST_ASSERT(StructuralLoadSingleDisplacement->getRotationalDisplacementRZ()==3);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadSingleForce* > (ListIfcStructuralLoad[4].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadSingleForce> StructuralLoadSingleForce = static_cast<ifc2x3::IfcStructuralLoadSingleForce* > (ListIfcStructuralLoad[4].get());
        TEST_ASSERT(StructuralLoadSingleForce->getForceX()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getForceY()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getForceZ()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentX()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentY()==4);
        TEST_ASSERT(StructuralLoadSingleForce->getMomentZ()==4);
    }
    if(dynamic_cast<ifc2x3::IfcStructuralLoadTemperature* > (ListIfcStructuralLoad[5].get()))
    {
        Step::RefPtr<ifc2x3::IfcStructuralLoadTemperature> StructuralLoadTemperature = static_cast<ifc2x3::IfcStructuralLoadTemperature* > (ListIfcStructuralLoad[5].get());
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Constant()==5);
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Y()==5);
        TEST_ASSERT(StructuralLoadTemperature->getDeltaT_Z()==5);
    }

    std::cout << std::endl << "Failure : " << failure_results << " Success : " << success_results << std::endl;

    return failure_results;
}
