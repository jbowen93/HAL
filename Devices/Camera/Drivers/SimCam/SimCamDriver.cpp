/*
   \file SimCamDriver.cpp
 */

#include "SimCamDriver.h"
#include <opencv/cv.h>	// for Mat structure
#include <stdio.h>

#include "TinyOgre.h"

///////////////////////////////////////////////////////////////////////////////
//  Releases the cameras and exits
SimCamDriver::SimCamDriver()
{
}

///////////////////////////////////////////////////////////////////////////////
//  Releases the cameras and exits
SimCamDriver::~SimCamDriver()
{
}

///////////////////////////////////////////////////////////////////////////////
bool SimCamDriver::Capture( std::vector<cv::Mat>& vImages )
{

    // allocate images if necessary
//    if( vImages.size() != m_nNumCams ){
//        vImages.resize( m_nNumCams );
//        // and setup empty images
//        for(unsigned int ii = 0; ii < m_nNumCams; ii++ ) {
////            vImages[ii] = cv::Mat( m_nViewportHeight, m_nViewportWidth, CV_8UC1 );
//              vImages[ii] = cv::Mat( m_nViewportHeight, m_nViewportWidth, CV_8UC3 );
//        }
//        // for depth info
//        vImages[ m_nNumCams ] = cv::Mat( m_nViewportHeight, m_nViewportWidth, CV_8UC1 );
//    }

	TinyOgre app;
	app.go();

    // get texture

    // convert to grayscale

    // save depth in matrix

    return true;
}


///////////////////////////////////////////////////////////////////////////////
bool SimCamDriver::Init()
{
    assert(m_pPropertyMap);
    m_pPropertyMap->PrintPropertyMap();

    // read property map
    m_nViewportWidth = m_pPropertyMap->GetProperty<unsigned int>( "ImageWidth", 512 );
    m_nViewportHeight = m_pPropertyMap->GetProperty<unsigned int>( "ImageHeight", 384 );
    m_nNumCams = m_pPropertyMap->GetProperty<unsigned int>( "NumCameras", 1 );

    m_dZNear = 1.0;
    m_dZFar = 10000.0;

    return true;
}