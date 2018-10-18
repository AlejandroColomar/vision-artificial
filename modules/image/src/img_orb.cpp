/******************************************************************************
 *	Copyright (C) 2018	Alejandro Colomar Andrés		      *
 ******************************************************************************/


/******************************************************************************
 ******* headers **************************************************************
 ******************************************************************************/
/* Standard C++ --------------------------------------------------------------*/
		/* std::vector */
	#include <vector>

/* Standard C ----------------------------------------------------------------*/
		/* snprintf() */
	#include <cstdio>

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
	# define	MAX_FEATURES	(50000)
	# define	GOOD_MATCH_P	(0.25)


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_orb_align	(struct _IplImage  *img_ref,
				struct _IplImage  **imgptr2);


/******************************************************************************
 ******* main *****************************************************************
 ******************************************************************************/
void	img_orb_act	(struct _IplImage  *img_ref,
			struct _IplImage  **imgptr2, int action)
{
	switch (action) {
	case IMG_ORB_ACT_ALIGN:
		img_orb_align(img_ref, imgptr2);
		break;
	}
}


/******************************************************************************
 ******* static functions *****************************************************
 ******************************************************************************/
static	void	img_orb_align	(struct _IplImage  *img_ref,
				struct _IplImage  **imgptr2)
{
	/* Transform (struct _IplImage *) to (class cv::Mat) */
	/* Make a copy so that they aren't modified */
	class cv::Mat	img_0;
	class cv::Mat	img_1;
	img_0	= cv::cvarrToMat(img_ref, true);
	img_1	= cv::cvarrToMat(*imgptr2, true);

	/* Variables to store keypoints & descriptors */
	std::vector <class cv::KeyPoint>	keypoints_0;
	std::vector <class cv::KeyPoint>	keypoints_1;
	class cv::Mat				descriptors_0;
	class cv::Mat				descriptors_1;

	/* Detect ORB features & compute descriptors */
	class cv::ORB	orb;
	orb(img_0, cv::Mat(), keypoints_0, descriptors_0);
	orb(img_1, cv::Mat(), keypoints_1, descriptors_1);
/*	class cv::Ptr <class cv::Feature2D>	orb;
	orb	= cv::ORB::create(MAX_FEATURES);
	orb->detectAndCompute(img_0, cv::Mat(), keypoints_0, descriptors_0);
	orb->detectAndCompute(img_1, cv::Mat(), keypoints_1, descriptors_1);
*/

	/* Match structures */
	std::vector <struct cv::DMatch>		matches;
	cv::Ptr <class cv::DescriptorMatcher>	matcher;
	matcher	= cv::DescriptorMatcher::create("BruteForce-Hamming");
	matcher->match(descriptors_1, descriptors_0, matches, cv::Mat());

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

	/* Write img_align into imgptr (need a tmp img;  don't know why) */
	struct _IplImage	imgtmp;
	int			cols;
	int			rows;
	int			depth;
	int			chan;
	cols		= img_align.cols;
	rows		= img_align.rows;
	depth		= (*imgptr2)->depth;
	chan		= (*imgptr2)->nChannels;
	cvReleaseImage(imgptr2);
	*imgptr2	= cvCreateImage(cvSize(cols, rows), depth, chan);
	imgtmp		= img_align;
	cvCopy(&imgtmp, *imgptr2);
	img_align.release();
}


/******************************************************************************
 ******* end of file **********************************************************
 ******************************************************************************/
