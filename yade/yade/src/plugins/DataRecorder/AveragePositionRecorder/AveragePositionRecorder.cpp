#include "AveragePositionRecorder.hpp"
#include "RigidBodyParameters.hpp"
#include "Omega.hpp"
#include "ComplexBody.hpp"

#include <boost/lexical_cast.hpp>

AveragePositionRecorder::AveragePositionRecorder () : Actor()
{
	outputFile = "";
	interval = 50;
}

void AveragePositionRecorder::postProcessAttributes(bool deserializing)
{
	if(deserializing)
	{
		ofile.open(outputFile.c_str());
	}
}

void AveragePositionRecorder::registerAttributes()
{
	Actor::registerAttributes();
	REGISTER_ATTRIBUTE(outputFile);
	REGISTER_ATTRIBUTE(interval);
}

bool AveragePositionRecorder::isActivated()
{
	return ((Omega::instance().getCurrentIteration() % interval == 0) && (ofile));
}


void AveragePositionRecorder::action(Body * body)
{
	ComplexBody * ncb = dynamic_cast<ComplexBody*>(body);
	
	Real x=0, y=0, z=0, size=0;
	for( ncb->bodies->gotoFirst() ; ncb->bodies->notAtEnd() ; ncb->bodies->gotoNext() )
	if( ncb->bodies->getCurrent()->isDynamic)
		{ 
			size+=1.0;
			x+=ncb->bodies->getCurrent()->physicalParameters->se3.translation[0];
			y+=ncb->bodies->getCurrent()->physicalParameters->se3.translation[1];
			z+=ncb->bodies->getCurrent()->physicalParameters->se3.translation[2];
		}

	x /= size;
	y /= size;
	z /= size;
	
	ofile << lexical_cast<string>(Omega::instance().getSimulationTime()) << " " 
		<< lexical_cast<string>(x) << " " 
		<< lexical_cast<string>(y) << " " 
		<< lexical_cast<string>(z) << endl;
}

