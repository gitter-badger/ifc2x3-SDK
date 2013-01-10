

#include <ifc2x3/ExpressDataSet.h>
#include <ifc2x3/IfcEdgeLoop.h>
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

    Step::RefPtr<ifc2x3::IfcEdgeLoop> edgeLoop = eds->createIfcEdgeLoop();
    TEST_VALIDITY(edgeLoop);
    TEST_ASSERT(edgeLoop->getNe()==0);

    {
        Step::RefPtr<ifc2x3::IfcOrientedEdge> edge = eds->createIfcOrientedEdge();
        TEST_VALIDITY(edge);
        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_start = eds->createIfcCartesianPoint();
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_start = eds->createIfcVertexPoint();
        p_start->setVertexGeometry(cp_start);
        edge->setEdgeStart(p_start);

        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_end = eds->createIfcCartesianPoint();
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_end = eds->createIfcVertexPoint();
        p_end->setVertexGeometry(cp_end);
        edge->setEdgeEnd(p_end);

        edgeLoop->getEdgeList().push_back(edge);
    }
    TEST_ASSERT(edgeLoop->getNe()==1);
    LOG_DEBUG("EdgeLoop->getNe() = " <<edgeLoop->getNe()<< std::endl);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        Step::RefPtr<ifc2x3::IfcOrientedEdge> edge = eds->createIfcOrientedEdge();
        TEST_VALIDITY(edge);
        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_start = eds->createIfcCartesianPoint();
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_start = eds->createIfcVertexPoint();
        p_start->setVertexGeometry(cp_start);
        edge->setEdgeStart(p_start);

        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_end = eds->createIfcCartesianPoint();
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_end = eds->createIfcVertexPoint();
        p_end->setVertexGeometry(cp_end);
        edge->setEdgeEnd(p_end);

        edgeLoop->getEdgeList().push_back(edge);
    }
    TEST_ASSERT(edgeLoop->getNe()==2);
    LOG_DEBUG("EdgeLoop->getNe() = " <<edgeLoop->getNe()<< std::endl);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Step::RefPtr<ifc2x3::IfcOrientedEdge> OrientedEdge3 = eds->createIfcOrientedEdge();
    TEST_VALIDITY(OrientedEdge3);
    {
        Step::RefPtr<ifc2x3::IfcOrientedEdge> edge = eds->createIfcOrientedEdge();
        TEST_VALIDITY(edge);
        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_start = eds->createIfcCartesianPoint();
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_start = eds->createIfcVertexPoint();
        p_start->setVertexGeometry(cp_start);
        edge->setEdgeStart(p_start);

        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_end = eds->createIfcCartesianPoint();
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_end = eds->createIfcVertexPoint();
        p_end->setVertexGeometry(cp_end);
        edge->setEdgeEnd(p_end);

        edgeLoop->getEdgeList().push_back(edge);
    }
    TEST_ASSERT(edgeLoop->getNe()==3);
    LOG_DEBUG("EdgeLoop->getNe() = " <<edgeLoop->getNe()<< std::endl);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
        Step::RefPtr<ifc2x3::IfcOrientedEdge> edge = eds->createIfcOrientedEdge();
        TEST_VALIDITY(edge);
        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_start = eds->createIfcCartesianPoint();
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        cp_start->getCoordinates().push_back(0);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_start = eds->createIfcVertexPoint();
        p_start->setVertexGeometry(cp_start);
        edge->setEdgeStart(p_start);

        Step::RefPtr<ifc2x3::IfcCartesianPoint> cp_end = eds->createIfcCartesianPoint();
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        cp_end->getCoordinates().push_back(1);
        Step::RefPtr<ifc2x3::IfcVertexPoint> p_end = eds->createIfcVertexPoint();
        p_end->setVertexGeometry(cp_end);
        edge->setEdgeEnd(p_end);

        edgeLoop->getEdgeList().push_back(edge);
    }
    TEST_ASSERT(edgeLoop->getNe()==4);
    LOG_DEBUG("EdgeLoop->getNe() = " <<edgeLoop->getNe()<< std::endl);
////////////////////////////////////////////////////////////////////////////////////////////////////////////


    std::cout << std::endl << "Failure : " << failure_results << " Success : " << success_results << std::endl;

    return failure_results;
}
