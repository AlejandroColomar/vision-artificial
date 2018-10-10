/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard (C++) ------------------------------------------------------------*/
		/* INT_MAX */
	#include <limits>
		/* std::vector */
	#include <vector>

/* Standard (C) --------------------------------------------------------------*/
		/* snprintf() */
	#include <stdio.h>

/* Packages ------------------------------------------------------------------*/
		/* opencv */
	#include <opencv2/opencv.hpp>
	#include <opencv2/features2d/features2d.hpp>

/* Project -------------------------------------------------------------------*/
		/* user_iface_log */
	#include "user_iface.hpp"

	#include "img_orb.hpp"


/******************************************************************************
 ******* macros ***************************************************************
 ******************************************************************************/
	# define	MAX_FEATURES	(500)
	# define	GOOD_MATCH_P	(0.15)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_orb_align	(struct _IplImage  *pattern,
				struct _IplImage  *imgptr);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_orb_act	(struct _IplImage  *pattern,
			struct _IplImage  **imgptr2, int action)
{
	switch (action) {
	case IMG_ORB_ACT_ALIGN:
		img_orb_align(pattern, *imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_orb_align	(struct _IplImage  *pattern,
				struct _IplImage  *imgptr)
{
	/* Transform (struct IplImage *) to (class cv::Mat) */
	/* Make a copy so that they aren't modified */
	class cv::Mat	img_0;
	class cv::Mat	img_1;
	img_0	= cv::cvarrToMat(pattern, true);
	img_1	= cv::cvarrToMat(imgptr, true);

	/* Variables to store keypoints & descriptors */
	std::vector <class cv::KeyPoint>	keypoints_0;
	std::vector <class cv::KeyPoint>	keypoints_1;
	class cv::Mat				descriptors_0;
	class cv::Mat				descriptors_1;

	/* Detect ORB features & compute descriptors */
	class cv::Ptr <class cv::Feature2D>	orb;
	orb	= cv::ORB::create(MAX_FEATURES);
	orb->detectAndCompute(img_0, cv::Mat::Mat(), keypoints_0, descriptors_0);
	orb->detectAndCompute(img_1, cv::Mat::Mat(), keypoints_1, descriptors_1);

	/* Match structures */
	std::vector <struct cv::DMatch>		matches;
	cv::Ptr <class cv::DescriptorMatcher>	matcher;
	matcher	= cv::DescriptorMatcher::create("BruteForce-Hamming");
	matcher->match(descriptors_1, descriptors_0, matches, cv::Mat::Mat());

	/* Sort matches by score */
	std::sort(matches.begin(), matches.end());

	/* Remove not so good matches */
	int	good_matches;
	good_matches	= GOOD_MATCH_P * matches.size();
	matches.erase(matches.begin() + good_matches, matches.end());

	/* Draw top matches */
	class cv::Mat	img_matches;
	cv::drawMatches(img_1, keypoints_1, img_0, keypoints_0, matches,
								img_matches);
	cv::imwrite("matches.jpg", img_matches);

	/* Extract location of good matches */
	std::vector <class cv::Point_ <float>>	points_0;
	std::vector <class cv::Point_ <float>>	points_1;
	int	i;	
	for (i = 0; i < matches.size(); i++) {
		points_1.push_back(keypoints_1[matches[i].queryIdx].pt);
		points_0.push_back(keypoints_0[matches[i].trainIdx].pt);
	}

	/* Find homography */
	class cv::Mat	img_hg;
	img_hg	= cv::findHomography(points_1, points_0, CV_RANSAC);

	/* Use homography to warp image */
	class cv::Mat	img_align;
	cv::warpPerspective(img_1, img_align, img_hg, img_0.size());

	/* Write img_align into imgptr */
	*imgptr	= img_align;
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
