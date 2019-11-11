//standardShit.c
#include "standardShit.h"


double CleanUpAngle(double Angle)
{
    double angle;
    angle = Angle;
    while ( (angle >= 2*PI)  ||  (angle < 0) )
    {
	if (angle < 0)
	    angle += 2*PI;
	else if (angle >= 2*PI)
	    angle -= 2*PI;
    }
    return angle;
}


double AngleFrom2Points(point2D centre, point2D end)
{
  return CleanUpAngle(PI/2 + atan2(centre.x - end.x, centre.y - end.y));
}



point2D PointFromPolar(point2D centre, double r, double angle)
{
    return (point2D){ 	.x = r * cos(angle) + centre.x, 
			.y = r * -sin(angle) + centre.y	};
}

double GetTimeMono() 
{
    // Calculate time taken by a request
    struct timespec request;
    double accum;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &request);
    accum = request.tv_sec + request.tv_nsec / BILLION;
    return accum;
}

double Distance(point2D p0, point2D p1)
{
    
    return hypot(p1.x-p0.x, p1.y-p0.y);
    //~ return sqrt((p1.X-p0.X)*(p1.X-p0.X) + (p1.Y-p0.Y)*(p1.Y-p0.Y));
}

void CleanUpAngles(double *StartAngle, double *EndAngle, int Direction)
{
    double start, end;
    
    // swap start and end for anticlockwise
    if(Direction == 1) // CW
	start = *StartAngle, 	end = *EndAngle;
    else if(Direction == -1) //ACW
	start = *EndAngle, 	end = *StartAngle;
    
    // if values are equal, make out of phase by 2PI
    if (start == end){  // special condition required due to rounding errir -    after adding 2PI to end, (end - start) !>= 2*PI)
	end += 2*PI;
    }
    else {
	// make start and end within 360degs
	while ( ((end - start) > 2*PI)  ||  ((end - start) < 0) )
	{
	    if (((end - start) < 0))
		    end += 2*PI;
	    else if ((end - start) > 2*PI) 
		end -= 2*PI;
	}
    }
    // phase shift both by -2*PI until one is less than zero (for vals greater than 4PI)
    while((start>0) && (end>0))
	start -= 2*PI,		end -= 2*PI;
    // phase shift both by +2*PI so they are both positive (for values < 0)
    while((start<0) || (end<0))
	start += 2*PI,		end += 2*PI;
    // swap and return start and end
    if(Direction == 1) //CW
	*StartAngle = start, 	*EndAngle = end;
    else if(Direction == -1) //ACW
	*EndAngle = start, 	*StartAngle = end;
}

point2D add2D(point2D p1, point2D p2)
{
    return (point2D){ .x = p1.x+p2.x, .y = p1.y+p2.y };
}
